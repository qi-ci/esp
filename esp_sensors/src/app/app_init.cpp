#include "app_init.h"
#include <Arduino.h>

// ======================
// Core Layer
// ======================
#include "../core/system_init.h"
#include "../config/system_storage.h"
#include "../core/sensor_manager.h"
#include "../drivers/wifi_manager.h"
#include "../core/system_health_manager.h"

// ======================
// Network Layer
// ======================
#include "../network/mqtt_client.h"

// ======================
// App Layer
// ======================
#include "../app/serial_reporter.h"

#include "../scheduler/task_scheduler.h"

// #include "../network/mqtt_broker.h"


void App_init()
{
    SystemInit();
    SystemStorage_begin();
    SystemStorage_loadWiFi();
    SystemStorage_loadIntervals();

    SensorManager_begin();
    
    WiFiManager_connect();
    delay(500);   // ⭐ WiFi稳定

    // broker.begin(); // broker服务器，启动 MQTT 服务器，监听端口 1883
    // delay(300);   // ⭐关键：给 TCP server 启动时间

    MQTT_begin();
    delay(200);   // ⭐关键：再等一层稳定
    MQTT_connect();
    delay(300);

    SystemHealth_begin();
    SystemHealth_update();

    Scheduler_begin();
}

void App_loop()
{
    Scheduler_update();
}