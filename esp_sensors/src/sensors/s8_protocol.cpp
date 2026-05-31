#include "s8_protocol.h"

// ================= CRC16 Modbus =================

uint16_t S8_crc16(
    const uint8_t* buf,
    uint8_t len)
{
    uint16_t crc = 0xFFFF;

    for(uint8_t i = 0; i < len; i++)
    {
        crc ^= buf[i];

        for(int j = 0; j < 8; j++)
        {
            if(crc & 1)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}

// ================= 请求帧 =================

void S8_buildRequest(uint8_t* cmd)
{
    cmd[0] = 0xFE;
    cmd[1] = 0x04;
    cmd[2] = 0x00;
    cmd[3] = 0x03;
    cmd[4] = 0x00;
    cmd[5] = 0x01;

    uint16_t crc =
        S8_crc16(cmd, 6);

    cmd[6] = crc & 0xFF;
    cmd[7] = crc >> 8;
}

// ================= 响应解析 =================

bool S8_parseFrame(
    const uint8_t* buf,
    S8Frame* out)
{
    uint16_t value =
        ((uint16_t)buf[3] << 8)
        | buf[4];

    out->co2 = value;

    out->valid = true;

    return true;
}