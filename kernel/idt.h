#ifndef INTERRUPT_H
#define INTERRUPT_H

/*
	kernel/idt.h
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <machine.h>

typedef void (*handler_t)(regs_t*);

void idt_init();
void idt_load();
void idt_set(uint8_t id, handler_t base, uint16_t sel, uint8_t flags);

#endif