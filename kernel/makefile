CSRC = $(wildcard ../common/*.c)
COBJ = $(CSRC:.c=.o)
SRC = $(wildcard *.c) $(wildcard *.asm) $(wildcard *.s)
OBJ = $(SRC:.c=.o)

AS = as
ASFLAGS = --32
CC = gcc
CFLAGS = -g -arch i386 -ffreestanding -I../include -nostdinc -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-stack-protector -fleading-underscore -fno-asynchronous-unwind-tables
LD = ld
LDFLAGS = -melf_i386

.PHONY: all clean

all: kernel.bin

clean:
	rm -fv $(OBJ) kernel.bin

kernel.bin: $(OBJ)
	$(LD) $(LDFLAGS) -T kernel.ld -o kernel.bin $(COBJ) $(OBJ) $(VOBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<