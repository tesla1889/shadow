#ifndef VGA_H
#define VGA_H

/*
	vga/vga.h
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

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
} vga_color_t;

void vga_clear();
void vga_color(vga_color_t bg, vga_color_t fg);
void vga_init();
void vga_putc(char c);
void vga_puts(const char* str);
void vga_scroll();

#endif