#include "app_init.h"
#include <Arduino.h>

#include "../drivers/wifi_manager.h"

// ======================
// Core Layer
// ======================
#include "../core/sensor_manager.h"
#include "../core/system_state.h"
#include "../core/system_init.h"
#include "../core/system_health_manager.h"
#include "../core/command_handler.h"
#include "../core/telemetry_manager.h"

// ======================
// Network Layer
// ======================
#include "../network/network_state_machine.h"
#include "../network/mqtt_reconnect_manager.h"
#include "../network/mqtt_client.h"

// ======================
// Config
// ======================
#include "../config/wifi_storage.h"

// ======================
// App Layer
// ======================
#include "../app/output_formatter.h"
#include "../app/serial_reporter.h"

#include "../scheduler/task_scheduler.h"

void App_init()
{
    SystemInit();
    NetworkStateMachine_begin();
    SensorManager_begin();
    WiFiStorage_loadWiFi();
    SystemHealth_begin();
}

void App_loop()
{
    // ---------- 1. 网络状态机 ----------
    NetworkStateMachine_update();
    NetworkStateMachine_APLoop();       // AP模式循环

    // ---------- 2. MQTT自愈 ----------
    MQTTReconnect_update();

    // ---------- 3. 传感器采集 ----------
    SensorManager_update();

    // ---------- 4. 数据上传 ----------
    // MQTTLoop 已在状态机 ONLINE 时调用
    if (NetworkStateMachine_getState() == NET_ONLINE)
    {
        MQTT_loop(); // 发布数据、处理订阅消息
    }

    // ---------- 5. 系统健康监控 ----------
    SystemHealth_update();
}