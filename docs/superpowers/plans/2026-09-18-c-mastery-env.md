# C Mastery Exercise Environment Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build the exercise environment (shared build rules, Unity, Phase 0–1 exercises, ROADMAP/README) described in the spec.

**Architecture:** Each exercise is a self-contained directory (`README.md`, `src/` stubs, `test/` Unity tests, 3-line `Makefile`) that includes `common/common.mk`. Learners implement `src/`; tests are given. Reference solutions are written only in the scratchpad to prove the tests are correct, then deleted.

**Tech Stack:** C11, gcc 11.4, GNU make, Unity v2.6.1 (ThrowTheSwitch), ASan/UBSan, valgrind, gdb.

**Spec:** `docs/superpowers/specs/2026-09-18-c-mastery-roadmap-design.md`

## Global Constraints

- Flags: `-std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror -g`, optimization `-O0` by default (overridable via `OPT`).
- Targets: `make test`, `make asan` (`-fsanitize=address,undefined`), `make valgrind`, `make clean`.
- Exercise Makefile only defines `SRCS` / `TESTS` and includes `../../common/common.mk`.
- Stubs: `make test` must BUILD successfully and FAIL at least one test (except ex02_bug_hunt, which ships buggy code instead of stubs).
- No reference solutions in the repository — not in `src/`, not in this plan. Reference code lives only in the scratchpad (`$SCRATCH`) and is deleted after verification.
- Learner-facing text (README) in Japanese; code, comments, identifiers in English.
- `$SCRATCH` = `/tmp/claude-1000/-home-m-horiuchi-persol-ws-c-master/82f0b4a6-f970-4f30-a0af-dc58a90b4c80/scratchpad`

### Standard verification procedure (used by Tasks 2–7)

For an exercise directory `$EX`:

1. Stub check:
   ```bash
   make -C $EX clean && make -C $EX test; echo "rc=$?"
   ```
   Expected: compiles without warnings, Unity summary line shows `N Failures` with N ≥ 1, `rc` ≠ 0.
2. Reference check:
   ```bash
   REF=$SCRATCH/ref && rm -rf $REF && mkdir -p $REF/phase && cp -r $EX $REF/phase/ && ln -s $PWD/common $REF/common
   # overwrite $REF/phase/<ex>/src/*.c with a reference implementation (written ad hoc, never in repo)
   make -C $REF/phase/<ex> test asan; echo "rc=$?"
   rm -rf $REF
   ```
   Expected: `0 Failures` for both runs, `rc=0`.

---

### Task 1: Shared build rules + Unity + Phase 0 ex01_toolchain

**Files:**
- Create: `common/unity/unity.c`, `common/unity/unity.h`, `common/unity/unity_internals.h`, `common/unity/LICENSE.txt` (from Unity v2.6.1)
- Create: `common/common.mk`
- Create: `phase0_env/ex01_toolchain/{Makefile,README.md,src/clamp.h,src/clamp.c,test/test_clamp.c}`

**Interfaces:**
- Produces: `common/common.mk` consuming `SRCS`, `TESTS`, optional `OPT`; targets `test`, `asan`, `valgrind`, `clean`; binaries `build/runner$(OPT)` and `build/runner_asan$(OPT)`.
- Produces: `int32_t clamp_i32(int32_t v, int32_t lo, int32_t hi);`

- [ ] **Step 1: Vendor Unity**

```bash
cd $SCRATCH && curl -sSL -o unity.tar.gz https://github.com/ThrowTheSwitch/Unity/archive/refs/tags/v2.6.1.tar.gz
echo "b41a66d45a6b99758fb3202ace6178177014d52fc524bf1f72687d93e9867292  unity.tar.gz" | sha256sum -c
tar xzf unity.tar.gz
mkdir -p /home/m-horiuchi/persol_ws/c_master/common/unity
cp Unity-2.6.1/src/unity.c Unity-2.6.1/src/unity.h Unity-2.6.1/src/unity_internals.h Unity-2.6.1/LICENSE.txt /home/m-horiuchi/persol_ws/c_master/common/unity/
```

- [ ] **Step 2: Write `common/common.mk`**

```make
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
```

- [ ] **Step 3: Write `phase0_env/ex01_toolchain/Makefile`**

```make
SRCS  := src/clamp.c
TESTS := test/test_clamp.c
include ../../common/common.mk
```

- [ ] **Step 4: Write `src/clamp.h`**

```c
#ifndef CLAMP_H
#define CLAMP_H

#include <stdint.h>

/* Returns v limited to the closed range [lo, hi].
 * Precondition: lo <= hi. */
int32_t clamp_i32(int32_t v, int32_t lo, int32_t hi);

#endif
```

- [ ] **Step 5: Write `src/clamp.c` (stub)**

```c
#include "clamp.h"

int32_t clamp_i32(int32_t v, int32_t lo, int32_t hi)
{
    /* TODO: implement */
    (void)v;
    (void)lo;
    (void)hi;
    return 0;
}
```

- [ ] **Step 6: Write `test/test_clamp.c`**

```c
#include <stdint.h>

#include "clamp.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

static void test_value_inside_range_is_unchanged(void)
{
    TEST_ASSERT_EQUAL_INT32(5, clamp_i32(5, 0, 10));
}

static void test_value_below_range_becomes_lo(void)
{
    TEST_ASSERT_EQUAL_INT32(0, clamp_i32(-3, 0, 10));
}

static void test_value_above_range_becomes_hi(void)
{
    TEST_ASSERT_EQUAL_INT32(10, clamp_i32(42, 0, 10));
}

static void test_bounds_are_inclusive(void)
{
    TEST_ASSERT_EQUAL_INT32(0, clamp_i32(0, 0, 10));
    TEST_ASSERT_EQUAL_INT32(10, clamp_i32(10, 0, 10));
}

static void test_degenerate_range(void)
{
    TEST_ASSERT_EQUAL_INT32(7, clamp_i32(-100, 7, 7));
    TEST_ASSERT_EQUAL_INT32(7, clamp_i32(100, 7, 7));
}

static void test_extreme_values(void)
{
    TEST_ASSERT_EQUAL_INT32(-1, clamp_i32(INT32_MIN, -1, 1));
    TEST_ASSERT_EQUAL_INT32(1, clamp_i32(INT32_MAX, -1, 1));
    TEST_ASSERT_EQUAL_INT32(INT32_MIN, clamp_i32(INT32_MIN, INT32_MIN, INT32_MAX));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_value_inside_range_is_unchanged);
    RUN_TEST(test_value_below_range_becomes_lo);
    RUN_TEST(test_value_above_range_becomes_hi);
    RUN_TEST(test_bounds_are_inclusive);
    RUN_TEST(test_degenerate_range);
    RUN_TEST(test_extreme_values);
    return UNITY_END();
}
```

- [ ] **Step 7: Write `README.md`**

````markdown
# Phase 0 / ex01_toolchain — 開発環境を整える

## 目的
以降の全課題で使う道具（厳格な警告・単体テスト・サニタイザ・valgrind・gdb）を一通り動かす。

## 手順
1. ツールを入れる
   ```bash
   sudo apt update && sudo apt install -y gdb valgrind
   gdb --version && valgrind --version
   ```
2. 失敗を確認する: `make test` → テストが失敗する（まだ実装していないため）
3. `src/clamp.c` の `clamp_i32` を実装する（仕様は `src/clamp.h`）
4. 次の3つがすべて通ることを確認する
   ```bash
   make test
   make asan
   make valgrind
   ```
5. gdb を触る
   ```bash
   gdb build/runner-O0
   (gdb) break clamp_i32
   (gdb) run
   (gdb) info args        # 引数の値
   (gdb) next             # 1行進む
   (gdb) print v          # 変数の値
   (gdb) finish           # 関数から抜けて戻り値を表示
   (gdb) bt               # 呼び出し履歴
   (gdb) continue
   (gdb) quit
   ```

## 説明できるようになるべきこと
- `-Wall -Wextra -Wconversion -Werror` はそれぞれ何をするか
- `make asan` と `make valgrind` は何を検出するか、どう違うか
- テストが通ることと、コードが正しいことは同じか
````

