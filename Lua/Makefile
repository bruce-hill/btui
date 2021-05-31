PREFIX=
CC ?= gcc
O ?= -O2
CFLAGS=-std=c99 -D_XOPEN_SOURCE=500 -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -Werror
CWARN=-Wall -Wpedantic -Wextra \
	-Wsign-conversion -Wtype-limits -Wunused-result -Wnull-dereference \
	-Waggregate-return -Walloc-zero -Walloca -Warith-conversion -Wcast-align -Wcast-align=strict \
	-Wdangling-else -Wdate-time -Wdisabled-optimization -Wdouble-promotion -Wduplicated-branches \
	-Wduplicated-cond -Wexpansion-to-defined -Wfloat-conversion -Wfloat-equal -Wformat-nonliteral \
	-Wformat-security -Wformat-signedness -Wframe-address -Winline -Winvalid-pch -Wjump-misses-init \
	-Wlogical-op -Wlong-long -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn \
	-Wnull-dereference -Woverlength-strings -Wpacked -Wpacked-not-aligned -Wpointer-arith \
	-Wredundant-decls -Wshadow -Wshadow=compatible-local -Wshadow=global -Wshadow=local \
	-Wsign-conversion -Wstack-protector -Wsuggest-attribute=const -Wswitch-default -Wswitch-enum \
	-Wsync-nand -Wtrampolines -Wundef -Wunused -Wunused-but-set-variable \
	-Wunused-const-variable -Wunused-local-typedefs -Wvariadic-macros -Wvector-operation-performance \
	-Wvla -Wwrite-strings
#CFLAGS += -fsanitize=address -fno-omit-frame-pointer
ifeq ($(shell uname -s),Darwin)
	CFLAGS += -D_DARWIN_C_SOURCE
endif
G=
LUA_DIR=/usr/local
LUA_INC=$(LUA_DIR)/include
LUA_BIN=$(LUA_DIR)/bin
LUA= lua
LUA_O_FLAGS=-fPIC
ifeq ($(shell uname -s),Darwin)
	LUA_SHARED_FLAGS=-bundle -undefined dynamic_lookup
else
	LUA_SHARED_FLAGS=-shared -fPIC
endif

all: btui.so

clean:
	rm -f lbtui.o btui.so

test: btui.so test.lua
	$(LUA) test.lua

btui.so: lbtui.o ../btui.h
	$(CC) $(CFLAGS) $(CWARN) $(G) $(O) $(LUA_SHARED_FLAGS) -I$(LUA_INC) -o $@ $<

lbtui.o: lbtui.c
	$(CC) $(CFLAGS) $(CWARN) $(G) $(O) $(LUA_O_FLAGS) -c -o $@ $<

.PHONY: all, clean, testlua, test
