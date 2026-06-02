#pragma once
#include <Arduino.h>

// 初始化 NVS/Preferences
void WiFiStorage_begin();

// 保存 WiFi 配置
void WiFiStorage_saveWiFi(const String& ssid, const String& password);

// 读取 WiFi 配置
bool WiFiStorage_loadWiFi(String& ssid, String& password);

// 保存设备信息（如 device_id）
void WiFiStorage_saveDeviceID(const String& device_id);

// 读取设备信息
bool WiFiStorage_loadDeviceID(String& device_id);

// 保存传感器采样周期
void WiFiStorage_saveIntervals(uint32_t htu, uint32_t ze08, uint32_t s8);

// 读取传感器采样周期
bool WiFiStorage_loadIntervals(uint32_t& htu, uint32_t& ze08, uint32_t& s8);