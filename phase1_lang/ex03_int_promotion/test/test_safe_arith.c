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
