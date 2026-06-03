#include "task_scheduler.h"

#include "../network/network_state_machine.h"
#include "../network/mqtt_reconnect_manager.h"

#include "../core/sensor_manager.h"
#include "../core/telemetry_manager.h"
#include "../core/system_health_manager.h"

#include "../app/serial_reporter.h"

// =====================
// 时间戳
// =====================

static uint32_t lastNetwork = 0;
static uint32_t lastSensor = 0;
static uint32_t lastTelemetry = 0;
static uint32_t lastHealth = 0;
static uint32_t lastSerial = 0;

void Scheduler_begin()
{
}

void Scheduler_update()
{
    uint32_t now = millis();

    // =====================
    // 网络层
    // =====================

    if(now - lastNetwork >= 100)
    {
        lastNetwork = now;

        NetworkStateMachine_update();
        MQTTReconnect_update();
    }

    // =====================
    // 传感器
    // =====================

    if(now - lastSensor >= 2000)
    {
        lastSensor = now;

        SensorManager_update();
    }

    // =====================
    // MQTT上传
    // =====================

    if(now - lastTelemetry >= 5000)
    {
        lastTelemetry = now;

        TelemetryManager_update();
    }

    // =====================
    // 健康监控
    // =====================

    if(now - lastHealth >= 5000)
    {
        lastHealth = now;

        SystemHealth_update();
    }

    // =====================
    // 串口输出
    // =====================

    if(now - lastSerial >= 2000)
    {
        lastSerial = now;

        SerialReporter_print();
    }
}