- [ ] **Step 8: Verify** — run the Standard verification procedure for `phase0_env/ex01_toolchain` (reference: a correct `clamp_i32`).

- [ ] **Step 9: Commit**

```bash
git add common phase0_env/ex01_toolchain
git commit -m "feat: add shared build rules, Unity, and phase0 ex01_toolchain"
```

---

### Task 2: Phase 0 ex02_bug_hunt

**Files:**
- Create: `phase0_env/ex02_bug_hunt/{Makefile,README.md,src/stats.h,src/stats.c,test/test_stats.c}`

**Interfaces:**
- Consumes: `common/common.mk` (Task 1)
- Produces: `int32_t sum_array(const int32_t *a, size_t n);`, `size_t count_positive(const int32_t *a, size_t n);`, `int32_t average2(int32_t a, int32_t b);`

- [ ] **Step 1: Write `Makefile`**

```make
SRCS  := src/stats.c
TESTS := test/test_stats.c
include ../../common/common.mk
```

- [ ] **Step 2: Write `src/stats.h`**

```c
#ifndef STATS_H
#define STATS_H

#include <stddef.h>
#include <stdint.h>

/* Returns the sum of a[0..n-1]. Returns 0 when n == 0.
 * Precondition: the sum fits in int32_t. */
int32_t sum_array(const int32_t *a, size_t n);

/* Returns how many elements of a[0..n-1] are > 0. */
size_t count_positive(const int32_t *a, size_t n);

/* Returns (a + b) / 2 rounded toward zero, for ANY pair of int32_t values. */
int32_t average2(int32_t a, int32_t b);

#endif
```

- [ ] **Step 3: Write `src/stats.c` (deliberately buggy — 3 bugs)**

```c
#include "stats.h"

int32_t sum_array(const int32_t *a, size_t n)
{
    int32_t sum = 0;
    for (size_t i = 0; i <= n; i++) {
        sum += a[i];
    }
    return sum;
}

size_t count_positive(const int32_t *a, size_t n)
{
    size_t count;
    for (size_t i = 0; i < n; i++) {
        if (a[i] > 0) {
            count++;
        }
    }
    return count;
}

int32_t average2(int32_t a, int32_t b)
{
    return (a + b) / 2;
}
```

- [ ] **Step 4: Write `test/test_stats.c`**

```c
#include <stdint.h>

#include "stats.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

static void test_sum_array(void)
{
    const int32_t a[] = {1, 2, 3};
    TEST_ASSERT_EQUAL_INT32(6, sum_array(a, 3));
}

static void test_sum_array_empty(void)
{
    const int32_t a[] = {99};
    TEST_ASSERT_EQUAL_INT32(0, sum_array(a, 0));
}

static void test_count_positive(void)
{
    const int32_t a[] = {-1, 0, 2, 5};
    TEST_ASSERT_EQUAL_size_t(2, count_positive(a, 4));
}

static void test_count_positive_none(void)
{
    const int32_t a[] = {-5, 0};
    TEST_ASSERT_EQUAL_size_t(0, count_positive(a, 2));
}

static void test_average2_small(void)
{
    TEST_ASSERT_EQUAL_INT32(3, average2(2, 4));
    TEST_ASSERT_EQUAL_INT32(0, average2(-3, 4));
    TEST_ASSERT_EQUAL_INT32(-2, average2(-5, 0));
}

static void test_average2_extremes(void)
{
    TEST_ASSERT_EQUAL_INT32(INT32_MAX - 1, average2(INT32_MAX, INT32_MAX - 2));
    TEST_ASSERT_EQUAL_INT32(INT32_MIN, average2(INT32_MIN, INT32_MIN));
    TEST_ASSERT_EQUAL_INT32(0, average2(INT32_MIN + 1, INT32_MAX));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_sum_array);
    RUN_TEST(test_sum_array_empty);
    RUN_TEST(test_count_positive);
    RUN_TEST(test_count_positive_none);
    RUN_TEST(test_average2_small);
    RUN_TEST(test_average2_extremes);
    return UNITY_END();
}
```

- [ ] **Step 5: Write `README.md`**

````markdown
# Phase 0 / ex02_bug_hunt — 道具でバグを捕まえる

## 目的
`src/stats.c` には **3つのバグ** が入っています。目で探す前に、道具に見つけさせる練習をします。
仕様は `src/stats.h` です。テスト（`test/`）は変更しないでください。

## 手順（この順で）
1. `make test` — 通るかもしれないし、落ちるかもしれません。結果が安定しないこと自体がヒントです。
2. `make test OPT=-O2` — 最適化を有効にすると、コンパイラが追加の警告を出します。1つ目を直す。
3. `make asan` — AddressSanitizer / UBSan のレポートを読み、ファイル名と行番号から原因を特定して直す。
   直したら再度 `make asan`。別のレポートが出たらそれも直す。
4. 最終確認:
   ```bash
   make test && make test OPT=-O2 && make asan && make valgrind
   ```
5. （任意）gdb で `average2` にブレークポイントを置き、`print a + b` を評価してみる。

## ルール
- 修正は最小限に。関数の仕様（`stats.h`）は変えない。
- `average2` は `a + b` がオーバーフローするケースも正しく計算すること。

## 説明できるようになるべきこと
- なぜ `-O0` では出ない警告が `-O2` で出るのか
- 未初期化変数・範囲外アクセス・符号付きオーバーフローは、なぜ「たまたま動く」ことがあるのか
- ASan / UBSan / valgrind / コンパイラ警告は、それぞれどのバグに強いか
````

- [ ] **Step 6: Verify**

```bash
EX=phase0_env/ex02_bug_hunt
make -C $EX clean && make -C $EX test OPT=-O2; echo "rc=$?"   # Expected: compile error '-Werror=maybe-uninitialized' on count
make -C $EX asan; echo "rc=$?"                                # Expected: sanitizer report (stack-buffer-overflow in sum_array), rc != 0
```
Then run the reference check with a fixed `stats.c` (loop `i < n`, `count = 0`, 64-bit intermediate in `average2`); also run `make test OPT=-O2` on the reference. Expected: all `0 Failures`.

- [ ] **Step 7: Commit**

```bash
git add phase0_env/ex02_bug_hunt
git commit -m "feat: add phase0 ex02_bug_hunt"
```

---

### Task 3: Phase 1 ex01_bitops

**Files:**
- Create: `phase1_lang/ex01_bitops/{Makefile,README.md,src/bitops.h,src/bitops.c,test/test_bitops.c}`

**Interfaces:**
- Consumes: `common/common.mk`
- Produces: `bit_set`, `bit_clear`, `bit_toggle`, `bit_test`, `bit_mask`, `field_get`, `field_set` (signatures below)

- [ ] **Step 1: Write `Makefile`**

```make
SRCS  := src/bitops.c
TESTS := test/test_bitops.c
include ../../common/common.mk
```

- [ ] **Step 2: Write `src/bitops.h`**

```c
#ifndef BITOPS_H
#define BITOPS_H

#include <stdbool.h>
#include <stdint.h>

/* All functions return a new value; nothing is modified in place.
 * Preconditions (not checked):
 *   bit   : 0..31
 *   width : 1..32 and shift + width <= 32
 */

uint32_t bit_set(uint32_t reg, unsigned bit);
uint32_t bit_clear(uint32_t reg, unsigned bit);
uint32_t bit_toggle(uint32_t reg, unsigned bit);
bool bit_test(uint32_t reg, unsigned bit);

/* Mask with `width` consecutive 1 bits starting at `shift`.
 * e.g. bit_mask(4, 3) == 0x70 */
uint32_t bit_mask(unsigned shift, unsigned width);

/* Extracts the field. e.g. field_get(0xABCD, 4, 8) == 0xBC */
uint32_t field_get(uint32_t reg, unsigned shift, unsigned width);

/* Returns reg with the field replaced by value.
 * Bits of value above `width` are ignored. e.g. field_set(0xABCD, 4, 8, 0x12) == 0xA12D */
uint32_t field_set(uint32_t reg, unsigned shift, unsigned width, uint32_t value);

#endif
```

- [ ] **Step 3: Write `src/bitops.c` (stub)**

