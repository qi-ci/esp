#pragma once
#include <Arduino.h>

void SystemStorage_begin();

// 保存 WiFi 配置
void SystemStorage_saveWiFi();

// 读取 WiFi 配置
bool SystemStorage_loadWiFi();

// 保存传感器采样周期
void SystemStorage_saveIntervals();

// 读取传感器采样周期
bool SystemStorage_loadIntervals();