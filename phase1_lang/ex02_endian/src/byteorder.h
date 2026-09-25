#ifndef BYTEORDER_H
#define BYTEORDER_H

#include <stdbool.h>
#include <stdint.h>

/* この CPU が多バイトの整数をリトルエンディアンで格納するなら true を返す。 */
bool is_little_endian(void);

/* store_*: v を指定のバイト順で dst[0..N-1] に書く。
 * load_*:  src[0..N-1] から指定のバイト順で値を読む。
 * dst/src のアラインメントは任意（例: buf + 1 でもよい）。 */
void store_be16(uint8_t *dst, uint16_t v);
uint16_t load_be16(const uint8_t *src);
void store_be32(uint8_t *dst, uint32_t v);
uint32_t load_be32(const uint8_t *src);

void store_le16(uint8_t *dst, uint16_t v);
uint16_t load_le16(const uint8_t *src);
void store_le32(uint8_t *dst, uint32_t v);
uint32_t load_le32(const uint8_t *src);

#endif
