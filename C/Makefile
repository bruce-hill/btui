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
