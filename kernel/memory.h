#ifndef MEMORY_H
#define MEMORY_H

/*
	kernel/memory.h
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <integer.h>

#define FRAME_SIZE (0x1000)
#define KERNEL_FRAMES (0x4000)

uint32_t kmem_allock();
uint32_t kmem_allocp();
void kmem_free(uint32_t frame);
void kmem_init(uint32_t kframes);

#endif