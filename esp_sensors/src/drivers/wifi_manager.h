#pragma once
#include <Arduino.h>

void WiFiManager_connect();

bool WiFiManager_isConnected();

void WiFiManager_reconfig();


// -----------------------------
// 新增接口：AP 配置模式
void WiFiManager_beginAPMode();
// 新增WiFi测试连接（不保存）
bool WiFiManager_testConnect();