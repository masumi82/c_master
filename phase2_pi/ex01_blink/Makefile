# Raspberry Pi 本体の上でビルドする（libgpiod v2 が必要: sudo apt install libgpiod-dev）。
# 実機を操作する課題で単体テストがないため、common.mk は使わない。

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
