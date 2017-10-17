/*
	kernel/kmain.c
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <integer.h>
#include "gdt.h"
#include "idt.h"
#include "kprint.h"
#include "memory.h"
#include "paging.h"
#include "test.h"
#include "vga.h"

extern uint32_t ksize __asm__("_ksize");

void kmain() {
	ksize = ((ksize >> 12) + 1);

	vga_init();

	kprint("Initializing GDT.\n");
	gdt_init();

	kprint("Initializing IDT.\n");
	idt_init();

	kprint("Initializing memory frame manager.\n");
	kmem_init(ksize); /* TODO: get kernel size */

	/*
	kprint("Initializing page manager.\n");
	kpage_init(ksize);
	kprint("Page manager initialized.\n");
	*/

	//test_memory(32);

	kprint("Success!\n");
	for (;;) {}
}