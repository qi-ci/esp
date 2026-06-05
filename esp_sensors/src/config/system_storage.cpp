#include "system_storage.h"
#include <Preferences.h>
#include "../core/system_state.h"

void SystemStorage_begin()
{
    // ======================
    // 初始化NVS分区
    // ======================
    static Preferences prefs;
    bool ok = prefs.begin("system_config", false);

    if(!ok)
    {
        Serial.println("[NVS] init failed");
        return;
    }

    Serial.println("[NVS] ready");
    
    prefs.end(); // 关键：这里只是测试初始化，不长期占用
}

// ======================
// WiFi保存
// ======================
void SystemStorage_saveWiFi()
{
    Preferences prefs;
    prefs.begin("esp_config", false);   // 每次打开

    prefs.putString("wifi_ssid", g_systemState.wifi_ssid);
    prefs.putString("wifi_pass", g_systemState.wifi_password);

    prefs.end(); // ✔ 立即释放NVS句柄
}

// ======================
// WiFi读取
// ======================
bool SystemStorage_loadWiFi()
{
    Preferences prefs;
    prefs.begin("esp_config", true);   // 只读模式

    if (!prefs.isKey("wifi_ssid") || !prefs.isKey("wifi_pass"))
    {
        prefs.end();
        return false;
    }

    g_systemState.wifi_ssid = prefs.getString("wifi_ssid", "");
    g_systemState.wifi_password = prefs.getString("wifi_pass", "");

    prefs.end(); // ✔ 立即释放

    return true;
}


// ======================
// 采样间隔保存
// ======================
void SystemStorage_saveIntervals()
{
    Preferences prefs;
    prefs.begin("esp_config", false);

    prefs.putUInt("htu_interval", g_systemState.htu_interval);
    prefs.putUInt("ze08_interval", g_systemState.ze08_interval);
    prefs.putUInt("s8_interval", g_systemState.s8_interval);

    prefs.end();
}

// ======================
// 采样间隔读取
// ======================
bool SystemStorage_loadIntervals()
{
    Preferences prefs;
    prefs.begin("esp_config", true);

    if (!prefs.isKey("htu_interval") ||
        !prefs.isKey("ze08_interval") ||
        !prefs.isKey("s8_interval"))
    {
        prefs.end();
        return false;
    }

    g_systemState.htu_interval  = prefs.getUInt("htu_interval", 5000);
    g_systemState.ze08_interval = prefs.getUInt("ze08_interval", 5000);
    g_systemState.s8_interval   = prefs.getUInt("s8_interval", 5000);

    prefs.end();

    return true;
}