```c
#include "bitops.h"

uint32_t bit_set(uint32_t reg, unsigned bit)
{
    /* TODO: implement */
    (void)reg;
    (void)bit;
    return 0;
}

uint32_t bit_clear(uint32_t reg, unsigned bit)
{
    /* TODO: implement */
    (void)reg;
    (void)bit;
    return 0;
}

uint32_t bit_toggle(uint32_t reg, unsigned bit)
{
    /* TODO: implement */
    (void)reg;
    (void)bit;
    return 0;
}

bool bit_test(uint32_t reg, unsigned bit)
{
    /* TODO: implement */
    (void)reg;
    (void)bit;
    return false;
}

uint32_t bit_mask(unsigned shift, unsigned width)
{
    /* TODO: implement */
    (void)shift;
    (void)width;
    return 0;
}

uint32_t field_get(uint32_t reg, unsigned shift, unsigned width)
{
    /* TODO: implement */
    (void)reg;
    (void)shift;
    (void)width;
    return 0;
}

uint32_t field_set(uint32_t reg, unsigned shift, unsigned width, uint32_t value)
{
    /* TODO: implement */
    (void)reg;
    (void)shift;
    (void)width;
    (void)value;
    return 0;
}
```

- [ ] **Step 4: Write `test/test_bitops.c`**

```c
#include <stdint.h>

#include "bitops.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

static void test_bit_set(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x00000001u, bit_set(0u, 0));
    TEST_ASSERT_EQUAL_HEX32(0x80000000u, bit_set(0u, 31));
    TEST_ASSERT_EQUAL_HEX32(0x0000000Fu, bit_set(0x0000000Fu, 2));
}

static void test_bit_clear(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x7FFFFFFFu, bit_clear(0xFFFFFFFFu, 31));
    TEST_ASSERT_EQUAL_HEX32(0x000000F7u, bit_clear(0x000000FFu, 3));
    TEST_ASSERT_EQUAL_HEX32(0x00000000u, bit_clear(0u, 5));
}

static void test_bit_toggle(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x0000000Eu, bit_toggle(0x0000000Fu, 0));
    TEST_ASSERT_EQUAL_HEX32(0x0000001Fu, bit_toggle(0x0000000Fu, 4));
    TEST_ASSERT_EQUAL_HEX32(0x12345678u, bit_toggle(bit_toggle(0x12345678u, 31), 31));
}

static void test_bit_test(void)
{
    TEST_ASSERT_TRUE(bit_test(0x80000000u, 31));
    TEST_ASSERT_FALSE(bit_test(0x80000000u, 30));
    TEST_ASSERT_TRUE(bit_test(0x00000001u, 0));
}

static void test_bit_mask(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x00000070u, bit_mask(4, 3));
    TEST_ASSERT_EQUAL_HEX32(0x00000001u, bit_mask(0, 1));
    TEST_ASSERT_EQUAL_HEX32(0x80000000u, bit_mask(31, 1));
    TEST_ASSERT_EQUAL_HEX32(0xFFFF0000u, bit_mask(16, 16));
}

static void test_bit_mask_full_width(void)
{
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFFu, bit_mask(0, 32));
}

static void test_field_get(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x000000BCu, field_get(0x0000ABCDu, 4, 8));
    TEST_ASSERT_EQUAL_HEX32(0x00000001u, field_get(0x80000000u, 31, 1));
    TEST_ASSERT_EQUAL_HEX32(0xDEADBEEFu, field_get(0xDEADBEEFu, 0, 32));
}

static void test_field_set(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x0000A12Du, field_set(0x0000ABCDu, 4, 8, 0x12u));
    TEST_ASSERT_EQUAL_HEX32(0xA0000000u, field_set(0x00000000u, 28, 4, 0xAu));
    TEST_ASSERT_EQUAL_HEX32(0x00000000u, field_set(0xFFFFFFFFu, 0, 32, 0u));
}

static void test_field_set_ignores_extra_value_bits(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x000000F0u, field_set(0x00000000u, 4, 4, 0xFFu));
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFF0Fu, field_set(0xFFFFFFFFu, 4, 4, 0x10u));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_bit_set);
    RUN_TEST(test_bit_clear);
    RUN_TEST(test_bit_toggle);
    RUN_TEST(test_bit_test);
    RUN_TEST(test_bit_mask);
    RUN_TEST(test_bit_mask_full_width);
    RUN_TEST(test_field_get);
    RUN_TEST(test_field_set);
    RUN_TEST(test_field_set_ignores_extra_value_bits);
    return UNITY_END();
}
```

- [ ] **Step 5: Write `README.md`**

````markdown
# Phase 1 / ex01_bitops — レジスタ操作のためのビット演算

## 目的
マイコンや SoC のレジスタは「32bit の中の特定ビット・特定フィールド」を読み書きして操作します。
その基本部品を関数として実装します。仕様は `src/bitops.h`。

## 手順
1. `make test` で失敗を確認
2. `src/bitops.c` を実装。**`make test` だけでなく必ず `make asan` も通すこと**
3. `make test && make asan`

## ヒント
- `1 << 31` と `1u << 31` は何が違う？
- `bit_mask(0, 32)` を素直に書くと何が起きる？（`make asan` が教えてくれます）

## 発展課題（任意）
- 同じ操作を関数形式マクロ（例: `BIT_SET(reg, bit)`）でも書いてみて、関数版との違い（型・副作用・デバッグしやすさ）を比較する
- `static inline` 関数をヘッダに置く方式との違いを説明する

## 説明できるようになるべきこと
- 符号付き整数でビット演算をすると何が危ないか
- シフト量が型の幅以上のとき、なぜ未定義動作になるのか
- read-modify-write とは何か、割り込みがあると何が問題になりうるか
````

- [ ] **Step 6: Verify** — Standard verification procedure (reference must handle `width == 32` without UB).

- [ ] **Step 7: Commit**

```bash
git add phase1_lang/ex01_bitops
git commit -m "feat: add phase1 ex01_bitops"
```

---

### Task 4: Phase 1 ex02_endian

**Files:**
- Create: `phase1_lang/ex02_endian/{Makefile,README.md,src/byteorder.h,src/byteorder.c,src/record.h,src/record.c,test/test_byteorder.c,test/test_record.c,test/test_main.c}`

Note: the header is `byteorder.h`, not `endian.h`, to avoid clashing with the system `<endian.h>`.

**Interfaces:**
- Consumes: `common/common.mk`
- Produces (byteorder.h): `bool is_little_endian(void);`, `void store_be16(uint8_t *dst, uint16_t v);`, `uint16_t load_be16(const uint8_t *src);`, `void store_be32(uint8_t *dst, uint32_t v);`, `uint32_t load_be32(const uint8_t *src);`, and `store_le16` / `load_le16` / `store_le32` / `load_le32` with the same shapes.
- Produces (record.h): `sensor_record_t`, `SENSOR_RECORD_WIRE_SIZE`, `size_t record_serialize(const sensor_record_t *rec, uint8_t *buf, size_t buf_len);`, `bool record_deserialize(const uint8_t *buf, size_t buf_len, sensor_record_t *out);`
- Test layout: `test_byteorder.c` and `test_record.c` each export `void run_byteorder_tests(void);` / `void run_record_tests(void);`; `test_main.c` owns `setUp`, `tearDown`, `main`.

- [ ] **Step 1: Write `Makefile`**

```make
SRCS  := src/byteorder.c src/record.c
TESTS := test/test_main.c test/test_byteorder.c test/test_record.c
include ../../common/common.mk
```

- [ ] **Step 2: Write `src/byteorder.h`**

```c
#ifndef BYTEORDER_H
#define BYTEORDER_H

#include <stdbool.h>
#include <stdint.h>

/* Returns true if this CPU stores multi-byte integers little-endian. */
bool is_little_endian(void);

/* store_*: write v into dst[0..N-1] in the given byte order.
 * load_*:  read a value from src[0..N-1] in the given byte order.
 * dst/src may have ANY alignment (e.g. buf + 1). */
void store_be16(uint8_t *dst, uint16_t v);
uint16_t load_be16(const uint8_t *src);
void store_be32(uint8_t *dst, uint32_t v);
uint32_t load_be32(const uint8_t *src);

void store_le16(uint8_t *dst, uint16_t v);
uint16_t load_le16(const uint8_t *src);
void store_le32(uint8_t *dst, uint32_t v);
uint32_t load_le32(const uint8_t *src);

#endif
```

