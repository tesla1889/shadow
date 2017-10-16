/*
	kernel/memory.c
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include "memory.h"

#define BITFIELD_COUNT (131072)
#define BITFIELD_LENGTH (2 * BITFIELD_COUNT)

static uint8_t frame_bitfield[BITFIELD_LENGTH];
static uint32_t frame_num = 0;

static int is_accessible(uint32_t frame);
static int is_free(uint32_t frame);
static void mark_inaccessible(uint32_t frame);
static void mark_alloc(uint32_t frame);

uint32_t kmem_alloc() {
	uint32_t n;

	for (n = frame_num; n < BITFIELD_COUNT; ++n) {
		if (is_accessible(n) && is_free(n)) {
			mark_alloc(n);
			frame_num = ((n + 1) % BITFIELD_COUNT);
			return n;
		}
	}

	for (n = 0; n < frame_num; ++n) {
		if (is_accessible(n) && is_free(n)) {
			mark_alloc(n);
			frame_num = ((n + 1) % BITFIELD_COUNT);
			return n;
		}
	}

	return 0;
}

void kmem_free(uint32_t frame) {
	if (is_accessible(frame)) {
		frame_bitfield[frame / 4] &= ~((uint8_t)(1 << (2 * (3 - (frame % 4)))));
		frame_num = frame;
	}
}

void kmem_init() {
	uint32_t n;

	for (n = 0; n < KERNEL_FRAMES; ++n) {
		mark_inaccessible(n);
	}
}

static int is_accessible(uint32_t frame) {
	return (!(frame_bitfield[frame / 4] & (1 << ((2 * (3 - (frame % 4))) + 1))));
}

static int is_free(uint32_t frame) {
	return (!(frame_bitfield[frame / 4] & (1 << (2 * (3 - (frame % 4))))));
}

static void mark_inaccessible(uint32_t frame) {
	frame_bitfield[frame / 4] |= (1 << ((2 * (3 - (frame % 4))) + 1));
}

static void mark_alloc(uint32_t frame) {
	(frame_bitfield[frame / 4] |= (1 << (2 * (3 - (frame % 4)))));
}