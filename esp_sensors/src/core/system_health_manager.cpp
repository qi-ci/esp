#include "system_health_manager.h"

#include "../core/system_state.h"

#include "sensor_manager.h"
#include "../drivers/wifi_manager.h"
#include "../network/mqtt_client.h"
#include "../processing/data_validator.h"

// ======================
// 健康状态
// ======================
static bool g_healthy = true;

// ======================
// 异常计数器
// ======================
static uint32_t sensorFailCount = 0;
static uint32_t wifiFailCount = 0;
static uint32_t mqttFailCount = 0; 

// ======================
// 配置阈值
// ======================
static const uint8_t SENSOR_FAIL_LIMIT = 5;
static const uint8_t WIFI_FAIL_LIMIT   = 5;
static const uint8_t MQTT_FAIL_LIMIT   = 5;

// ======================
// 初始化
// ======================
void SystemHealth_begin()
{
    sensorFailCount = 0;
    wifiFailCount = 0;
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
// 检查WiFi状态
// ======================
static void checkWifi()
{
    bool ok = WiFiManager_isConnected();

    if(!ok)
    {
        wifiFailCount++;
    }
    else
    {
        wifiFailCount = 0;
    }

    // ======================
    // WiFi恢复策略
    // ======================
    if(wifiFailCount >= WIFI_FAIL_LIMIT)
    {
        Serial.println("[HEALTH] wifi force reconnect");

        WiFiManager_reconfig();
        wifiFailCount = 0;
    }
}

// ======================
// 检查MQTT状态
// ======================
static void checkMQTT()
{
    bool ok = MQTT_isConnected();

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

        MQTT_connect();
        mqttFailCount = 0;
    }
}

// ======================
// 检查数据异常
// ======================
static void checkData()
{
    if(
        Validator_temperature(g_systemState.temperature) ||
        Validator_humidity(g_systemState.humidity) ||
        Validator_co2(g_systemState.co2) ||
        Validator_hcho(g_systemState.hcho)
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
    checkWifi();
    checkData();

    // ======================
    // 综合健康判断
    // ======================
    g_healthy =
        sensorFailCount < SENSOR_FAIL_LIMIT &&
        wifiFailCount < WIFI_FAIL_LIMIT;

    if(g_healthy)
    {
        g_systemState.sensor_ok = true;
        g_systemState.wifi_connected = true;
        g_systemState.mqtt_connected = true;
        g_systemState.deviceStatus = "running";
    }
}

// ======================
// 状态查询
// ======================
bool SystemHealth_isHealthy()
{
    return g_healthy;
}