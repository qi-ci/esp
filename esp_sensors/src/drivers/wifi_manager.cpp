#include "wifi_manager.h"
#include "../config/wifi_storage.h"
#include "../core/system_state.h"
#include "../config/network_config.h"

#include <WiFi.h>

// ======================
// 当前连接状态
// ======================
static bool g_connecting = false;

// ======================
// 初始化 WiFi
// ======================
void WiFiManager_begin()
{
    WiFi.mode(WIFI_STA);
}

// ======================
// 非阻塞开始连接 WiFi
// ======================
void WiFiManager_connect()
{
    WiFi.disconnect(true);
    delay(100); // 小延迟，保证断开
    WiFi.begin(g_systemState.wifi_ssid.c_str(), g_systemState.wifi_password.c_str());

    g_connecting = true; // 标记正在连接
}

// ======================
// 轮询更新 WiFi 状态
// ======================
bool WiFiManager_update()
{
    if (!g_connecting)
        return WiFi.status() == WL_CONNECTED;

    wl_status_t status = WiFi.status();

    if (status == WL_CONNECTED)
    {
        g_connecting = false;
        WiFiStorage_saveWiFi(); // 成功保存
        Serial.println("[WIFI] Connected to " + g_systemState.wifi_ssid);
        return true;
    }
    else if (status == WL_DISCONNECTED || status == WL_CONNECT_FAILED)
    {
        g_connecting = false;
        Serial.println("[WIFI] Connection failed or lost");
        return false;
    }
    return false;
}

// ======================
// 查询 WiFi 状态
// ======================
bool WiFiManager_isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

// ======================
// 断开 WiFi
// ======================
void WiFiManager_disconnect()
{
    WiFi.disconnect();
    g_connecting = false;
}

// ==============================
// 新增 AP 模式
// ==============================
void WiFiManager_beginAPMode()
{
    WiFi.mode(WIFI_AP_STA);
    const char* ap_ssid = "ESP32_Setup";
    const char* ap_password = "12345678";
    WiFi.softAP(ap_ssid, ap_password);

    Serial.println("AP Mode started. Connect to WiFi 'ESP32_Setup' and configure network.");
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