#include "mqtt_reconnect_manager.h"

#include "mqtt_client.h"
#include "mqtt_topics.h"
#include "../drivers/wifi_manager.h"

// ======================
// 重连控制参数
// ======================
static uint32_t lastReconnectTime = 0;
static const uint32_t RECONNECT_INTERVAL = 5000; // 5秒节流

static bool g_online = false;

// ======================
// 初始化
// ======================
void MQTTReconnect_begin()
{
    MQTT_begin();
}

// ======================
// 内部：执行重连
// ======================
static bool reconnect()
{
    if(!WiFiManager_isConnected())
        return false;

    bool ok = MQTT_connect();

    if(ok)
    {
        // ======================
        // 订阅恢复（关键）
        // ======================
        MQTT_subscribe(MQTTTopic_cmd().c_str());

        g_online = true;
    }
    else
    {
        g_online = false;
    }

    return ok;
}

// ======================
// 主循环
// ======================
void MQTTReconnect_update()
{
    uint32_t now = millis();

    // WiFi不在线，MQTT直接失败
    if(!WiFiManager_isConnected())
    {
        g_online = false;
        return;
    }

    // MQTT正常
    if(MQTT_isConnected())
    {
        g_online = true;
        return;
    }

    // ======================
    // 节流重连
    // ======================
    if(now - lastReconnectTime > RECONNECT_INTERVAL)
    {
        lastReconnectTime = now;
        reconnect();
    }
}

// ======================
// 状态
// ======================
bool MQTTReconnect_isOnline()
{
    return g_online;
}

// ======================
// 强制重连
// ======================
void MQTTReconnect_forceReconnect()
{
    lastReconnectTime = 0;
    reconnect();
}