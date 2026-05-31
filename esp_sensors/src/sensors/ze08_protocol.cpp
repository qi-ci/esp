#include "ze08_protocol.h"

// ===============================
// ZE08校验策略（可替换点）
// ===============================

// 当前版本：简单和校验（常见ZE08实现）
static uint8_t ZE08_checksum(
    const uint8_t* data)
{
    uint8_t sum = 0;

    for(int i = 0; i < 8; i++)
    {
        sum += data[i];
    }

    return (uint8_t)(~sum + 1);
}

// ===============================
// 帧解析
// ===============================

bool ZE08_parseFrame(
    const uint8_t* buf,
    ZE08Frame* out)
{
    if(buf[0] != 0xFF)
    {
        out->valid = false;
        return false;
    }

    uint8_t crc =
        ZE08_checksum(buf);

    if(crc != buf[8])
    {
        out->valid = false;
        return false;
    }

    uint16_t value =
        ((uint16_t)buf[4] << 8)
        | buf[5];

    out->hcho =
        value / 1000.0f;

    out->valid = true;

    return true;
}