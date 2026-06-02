#pragma once
#include <Arduino.h>

void WiFiManager_begin();

bool WiFiManager_connect(const String& ssid, const String& password);

bool WiFiManager_isConnected();

void WiFiManager_disconnect();