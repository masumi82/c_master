#include "stats.h"

int32_t sum_array(const int32_t *a, size_t n)
{
    int32_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}

size_t count_positive(const int32_t *a, size_t n)
{
    size_t count;
    count = 0;
    for (size_t i = 0; i < n; i++) {
        if (a[i] > 0) {
            count++;
        }
    }
    return count;
}

int32_t average2(int32_t a, int32_t b)
{
    int32_t half = a / 2 + b / 2;   /* 溢れない: |a/2| + |b/2| は int32_t に収まる */
    int32_t rem  = a % 2 + b % 2;   /* -2, -1, 0, 1, 2 のどれか */

    /* rem が ±1（端数 0.5）のときは、全体を 0 方向に丸める。 */
    if (rem == -1 && half > 0) {
        return half - 1;
    }
    if (rem == 1 && half < 0) {
        return half + 1;
    }
    return half + rem / 2;
}
