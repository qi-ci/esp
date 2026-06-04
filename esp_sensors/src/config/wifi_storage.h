#pragma once
#include <Arduino.h>

// 保存 WiFi 配置
void WiFiStorage_saveWiFi();

// 读取 WiFi 配置
bool WiFiStorage_loadWiFi();

// 保存传感器采样周期
void WiFiStorage_saveIntervals();

// 读取传感器采样周期
bool WiFiStorage_loadIntervals();