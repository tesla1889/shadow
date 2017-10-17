/*
	kernel/kmain.c
	Copyright (C) 2017 Nick Trebes
	MIT License (MIT)
*/

#include <vga.h>
#include "gdt.h"
#include "idt.h"
#include "kprint.h"
#include "memory.h"
#include "paging.h"

void kmain() {
	vga_init();

	kprint("Initializing GDT.\n");
	gdt_init();

	kprint("Initializing IDT.\n");
	idt_init();

	kprint("Initializing memory frame manager.\n");
	kmem_init(0); /* TODO: get kernel size */

	/*
	kprint("Initializing page manager.\n");
	kpage_init();
	kprint("Page manager initialized.\n");
	*/

	kprint("End of kmain.\n");
	for (;;) {}
}