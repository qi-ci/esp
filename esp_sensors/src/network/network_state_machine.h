#pragma once
#include <Arduino.h>

enum NetworkState {
    NET_BOOT,
    NET_WIFI_CONNECTING,
    NET_WIFI_CONNECTED,
    NET_MQTT_CONNECTING,
    NET_ONLINE,
    NET_WIFI_LOST,
    NET_MQTT_LOST,
    NET_AP_MODE,
    NET_RECOVERY
};

// ======================
// 状态管理接口
// ======================
void NetworkStateMachine_begin();
void NetworkStateMachine_update();
String NetworkStateMachine_getStateString();
NetworkState NetworkStateMachine_getState();

// ======================
// AP 模式接口
// ======================
void NetworkStateMachine_beginAP();
void NetworkStateMachine_APLoop();