#include "task_scheduler.h"

#include "../core/sensor_manager.h"
#include "../core/system_health_manager.h"

#include "../drivers/wifi_manager.h"
#include "../network/wifi_ap_server.h"
// #include "../network/mqtt_broker.h"
#include "../network/mqtt_client.h"

#include "../core/system_state.h"

#include "../app/serial_reporter.h"

#include <Arduino.h>

// =====================
// 上次执行时间戳
// =====================
static uint32_t lastSensorHTU  = 0;
static uint32_t lastSensorZE08 = 0;
static uint32_t lastSensorS8   = 0;

static uint32_t lastTelemetry  = 0;
static uint32_t lastHealth     = 0;
static uint32_t lastSerial     = 0;

static uint32_t lastAPServer = 0;
static uint32_t lastmqtt = 0;
// static uint32_t lastbroker = 0;

// =====================
// 默认间隔（毫秒）
// =====================
static const uint32_t HEALTH_INTERVAL  = 10000;  // 健康检查 1min
static const uint32_t SERIAL_INTERVAL  = 4000;  // 串口输出 4s
const uint32_t APSERVER_INTERVAL = 50; // ms
const uint32_t MQTT_INTERVAL = 100; //ms
// const uint32_t BROKER_INTERVAL = 30; //ms


void Scheduler_begin()
{
    lastSensorHTU  = millis();
    lastSensorZE08 = millis();
    lastSensorS8   = millis();
    lastTelemetry  = millis();
    lastHealth     = millis();
    lastSerial     = millis();
    lastAPServer   = millis();
    lastmqtt       = millis();
    // lastbroker     = millis();
}

void Scheduler_update()
{
    uint32_t now = millis();

    uint32_t min_interval = min(g_systemState.htu_interval,min(g_systemState.ze08_interval,g_systemState.s8_interval));

    // =====================
    // HTU21D
    if(now - lastSensorHTU >= g_systemState.htu_interval)
    {
        lastSensorHTU += g_systemState.htu_interval;
        SensorManager_HTU21Dupdate();
    }

    // =====================
    // ZE08
    if(now - lastSensorZE08 >= g_systemState.ze08_interval)
    {
        lastSensorZE08 += g_systemState.ze08_interval;
        SensorManager_ZE08update();
    }

    // =====================
    // S8
    if(now - lastSensorS8 >= g_systemState.s8_interval)
    {
        lastSensorS8 += g_systemState.s8_interval;
        SensorManager_S8update();
    }

    // =====================
    // Telemetry 上传
    if(now - lastTelemetry >= min_interval)  // 可自定义，通常 1s 发布一次
    {
        lastTelemetry += min_interval;
        MQTT_AutoPublish();
    }

    // =====================
    // 健康检查
    if(now - lastHealth >= HEALTH_INTERVAL)
    {
        lastHealth += HEALTH_INTERVAL;
        SystemHealth_update();
    }

    // =====================
    // 串口输出
    if(now - lastSerial >= SERIAL_INTERVAL)
    {
        lastSerial += SERIAL_INTERVAL;
        SerialReporter_print();
    }

    // AP server
    if(now - lastAPServer >= APSERVER_INTERVAL)
    {
        lastAPServer += APSERVER_INTERVAL;
        if(WiFiManager_isConnected())
            return;
        else
            WiFiAPServer_loop();  //必须一直快速轮询
    }

    // // broker服务器
    // if(now - lastbroker >= BROKER_INTERVAL)
    // {
    //     lastbroker += BROKER_INTERVAL;
    //     if(WiFiManager_isConnected())
    //         broker.loop();
    //     else
    //         return;
    // }

    // MQTT保活
    if(now - lastmqtt >= MQTT_INTERVAL)
    {
        lastmqtt += MQTT_INTERVAL;
        if(WiFiManager_isConnected())
            MQTT_loop();  //MQTT保活
        else
            return;
    }
}