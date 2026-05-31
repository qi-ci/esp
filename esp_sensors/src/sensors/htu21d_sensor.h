// #pragma once

// void HTU21D_begin();
// void HTU21D_read(float* temperature, float* humidity);

#pragma once

// 初始化HTU21D + 状态检测
bool HTU21D_begin();

// 读取温湿度
bool HTU21D_read(
    float* temperature,
    float* humidity);

// 设备状态
bool HTU21D_isReady();