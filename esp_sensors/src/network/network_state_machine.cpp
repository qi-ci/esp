#include "network_state_machine.h"

#include "../drivers/wifi_manager.h"
#include "../network/mqtt_client.h"
#include "../network/mqtt_reconnect_manager.h"

#include "../config/wifi_storage.h"
#include "../config/network_config.h"

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
// 初始化
// ======================
void NetworkStateMachine_begin()
{
    WiFiManager_begin();
    MQTT_begin();

    g_state = NET_BOOT;
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
        // BOOT
        // ======================
        case NET_BOOT:
        {
            String ssid, pass;

            WiFiStorage_loadWiFi(ssid, pass);

            if(ssid.length() > 0)
            {
                g_state = NET_WIFI_CONNECTING;
            }
            else
            {
                g_state = NET_WIFI_LOST;
            }
            break;
        }

        // ======================
        // WIFI CONNECTING
        // ======================
        case NET_WIFI_CONNECTING:
        {
            String ssid, pass;
            WiFiStorage_loadWiFi(ssid, pass);

            bool ok = WiFiManager_connect(ssid, pass);

            if(ok)
            {
                g_state = NET_WIFI_CONNECTED;
            }
            else
            {
                g_state = NET_WIFI_LOST;
            }
            break;
        }

        // ======================
        // WIFI CONNECTED
        // ======================
        case NET_WIFI_CONNECTED:
        {
            if(!WiFiManager_isConnected())
            {
                g_state = NET_WIFI_LOST;
                break;
            }

            g_state = NET_MQTT_CONNECTING;
            break;
        }

        // ======================
        // MQTT CONNECTING
        // ======================
        case NET_MQTT_CONNECTING:
        {
            MQTTReconnect_update();

            if(MQTTReconnect_isOnline())
                g_state = NET_ONLINE;

            break;
        }

        // ======================
        // ONLINE
        // ======================
        case NET_ONLINE:
        {
            if(!WiFiManager_isConnected())
            {
                g_state = NET_WIFI_LOST;
            }
            else if(!MQTT_isConnected())
            {
                g_state = NET_MQTT_LOST;
            }
            else
            {
                MQTT_loop();
            }
            break;
        }

        // ======================
        // WIFI LOST
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
        // MQTT LOST
        // ======================
        case NET_MQTT_LOST:
        {
            if(now - lastReconnectTime > MQTT_RETRY_INTERVAL)
            {
                lastReconnectTime = now;
                g_state = NET_MQTT_CONNECTING;
            }
            break;
        }

        // ======================
        // RECOVERY
        // ======================
        case NET_RECOVERY:
        {
            g_state = NET_WIFI_CONNECTING;
            break;
        }
    }
}