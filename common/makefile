SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -g -arch i386 -ffreestanding -I../include -nostdinc -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-stack-protector -fleading-underscore -fno-asynchronous-unwind-tables

.PHONY: all clean

all: $(OBJ)

clean:
	rm -fv $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<