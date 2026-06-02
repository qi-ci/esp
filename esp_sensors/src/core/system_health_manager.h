#pragma once
#include <Arduino.h>

// ======================
// 初始化
// ======================
void SystemHealth_begin();

// ======================
// 周期调用
// ======================
void SystemHealth_update();

// ======================
// 状态查询
// ======================
bool SystemHealth_isHealthy();