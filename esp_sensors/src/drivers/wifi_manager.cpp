#include "wifi_manager.h"

#include "../config/system_storage.h"
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

    if (g_systemState.wifi_ssid.length() != 0)
    {
        WiFi.mode(WIFI_STA);//声明当前为基站(Station)模式
        WiFi.begin(g_systemState.wifi_ssid.c_str(), g_systemState.wifi_password.c_str());
    }
    else
    {
        WiFiManager_NetAPModeConfig();
    }
}

// ======================
// 查询 WiFi 状态
// ======================
bool WiFiManager_isConnected()
{
    return WiFi.isConnected();
}

// ======================
// 轮询更新 WiFi 状态
// ======================
void WiFiManager_WifiUpdate()
{
    if (!WiFiManager_isConnected())
    {
        WiFiManager_NetAPModeConfig(); // 进入AP配置
    }
}

// ==============================
// AP配置模式
// ==============================
void WiFiManager_beginAPMode()
{
    WiFi.mode(WIFI_AP_STA);                     // AP + STA 模式

    // IPAddress local_ip(192,168,5,1);
    // IPAddress gateway(192,168,5,1);
    // IPAddress subnet(255,255,255,0);
    // WiFi.softAPConfig(local_ip, gateway, subnet);
    
    WiFi.softAP("ESP32_Setup", "12345678");    // 启动AP
}

// ==============================
// AP 配网
// ==============================
void WiFiManager_NetAPModeConfig()
{
    WiFiManager_beginAPMode();
    
    WiFiAPServer_begin(); // 启动内置 Web 配置服务器

    
}

// ==============================
// 工业级：WiFi测试连接（不保存）
// ==============================
bool WiFiManager_testConnect(const String& ssid, const String& password)
{
    WiFi.mode(WIFI_AP_STA);  // 保持 AP + STA
    WiFi.begin(ssid.c_str(), password.c_str());

    uint32_t start = millis();
    while (millis() - start < 10000)  // 最多尝试 10 秒
    {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("[WiFi] Connected to " + ssid);
            return true;
        }
        delay(200);
    }
    Serial.println("[WiFi] Connection failed to " + ssid);
    return false;
}