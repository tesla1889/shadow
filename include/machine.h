#ifndef MACHINE_H
#define MACHINE_H

/*
	include/machine.h
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <integer.h>

typedef struct {
	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;

	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;

	uint32_t intno;
	uint32_t error;

	uint32_t eip;
	uint32_t cs;
	uint32_t flags;

	uint32_t user_esp;
	uint32_t ss;
} regs_t;

#endif