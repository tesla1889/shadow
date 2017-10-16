/*
	kernel/panic.c
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include "kprint.h"
#include "panic.h"

void kassert(uint32_t val, const char* str, const char* file, uint32_t line) {
	if (val == 0) {
		kpanic(file,line,"assert(%s) failed!\n",str);
	}
}

void kpanic(const char* file, uint32_t line, const char* fmt, ...) {
	va_list args;

	va_start(args,fmt);
	kpanicv(file,line,fmt,args);
}

void kpanicv(const char* file, uint32_t line, const char* fmt, va_list args) {
	kprint("\n[KERNEL PANIC] %s:%lu\n",file,line);
	kprintv(fmt,args);

	for (;;) {}
}