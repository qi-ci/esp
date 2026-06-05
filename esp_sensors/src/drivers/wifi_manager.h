#pragma once
#include <Arduino.h>

void WiFiManager_connect();

bool WiFiManager_isConnected();

void WiFiManager_WifiUpdate();

// AP配置模式
void WiFiManager_beginAPMode();
// AP配网
void WiFiManager_NetAPModeConfig();
// WiFi测试连接（不保存）
bool WiFiManager_testConnect(const String& ssid, const String& password);