- [ ] **Step 3: Write `src/byteorder.c` (stub)**

```c
#include "byteorder.h"

bool is_little_endian(void)
{
    /* TODO: implement */
    return false;
}

void store_be16(uint8_t *dst, uint16_t v)
{
    /* TODO: implement */
    (void)dst;
    (void)v;
}

uint16_t load_be16(const uint8_t *src)
{
    /* TODO: implement */
    (void)src;
    return 0;
}

void store_be32(uint8_t *dst, uint32_t v)
{
    /* TODO: implement */
    (void)dst;
    (void)v;
}

uint32_t load_be32(const uint8_t *src)
{
    /* TODO: implement */
    (void)src;
    return 0;
}

void store_le16(uint8_t *dst, uint16_t v)
{
    /* TODO: implement */
    (void)dst;
    (void)v;
}

uint16_t load_le16(const uint8_t *src)
{
    /* TODO: implement */
    (void)src;
    return 0;
}

void store_le32(uint8_t *dst, uint32_t v)
{
    /* TODO: implement */
    (void)dst;
    (void)v;
}

uint32_t load_le32(const uint8_t *src)
{
    /* TODO: implement */
    (void)src;
    return 0;
}
```

- [ ] **Step 4: Write `src/record.h`**

```c
#ifndef RECORD_H
#define RECORD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Wire format: big-endian, no padding, 7 bytes.
 *   [0]    id         uint8
 *   [1..2] temp_x10   int16 (two's complement)  e.g. -12.5 C -> -125 -> FF 83
 *   [3..6] timestamp  uint32
 */
#define SENSOR_RECORD_WIRE_SIZE 7u

typedef struct {
    uint8_t id;
    int16_t temp_x10;
    uint32_t timestamp;
} sensor_record_t;

/* Writes rec into buf. Returns SENSOR_RECORD_WIRE_SIZE on success,
 * or 0 if buf_len is too small (buf is left unchanged). */
size_t record_serialize(const sensor_record_t *rec, uint8_t *buf, size_t buf_len);

/* Reads a record from buf. Returns true on success,
 * or false if buf_len is too small (*out is left unchanged). */
bool record_deserialize(const uint8_t *buf, size_t buf_len, sensor_record_t *out);

#endif
```

- [ ] **Step 5: Write `src/record.c` (stub)**

```c
#include "record.h"

#include "byteorder.h"

size_t record_serialize(const sensor_record_t *rec, uint8_t *buf, size_t buf_len)
{
    /* TODO: implement (use store_be16 / store_be32) */
    (void)rec;
    (void)buf;
    (void)buf_len;
    return 0;
}

bool record_deserialize(const uint8_t *buf, size_t buf_len, sensor_record_t *out)
{
    /* TODO: implement (use load_be16 / load_be32) */
    (void)buf;
    (void)buf_len;
    (void)out;
    return false;
}
```

- [ ] **Step 6: Write `test/test_main.c`**

```c
#include "unity.h"

void run_byteorder_tests(void);
void run_record_tests(void);

void setUp(void) {}
void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    run_byteorder_tests();
    run_record_tests();
    return UNITY_END();
}
```

- [ ] **Step 7: Write `test/test_byteorder.c`**

```c
#include <stdint.h>

#include "byteorder.h"
#include "unity.h"

void run_byteorder_tests(void);

static void test_is_little_endian_matches_compiler(void)
{
    TEST_ASSERT_EQUAL(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, is_little_endian());
}

static void test_store_be16(void)
{
    uint8_t buf[2] = {0};
    const uint8_t expected[2] = {0x12, 0x34};
    store_be16(buf, 0x1234u);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, buf, 2);
}

static void test_store_be32(void)
{
    uint8_t buf[4] = {0};
    const uint8_t expected[4] = {0x12, 0x34, 0x56, 0x78};
    store_be32(buf, 0x12345678u);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, buf, 4);
}

static void test_store_le16(void)
{
    uint8_t buf[2] = {0};
    const uint8_t expected[2] = {0x34, 0x12};
    store_le16(buf, 0x1234u);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, buf, 2);
}

static void test_store_le32(void)
{
    uint8_t buf[4] = {0};
    const uint8_t expected[4] = {0x78, 0x56, 0x34, 0x12};
    store_le32(buf, 0x12345678u);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, buf, 4);
}

static void test_load(void)
{
    const uint8_t b[4] = {0x12, 0x34, 0x56, 0x78};
    TEST_ASSERT_EQUAL_HEX16(0x1234u, load_be16(b));
    TEST_ASSERT_EQUAL_HEX32(0x12345678u, load_be32(b));
    TEST_ASSERT_EQUAL_HEX16(0x3412u, load_le16(b));
    TEST_ASSERT_EQUAL_HEX32(0x78563412u, load_le32(b));
}

static void test_load_high_bytes(void)
{
    const uint8_t ff[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    TEST_ASSERT_EQUAL_HEX16(0xFFFFu, load_be16(ff));
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFFu, load_be32(ff));
    TEST_ASSERT_EQUAL_HEX16(0xFFFFu, load_le16(ff));
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFFu, load_le32(ff));
}

static void test_unaligned_access(void)
{
    uint8_t buf[8] = {0};
    const uint8_t expected[8] = {0x00, 0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x00, 0x00};
    store_be32(buf + 1, 0xDEADBEEFu);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, buf, 8);
    TEST_ASSERT_EQUAL_HEX32(0xDEADBEEFu, load_be32(buf + 1));
}

void run_byteorder_tests(void)
{
    RUN_TEST(test_is_little_endian_matches_compiler);
    RUN_TEST(test_store_be16);
    RUN_TEST(test_store_be32);
    RUN_TEST(test_store_le16);
    RUN_TEST(test_store_le32);
    RUN_TEST(test_load);
    RUN_TEST(test_load_high_bytes);
    RUN_TEST(test_unaligned_access);
}
```

- [ ] **Step 8: Write `test/test_record.c`**

```c
#include <stdint.h>
#include <string.h>

#include "record.h"
#include "unity.h"

void run_record_tests(void);

static void test_serialize_layout(void)
{
    const sensor_record_t rec = {.id = 0x01, .temp_x10 = -125, .timestamp = 0x5F5E1000u};
    const uint8_t expected[SENSOR_RECORD_WIRE_SIZE] = {0x01, 0xFF, 0x83, 0x5F, 0x5E, 0x10, 0x00};
    uint8_t buf[SENSOR_RECORD_WIRE_SIZE] = {0};

    TEST_ASSERT_EQUAL_size_t(SENSOR_RECORD_WIRE_SIZE, record_serialize(&rec, buf, sizeof buf));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, buf, SENSOR_RECORD_WIRE_SIZE);
}

static void test_serialize_buffer_too_small(void)
{
    const sensor_record_t rec = {.id = 0x01, .temp_x10 = 1, .timestamp = 2u};
    uint8_t buf[SENSOR_RECORD_WIRE_SIZE];
    uint8_t untouched[SENSOR_RECORD_WIRE_SIZE];
    memset(buf, 0xAA, sizeof buf);
    memset(untouched, 0xAA, sizeof untouched);

    TEST_ASSERT_EQUAL_size_t(0, record_serialize(&rec, buf, SENSOR_RECORD_WIRE_SIZE - 1));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(untouched, buf, SENSOR_RECORD_WIRE_SIZE);
}

static void test_deserialize(void)
{
    const uint8_t buf[SENSOR_RECORD_WIRE_SIZE] = {0x07, 0x01, 0x2C, 0x00, 0x00, 0x01, 0x00};
    sensor_record_t out = {0};

    TEST_ASSERT_TRUE(record_deserialize(buf, sizeof buf, &out));
    TEST_ASSERT_EQUAL_UINT8(0x07, out.id);
    TEST_ASSERT_EQUAL_INT16(300, out.temp_x10);
    TEST_ASSERT_EQUAL_UINT32(256u, out.timestamp);
}

static void test_deserialize_buffer_too_small(void)
{
    const uint8_t buf[SENSOR_RECORD_WIRE_SIZE] = {0};
    sensor_record_t out = {.id = 9, .temp_x10 = 9, .timestamp = 9u};

    TEST_ASSERT_FALSE(record_deserialize(buf, SENSOR_RECORD_WIRE_SIZE - 1, &out));
    TEST_ASSERT_EQUAL_UINT8(9, out.id);
    TEST_ASSERT_EQUAL_INT16(9, out.temp_x10);
    TEST_ASSERT_EQUAL_UINT32(9u, out.timestamp);
}

static void test_roundtrip_extremes(void)
{
    const sensor_record_t cases[] = {
        {.id = 0x00, .temp_x10 = INT16_MIN, .timestamp = 0u},
        {.id = 0xFF, .temp_x10 = INT16_MAX, .timestamp = UINT32_MAX},
        {.id = 0x42, .temp_x10 = -1, .timestamp = 0x80000000u},
    };
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        uint8_t buf[SENSOR_RECORD_WIRE_SIZE];
        sensor_record_t out = {0};
        TEST_ASSERT_EQUAL_size_t(SENSOR_RECORD_WIRE_SIZE, record_serialize(&cases[i], buf, sizeof buf));
        TEST_ASSERT_TRUE(record_deserialize(buf, sizeof buf, &out));
        TEST_ASSERT_EQUAL_UINT8(cases[i].id, out.id);
        TEST_ASSERT_EQUAL_INT16(cases[i].temp_x10, out.temp_x10);
        TEST_ASSERT_EQUAL_UINT32(cases[i].timestamp, out.timestamp);
    }
}

void run_record_tests(void)
{
    RUN_TEST(test_serialize_layout);
    RUN_TEST(test_serialize_buffer_too_small);
    RUN_TEST(test_deserialize);
    RUN_TEST(test_deserialize_buffer_too_small);
    RUN_TEST(test_roundtrip_extremes);
}
```

