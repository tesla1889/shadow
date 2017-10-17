#ifndef KPRINT_H
#define KPRINT_H

/*
	kernel/kprint.h
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <stdarg.h>

void kprint(const char* fmt, ...);
void kprintv(const char* fmt, va_list args);

#endif