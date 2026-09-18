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
