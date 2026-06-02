#include "recovery_manager.h"
#include "system_state.h"

#include "../sensors/htu21d_sensor.h"
#include "../sensors/ze08_sensor.h"
#include "../sensors/s8_sensor.h"

#include "../network/mqtt_client.h"
#include <Arduino.h>

// ======================
// 传感器失败计数
// ======================
static int sensorFailCount = 0;
static int mqttFailCount   = 0;

// ======================
// 强制重启传感器
// ======================
void RecoveryManager_forceSensorReset()
{
    Serial.println("[RECOVERY] Restart sensors...");

    HTU21D_begin();
    ZE08_begin();
    S8_begin();

    sensorFailCount = 0;
}

// ======================
// 强制重连MQTT
// ======================
void RecoveryManager_forceMQTTReconnect()
{
    Serial.println("[RECOVERY] Reconnect MQTT...");

    MQTT_begin();

    mqttFailCount = 0;
}

// ======================
// 自动恢复调度器
// ======================
void RecoveryManager_update()
{
    // ======================
    // 1. 传感器异常处理
    // ======================
    if (!g_systemState.sensor_ok)
    {
        sensorFailCount++;
    }
    else
    {
        sensorFailCount = 0;
    }

    if (sensorFailCount > 5)
    {
        g_systemState.deviceStatus = "degraded";
        RecoveryManager_forceSensorReset();
    }

    // ======================
    // 2. MQTT异常处理
    // ======================
    if (!g_systemState.mqtt_connected)
    {
        mqttFailCount++;
    }
    else
    {
        mqttFailCount = 0;
    }

    if (mqttFailCount > 5)
    {
        g_systemState.deviceStatus = "degraded";
        RecoveryManager_forceMQTTReconnect();
    }

    // ======================
    // 3. 系统恢复正常
    // ======================
    if (g_systemState.sensor_ok &&
        g_systemState.mqtt_connected)
    {
        g_systemState.deviceStatus = "running";
    }
}