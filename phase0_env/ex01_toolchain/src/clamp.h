#ifndef CLAMP_H
#define CLAMP_H

#include <stdint.h>

/* v を閉区間 [lo, hi] に収めた値を返す。
 * 前提条件: lo <= hi。 */
int32_t clamp_i32(int32_t v, int32_t lo, int32_t hi);

#endif
