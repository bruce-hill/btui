PREFIX=
CC ?= gcc
O ?= -O2
CFLAGS=-std=c99 -D_XOPEN_SOURCE=500 -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L
CWARN=-Wall -Wpedantic -Wextra -Wno-unknown-pragmas -Wno-missing-field-initializers\
	  -Wno-padded -Wsign-conversion -Wno-missing-noreturn -Wno-cast-qual -Wtype-limits
#CFLAGS += -fsanitize=address -fno-omit-frame-pointer
G=

all:

clean:
	rm -f test

ctest: test.c ../btui.h
	$(CC) $(CFLAGS) $(CWARN) $(G) $(O) $< -o $@

test: ctest
	./ctest

.PHONY: all, clean, test
