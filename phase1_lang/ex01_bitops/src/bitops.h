#ifndef BITOPS_H
#define BITOPS_H

#include <stdbool.h>
#include <stdint.h>

/* どの関数も新しい値を返すだけで、引数そのものは書き換えない。
 * 前提条件（関数の中では検査しない）:
 *   bit   : 0..31
 *   width : 1..32 かつ shift + width <= 32
 */

uint32_t bit_set(uint32_t reg, unsigned bit);
uint32_t bit_clear(uint32_t reg, unsigned bit);
uint32_t bit_toggle(uint32_t reg, unsigned bit);
bool bit_test(uint32_t reg, unsigned bit);

/* ビット `shift` から `width` ビット分だけ 1 が並んだマスクを返す。
 * 例: bit_mask(4, 3) == 0x70 */
uint32_t bit_mask(unsigned shift, unsigned width);

/* フィールドを取り出す。例: field_get(0xABCD, 4, 8) == 0xBC */
uint32_t field_get(uint32_t reg, unsigned shift, unsigned width);

/* reg のフィールドを value で置き換えた値を返す。
 * value のうち `width` ビットを超える部分は無視する。例: field_set(0xABCD, 4, 8, 0x12) == 0xA12D */
uint32_t field_set(uint32_t reg, unsigned shift, unsigned width, uint32_t value);

#endif
