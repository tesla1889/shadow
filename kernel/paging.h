#ifndef PAGING_H
#define PAGING_H

/*
	kernel/paging.h
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <machine.h>
#include "memory.h"

#define PAGE_SIZE FRAME_SIZE
#define PAGE_ENTRIES (PAGE_SIZE / 4)

typedef uint32_t page_t;

void kpage_free(uint32_t page);
void kpage_handler(regs_t* regs);
void kpage_init();

#endif