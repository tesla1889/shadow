.PHONY: all clean common kernel

all: kernel

clean:
	make -C common clean
	make -C kernel clean

common:
	make -C common

kernel: common
	make -C kernel