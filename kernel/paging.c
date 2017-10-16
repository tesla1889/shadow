/*
	kernel/paging.c
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include "paging.h"
#include "panic.h"

#define PDE(pdi) ((page_t*)(0xFFFFF000 | (pdi << 2)))
#define PTE(pdi,pti) ((page_t*)(0xFF300000 | (pdi << 12) | (pti << 2)))

typedef enum {
	SUPERVISOR,
	USER = 4
} bit_u_t;

typedef enum {
	READ_ONLY,
	READ_WRITE = 2
} bit_r_t;

typedef enum {
	NOT_PRESENT,
	PRESENT = 1
} bit_p_t;

static page_t create_entry(uint32_t addr, bit_u_t u, bit_r_t r, bit_p_t p);
static uint32_t read_cr0();
static uint32_t read_cr2();
static uint32_t read_cr3();
static void write_cr0(uint32_t val);
static void write_cr3(uint32_t val);

void kpage_handler(regs_t* regs) {
	page_t* pde;
	page_t* pte;
	uint32_t addr;
	uint32_t flags;
	uint32_t pdi;
	uint32_t pti;

	addr = read_cr2();
	pdi = ((addr >> 22) & 0x03FF);
	pti = ((addr >> 12) & 0x03FF);

	pde = PDE(pdi);
	if ((*pde & PRESENT) == 0) {
		uint32_t pt_addr;
		uint32_t n;

		assert(pt_addr = (kmem_allocp() << 12));
		*pde = create_entry(pt_addr,SUPERVISOR,READ_WRITE,PRESENT);

		for (n = 0; n < PAGE_ENTRIES; ++n) {
			*PTE(pdi,n) = create_entry(0,0,0,0);
		}
	}

	pte = PTE(pdi,pti);
	flags = (*pte & 0x0FFF);

	switch (regs->error & 7) {
	case 0:
	case 2:
		*pte = ((kmem_allocp() << 12) | flags | PRESENT);
		break;
	case 4:
	case 6:
		/* TODO: check if valid user address */
		*pte = ((kmem_allocp() << 12) | flags | USER | PRESENT);
		break;
	default:
		panic("Function address @%lx caused a protection fault! (Error code: %lu)\n",regs->eip,regs->error);
		break;
	}
}

void kpage_init() {
	page_t* pd_ptr;
	page_t* pt_ptr;
	uint32_t pd_addr;
	uint32_t pt_addr;
	uint32_t n;

	assert(pd_addr = (kmem_allock() << 12));
	pd_ptr = (page_t*)pd_addr;

	for (n = 0; n < (KERNEL_FRAMES / PAGE_ENTRIES); ++n) {
		uint32_t e;

		assert(pt_addr = (kmem_allocp() << 12));
		pt_ptr = (page_t*)pt_addr;

		for (e = 0; e < PAGE_ENTRIES; ++n) {
			pt_ptr[e] = create_entry((e << 12),SUPERVISOR,READ_WRITE,PRESENT);
		}

		pd_ptr[n] = create_entry(pt_addr,SUPERVISOR,READ_WRITE,PRESENT);
	}

	for (; n < (PAGE_ENTRIES - 1); ++n) {
		pd_ptr[n] = create_entry(0,0,0,0);
	}

	pd_ptr[PAGE_ENTRIES - 1] = create_entry(pd_addr,SUPERVISOR,READ_WRITE,PRESENT);

	write_cr3(pd_addr);
	write_cr0(read_cr0() | 0x80000001);
}

void kpage_free(uint32_t page) {
	page_t* pde;
	page_t* pte;
	uint32_t pdi;
	uint32_t pti;

	pdi = ((page >> 10) & 0x03FF);
	pti = (page & 0x03FF);

	pde = PDE(pdi);
	if ((*pde & PRESENT) == 0) {
		goto invalid_page;
	}

	pte = PTE(pdi,pti);
	if ((*pte & PRESENT) == 0) {
		goto invalid_page;
	}

	*pte ^= PRESENT;
	write_cr3(read_cr3());
	kmem_free(*pte >> 12);
	return;

invalid_page:
	panic("Attempt to free page %lu!\n",page);
}

static page_t create_entry(uint32_t addr, bit_u_t u, bit_r_t r, bit_p_t p) {
	return ((addr & 0xFFFFF000) | u | r | p);
}

static uint32_t read_cr0() {
	uint32_t val;
	__asm__ __volatile__ ("movl %%cr0, %0" : "=r" (val));
	return val;
}

static uint32_t read_cr2() {
	uint32_t val;
	__asm__ __volatile__ ("movl %%cr2, %0" : "=r" (val));
	return val;
}

static uint32_t read_cr3() {
	uint32_t val;
	__asm__ __volatile__ ("movl %%cr3, %0" : "=r" (val));
	return val;
}

static void write_cr0(uint32_t val) {
	__asm__ __volatile__ ("movl %0, %%cr0" : : "r" (val));
}

static void write_cr3(uint32_t val) {
	__asm__ __volatile__ ("movl %0, %%cr3" : : "r" (val));
}