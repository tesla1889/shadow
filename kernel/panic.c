/*
	kernel/panic.c
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include "panic.h"

void kassert(uint32_t val, const char* str, const char* file, uint32_t line) {
	if (val == 0) {
		kpanic(file,line,"assert(%s) failed!\n",str);
	}
}

void kpanic(const char* file, uint32_t line, const char* fmt, ...) {
	/* TODO: print panic message */
	for (;;) {}
}