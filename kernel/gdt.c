/*
	kernel/gdt.c
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <integer.h>
#include "gdt.h"

typedef struct {
	uint16_t limit;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

static gdt_entry_t gdt[3];

static struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) gdtp;

void gdt_init() {
	gdtp.limit = ((sizeof(gdt_entry_t) * 3) - 1);
	gdtp.base = (uint32_t)(&gdt);

	gdt_set(0,0,0,0,0);
	gdt_set(1,0,0xFFFFFFFF,0x9A,0xCF);
	gdt_set(2,0,0xFFFFFFFF,0x92,0xCF);

	gdt_flush();
}

void gdt_set(unsigned gate, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
	switch (gate) {
	case 0:
	case 1:
	case 2:
		gdt[gate].limit = (uint16_t)(limit & 0x0000FFFF);
		gdt[gate].base_low = (uint16_t)(base & 0x0000FFFF);
		gdt[gate].base_mid = (uint8_t)((base >> 16) & 0x000000FF);
		gdt[gate].access = access;
		gdt[gate].granularity = ((granularity & 0xF0) | ((uint8_t)(limit >> 16) & 0x0F));
		gdt[gate].base_high = (uint8_t)((base >> 24) & 0x000000FF);
	}
}