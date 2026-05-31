#pragma once
#include <stdint.h>

// 帧解析结果
struct ZE08Frame
{
    float hcho;
    bool valid;
};

// 解析一帧数据
bool ZE08_parseFrame(
    const uint8_t* buf,
    ZE08Frame* out);