- [ ] **Step 9: Write `README.md`**

````markdown
# Phase 1 / ex02_endian — エンディアンとバイト列

## 目的
センサや通信プロトコルは「バイト列」でデータをやり取りします。
CPU のエンディアンやアラインメントに依存せず、整数・構造体とバイト列を相互変換できるようにします。

## 手順
1. `make test` で失敗を確認
2. `src/byteorder.c` を実装 → 次に `src/record.c` を実装（`byteorder.h` の関数を使う）
3. `make test && make asan`

## ルール
- `uint8_t *` を `uint32_t *` にキャストして読み書きしない（理由を説明できるように）
- `memcpy(buf, &rec, sizeof rec)` で構造体を丸ごとコピーしない

## ヒント
- `src[0] << 24` の `src[0]` は、シフトされる前に何型になっている？ 0xFF のとき何が起きる？
- `int16_t` の -125 を `uint16_t` にするとどうなる？ 逆は？（`-Wconversion` が明示を求めます）

## 考察（READMEの下に自分の答えをメモしておくと、レビューで扱います）
- `sizeof(sensor_record_t)` はいくつか。なぜ 7 ではないのか
- Raspberry Pi（ARM）と x86 PC は同じエンディアンか。それでも変換関数が必要なのはなぜか

## 説明できるようになるべきこと
- 整数昇格（integer promotion）と、それがシフト演算で問題になる理由
- アラインメント・パディングと、ワイヤフォーマットを明示的に定義する理由
- strict aliasing ルールの概要
````

- [ ] **Step 10: Verify** — Standard verification procedure (reference `load_be32` must cast bytes to `uint32_t` before shifting, so `make asan` passes `test_load_high_bytes`).

- [ ] **Step 11: Commit**

```bash
git add phase1_lang/ex02_endian
git commit -m "feat: add phase1 ex02_endian"
```

---

### Task 5: Phase 1 ex03_int_promotion

**Files:**
- Create: `phase1_lang/ex03_int_promotion/{Makefile,README.md,src/answers.h,src/answers.c,src/safe_arith.h,src/safe_arith.c,test/test_main.c,test/test_quiz.c,test/test_safe_arith.c}`

**Interfaces:**
- Consumes: `common/common.mk`
- Produces: `extern const long long answer_q1 .. answer_q8;`, `ANSWER_UNSET`; `bool safe_add_i32(int32_t a, int32_t b, int32_t *out);`, `bool safe_mul_i32(int32_t a, int32_t b, int32_t *out);`, `bool safe_add_u32(uint32_t a, uint32_t b, uint32_t *out);`
- Test layout: `run_quiz_tests()`, `run_safe_arith_tests()`, `test_main.c` owns `setUp`/`tearDown`/`main`.

- [ ] **Step 1: Write `Makefile`**

```make
SRCS  := src/answers.c src/safe_arith.c
TESTS := test/test_main.c test/test_quiz.c test/test_safe_arith.c
include ../../common/common.mk
```

- [ ] **Step 2: Write `src/answers.h`**

```c
#ifndef ANSWERS_H
#define ANSWERS_H

/* Sentinel meaning "not answered yet". */
#define ANSWER_UNSET (-999999LL)

extern const long long answer_q1;
extern const long long answer_q2;
extern const long long answer_q3;
extern const long long answer_q4;
extern const long long answer_q5;
extern const long long answer_q6;
extern const long long answer_q7;
extern const long long answer_q8;

#endif
```

- [ ] **Step 3: Write `src/answers.c` (learner fills in)**

```c
#include "answers.h"

/* Write your prediction for each question in README.md BEFORE running the tests.
 * Each answer is the value of the expression converted to long long
 * (true = 1, false = 0). */

const long long answer_q1 = ANSWER_UNSET; /* -1 < 0u */
const long long answer_q2 = ANSWER_UNSET; /* (uint8_t)200 + (uint8_t)100 */
const long long answer_q3 = ANSWER_UNSET; /* (uint8_t)((uint8_t)200 + (uint8_t)100) */
const long long answer_q4 = ANSWER_UNSET; /* ~(uint8_t)0x0F */
const long long answer_q5 = ANSWER_UNSET; /* (int8_t)0x80 */
const long long answer_q6 = ANSWER_UNSET; /* 5 / -2 */
const long long answer_q7 = ANSWER_UNSET; /* -5 % 3 */
const long long answer_q8 = ANSWER_UNSET; /* (-7) >> 1 */
```

- [ ] **Step 4: Write `src/safe_arith.h`**

```c
#ifndef SAFE_ARITH_H
#define SAFE_ARITH_H

#include <stdbool.h>
#include <stdint.h>

/* On success: store the exact result in *out and return true.
 * On overflow: return false and leave *out unchanged.
 *
 * Rules: check the operands BEFORE operating. Do not compute in a wider
 * type (int64_t etc.) and do not use __builtin_*_overflow. */
bool safe_add_i32(int32_t a, int32_t b, int32_t *out);
bool safe_mul_i32(int32_t a, int32_t b, int32_t *out);
bool safe_add_u32(uint32_t a, uint32_t b, uint32_t *out);

#endif
```

- [ ] **Step 5: Write `src/safe_arith.c` (stub)**

```c
#include "safe_arith.h"

bool safe_add_i32(int32_t a, int32_t b, int32_t *out)
{
    /* TODO: implement */
    (void)a;
    (void)b;
    (void)out;
    return false;
}

bool safe_mul_i32(int32_t a, int32_t b, int32_t *out)
{
    /* TODO: implement */
    (void)a;
    (void)b;
    (void)out;
    return false;
}

bool safe_add_u32(uint32_t a, uint32_t b, uint32_t *out)
{
    /* TODO: implement */
    (void)a;
    (void)b;
    (void)out;
    return false;
}
```

- [ ] **Step 6: Write `test/test_main.c`**

```c
#include "unity.h"

void run_quiz_tests(void);
void run_safe_arith_tests(void);

void setUp(void) {}
void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    run_quiz_tests();
    run_safe_arith_tests();
    return UNITY_END();
}
```

- [ ] **Step 7: Write `test/test_quiz.c`**

