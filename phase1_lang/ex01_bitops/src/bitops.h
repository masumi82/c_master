#ifndef BITOPS_H
#define BITOPS_H

#include <stdbool.h>
#include <stdint.h>

/* All functions return a new value; nothing is modified in place.
 * Preconditions (not checked):
 *   bit   : 0..31
 *   width : 1..32 and shift + width <= 32
 */

uint32_t bit_set(uint32_t reg, unsigned bit);
uint32_t bit_clear(uint32_t reg, unsigned bit);
uint32_t bit_toggle(uint32_t reg, unsigned bit);
bool bit_test(uint32_t reg, unsigned bit);

/* Mask with `width` consecutive 1 bits starting at `shift`.
 * e.g. bit_mask(4, 3) == 0x70 */
uint32_t bit_mask(unsigned shift, unsigned width);

/* Extracts the field. e.g. field_get(0xABCD, 4, 8) == 0xBC */
uint32_t field_get(uint32_t reg, unsigned shift, unsigned width);

/* Returns reg with the field replaced by value.
 * Bits of value above `width` are ignored. e.g. field_set(0xABCD, 4, 8, 0x12) == 0xA12D */
uint32_t field_set(uint32_t reg, unsigned shift, unsigned width, uint32_t value);

#endif
