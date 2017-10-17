CSRC = $(wildcard ../common/*.c)
COBJ = $(CSRC:.c=.o)
SRC = $(wildcard *.c) $(wildcard *.asm) $(wildcard *.s)
OBJ = $(SRC:.c=.o)

AS = as
ASFLAGS = --32
CC = gcc
CFLAGS = -g -arch i386 -ffreestanding -I../include -nostdinc -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-stack-protector -fleading-underscore -fno-asynchronous-unwind-tables

.PHONY: all clean

all: kernel.bin

clean:
	rm -fv $(OBJ) kernel.bin

kernel.bin: $(OBJ)
	$(CC) $(CFLAGS) -T kernel.ld -o kernel.bin $(COBJ) $(OBJ) $(VOBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<