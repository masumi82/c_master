# C++ 課題で共通のビルド規則（Unity のテスト、ヘッダだけの実装か .cpp）。
#
# 課題側の Makefile の書き方:
#   SRCS  := src/foo.cpp              # 任意。ヘッダだけの課題では書かない
#   TESTS := test/test_foo.cpp        # 提供する Unity のテスト
#   include ../../common/common_cpp.mk
#
# ターゲットは common.mk と同じ:
#   make test / make test OPT=-O2 / make asan / make valgrind / make clean
#
# unity.c は $(CC) で C としてコンパイルする。それ以外は $(CXX) で C++17 として
# コンパイル・リンクし、警告の設定は C の課題と同じにする。

COMMON_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
UNITY_DIR  := $(COMMON_DIR)/unity

OPT      ?= -O0
WARN     := -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror
CFLAGS   := -std=c11 $(WARN) -g $(OPT) -I$(UNITY_DIR)
CXXFLAGS := -std=c++17 $(WARN) -g $(OPT) -Isrc -I$(UNITY_DIR)
SAN      := -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all

BUILD    := build
RUNNER   := $(BUILD)/runner$(OPT)
RUNNER_S := $(BUILD)/runner_asan$(OPT)
UNITY_O  := $(BUILD)/unity$(OPT).o
UNITY_SO := $(BUILD)/unity_asan$(OPT).o
CXX_SRCS := $(SRCS) $(TESTS)
DEPS     := $(CXX_SRCS) $(wildcard src/*.h src/*.hpp) $(MAKEFILE_LIST)

.PHONY: test asan valgrind clean

test: $(RUNNER)
	./$(RUNNER)

asan: $(RUNNER_S)
	./$(RUNNER_S)

valgrind: $(RUNNER)
	valgrind --error-exitcode=1 --leak-check=full --track-origins=yes ./$(RUNNER)

$(UNITY_O): $(UNITY_DIR)/unity.c $(MAKEFILE_LIST) | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(UNITY_SO): $(UNITY_DIR)/unity.c $(MAKEFILE_LIST) | $(BUILD)
	$(CC) $(CFLAGS) $(SAN) -c $< -o $@

$(RUNNER): $(DEPS) $(UNITY_O) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CXX_SRCS) $(UNITY_O) -o $@

$(RUNNER_S): $(DEPS) $(UNITY_SO) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(SAN) $(CXX_SRCS) $(UNITY_SO) -o $@

$(BUILD):
	mkdir -p $@

clean:
	rm -rf $(BUILD)
