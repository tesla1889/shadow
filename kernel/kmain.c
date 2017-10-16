/*
	kernel/kmain.c
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include "kprint.h"
#include "memory.h"
#include "paging.h"

void kmain() {
	vga_init();

	kprint("Initializing memory frame manager.\n");
	kmem_init(0); /* TODO: get kernel size */
	kprint("Memory frame manager initialized.\n");

	/* TODO: initialize interrupt and exception handlers */

	kprint("Initializing page manager.\n");
	kpage_init();
	kprint("Page manager initialized.\n");

	kprint("End of kmain.\n");
	for (;;) {}
}