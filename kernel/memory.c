/*
	kernel/memory.c
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <string.h>
#include "memory.h"

#define BITFIELD_COUNT (131072)
#define BITFIELD_LENGTH (2 * BITFIELD_COUNT)

static uint8_t frame_bitfield[BITFIELD_LENGTH];
static uint32_t kframe_num = 0;
static uint32_t pframe_num = 0;

static int is_accessible(uint32_t frame);
static int is_free(uint32_t frame);
static void mark_inaccessible(uint32_t frame);
static void mark_alloc(uint32_t frame);

uint32_t kmem_allock() {
	uint32_t n;

	for (n = kframe_num; n < KERNEL_FRAMES; ++n) {
		if (is_accessible(n) && is_free(n)) {
			goto kframe_found;
		}
	}

	for (n = 0; n < kframe_num; ++n) {
		if (is_accessible(n) && is_free(n)) {
			goto kframe_found;
		}
	}

	return 0;

kframe_found:
	mark_alloc(n);
	kframe_num = ((n + 1) % KERNEL_FRAMES);
	return n;
}

uint32_t kmem_allocp() {
	uint32_t n;

	for (n = pframe_num; n < BITFIELD_COUNT; ++n) {
		if (is_accessible(n) && is_free(n)) {
			goto pframe_found;
		}
	}

	for (n = KERNEL_FRAMES; n < pframe_num; ++n) {
		if (is_accessible(n) && is_free(n)) {
			goto pframe_found;
		}
	}

	return 0;

pframe_found:
	mark_alloc(n);
	pframe_num = ((n + 1) % BITFIELD_COUNT);
	if (pframe_num == 0) pframe_num = KERNEL_FRAMES;
	return n;
}

void kmem_free(uint32_t frame) {
	if (is_accessible(frame)) {
		frame_bitfield[frame / 4] &= ~((uint8_t)(1 << (2 * (3 - (frame % 4)))));
		if (frame < KERNEL_FRAMES) {
			kframe_num = frame;
		} else {
			pframe_num = frame;
		}
	}
}

void kmem_init(uint32_t kframes) {
	uint32_t n;

	pframe_num = KERNEL_FRAMES;

	for (n = 0; n < kframes; ++n) {
		mark_inaccessible(n);
	}

	for (n = 3840; n < 4096; ++n) {
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