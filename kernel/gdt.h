#ifndef GDT_H
#define GDT_H

/*
	kernel/gdt.h
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

void gdt_flush();
void gdt_init();
void gdt_set(unsigned gate, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

#endif