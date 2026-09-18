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
