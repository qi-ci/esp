// #include <Arduino.h>
// #include "core/sensor_manager.h"
// #include "app/serial_reporter.h"
// #include "core/system_init.h"

// void setup()
// {
//     SystemInit();
//     SensorManager_begin();
//     SerialReporter_begin();
// }

// void loop()
// {
//     SensorManager_update();
//     SerialReporter_print();
//     delay(5000);
// }

// #include <Arduino.h>

// //#include "core/system_state.h"
// #include "core/sensor_manager.h"
// #include "core/system_init.h"
// #include "app/serial_reporter.h"

// void setup()
// {
//     SystemInit();
//     SensorManager_begin();
// }

// void loop()
// {
//     SensorManager_update();

//     SerialReporter_printHuman();

//     delay(1000);
// }

#include <Arduino.h>

// ======================
// Core Layer
// ======================
#include "core/sensor_manager.h"
#include "core/system_state.h"
#include "core/system_init.h"
#include "core/system_health_manager.h"
#include "core/recovery_manager.h"
#include "core/command_handler.h"
#include "core/telemetry_manager.h"

// ======================
// Network Layer
// ======================
#include "network/network_state_machine.h"
#include "config/wifi_storage.h"
#include "network/mqtt_reconnect_manager.h"
#include "network/mqtt_client.h"

// ======================
// App Layer
// ======================
#include "app/output_formatter.h"
#include "app/serial_reporter.h"

// ======================
// Arduino setup
// ======================
void setup()
{
    SystemInit();// 调试串口
    WiFiStorage_begin();// 网络底层初始化（WiFi能力）

    NetworkStateMachine_begin();// 网络状态机初始化
    MQTTReconnect_begin();

    SensorManager_begin();// Core系统
    
    CommandHandler_begin();// App通信准备

    TelemetryManager_begin();

    SystemHealth_begin();// 系统状态
}

// ======================
// 主循环（调度器）
// ======================
void loop()
{
    NetworkStateMachine_update();   // WiFi + MQTT状态机
    MQTTReconnect_update();         // MQTT自愈层（双保险）
    
    SensorManager_update();

    TelemetryManager_update();      // 传感器数据上传

    SerialReporter_print();       // 本地串口输出

    SystemHealth_update();// 核心健康监控


}