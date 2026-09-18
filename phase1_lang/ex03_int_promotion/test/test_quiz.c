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
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_q1);
    RUN_TEST(test_q2);
    RUN_TEST(test_q3);
    RUN_TEST(test_q4);
    RUN_TEST(test_q5);
    RUN_TEST(test_q6);
    RUN_TEST(test_q7);
    RUN_TEST(test_q8);
}
