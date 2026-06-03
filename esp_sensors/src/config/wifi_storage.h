#pragma once
#include <Arduino.h>

// 初始化 NVS/Preferences
void WiFiStorage_begin();

// 保存 WiFi 配置
void WiFiStorage_saveWiFi();

// 读取 WiFi 配置
bool WiFiStorage_loadWiFi();

// 保存设备信息（如 device_id），WiFi配网成功后保存
void WiFiStorage_saveDeviceID();

// 读取设备信息，初始化时读取
bool WiFiStorage_loadDeviceID();

// 保存传感器采样周期
void WiFiStorage_saveIntervals();

// 读取传感器采样周期
bool WiFiStorage_loadIntervals();