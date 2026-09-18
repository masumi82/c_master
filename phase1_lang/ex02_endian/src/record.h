#ifndef RECORD_H
#define RECORD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Wire format: big-endian, no padding, 7 bytes.
 *   [0]    id         uint8
 *   [1..2] temp_x10   int16 (two's complement)  e.g. -12.5 C -> -125 -> FF 83
 *   [3..6] timestamp  uint32
 */
#define SENSOR_RECORD_WIRE_SIZE 7u

typedef struct {
    uint8_t id;
    int16_t temp_x10;
    uint32_t timestamp;
} sensor_record_t;

/* Writes rec into buf. Returns SENSOR_RECORD_WIRE_SIZE on success,
 * or 0 if buf_len is too small (buf is left unchanged). */
size_t record_serialize(const sensor_record_t *rec, uint8_t *buf, size_t buf_len);

/* Reads a record from buf. Returns true on success,
 * or false if buf_len is too small (*out is left unchanged). */
bool record_deserialize(const uint8_t *buf, size_t buf_len, sensor_record_t *out);

#endif
