#include "app_init.h"
#include <Arduino.h>

// ======================
// Core Layer
// ======================
#include "../core/system_init.h"
#include "../core/sensor_manager.h"
#include "../drivers/wifi_manager.h"
#include "../core/system_health_manager.h"

#include "../core/telemetry_manager.h"

// ======================
// Network Layer
// ======================
#include "../network/mqtt_client.h"

// ======================
// App Layer
// ======================
#include "../app/serial_reporter.h"

#include "../scheduler/task_scheduler.h"

void App_init()
{
    SystemInit();
    SensorManager_begin();
    
    WiFiManager_connect();
    MQTT_begin();

    SystemHealth_begin();

    Scheduler_begin();
}

void App_loop()
{
    Scheduler_update();
}