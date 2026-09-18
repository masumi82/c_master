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
