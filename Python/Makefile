PREFIX=
CC ?= gcc
O ?= -O2
CFLAGS=-std=c99 -D_XOPEN_SOURCE=500 -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L
CWARN=-Wall -Wpedantic -Wextra -Wno-unknown-pragmas -Wno-missing-field-initializers\
	  -Wno-padded -Wsign-conversion -Wno-missing-noreturn -Wno-cast-qual -Wtype-limits
#CFLAGS += -fsanitize=address -fno-omit-frame-pointer
ifeq($(shell uname -s),Darwin)
	CFLAGS += -D_DARWIN_C_SOURCE
endif
G=

all: libbtui.so

clean:
	rm -f btui.o libbtui.so

libbtui.so: btui.o
	$(CC) $(CFLAGS) $(CWARN) $(G) $(O) -shared $< -o $@

btui.o: btui.c ../btui.h
	$(CC) $(CFLAGS) $(CWARN) $(G) $(O) -fPIC -c $< -o $@

test: libbtui.so
	python3 test.py

.PHONY: all, clean, test
