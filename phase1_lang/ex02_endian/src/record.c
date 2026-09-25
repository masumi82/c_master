#include "record.h"

#include "byteorder.h"

size_t record_serialize(const sensor_record_t *rec, uint8_t *buf, size_t buf_len)
{
    /* TODO: 実装する（store_be16 / store_be32 を使う） */
    (void)rec;
    (void)buf;
    (void)buf_len;
    return 0;
}

bool record_deserialize(const uint8_t *buf, size_t buf_len, sensor_record_t *out)
{
    /* TODO: 実装する（load_be16 / load_be32 を使う） */
    (void)buf;
    (void)buf_len;
    (void)out;
    return false;
}
