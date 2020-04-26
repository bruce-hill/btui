PREFIX=
CC ?= gcc
O ?= -O2
CFLAGS=-std=c99 -D_XOPEN_SOURCE=500 -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L
CWARN=-Wall -Wpedantic -Wextra -Wno-unknown-pragmas -Wno-missing-field-initializers\
	  -Wno-padded -Wsign-conversion -Wno-missing-noreturn -Wno-cast-qual -Wtype-limits
#CFLAGS += -fsanitize=address -fno-omit-frame-pointer
ifeq ($(shell uname -s),Darwin)
	CFLAGS += -D_DARWIN_C_SOURCE
endif
G=

all: ctest rainbow

clean:
	rm -f ctest rainbow

ctest: test.c ../btui.h
	$(CC) $(CFLAGS) $(CWARN) $(G) $(O) $< -o $@

rainbow: rainbow.c ../btui.h
	$(CC) $(CFLAGS) $(CWARN) $(G) $(O) -lm $< -o $@

rainbowdemo: rainbow
	./rainbow

test: ctest
	./ctest

.PHONY: all, clean, test
