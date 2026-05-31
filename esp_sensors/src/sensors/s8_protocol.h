#pragma once
#include <stdint.h>

// S8解析结果
struct S8Frame
{
    int co2;
    bool valid;
};

// 构造请求帧
void S8_buildRequest(uint8_t* cmd);

// CRC16
uint16_t S8_crc16(const uint8_t* data, uint8_t len);

// 解析响应帧
bool S8_parseFrame(
    const uint8_t* buf,
    S8Frame* out);