#include "wifi_manager.h"
#include "../config/wifi_storage.h"
#include <WiFi.h>

static bool g_connected = false;

void WiFiManager_begin()
{
    WiFi.mode(WIFI_STA);
}

// 在连接 WiFi 成功后自动保存
bool WiFiManager_connect(const String& ssid, const String& password)
{
    WiFi.disconnect(true);
    delay(200);
    WiFi.begin(ssid.c_str(), password.c_str());

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 20)
    {
        delay(500);
        retry++;
    }

    bool ok = (WiFi.status() == WL_CONNECTED);

    // 成功连接则写入 NVS
    if (ok)
    {
        WiFiStorage_saveWiFi(ssid, password);
    }

    return ok;
}

bool WiFiManager_isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager_disconnect()
{
    WiFi.disconnect();
    g_connected = false;
}
