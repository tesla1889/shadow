#ifndef STDARG_H
#define STDARG_H

/*
	include/stdarg.h
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

typedef void* va_list;

#define stack_size(type) (sizeof(type) + ((sizeof(type) % 4) ? (4 - (sizeof(type) % 4)) : 0))
#define va_arg(args,type) (*(type *)(args -= stack_size(type)))
#define va_copy(dst, src) (dst = src)
#define va_end(args) (args = 0)
#define va_start(args, start) (args = (va_list)(& start))

#endif