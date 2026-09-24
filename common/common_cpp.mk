# Shared build rules for C++ exercises (Unity tests, header-only or .cpp sources).
#
# Exercise Makefile:
#   SRCS  := src/foo.cpp              # optional; omit for header-only exercises
#   TESTS := test/test_foo.cpp        # provided Unity tests
#   include ../../common/common_cpp.mk
#
# Targets are the same as common.mk:
#   make test / make test OPT=-O2 / make asan / make valgrind / make clean
#
# unity.c is compiled as C with $(CC); everything else is compiled and linked
# as C++17 with $(CXX) using the same warning set as the C exercises.

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
