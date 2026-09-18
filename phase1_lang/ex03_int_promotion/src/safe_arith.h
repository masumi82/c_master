#ifndef SAFE_ARITH_H
#define SAFE_ARITH_H

#include <stdbool.h>
#include <stdint.h>

/* On success: store the exact result in *out and return true.
 * On overflow: return false and leave *out unchanged.
 *
 * Rules: check the operands BEFORE operating. Do not compute in a wider
 * type (int64_t etc.) and do not use __builtin_*_overflow. */
bool safe_add_i32(int32_t a, int32_t b, int32_t *out);
bool safe_mul_i32(int32_t a, int32_t b, int32_t *out);
bool safe_add_u32(uint32_t a, uint32_t b, uint32_t *out);

#endif
