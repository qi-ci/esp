#include "system_health_manager.h"

#include "../core/system_state.h"

#include "sensor_manager.h"
#include "../drivers/wifi_manager.h"
#include "../network/mqtt_client.h"
#include "../network/mqtt_broker.h"
#include "../processing/data_validator.h"


// ======================
// 健康状态
// ======================
static bool g_healthy = false;

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
static const uint8_t WIFI_FAIL_LIMIT = 5;
static const uint8_t MQTT_FAIL_LIMIT = 5;

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
        Serial.println("sensors error!");
    }
    else
    {
        sensorFailCount = 0;
        Serial.println("sensors running");
    }

    // ======================
    // 传感器恢复策略
    // ======================
    if(sensorFailCount >= SENSOR_FAIL_LIMIT)
    {
        Serial.println("[HEALTH] sensor recovery triggered");

        SensorManager_begin(); // 重新初始化传感器
        if(ok)
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
        Serial.println("wifi disconencted!");
    }
    else
    {
        wifiFailCount = 0;
        Serial.println("wifi connected, ssid: " + g_systemState.wifi_ssid + ", password: " + g_systemState.wifi_password);
    }

    // ======================
    // WiFi恢复策略
    // ======================
    if(wifiFailCount >= WIFI_FAIL_LIMIT)
    {
        Serial.println("[HEALTH] wifi force reconnect");

        WiFiManager_connect();
        if(WiFiManager_isConnected())
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
        Serial.println("mqtt disconnected");
    }
    else
    {
        mqttFailCount = 0;
        Serial.println("mqtt connected");
    }

    // ======================
    // MQTT恢复策略
    // ======================
    if(mqttFailCount >= MQTT_FAIL_LIMIT)
    {
        Serial.println("[HEALTH] mqtt force reconnect");
        MQTT_connect();
        if(MQTT_isConnected())
            mqttFailCount = 0;
    }
}

// ======================
// 主更新
// ======================
void SystemHealth_update()
{
    checkSensors();
    if(g_systemState.htuReady && g_systemState.ze08Ready && g_systemState.s8Ready)
        g_systemState.sensor_ok = true;
    
    checkWifi();
    if(WiFiManager_isConnected())
        g_systemState.wifi_connected = true;

    checkMQTT();
    if(MQTT_isConnected())
        g_systemState.mqtt_connected = true;
    
    // ======================
    // 系统状态判断
    // ======================
    g_systemState.system_ok = 
        g_systemState.sensor_ok && g_systemState.wifi_connected && g_systemState.mqtt_connected;


    // ======================
    // 综合健康判断
    // ======================
    g_healthy =
        sensorFailCount < SENSOR_FAIL_LIMIT &&
        wifiFailCount < WIFI_FAIL_LIMIT &&
        mqttFailCount < MQTT_FAIL_LIMIT;
}

// ======================
// 状态查询
// ======================
bool SystemHealth_isHealthy()
{
    return g_healthy;
}