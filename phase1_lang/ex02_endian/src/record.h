#ifndef RECORD_H
#define RECORD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* 通信上の形式（ワイヤフォーマット）: ビッグエンディアン、パディングなし、7 バイト。
 *   [0]    id         uint8
 *   [1..2] temp_x10   int16（2の補数）  例: -12.5℃ -> -125 -> FF 83
 *   [3..6] timestamp  uint32
 */
#define SENSOR_RECORD_WIRE_SIZE 7u

typedef struct {
    uint8_t id;
    int16_t temp_x10;
    uint32_t timestamp;
} sensor_record_t;

/* rec を buf に書く。成功したら SENSOR_RECORD_WIRE_SIZE を返す。
 * buf_len が足りなければ 0 を返し、buf は変更しない。 */
size_t record_serialize(const sensor_record_t *rec, uint8_t *buf, size_t buf_len);

/* buf からレコードを読む。成功したら true を返す。
 * buf_len が足りなければ false を返し、*out は変更しない。 */
bool record_deserialize(const uint8_t *buf, size_t buf_len, sensor_record_t *out);

#endif
