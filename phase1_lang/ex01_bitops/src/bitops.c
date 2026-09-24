#include "bitops.h"

uint32_t bit_set(uint32_t reg, unsigned bit)
{
    return reg | (UINT32_C(1) << bit);
}

uint32_t bit_clear(uint32_t reg, unsigned bit)
{
    return reg & ~(UINT32_C(1) << bit);
}

uint32_t bit_toggle(uint32_t reg, unsigned bit)
{
    return reg ^ (UINT32_C(1) << bit);
}

bool bit_test(uint32_t reg, unsigned bit)
{
    return (reg & (UINT32_C(1) << bit)) != UINT32_C(0);
}

uint32_t bit_mask(unsigned shift, unsigned width)
{
    if (width == 32U) {
        return UINT32_MAX;
    }

    return ((UINT32_C(1) << width) - UINT32_C(1)) << shift;
}

uint32_t field_get(uint32_t reg, unsigned shift, unsigned width)
{
    return (reg >> shift) & bit_mask(0U, width);
}

uint32_t field_set(
    uint32_t reg,
    unsigned shift,
    unsigned width,
    uint32_t value)
{
    const uint32_t mask = bit_mask(shift, width);
    const uint32_t shifted_value = (value << shift) & mask;

    return (reg & ~mask) | shifted_value;
}