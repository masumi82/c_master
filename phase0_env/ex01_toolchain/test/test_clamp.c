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
