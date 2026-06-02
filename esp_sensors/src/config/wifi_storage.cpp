#include "wifi_storage.h"
#include <Preferences.h>

static Preferences prefs;

// ======================
// 初始化
// ======================
void WiFiStorage_begin()
{
    prefs.begin("esp_config", false); // namespace "esp_config", RW模式
}

// ======================
// WiFi保存与读取
// ======================
void WiFiStorage_saveWiFi(const String& ssid, const String& password)
{
    prefs.putString("wifi_ssid", ssid);
    prefs.putString("wifi_pass", password);
}

bool WiFiStorage_loadWiFi(String& ssid, String& password)
{
    if (!prefs.isKey("wifi_ssid") || !prefs.isKey("wifi_pass"))
        return false;

    ssid = prefs.getString("wifi_ssid", "");
    password = prefs.getString("wifi_pass", "");
    return true;
}

// ======================
// Device ID保存与读取
// ======================
void WiFiStorage_saveDeviceID(const String& device_id)
{
    prefs.putString("device_id", device_id);
}

bool WiFiStorage_loadDeviceID(String& device_id)
{
    if (!prefs.isKey("device_id"))
        return false;

    device_id = prefs.getString("device_id", "");
    return true;
}

// ======================
// 传感器采样周期保存与读取
// ======================
void WiFiStorage_saveIntervals(uint32_t htu, uint32_t ze08, uint32_t s8)
{
    prefs.putUInt("htu_interval", htu);
    prefs.putUInt("ze08_interval", ze08);
    prefs.putUInt("s8_interval", s8);
}

bool WiFiStorage_loadIntervals(uint32_t& htu, uint32_t& ze08, uint32_t& s8)
{
    if (!prefs.isKey("htu_interval") || !prefs.isKey("ze08_interval") || !prefs.isKey("s8_interval"))
        return false;

    htu   = prefs.getUInt("htu_interval", 5000);
    ze08  = prefs.getUInt("ze08_interval", 5000);
    s8    = prefs.getUInt("s8_interval", 5000);
    return true;
}