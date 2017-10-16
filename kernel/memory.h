#ifndef MEMORY_H
#define MEMORY_H

/*
	kernel/memory.h
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <integer.h>

#define FRAME_SIZE (0x1000)
#define KERNEL_FRAMES (0x1000)

uint32_t kmem_alloc();
void kmem_free(uint32_t frame);
void kmem_init();

#endif