#ifndef STRING_H
#define STRING_H

/*
	include/string.h
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <integer.h>

void* memset(void* ptr, int val, uint32_t num);
char* strcat(char* dst, const char* src);
char* strcpy(char* dst, const char* src);
uint32_t strlen(const char* str);

#endif