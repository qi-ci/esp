#include "wifi_storage.h"
#include <Preferences.h>
#include "../core/system_state.h"

// ======================
// WiFi保存
// ======================
void WiFiStorage_saveWiFi()
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
bool WiFiStorage_loadWiFi()
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
void WiFiStorage_saveIntervals()
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
bool WiFiStorage_loadIntervals()
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