```c
#include <stdint.h>

#include "answers.h"
#include "unity.h"

void run_quiz_tests(void);

/* Expected values are computed by the compiler, not hard-coded. */
static void test_q1(void) { TEST_ASSERT_EQUAL_INT64((long long)(-1 < 0u), answer_q1); }
static void test_q2(void) { TEST_ASSERT_EQUAL_INT64((long long)((uint8_t)200 + (uint8_t)100), answer_q2); }
static void test_q3(void) { TEST_ASSERT_EQUAL_INT64((long long)(uint8_t)((uint8_t)200 + (uint8_t)100), answer_q3); }
static void test_q4(void) { TEST_ASSERT_EQUAL_INT64((long long)(~(uint8_t)0x0F), answer_q4); }
static void test_q5(void) { TEST_ASSERT_EQUAL_INT64((long long)(int8_t)0x80, answer_q5); }
static void test_q6(void) { TEST_ASSERT_EQUAL_INT64((long long)(5 / -2), answer_q6); }
static void test_q7(void) { TEST_ASSERT_EQUAL_INT64((long long)(-5 % 3), answer_q7); }
static void test_q8(void) { TEST_ASSERT_EQUAL_INT64((long long)((-7) >> 1), answer_q8); }

void run_quiz_tests(void)
{
    RUN_TEST(test_q1);
    RUN_TEST(test_q2);
    RUN_TEST(test_q3);
    RUN_TEST(test_q4);
    RUN_TEST(test_q5);
    RUN_TEST(test_q6);
    RUN_TEST(test_q7);
    RUN_TEST(test_q8);
}
```

- [ ] **Step 8: Write `test/test_safe_arith.c`**

```c
#include <stdint.h>

#include "safe_arith.h"
#include "unity.h"

void run_safe_arith_tests(void);

#define SENTINEL_I32 12345
#define SENTINEL_U32 12345u

static void assert_add_i32_ok(int32_t a, int32_t b, int32_t expected)
{
    int32_t out = SENTINEL_I32;
    TEST_ASSERT_TRUE(safe_add_i32(a, b, &out));
    TEST_ASSERT_EQUAL_INT32(expected, out);
}

static void assert_add_i32_overflow(int32_t a, int32_t b)
{
    int32_t out = SENTINEL_I32;
    TEST_ASSERT_FALSE(safe_add_i32(a, b, &out));
    TEST_ASSERT_EQUAL_INT32(SENTINEL_I32, out);
}

static void assert_mul_i32_ok(int32_t a, int32_t b, int32_t expected)
{
    int32_t out = SENTINEL_I32;
    TEST_ASSERT_TRUE(safe_mul_i32(a, b, &out));
    TEST_ASSERT_EQUAL_INT32(expected, out);
}

static void assert_mul_i32_overflow(int32_t a, int32_t b)
{
    int32_t out = SENTINEL_I32;
    TEST_ASSERT_FALSE(safe_mul_i32(a, b, &out));
    TEST_ASSERT_EQUAL_INT32(SENTINEL_I32, out);
}

static void test_add_i32(void)
{
    assert_add_i32_ok(1, 2, 3);
    assert_add_i32_ok(INT32_MAX, INT32_MIN, -1);
    assert_add_i32_ok(INT32_MIN, 0, INT32_MIN);
    assert_add_i32_ok(INT32_MAX - 1, 1, INT32_MAX);
}

static void test_add_i32_overflow(void)
{
    assert_add_i32_overflow(INT32_MAX, 1);
    assert_add_i32_overflow(INT32_MIN, -1);
    assert_add_i32_overflow(INT32_MAX, INT32_MAX);
    assert_add_i32_overflow(INT32_MIN, INT32_MIN);
}

static void test_mul_i32(void)
{
    assert_mul_i32_ok(6, 7, 42);
    assert_mul_i32_ok(-6, 7, -42);
    assert_mul_i32_ok(0, INT32_MIN, 0);
    assert_mul_i32_ok(INT32_MIN, 1, INT32_MIN);
    assert_mul_i32_ok(46340, 46340, 2147395600);
    assert_mul_i32_ok(-1073741824, 2, INT32_MIN);
    assert_mul_i32_ok(-1, INT32_MAX, -INT32_MAX);
}

static void test_mul_i32_overflow(void)
{
    assert_mul_i32_overflow(INT32_MAX, 2);
    assert_mul_i32_overflow(INT32_MIN, -1);
    assert_mul_i32_overflow(-1, INT32_MIN);
    assert_mul_i32_overflow(46341, 46341);
    assert_mul_i32_overflow(-46341, 46341);
    assert_mul_i32_overflow(INT32_MIN, 2);
}

static void test_add_u32(void)
{
    uint32_t out = SENTINEL_U32;
    TEST_ASSERT_TRUE(safe_add_u32(UINT32_MAX, 0u, &out));
    TEST_ASSERT_EQUAL_UINT32(UINT32_MAX, out);

    out = SENTINEL_U32;
    TEST_ASSERT_FALSE(safe_add_u32(UINT32_MAX, 1u, &out));
    TEST_ASSERT_EQUAL_UINT32(SENTINEL_U32, out);

    out = SENTINEL_U32;
    TEST_ASSERT_FALSE(safe_add_u32(0x80000000u, 0x80000000u, &out));
    TEST_ASSERT_EQUAL_UINT32(SENTINEL_U32, out);
}

void run_safe_arith_tests(void)
{
    RUN_TEST(test_add_i32);
    RUN_TEST(test_add_i32_overflow);
    RUN_TEST(test_mul_i32);
    RUN_TEST(test_mul_i32_overflow);
    RUN_TEST(test_add_u32);
}
```

- [ ] **Step 9: Write `README.md`**

````markdown
# Phase 1 / ex03_int_promotion — 整数の型変換と安全な算術

C のバグの多くは「整数が思った型・値になっていない」ことから生まれます。
前提: gcc、`int` は 32bit（x86_64 PC と 64bit Raspberry Pi OS の両方で成り立ちます）。

## Part 1: 予想クイズ
**テストを実行する前に**、各式の値を予想して `src/answers.c` に書いてください（真は 1、偽は 0）。

| Q | 式 |
|---|---|
| 1 | `-1 < 0u` |
| 2 | `(uint8_t)200 + (uint8_t)100` |
| 3 | `(uint8_t)((uint8_t)200 + (uint8_t)100)` |
| 4 | `~(uint8_t)0x0F` |
| 5 | `(int8_t)0x80` |
| 6 | `5 / -2` |
| 7 | `-5 % 3` |
| 8 | `(-7) >> 1` |

`make test` で答え合わせをします。外れた問題は「なぜその値になるか」を規格の用語（整数昇格、通常の算術型変換など）で説明できるまで調べてください。
Q5 と Q8 は規格上「処理系定義」です。「処理系定義」「未規定」「未定義」の違いも調べましょう。

### 考察問題（テストなし、レビューで扱います）
- `(uint16_t)65535 * (uint16_t)65535` は何が起きるか（ヒント: 符号なし同士の掛け算なのに…）
- `for (uint8_t i = 0; i < 256; i++)` はなぜ終わらないか
- `size_t n = strlen(s); for (int i = n - 1; i >= 0; i--)` の潜在的な問題は何か

## Part 2: オーバーフローしない算術
`src/safe_arith.c` を実装します。仕様とルールは `src/safe_arith.h` を参照してください（64bit 型での計算と、コンパイラ組み込み関数の使用は禁止）。

## 手順
1. `src/answers.c` に予想を書く → `make test`
2. `src/safe_arith.c` を実装 → `make test && make asan`

## 説明できるようになるべきこと
- 整数昇格と通常の算術型変換のルール
- 符号付きのオーバーフローは未定義動作だが、符号なしは wrap-around として定義されていること
- オーバーフローを「計算した後」ではなく「計算する前」に検出しなければならない理由
````

- [ ] **Step 10: Verify** — Standard verification procedure; the reference `answers.c` uses the correct values (0, 300, 44, -16, -128, -2, -2, -4) and the reference `safe_arith.c` checks operands before operating. `make asan` must pass on the reference, which proves it has no signed-overflow UB.

- [ ] **Step 11: Commit**

