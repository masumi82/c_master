# 全課題で共通のビルド規則。
#
# 課題側の Makefile の書き方:
#   SRCS  := src/foo.c           # 学習者が実装するコード
#   TESTS := test/test_foo.c     # 提供する Unity のテスト
#   include ../../common/common.mk
#
# ターゲット:
#   make test            テストをビルドして実行
#   make test OPT=-O2    最適化を有効にして同じことをする（追加の警告が出る）
#   make asan            AddressSanitizer と UndefinedBehaviorSanitizer 付きで実行
#   make valgrind        valgrind memcheck の上で実行
#   make clean           ビルド成果物を削除

COMMON_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
UNITY_DIR  := $(COMMON_DIR)/unity

OPT    ?= -O0
WARN   := -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror
CFLAGS := -std=c11 $(WARN) -g $(OPT) -Isrc -I$(UNITY_DIR)
SAN    := -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all

BUILD    := build
RUNNER   := $(BUILD)/runner$(OPT)
RUNNER_S := $(BUILD)/runner_asan$(OPT)
ALL_SRCS := $(SRCS) $(TESTS) $(UNITY_DIR)/unity.c
DEPS     := $(ALL_SRCS) $(wildcard src/*.h) $(MAKEFILE_LIST)

.PHONY: test asan valgrind clean

test: $(RUNNER)
	./$(RUNNER)

asan: $(RUNNER_S)
	./$(RUNNER_S)

valgrind: $(RUNNER)
	valgrind --error-exitcode=1 --leak-check=full --track-origins=yes ./$(RUNNER)

$(RUNNER): $(DEPS) | $(BUILD)
	$(CC) $(CFLAGS) $(ALL_SRCS) -o $@

$(RUNNER_S): $(DEPS) | $(BUILD)
	$(CC) $(CFLAGS) $(SAN) $(ALL_SRCS) -o $@

$(BUILD):
	mkdir -p $@

clean:
	rm -rf $(BUILD)
