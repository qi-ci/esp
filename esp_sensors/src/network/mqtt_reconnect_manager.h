#pragma once
#include <Arduino.h>

// ======================
// 初始化
// ======================
void MQTTReconnect_begin();

// ======================
// 主循环调用
// ======================
void MQTTReconnect_update();

// ======================
// 状态查询
// ======================
bool MQTTReconnect_isOnline();

// ======================
// 主动触发重连
// ======================
void MQTTReconnect_forceReconnect();