```bash
git add phase1_lang/ex03_int_promotion
git commit -m "feat: add phase1 ex03_int_promotion"
```

---

### Task 6: Phase 1 ex04_cmd_table

**Files:**
- Create: `phase1_lang/ex04_cmd_table/{Makefile,README.md,src/cmd.h,src/cmd.c,test/test_cmd.c}`

**Interfaces:**
- Consumes: `common/common.mk`
- Produces: `CMD_MAX_ARGS`, `cmd_status_t`, `cmd_handler_t`, `cmd_entry_t`, `int cmd_dispatch(const cmd_entry_t *table, size_t table_len, char *line, char *out, size_t out_len);`

- [ ] **Step 1: Write `Makefile`**

```make
SRCS  := src/cmd.c
TESTS := test/test_cmd.c
include ../../common/common.mk
```

- [ ] **Step 2: Write `src/cmd.h`**

```c
#ifndef CMD_H
#define CMD_H

#include <stddef.h>

#define CMD_MAX_ARGS 8

typedef enum {
    CMD_OK = 0,
    CMD_ERR_EMPTY = -1,         /* line contains no tokens */
    CMD_ERR_UNKNOWN = -2,       /* argv[0] is not in the table */
    CMD_ERR_TOO_MANY_ARGS = -3, /* more than CMD_MAX_ARGS tokens */
} cmd_status_t;

/* A command handler. argv[argc] is NULL.
 * out/out_len is a buffer the handler may write a response into. */
typedef int (*cmd_handler_t)(int argc, char *argv[], char *out, size_t out_len);

typedef struct {
    const char *name;
    cmd_handler_t handler;
} cmd_entry_t;

/* Splits `line` in place on runs of ' ' characters, finds the entry whose
 * name exactly equals argv[0], and calls its handler with out/out_len.
 * Returns the handler's return value, or a negative cmd_status_t on error
 * (the handler is not called in that case). No dynamic allocation. */
int cmd_dispatch(const cmd_entry_t *table, size_t table_len, char *line, char *out, size_t out_len);

#endif
```

- [ ] **Step 3: Write `src/cmd.c` (stub)**

```c
#include "cmd.h"

int cmd_dispatch(const cmd_entry_t *table, size_t table_len, char *line, char *out, size_t out_len)
{
    /* TODO: implement */
    (void)table;
    (void)table_len;
    (void)line;
    (void)out;
    (void)out_len;
    return CMD_OK;
}
```

- [ ] **Step 4: Write `test/test_cmd.c`**

```c
#include <stdbool.h>
#include <stdio.h>

#include "cmd.h"
#include "unity.h"

/* Spy: records how it was called. */
static int spy_calls;
static int spy_argc;
static char spy_args[CMD_MAX_ARGS][32];
static bool spy_argv_null_terminated;
static char *spy_out;
static size_t spy_out_len;

static int spy_handler(int argc, char *argv[], char *out, size_t out_len)
{
    spy_calls++;
    spy_argc = argc;
    for (int i = 0; i < argc && i < CMD_MAX_ARGS; i++) {
        snprintf(spy_args[i], sizeof spy_args[i], "%s", argv[i]);
    }
    spy_argv_null_terminated = (argv[argc] == NULL);
    spy_out = out;
    spy_out_len = out_len;
    return 42;
}

static int other_handler(int argc, char *argv[], char *out, size_t out_len)
{
    (void)argc;
    (void)argv;
    (void)out;
    (void)out_len;
    return 7;
}

static const cmd_entry_t table[] = {
    {"led", spy_handler},
    {"reset", other_handler},
};
#define TABLE_LEN (sizeof table / sizeof table[0])

void setUp(void)
{
    spy_calls = 0;
    spy_argc = -1;
    spy_argv_null_terminated = false;
    spy_out = NULL;
    spy_out_len = 0;
}

void tearDown(void) {}

static void test_dispatch_calls_handler_with_args(void)
{
    char line[] = "led on 3";
    char out[16];

    TEST_ASSERT_EQUAL_INT(42, cmd_dispatch(table, TABLE_LEN, line, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(1, spy_calls);
    TEST_ASSERT_EQUAL_INT(3, spy_argc);
    TEST_ASSERT_EQUAL_STRING("led", spy_args[0]);
    TEST_ASSERT_EQUAL_STRING("on", spy_args[1]);
    TEST_ASSERT_EQUAL_STRING("3", spy_args[2]);
    TEST_ASSERT_TRUE(spy_argv_null_terminated);
    TEST_ASSERT_EQUAL_PTR(out, spy_out);
    TEST_ASSERT_EQUAL_size_t(sizeof out, spy_out_len);
}

static void test_dispatch_selects_matching_entry(void)
{
    char line[] = "reset";
    char out[16];

    TEST_ASSERT_EQUAL_INT(7, cmd_dispatch(table, TABLE_LEN, line, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(0, spy_calls);
}

static void test_dispatch_collapses_repeated_spaces(void)
{
    char line[] = "  led   on  ";
    char out[16];

    TEST_ASSERT_EQUAL_INT(42, cmd_dispatch(table, TABLE_LEN, line, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(2, spy_argc);
    TEST_ASSERT_EQUAL_STRING("led", spy_args[0]);
    TEST_ASSERT_EQUAL_STRING("on", spy_args[1]);
}

static void test_dispatch_empty_line(void)
{
    char empty[] = "";
    char spaces[] = "   ";
    char out[16];

    TEST_ASSERT_EQUAL_INT(CMD_ERR_EMPTY, cmd_dispatch(table, TABLE_LEN, empty, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(CMD_ERR_EMPTY, cmd_dispatch(table, TABLE_LEN, spaces, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(0, spy_calls);
}

static void test_dispatch_unknown_command(void)
{
    char unknown[] = "blink 1";
    char prefix[] = "le";
    char longer[] = "ledx";
    char out[16];

    TEST_ASSERT_EQUAL_INT(CMD_ERR_UNKNOWN, cmd_dispatch(table, TABLE_LEN, unknown, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(CMD_ERR_UNKNOWN, cmd_dispatch(table, TABLE_LEN, prefix, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(CMD_ERR_UNKNOWN, cmd_dispatch(table, TABLE_LEN, longer, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(0, spy_calls);
}

static void test_dispatch_empty_table(void)
{
    char line[] = "led";
    char out[16];

    TEST_ASSERT_EQUAL_INT(CMD_ERR_UNKNOWN, cmd_dispatch(table, 0, line, out, sizeof out));
}

static void test_dispatch_max_args(void)
{
    char ok[] = "led 1 2 3 4 5 6 7";
    char too_many[] = "led 1 2 3 4 5 6 7 8";
    char out[16];

    TEST_ASSERT_EQUAL_INT(42, cmd_dispatch(table, TABLE_LEN, ok, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(CMD_MAX_ARGS, spy_argc);
    TEST_ASSERT_EQUAL_STRING("7", spy_args[CMD_MAX_ARGS - 1]);

    spy_calls = 0;
    TEST_ASSERT_EQUAL_INT(CMD_ERR_TOO_MANY_ARGS, cmd_dispatch(table, TABLE_LEN, too_many, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(0, spy_calls);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_dispatch_calls_handler_with_args);
    RUN_TEST(test_dispatch_selects_matching_entry);
    RUN_TEST(test_dispatch_collapses_repeated_spaces);
    RUN_TEST(test_dispatch_empty_line);
    RUN_TEST(test_dispatch_unknown_command);
    RUN_TEST(test_dispatch_empty_table);
    RUN_TEST(test_dispatch_max_args);
    return UNITY_END();
}
```

- [ ] **Step 5: Write `README.md`**

````markdown
# Phase 1 / ex04_cmd_table — 関数ポインタでコマンドを振り分ける

## 目的
組込み機器では、UART などから届いた `"led on 3"` のような文字列コマンドを解釈して処理を呼び分けることがよくあります。
`switch` や `if` を連ねる代わりに「名前と関数ポインタの表」を使うと、コマンド追加が表への1行追加で済みます。
仕様は `src/cmd.h`。

## 手順
1. `make test` で失敗を確認
2. `src/cmd.c` の `cmd_dispatch` を実装
3. `make test && make asan`

