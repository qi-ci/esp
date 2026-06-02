#include "system_health_manager.h"

#include "system_state.h"
#include "sensor_manager.h"

#include "../network/mqtt_reconnect_manager.h"
#include "../drivers/wifi_manager.h"

// ======================
// 健康状态
// ======================
static bool g_healthy = true;

// ======================
// 异常计数器
// ======================
static uint32_t sensorFailCount = 0;
static uint32_t mqttFailCount   = 0;

// ======================
// 配置阈值
// ======================
static const uint8_t SENSOR_FAIL_LIMIT = 5;
static const uint8_t MQTT_FAIL_LIMIT   = 5;

// ======================
// 初始化
// ======================
void SystemHealth_begin()
{
    g_healthy = true;
    sensorFailCount = 0;
    mqttFailCount = 0;
}

// ======================
// 检查传感器状态
// ======================
static void checkSensors()
{
    bool ok =
        g_systemState.htuReady &&
        g_systemState.ze08Ready &&
        g_systemState.s8Ready;

    if(!ok)
    {
        sensorFailCount++;
    }
    else
    {
        sensorFailCount = 0;
    }

    // ======================
    // 传感器恢复策略
    // ======================
    if(sensorFailCount >= SENSOR_FAIL_LIMIT)
    {
        Serial.println("[HEALTH] sensor recovery triggered");

        SensorManager_begin(); // 重新初始化传感器
        sensorFailCount = 0;
    }
}

// ======================
// 检查MQTT状态
// ======================
static void checkMQTT()
{
    bool ok = MQTTReconnect_isOnline();

    if(!ok)
    {
        mqttFailCount++;
    }
    else
    {
        mqttFailCount = 0;
    }

    // ======================
    // MQTT恢复策略
    // ======================
    if(mqttFailCount >= MQTT_FAIL_LIMIT)
    {
        Serial.println("[HEALTH] mqtt force reconnect");

        MQTTReconnect_forceReconnect();
        mqttFailCount = 0;
    }
}

// ======================
// 检查数据异常
// ======================
static void checkData()
{
    if(
        isnan(g_systemState.temperature) ||
        isnan(g_systemState.humidity) ||
        isnan(g_systemState.co2) ||
        isnan(g_systemState.hcho)
    )
    {
        Serial.println("[HEALTH] invalid sensor data detected");

        // 降级策略：重启传感器
        SensorManager_begin();
    }
}

// ======================
// 主更新
// ======================
void SystemHealth_update()
{
    checkSensors();
    checkMQTT();
    checkData();

    // ======================
    // 综合健康判断
    // ======================
    g_healthy =
        sensorFailCount < SENSOR_FAIL_LIMIT &&
        mqttFailCount < MQTT_FAIL_LIMIT;
}

// ======================
// 状态查询
// ======================
bool SystemHealth_isHealthy()
{
    return g_healthy;
}