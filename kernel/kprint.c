/*
	kernel/kprint.c
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <string.h>
#include "kprint.h"
#include "vga.h"

typedef enum {
	P_NORMAL,
	P_L,
	P_LL
} psize_t;

static char escape(const char* str, const char** ptr, psize_t* size);
static void kputd(int32_t d);
static void kputdll(int64_t d);
static void kputu(uint32_t u);
static void kputull(uint64_t u);
static void kputx(uint32_t u);
static void kputxll(uint64_t u);

static const char* hexchar = "0123456789abcdef";

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
				vga_putc('%');
				break;
			case 'c':
				vga_putc(va_arg(args,char));
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
				vga_puts(va_arg(args,const char*));
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
			vga_putc(c);
		}
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

	*ptr = (--str);
	*size = s;

	return c;
}

static void kputd(int32_t d) {
	if (d < 0) {
		vga_putc('-');
		d = (-d);
	}
	kputu((uint32_t)d);
}

static void kputdll(int64_t d) {
	if (d < 0) {
		vga_putc('-');
		d = (-d);
	}
	kputull((uint64_t)d);
}

static void kputu(uint32_t u) {
	uint32_t digit;
	uint32_t div;
	int start = 0;

	for (div = 1000000000; div; div /= 10) {
		digit = (u / div);
		if (digit || start) {
			vga_putc((char)(digit + '0'));
			start = 1;
		}
		u %= div;
	}
}

static void kputull(uint64_t u) {
	uint64_t digit;
	uint64_t div;
	int start = 0;

	for (div = 10000000000000000000ULL; div; div /= 10) {
		digit = (u / div);
		if (digit || start) {
			vga_putc((char)(digit + '0'));
			start = 1;
		}
		u %= div;
	}
}

static void kputx(uint32_t u) {
	uint32_t div;

	for (div = 0xF0000000; div; div >>= 4) {
		vga_putc(hexchar[u / div]);
		u %= div;
	}
}

static void kputxll(uint64_t u) {
	uint64_t div;

	for (div = 0xF000000000000000L; div; div >>= 4) {
		vga_putc(hexchar[u / div]);
		u %= div;
	}
}