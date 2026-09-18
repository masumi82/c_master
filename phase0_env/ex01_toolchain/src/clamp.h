#ifndef CLAMP_H
#define CLAMP_H

#include <stdint.h>

/* Returns v limited to the closed range [lo, hi].
 * Precondition: lo <= hi. */
int32_t clamp_i32(int32_t v, int32_t lo, int32_t hi);

#endif
