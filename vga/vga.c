/*
	vga/vga.c
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <integer.h>
#include "vga.h"

#define VGA_HEIGHT (25)
#define VGA_TAB (4)
#define VGA_WIDTH (80)

static uint16_t* vga_buffer;
static uint16_t _vga_color;
static unsigned vga_column;
static unsigned vga_row;

void vga_clear() {
	unsigned x;
	unsigned y;

	for (y = 0; y < VGA_HEIGHT; ++y) {
		for (x = 0; x < VGA_WIDTH; ++x) {
			vga_buffer[(y * VGA_WIDTH) + x] = (_vga_color | ' ');
		}
	}

	vga_column = 0;
	vga_row = 0;
}

void vga_color(vga_color_t bg, vga_color_t fg) {
	_vga_color = (uint16_t)((bg << 12) | (fg << 8));
}

void vga_init() {
	vga_buffer = (uint16_t*)(0x000B8000);

	vga_color(VGA_BLACK,VGA_LIGHT_GREY);
	vga_clear();
}

void vga_putc(char c) {
	switch (c) {
	case '\n':
		do { vga_putc(' '); } while (vga_column);
		return;
	case '\t':
		do { vga_putc(' '); } while (vga_column % VGA_TAB);
		return;
	}

	vga_buffer[(vga_row * VGA_WIDTH) + vga_column] = (_vga_color | c);

	if ((++vga_column) == VGA_WIDTH) {
		vga_column = 0;
		if ((++vga_row) == VGA_HEIGHT) {
			vga_scroll();
			vga_row = (VGA_HEIGHT - 1);
		}
	}
}

void vga_puts(const char* str) {
	char c;

	for (c = *str; c; c = *(++str)) {
		vga_putc(c);
	}
}

void vga_scroll() {
	unsigned x;
	unsigned y;

	for (y = 0; y < (VGA_HEIGHT - 1); ++y) {
		for (x = 0; x < VGA_WIDTH; ++x) {
			vga_buffer[(y * VGA_WIDTH) + x] = vga_buffer[((y + 1) * VGA_WIDTH) + x];
		}
	}

	y *= VGA_WIDTH;

	for (x = 0; x < VGA_WIDTH; ++x) {
		vga_buffer[y + x] = (_vga_color | ' ');
	}

	if (vga_row) {
		--vga_row;
	} else {
		vga_column = 0;
	}
}