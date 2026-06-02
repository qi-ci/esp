#pragma once
#include <Arduino.h>

// ======================
// 网络状态定义
// ======================
enum NetworkState
{
    NET_BOOT = 0,

    NET_WIFI_CONNECTING,
    NET_WIFI_CONNECTED,

    NET_MQTT_CONNECTING,
    NET_ONLINE,

    NET_WIFI_LOST,
    NET_MQTT_LOST,
    NET_RECOVERY
};

// ======================
// API
// ======================
void NetworkStateMachine_begin();

void NetworkStateMachine_update();

NetworkState NetworkStateMachine_getState();

String NetworkStateMachine_getStateString();