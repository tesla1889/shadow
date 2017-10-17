/*
	common/string.c
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <string.h>

void* memset(void* ptr, int val, uint32_t num) {
	uint32_t n;

	for (n = 0; n < num; ++n) {
		*((uint8_t*)ptr) = (uint8_t)val;
	}

	return ptr;
}

char* strcat(char* dst, const char* src) {
	strcpy((char*)((uint32_t)dst + strlen(dst)),src);
	return dst;
}

char* strcpy(char* dst, const char* src) {
	uint32_t len;
	uint32_t n;

	len = strlen(src);
	for (n = 0; n < len; ++n) {
		dst[n] = src[n];
	}

	return dst;
}

uint32_t strlen(const char* str) {
	uint32_t len;

	for (len = 0; str[len]; ++len) {}

	return len;
}