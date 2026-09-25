#ifndef SAFE_ARITH_H
#define SAFE_ARITH_H

#include <stdbool.h>
#include <stdint.h>

/* 成功したら: 正確な結果を *out に入れて true を返す。
 * 溢れるなら: false を返し、*out は変更しない。
 *
 * ルール: 演算する「前に」オペランドを検査する。より広い型（int64_t など）で
 * 計算しない。__builtin_*_overflow も使わない。 */
bool safe_add_i32(int32_t a, int32_t b, int32_t *out);
bool safe_mul_i32(int32_t a, int32_t b, int32_t *out);
bool safe_add_u32(uint32_t a, uint32_t b, uint32_t *out);

#endif
