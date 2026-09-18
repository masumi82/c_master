#ifndef STATS_H
#define STATS_H

#include <stddef.h>
#include <stdint.h>

/* Returns the sum of a[0..n-1]. Returns 0 when n == 0.
 * Precondition: the sum fits in int32_t. */
int32_t sum_array(const int32_t *a, size_t n);

/* Returns how many elements of a[0..n-1] are > 0. */
size_t count_positive(const int32_t *a, size_t n);

/* Returns (a + b) / 2 rounded toward zero, for ANY pair of int32_t values. */
int32_t average2(int32_t a, int32_t b);

#endif
