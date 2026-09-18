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
