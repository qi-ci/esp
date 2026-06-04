// #pragma once

// #include <Arduino.h>

// struct SystemState
// {
//     // 传感器数据

//     float temperature;
//     float humidity;

//     float hcho;
//     int co2;

//     // 空气质量

//     const char* airQuality;

//     // 传感器状态

//     bool htuReady;
//     bool ze08Ready;
//     bool s8Ready;

//     // 错误计数

//     uint32_t errorCount;

//     // 启动计数

//     uint32_t bootCount;


//     uint32_t timestamp;
//     uint32_t sampleInterval;
// };

// extern SystemState g_systemState;


#pragma once
#include <stdint.h>
#include <Arduino.h>

// ======================
// 系统全局状态中心
// ======================
struct SystemState
{
    // ---- WiFi ----
    String wifi_ssid;
    String wifi_password;

    // WiFi 测试结果（AP 配网专用）
    bool wifi_test_result;   // true=测试成功 false=失败中

    // ---- 设备ID ----
    String device_id;

    // ---- 传感器数据 ----
    float temperature;
    float humidity;
    int   co2;
    float hcho;

    // ---- 时间信息 ----
    uint32_t timestamp;        // 当前时间戳
    uint32_t lastUpdateTime;   // 最近更新时间

    // ---- 设备状态 ----
    bool htuReady;
    bool ze08Ready;
    bool s8Ready;

    // ---- 核心状态统一 ----
    bool sensor_ok;
    bool wifi_connected;
    bool mqtt_connected;
    const char* deviceStatus;  // running / booting

    // ---- 采样间隔(ms) ----
    uint32_t htu_interval;
    uint32_t ze08_interval;
    uint32_t s8_interval;
};

extern SystemState g_systemState;