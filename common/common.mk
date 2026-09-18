# Shared build rules for all exercises.
#
# Exercise Makefile:
#   SRCS  := src/foo.c           # code the learner implements
#   TESTS := test/test_foo.c     # provided Unity tests
#   include ../../common/common.mk
#
# Targets:
#   make test            build and run tests
#   make test OPT=-O2    same, with optimization (reveals more warnings)
#   make asan            run tests with AddressSanitizer + UndefinedBehaviorSanitizer
#   make valgrind        run tests under valgrind memcheck
#   make clean

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
