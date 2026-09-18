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
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_is_little_endian_matches_compiler);
    RUN_TEST(test_store_be16);
    RUN_TEST(test_store_be32);
    RUN_TEST(test_store_le16);
    RUN_TEST(test_store_le32);
    RUN_TEST(test_load);
    RUN_TEST(test_load_high_bytes);
    RUN_TEST(test_unaligned_access);
}
