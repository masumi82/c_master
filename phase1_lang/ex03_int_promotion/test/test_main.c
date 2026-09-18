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
