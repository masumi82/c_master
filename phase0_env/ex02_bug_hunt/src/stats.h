#ifndef STATS_H
#define STATS_H

#include <stddef.h>
#include <stdint.h>

/* a[0..n-1] の合計を返す。n == 0 なら 0 を返す。
 * 前提条件: 合計は int32_t に収まる。 */
int32_t sum_array(const int32_t *a, size_t n);

/* a[0..n-1] のうち 0 より大きい要素の個数を返す。 */
size_t count_positive(const int32_t *a, size_t n);

/* (a + b) / 2 を 0 方向に丸めて返す。int32_t のどんな組み合わせでも正しく計算する。 */
int32_t average2(int32_t a, int32_t b);

#endif
