// 提供するテスト。変更しないこと。
#include "unity.h"
#include "ring_buffer.hpp"

#include <cstdint>

void setUp(void) {}
void tearDown(void) {}

// 単純な構造体の要素型。C++ のクラスは int に限らず任意の T を持てる。
struct Sample {
    std::uint16_t adc;
    std::int8_t   temp;
};

// size()/empty()/full() が const メンバ関数でなければコンパイルできない。
static void check_via_const_ref(const RingBuffer<int, 4> &rb, std::size_t expected)
{
    TEST_ASSERT_EQUAL_size_t(expected, rb.size());
    TEST_ASSERT_EQUAL(expected == 0, rb.empty());
    TEST_ASSERT_EQUAL(expected == rb.capacity(), rb.full());
}

static void test_new_buffer_is_empty(void)
{
    RingBuffer<int, 4> rb;          // init() は呼ばない。コンストラクタが済ませている
    TEST_ASSERT_TRUE(rb.empty());
    TEST_ASSERT_FALSE(rb.full());
    TEST_ASSERT_EQUAL_size_t(0, rb.size());
    TEST_ASSERT_EQUAL_size_t(4, rb.capacity());
}

static void test_push_then_pop_fifo(void)
{
    RingBuffer<int, 4> rb;
    TEST_ASSERT_TRUE(rb.push(10));
    TEST_ASSERT_TRUE(rb.push(20));
    TEST_ASSERT_TRUE(rb.push(30));
    TEST_ASSERT_EQUAL_size_t(3, rb.size());
    TEST_ASSERT_FALSE(rb.full());       // 4 個中 3 個。まだ満杯ではない

    int v = 0;
    TEST_ASSERT_TRUE(rb.pop(v));
    TEST_ASSERT_EQUAL_INT(10, v);
    TEST_ASSERT_TRUE(rb.pop(v));
    TEST_ASSERT_EQUAL_INT(20, v);
    TEST_ASSERT_TRUE(rb.pop(v));
    TEST_ASSERT_EQUAL_INT(30, v);
    TEST_ASSERT_TRUE(rb.empty());
}

static void test_push_fails_when_full(void)
{
    RingBuffer<int, 2> rb;
    TEST_ASSERT_TRUE(rb.push(1));
    TEST_ASSERT_TRUE(rb.push(2));
    TEST_ASSERT_TRUE(rb.full());
    TEST_ASSERT_FALSE(rb.push(3));      // 拒否され、何も上書きされない
    TEST_ASSERT_EQUAL_size_t(2, rb.size());

    int v = 0;
    TEST_ASSERT_TRUE(rb.pop(v));
    TEST_ASSERT_EQUAL_INT(1, v);
}

static void test_pop_fails_when_empty(void)
{
    RingBuffer<int, 2> rb;
    int v = 99;
    TEST_ASSERT_FALSE(rb.pop(v));
    TEST_ASSERT_EQUAL_INT(99, v);       // 出力先は変更されていないこと
}

static void test_wraparound(void)
{
    RingBuffer<int, 3> rb;
    int v = 0;
    // 満たす、2 個取り出す、また入れる。書き込み位置が先頭に戻るはず。
    TEST_ASSERT_TRUE(rb.push(1));
    TEST_ASSERT_TRUE(rb.push(2));
    TEST_ASSERT_TRUE(rb.push(3));
    TEST_ASSERT_TRUE(rb.pop(v));
    TEST_ASSERT_TRUE(rb.pop(v));
    TEST_ASSERT_TRUE(rb.push(4));
    TEST_ASSERT_TRUE(rb.push(5));
    TEST_ASSERT_TRUE(rb.full());

    TEST_ASSERT_TRUE(rb.pop(v)); TEST_ASSERT_EQUAL_INT(3, v);
    TEST_ASSERT_TRUE(rb.pop(v)); TEST_ASSERT_EQUAL_INT(4, v);
    TEST_ASSERT_TRUE(rb.pop(v)); TEST_ASSERT_EQUAL_INT(5, v);
    TEST_ASSERT_TRUE(rb.empty());
}

static void test_many_cycles_keep_order(void)
{
    RingBuffer<int, 5> rb;
    int v = 0;
    for (int i = 0; i < 100; ++i) {
        TEST_ASSERT_TRUE(rb.push(i));
        TEST_ASSERT_TRUE(rb.push(i + 1000));
        TEST_ASSERT_TRUE(rb.pop(v));
        TEST_ASSERT_EQUAL_INT(i, v);
        TEST_ASSERT_TRUE(rb.pop(v));
        TEST_ASSERT_EQUAL_INT(i + 1000, v);
    }
    TEST_ASSERT_TRUE(rb.empty());
}

static void test_capacity_one(void)
{
    RingBuffer<int, 1> rb;
    int v = 0;
    TEST_ASSERT_TRUE(rb.push(7));
    TEST_ASSERT_TRUE(rb.full());
    TEST_ASSERT_FALSE(rb.push(8));
    TEST_ASSERT_TRUE(rb.pop(v));
    TEST_ASSERT_EQUAL_INT(7, v);
    TEST_ASSERT_FALSE(rb.pop(v));
}

static void test_clear(void)
{
    RingBuffer<int, 4> rb;
    TEST_ASSERT_TRUE(rb.push(1));
    TEST_ASSERT_TRUE(rb.push(2));
    rb.clear();
    TEST_ASSERT_TRUE(rb.empty());
    TEST_ASSERT_EQUAL_size_t(0, rb.size());
    int v = 0;
    TEST_ASSERT_FALSE(rb.pop(v));
    TEST_ASSERT_TRUE(rb.push(3));
    TEST_ASSERT_TRUE(rb.pop(v));
    TEST_ASSERT_EQUAL_INT(3, v);
}

static void test_struct_elements(void)
{
    RingBuffer<Sample, 2> rb;
    Sample a = {1023, -5};
    Sample b = {512, 30};
    TEST_ASSERT_TRUE(rb.push(a));
    TEST_ASSERT_TRUE(rb.push(b));

    Sample out = {0, 0};
    TEST_ASSERT_TRUE(rb.pop(out));
    TEST_ASSERT_EQUAL_UINT16(1023, out.adc);
    TEST_ASSERT_EQUAL_INT8(-5, out.temp);
    TEST_ASSERT_TRUE(rb.pop(out));
    TEST_ASSERT_EQUAL_UINT16(512, out.adc);
    TEST_ASSERT_EQUAL_INT8(30, out.temp);
}

static void test_const_access(void)
{
    RingBuffer<int, 4> rb;
    check_via_const_ref(rb, 0);
    TEST_ASSERT_TRUE(rb.push(1));
    check_via_const_ref(rb, 1);
    TEST_ASSERT_TRUE(rb.push(2));
    TEST_ASSERT_TRUE(rb.push(3));
    check_via_const_ref(rb, 3);
    TEST_ASSERT_TRUE(rb.push(4));
    check_via_const_ref(rb, 4);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_new_buffer_is_empty);
    RUN_TEST(test_push_then_pop_fifo);
    RUN_TEST(test_push_fails_when_full);
    RUN_TEST(test_pop_fails_when_empty);
    RUN_TEST(test_wraparound);
    RUN_TEST(test_many_cycles_keep_order);
    RUN_TEST(test_capacity_one);
    RUN_TEST(test_clear);
    RUN_TEST(test_struct_elements);
    RUN_TEST(test_const_access);
    return UNITY_END();
}
