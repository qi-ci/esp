#include "network_state_machine.h"

#include "../drivers/wifi_manager.h"
#include "../network/mqtt_client.h"
#include "../network/mqtt_reconnect_manager.h"
#include "../network/wifi_ap_server.h"

#include "../config/wifi_storage.h"
#include "../config/network_config.h"
#include "../core/system_state.h"

// ======================
// 当前状态
// ======================
static NetworkState g_state = NET_BOOT;

// ======================
// 上次重连时间
// ======================
static uint32_t lastReconnectTime = 0;

// ======================
// 状态字符串
// ======================
String NetworkStateMachine_getStateString()
{
    switch(g_state)
    {
        case NET_BOOT: return "BOOT";
        case NET_WIFI_CONNECTING: return "WIFI_CONNECTING";
        case NET_WIFI_CONNECTED: return "WIFI_CONNECTED";
        case NET_MQTT_CONNECTING: return "MQTT_CONNECTING";
        case NET_ONLINE: return "ONLINE";
        case NET_WIFI_LOST: return "WIFI_LOST";
        case NET_MQTT_LOST: return "MQTT_LOST";
        case NET_AP_MODE: return "AP_MODE";
        case NET_RECOVERY: return "RECOVERY";
    }
    return "UNKNOWN";
}

// ======================
// 状态获取
// ======================
NetworkState NetworkStateMachine_getState()
{
    return g_state;
}

// ======================
// 初始化状态机
// ======================
void NetworkStateMachine_begin()
{
    WiFiManager_begin();
    MQTT_begin();

    // ---------- 设备ID处理 ----------
    if (!WiFiStorage_loadDeviceID())
    {
        // 第一次启动没有ID，生成默认ID（可改为UUID/芯片ID）
        g_systemState.device_id = "airmon_default_001";
        WiFiStorage_saveDeviceID();
    }

    g_state = NET_BOOT;

    // 初始化全局状态
    g_systemState.mqtt_connected = false;
}

// ======================
// 启动 AP
// ======================
void NetworkStateMachine_beginAP()
{
    WiFiManager_beginAPMode();   // 启动 AP 热点
    WiFiAPServer_begin();        // 启动内置 Web 配置服务器

    g_state = NET_AP_MODE;
}

// ======================
// AP 循环处理
// ======================
void NetworkStateMachine_APLoop()
{
    if(g_state == NET_AP_MODE)
        WiFiAPServer_loop();     // 处理网页请求
}

// ======================
// 状态机核心
// ======================
void NetworkStateMachine_update()
{
    uint32_t now = millis();

    switch(g_state)
    {
        // ======================
        case NET_BOOT:
        {
            WiFiStorage_loadWiFi();
            if(g_systemState.wifi_ssid.length() > 0)
                g_state = NET_WIFI_CONNECTING;
            else
                g_state = NET_AP_MODE;   // 没保存 WiFi → AP 配置
            break;
        }

        // ======================
        case NET_WIFI_CONNECTING:
        {
            WiFiStorage_loadWiFi();
            // 首次进入时发起非阻塞连接
            if (!WiFiManager_isConnected() && !WiFiManager_isConnected())
            {
                WiFiManager_connect(); // 发起连接
            }

            // 轮询状态
            if (WiFiManager_update()) // 非阻塞更新
                g_state = NET_WIFI_CONNECTED;
            else if (!WiFiManager_isConnected() && millis() - lastReconnectTime > WIFI_RETRY_LIMIT * WIFI_RETRY_DELAY_MS)
                g_state = NET_AP_MODE; // 多次失败，进入 AP 配置

            break;
        }

        // ======================
        case NET_WIFI_CONNECTED:
        {

            bool ok = WiFiManager_isConnected();

            if(!WiFiManager_isConnected())
            {
                g_state = NET_WIFI_LOST;
                break;
            }
            g_state = NET_MQTT_CONNECTING;
            break;
        }

        // ======================
        case NET_MQTT_CONNECTING:
        {
            MQTTReconnect_update();
            g_systemState.mqtt_connected = MQTTReconnect_isOnline();

            if(g_systemState.mqtt_connected)
                g_state = NET_ONLINE;
            break;
        }

        // ======================
        case NET_ONLINE:
        {
            g_systemState.mqtt_connected = MQTTReconnect_isOnline();

            if(!WiFiManager_isConnected())
                g_state = NET_WIFI_LOST;
            else if(!g_systemState.mqtt_connected)
                g_state = NET_MQTT_LOST;
            else
                MQTT_loop();   // 发布数据、处理订阅消息
            break;
        }

        // ======================
        case NET_WIFI_LOST:
        {

            if(now - lastReconnectTime > MQTT_RETRY_INTERVAL)
            {
                lastReconnectTime = now;
                g_state = NET_WIFI_CONNECTING;
            }
            break;
        }

        // ======================
        case NET_MQTT_LOST:
        {
            g_systemState.mqtt_connected = false;

            if(now - lastReconnectTime > MQTT_RETRY_INTERVAL)
            {
                lastReconnectTime = now;
                g_state = NET_MQTT_CONNECTING;
            }
            break;
        }

        // ======================
        case NET_AP_MODE:
        {
            g_systemState.mqtt_connected = false;
            // AP 循环在 NetworkStateMachine_APLoop() 中处理
            break;
        }

        // ======================
        case NET_RECOVERY:
        {
            g_state = NET_WIFI_CONNECTING;
            break;
        }
    }
}