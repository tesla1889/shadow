#ifndef KPRINT_H
#define KPRINT_H

/*
	kernel/kprint.h
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <stdarg.h>

typedef enum {
	VGA_BLACK,
	VGA_BLUE,
	VGA_GREEN,
	VGA_CYAN,
	VGA_RED,
	VGA_MAGENTA,
	VGA_BROWN,
	VGA_LIGHT_GREY,
	VGA_DARK_GREY,
	VGA_LIGHT_BLUE,
	VGA_LIGHT_GREEN,
	VGA_LIGHT_CYAN,
	VGA_LIGHT_RED,
	VGA_LIGHT_MAGENTA,
	VGA_LIGHT_BROWN,
	VGA_WHITE
} color_t;

void kprint(const char* fmt, ...);
void kprintv(const char* fmt, va_list args);
void vga_clear();
void vga_color(color_t bg, color_t fg);
void vga_init();
void vga_scroll();

#endif