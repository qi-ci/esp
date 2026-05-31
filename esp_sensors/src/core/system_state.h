#pragma once

#include <Arduino.h>

struct SystemState
{
    // 传感器数据

    float temperature;
    float humidity;

    float hcho;
    int co2;

    // 空气质量

    const char* airQuality;

    // 传感器状态

    bool htuReady;
    bool ze08Ready;
    bool s8Ready;

    // 错误计数

    uint32_t errorCount;

    // 启动计数

    uint32_t bootCount;
};

extern SystemState g_systemState;