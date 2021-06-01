PREFIX=
CC ?= gcc
O ?= -O2
CFLAGS=-std=c99 -D_XOPEN_SOURCE=500 -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -Werror -flto
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
	@cd C; make G=$(G) O=$(O) test

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

install:
	mkdir -pv -m 755 "${PREFIX}/include" "${PREFIX}/man/man3" \
	&& cp -v btui.h "${PREFIX}/include/" \
	&& cp -v btui.3 "${PREFIX}/man/man3/"

uninstall:
	rm -f "${PREFIX}/include/btui.h" "${PREFIX}/man/man3/btui.3"

.PHONY: all, checksyntax, clean, c, testc, lua, testlua, python, testpython, install, uninstall
