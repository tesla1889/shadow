.PHONY: all clean common kernel vga

all: kernel

clean:
	make -C common clean
	make -C kernel clean
	make -C vga clean

common:
	make -C common

kernel: common vga
	make -C kernel

vga:
	make -C vga