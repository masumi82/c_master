#include "stats.h"

int32_t sum_array(const int32_t *a, size_t n)
{
    int32_t sum = 0;
    for (size_t i = 0; i <= n; i++) {
        sum += a[i];
    }
    return sum;
}

size_t count_positive(const int32_t *a, size_t n)
{
    size_t count;
    for (size_t i = 0; i < n; i++) {
        if (a[i] > 0) {
            count++;
        }
    }
    return count;
}

int32_t average2(int32_t a, int32_t b)
{
    return (a + b) / 2;
}
