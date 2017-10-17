SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

.PHONY: all clean

CC = gcc
CFLAGS = -g -arch i386 -ffreestanding -I../include -nostdinc -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-stack-protector -fleading-underscore -fno-asynchronous-unwind-tables

all: $(OBJ)

clean:
	rm -fv $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<