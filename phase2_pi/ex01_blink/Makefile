# Build on the Raspberry Pi itself (needs libgpiod v2: sudo apt install libgpiod-dev).
# Not part of common.mk: this exercise talks to real hardware and has no unit tests.

CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror -g -O0

.PHONY: all run clean

all: build/blink

build/blink: src/blink.c
	mkdir -p build
	$(CC) $(CFLAGS) $< -o $@ -lgpiod

run: build/blink
	./build/blink

clean:
	rm -rf build
