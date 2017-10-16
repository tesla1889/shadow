#ifndef PANIC_H
#define PANIC_H

/*
	kernel/panic.h
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <integer.h>

void kassert(uint32_t val, const char* str, const char* file, uint32_t line);
void kpanic(const char* file, uint32_t line, const char* fmt, ...);

#define assert(val) (kassert(val,(#val),__FILE__,__LINE__))
#define panic(fmt,...) (kpanic(__FILE__,__LINE__,fmt,__VA_ARGS__))

#endif