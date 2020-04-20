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
LUA_DIR=/usr/local
LUA_INC=$(LUA_DIR)/include
LUA_BIN=$(LUA_DIR)/bin
LUA= lua
#LUA_SHARED_FLAGS=-bundle -undefined dynamic_lookup
LUA_SHARED_FLAGS=-shared -fPIC

all: btui.so

clean:
	rm -f lbtui.o btui.so

test: btui.so test.lua
	$(LUA) test.lua

btui.so: lbtui.o ../btui.h
	$(CC) $(CFLAGS) $(CWARN) $(G) $(O) $(LUA_SHARED_FLAGS) -I$(LUA_INC) -o $@ $<

lbtui.o: lbtui.c
	$(CC) $(CFLAGS) $(CWARN) $(G) $(O) -fPIC -c -o $@ $<

.PHONY: all, clean, testlua, test
