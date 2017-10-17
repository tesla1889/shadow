/*
	kernel/idt.c
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <string.h>
#include "idt.h"

#define IDT_COUNT (256)

typedef struct {
	uint16_t offl;
	uint16_t selector;
	uint8_t zero;
	uint8_t flags;
	uint16_t offh;
} __attribute__((packed)) idt_entry_t;

static idt_entry_t idt[IDT_COUNT];

static struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idtp;

void idt_init() {
	idtp.limit = ((sizeof(idt_entry_t) * IDT_COUNT) - 1);
	idtp.base = (uint32_t)(&idt);

	memset(&idt,0,(sizeof(idt_entry_t) * IDT_COUNT));
}

void idt_set(uint8_t id, handler_t base, uint16_t sel, uint8_t flags) {
	uint32_t addr = (uint32_t)base;

	idt[id].offl = (uint16_t)(addr & 0x0000FFFF);
	idt[id].selector = sel;
	idt[id].zero = 0;
	idt[id].flags = flags;
	idt[id].offh = (uint16_t)((addr >> 16) & 0x0000FFFF);
}