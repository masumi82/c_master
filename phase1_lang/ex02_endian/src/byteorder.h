#ifndef BYTEORDER_H
#define BYTEORDER_H

#include <stdbool.h>
#include <stdint.h>

/* Returns true if this CPU stores multi-byte integers little-endian. */
bool is_little_endian(void);

/* store_*: write v into dst[0..N-1] in the given byte order.
 * load_*:  read a value from src[0..N-1] in the given byte order.
 * dst/src may have ANY alignment (e.g. buf + 1). */
void store_be16(uint8_t *dst, uint16_t v);
uint16_t load_be16(const uint8_t *src);
void store_be32(uint8_t *dst, uint32_t v);
uint32_t load_be32(const uint8_t *src);

void store_le16(uint8_t *dst, uint16_t v);
uint16_t load_le16(const uint8_t *src);
void store_le32(uint8_t *dst, uint32_t v);
uint32_t load_le32(const uint8_t *src);

#endif
