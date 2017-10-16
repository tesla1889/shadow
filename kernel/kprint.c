/*
	kernel/kprint.c
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <string.h>
#include "kprint.h"

#define VGA_HEIGHT (25)
#define VGA_TAB (4)
#define VGA_WIDTH (80)

typedef enum {
	P_NORMAL,
	P_L,
	P_LL
} psize_t;

static char escape(const char* str, const char** ptr, psize_t* size);
static void kputc(char c);
static void kputd(int32_t d);
static void kputdll(int64_t d);
static void kputs(const char* str);
static void kputu(uint32_t u);
static void kputull(uint64_t u);
static void kputx(uint32_t u);
static void kputxll(uint64_t u);

static const char* hexchar = "0123456789abcdef";
static uint16_t* vga_buffer;
static uint16_t _vga_color;
static unsigned vga_column;
static unsigned vga_row;

void kprint(const char* fmt, ...) {
	va_list args;
	va_start(args,fmt);
	kprintv(fmt,args);
	va_end(args);
}

void kprintv(const char* fmt, va_list args) {
	const char* ptr;
	char c;
	psize_t size = 0;

	for (c = *fmt; c; c = *(++fmt)) {
		switch (c) {
		case '/':
			break;
		case '%':
			switch (escape((const char*)((uint32_t)fmt + 1),&ptr,&size)) {
			case '%':
				kputc('%');
				break;
			case 'c':
				kputc(va_arg(args,char));
				break;
			case 'd':
				switch (size) {
				case P_NORMAL:
					kputd((int32_t)va_arg(args,int16_t));
					break;
				case P_L:
					kputd(va_arg(args,int32_t));
					break;
				case P_LL:
					kputdll(va_arg(args,int64_t));
					break;
				}
				break;
			case 's':
				kputs(va_arg(args,const char*));
				break;
			case 'u':
				switch (size) {
				case P_NORMAL:
					kputu((uint32_t)va_arg(args,uint16_t));
					break;
				case P_L:
					kputu(va_arg(args,uint32_t));
					break;
				case P_LL:
					kputull(va_arg(args,uint64_t));
					break;
				}
				break;
			case 'x':
				switch (size) {
				case P_NORMAL:
					kputx((uint32_t)va_arg(args,uint16_t));
					break;
				case P_L:
					kputx(va_arg(args,uint32_t));
					break;
				case P_LL:
					kputxll(va_arg(args,uint64_t));
					break;
				}
				break;
			default:
				return;
			}
			fmt = ptr;
			break;
		default:
			kputc(c);
		}
	}
}

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

void vga_color(color_t bg, color_t fg) {
	_vga_color = (uint16_t)((bg << 12) | (fg << 8));
}

void vga_init() {
	vga_buffer = (uint16_t*)(0x000B8000);

	vga_color(VGA_BLACK,VGA_LIGHT_GREY);
	vga_clear();
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

static char escape(const char* str, const char** ptr, psize_t* size) {
	char c;
	psize_t s = P_NORMAL;

	for (c = *str; c; c = *(++str)) {
		switch (c) {
		case '%':
			if (s != P_NORMAL) return 0;
			break;
		case 'l':
			switch (s) {
			case P_NORMAL:
				s = P_L;
				break;
			case P_L:
				s = P_LL;
				break;
			default:
				return 0;
			}
		case 'c':
		case 'd':
		case 's':
		case 'u':
		case 'x':
			break;
		default:
			return 0;
		}
	}

	*ptr = str;
	*size = s;

	return c;
}

static void kputc(char c) {
	switch (c) {
	case '\n':
		do { kputc(' '); } while (vga_column);
		return;
	case '\t':
		do { kputc(' '); } while (vga_column % VGA_TAB);
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

static void kputd(int32_t d) {
	if (d < 0) {
		kputc('-');
		d = (-d);
	}
	kputu((uint32_t)d);
}

static void kputdll(int64_t d) {
	if (d < 0) {
		kputc('-');
		d = (-d);
	}
	kputull((uint64_t)d);
}

static void kputs(const char* str) {
	uint32_t len;
	uint32_t n;

	len = strlen(str);
	for (n = 0; n < len; ++n) {
		kputs(str[n]);
	}
}

static void kputu(uint32_t u) {
	uint32_t digit;
	uint32_t div;
	int start = 0;

	for (div = 1000000000; div; div /= 10) {
		digit = (u / div);
		if (digit || start) {
			kputc((char)(digit + '0'));
			start = 1;
		}
		u %= div;
	}
}

static void kputull(uint64_t u) {
	uint64_t digit;
	uint64_t div;
	int start = 0;

	for (div = 10000000000000000000L; div; div /= 10) {
		digit = (u / div);
		if (digit || start) {
			kputc((char)(digit + '0'));
			start = 1;
		}
		u %= div;
	}
}

static void kputx(uint32_t u) {
	uint32_t div;

	for (div = 0xF0000000; div; div >>= 4) {
		kputc(hexchar[u / div]);
		u %= div;
	}
}

static void kputxll(uint64_t u) {
	uint64_t div;

	for (div = 0xF000000000000000L; div; div >>= 4) {
		kputc(hexchar[u / div]);
		u %= div;
	}
}