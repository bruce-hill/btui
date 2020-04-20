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

all: checksyntax

checksyntax: btui.h
	$(CC) $(CFLAGS) $(CWARN) $(G) $(O) -fsyntax-only $<

clean:
	@cd Lua; make clean
	@cd C; make clean
	@cd Python; make clean

%: %.c btui.h
	$(CC) $(CFLAGS) $(CWARN) $(G) $(O) $< -o $@

c:
	@cd C; make

testc:
	@cd C; make test

rainbow:
	@cd C; make rainbowdemo

lua:
	@cd Lua; make

testlua:
	@cd Lua; make test

python:
	@cd Python; make

testpython:
	@cd Python; make test

.PHONY: all, checksyntax, clean, c, testc, lua, testlua, python, testpython
