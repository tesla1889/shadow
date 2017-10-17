.PHONY: all clean common disk kernel

all: kernel

clean:
	make -C common clean
	make -C kernel clean

common:
	make -C common

disk: kernel
	sudo mount -o loop shadow.img disk
	sudo cp kernel/kernel.bin disk
	sleep 1s
	sudo umount disk

kernel: common
	make -C kernel