## ルール
- `malloc` を使わない（引数配列はスタック上の `char *argv[CMD_MAX_ARGS + 1]` で足ります）
- `strtok` は使わずに自分で分割してみる（使った場合は、なぜ組込みで嫌われるかを説明できること）

## テストについて
`test/test_cmd.c` の `spy_handler` は「どう呼ばれたかを記録する偽物のハンドラ」（テストダブルの一種、spy）です。
Phase 2 ではこの考え方で、ハードウェアを偽物に差し替えてテストします。

## 発展課題（任意）
- テーブルに `help` 文字列を追加し、全コマンドの一覧を `out` に書く `help` ハンドラを作る（`snprintf` で `out_len` を守ること）
- テーブルを `static const` にすると、マイコンではどのメモリ領域に置かれるか調べる

## 説明できるようになるべきこと
- 関数ポインタ型の宣言の読み方（`typedef` なし版も書けるか）
- `char line[] = "..."` と `char *line = "..."` の違い（なぜ前者でないとこの関数に渡せないか）
- `const` をどこに付けると何が変わらなくなるのか
````

- [ ] **Step 6: Verify** — Standard verification procedure.

- [ ] **Step 7: Commit**

```bash
git add phase1_lang/ex04_cmd_table
git commit -m "feat: add phase1 ex04_cmd_table"
```

---

### Task 7: ROADMAP.md and top-level README.md

**Files:**
- Create: `ROADMAP.md`, `README.md`

**Interfaces:**
- Consumes: exercise directory names from Tasks 1–6.

- [ ] **Step 1: Write `ROADMAP.md`**

````markdown
# C言語 習熟ロードマップ（組込み / Raspberry Pi）

- 想定: 週3〜5時間 × 約6か月（合計約100時間）
- 進め方: 各フェーズの小課題で部品と知識を積み上げ、Phase 6 の卒業制作で Raspberry Pi 上に統合する
- 各課題は `make test && make asan` が通ったら、Claude に「phaseX/exY をレビューして」と依頼する

## 全体像

| Phase | 目安 | テーマ | 到達目標 |
|---|---|---|---|
| 0 | 1週 | 開発環境 | 厳格な警告・サニタイザ・デバッガ・単体テストを日常的に使える |
| 1 | 5週 | 言語の深部 | 未定義動作・整数昇格・ポインタ・ビット演算・メモリレイアウトを説明できる |
| 2 | 5週 | 組込み向け設計 | 動的確保なしで、テスト可能なモジュールを設計できる |
| 3 | 4週 | Raspberry Pi 実機 | 実機で GPIO / I2C を C から操作できる |
| 4 | 5週 | 並行・時間 | スレッド・シグナル・イベント待ちを安全に扱える |
| 5 | 3週 | 品質 | 規約と静的解析でコード品質を評価・改善できる |
| 6 | 3週〜 | 卒業制作 | 部品を統合した実機アプリを完成させる |

## Phase 0: 開発環境（1週）
- [ ] ex01_toolchain — gdb / valgrind の導入、`make test` / `make asan` / `make valgrind`、gdb の基本操作
- [ ] ex02_bug_hunt — 警告・ASan・UBSan で3つのバグを特定して直す

## Phase 1: 言語の深部（5週）
- [ ] ex01_bitops — ビット操作とフィールド抽出/挿入（レジスタ操作の基礎）
- [ ] ex02_endian — エンディアン変換とバイト列シリアライズ、パディング
- [ ] ex03_int_promotion — 整数昇格クイズ、オーバーフローしない算術
- [ ] ex04_cmd_table — 関数ポインタのテーブルによるコマンド振り分け
- 並行して読む: ポインタと配列の違い、`volatile` / `const`、記憶域クラス（`static` / `extern`）、未定義動作

## Phase 2: 組込み向け設計（5週）※課題は Phase 1 完了時に追加
- 静的メモリ設計（`malloc` を使わない）、リングバッファ
- 状態機械（switch 版 / テーブル版）
- 固定小数点演算
- 不透明型（opaque pointer）によるモジュール分割
- HAL 抽象化と TDD、テストダブル（spy / fake / mock）で LED / ボタンドライバを PC 上で開発

## Phase 3: Raspberry Pi 実機（4週）
- Pi 上でのネイティブビルド → PC からのクロスコンパイル
- GPIO: libgpiod と、`/dev/gpiomem` の mmap によるレジスタ直接操作の両方
- I2C（`/dev/i2c-*`）でセンサを読む、SPI / UART の概要
- Phase 2 の HAL の実機実装を作る

## Phase 4: 並行・時間（5週）
- シグナル、pthread、mutex / 条件変数、アトミック
- GPIO エッジイベント待ち（割り込みに相当）とスレッド間通知
- 周期処理、`clock_nanosleep`、ジッタ計測、リアルタイム性の限界

## Phase 5: 品質（3週）
- MISRA C / CERT C の考え方（主要ルールの理由を理解する）
- cppcheck / clang-tidy による静的解析
- gcov によるカバレッジ計測
- これまでのコードの改善

## Phase 6: 卒業制作（3週〜）
センサロガー: I2C センサ → リングバッファ → 状態機械 → ファイル / UART 出力。
HAL を差し替えて PC 上でもテストできる構成にする。

## その先（任意）
- Raspberry Pi Pico などでのベアメタル開発（スタートアップコード、リンカスクリプト、割り込みベクタ）
- FreeRTOS
- Linux カーネルモジュール / デバイスドライバ

## 副読本（任意）
- 『Effective C』 Robert C. Seacord
- 『Test-Driven Development for Embedded C』 James W. Grenning
- 『Making Embedded Systems』 Elecia White
- 『詳説 Cポインタ』 Richard Reese
````

- [ ] **Step 2: Write `README.md`**

````markdown
# c_master — C言語 習熟演習

組込み（Raspberry Pi）を目標に、C言語を「使える」レベルまで鍛えるための演習環境です。
学習の全体像は [ROADMAP.md](ROADMAP.md) を参照してください。

## 必要なもの
```bash
sudo apt install -y build-essential gdb valgrind
```

## 構成
```
common/          共通のビルド設定（common.mk）とテストフレームワーク Unity
phase0_env/      Phase 0 の課題
phase1_lang/     Phase 1 の課題
docs/            設計書・計画書
```
各課題ディレクトリは `README.md`（課題文）、`src/`（あなたが実装する）、`test/`（提供テスト）、`Makefile` からなります。

## 課題の進め方
1. 課題の `README.md` を読む
2. `make test` で、テストが失敗することを確認する
3. `src/` を実装する（`test/` は変更しない）
4. 以下がすべて通ったら完了
   ```bash
   make test
   make asan
   ```
5. Claude Code に「phase1_lang/ex01_bitops をレビューして」のように依頼する。README の「説明できるようになるべきこと」についても質問されます
6. `git commit` して、ROADMAP.md のチェックボックスを埋める

## make ターゲット
| コマンド | 内容 |
|---|---|
| `make test` | テストをビルドして実行 |
| `make test OPT=-O2` | 最適化を有効にしてビルド（追加の警告が出ることがある） |
| `make asan` | AddressSanitizer + UndefinedBehaviorSanitizer 付きで実行 |
| `make valgrind` | valgrind memcheck 上で実行 |
| `make clean` | ビルド成果物を削除 |

## ライセンス
`common/unity/` は [ThrowTheSwitch/Unity](https://github.com/ThrowTheSwitch/Unity) v2.6.1（MIT License）です。
````

- [ ] **Step 3: Verify all exercises build from a clean state**

```bash
for ex in phase0_env/ex01_toolchain phase1_lang/ex01_bitops phase1_lang/ex02_endian phase1_lang/ex03_int_promotion phase1_lang/ex04_cmd_table; do
  make -C $ex clean >/dev/null && make -C $ex test 2>&1 | grep -E "Tests .* Failures|error" ; done
git status --short   # Expected: only ROADMAP.md and README.md untracked; no build/ dirs
```
Expected: each exercise prints a Unity summary with ≥1 failure and no compile errors.

- [ ] **Step 4: Commit**

```bash
git add ROADMAP.md README.md
git commit -m "docs: add roadmap and top-level README"
```
