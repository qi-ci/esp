#include "wifi_manager.h"

#include "../config/wifi_storage.h"
#include "../network/wifi_ap_server.h"

#include "../core/system_state.h"

#include <WiFi.h>

// ======================
// 非阻塞开始连接 WiFi
// ======================
void WiFiManager_connect()
{
    WiFi.disconnect(true);
    delay(100); // 小延迟，保证断开
    WiFi.begin(g_systemState.wifi_ssid.c_str(), g_systemState.wifi_password.c_str());
}

// ======================
// 查询 WiFi 状态
// ======================
bool WiFiManager_isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

// ======================
// 轮询更新 WiFi 状态
// ======================
void WiFiManager_reconfig()
{
    if (WiFiManager_isConnected() != WL_CONNECTED)
    {
        WiFiManager_beginAPMode(); // 进入AP配置
    }
}

// ==============================
// AP 模式
// ==============================
void WiFiManager_beginAPMMode()
{
    WiFi.mode(WIFI_AP_STA);
    const char* ap_ssid = "ESP32_Setup";
    const char* ap_password = "12345678";
    WiFi.softAP(ap_ssid, ap_password);
    Serial.println("AP Mode started. Connect to WiFi 'ESP32_Setup' and configure network.");
    
    WiFiAPServer_begin(); // 启动内置 Web 配置服务器
}

// ==============================
// 工业级：WiFi测试连接（不保存）
// ==============================
bool WiFiManager_testConnect()
{
    WiFi.disconnect(true);
    delay(100);

    WiFi.begin(g_systemState.wifi_ssid.c_str(), g_systemState.wifi_password.c_str());

    uint32_t start = millis();

    while (millis() - start < 5000) // 5秒测试窗口
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            return true;
        }
        delay(100);
    }

    return false;
}