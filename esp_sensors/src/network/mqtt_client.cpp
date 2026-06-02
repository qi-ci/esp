#include "mqtt_client.h"
#include "mqtt_topics.h"

#include "../core/command_handler.h"

#include "../config/network_config.h"

#include "../app/output_formatter.h"
#include "../core/system_state.h"

#include <WiFiClient.h>
#include <PubSubClient.h>

// ======================
// 底层MQTT驱动（只负责连接）
// ======================
static WiFiClient wifiClient;
static PubSubClient mqtt(wifiClient);

static bool g_connected = false;

// ======================
// 初始化
// ======================
void MQTT_begin()
{
    mqtt.setServer(MQTT_HOST, MQTT_PORT);
}

// ======================
// 内部连接（只做connect）
// ======================
bool MQTT_connect()
{
    String clientId = "esp32_" + String((uint32_t)ESP.getEfuseMac(), HEX);

    bool ok = mqtt.connect(
        clientId.c_str(),
        MQTT_USERNAME,
        MQTT_PASSWORD
    );

    g_connected = ok;
    return ok;
}

// ======================
// loop（只保留底层处理）
// ======================
void MQTT_loop()
{
    if(mqtt.connected())
        mqtt.loop();
}

// ======================
// 状态
// ======================
bool MQTT_isConnected()
{
    return mqtt.connected();
}

// ======================
// publish
// ======================
bool MQTT_publishTelemetry(const String& payload)
{
    if(!mqtt.connected()) return false;

    return mqtt.publish(MQTTTopic_telemetry().c_str(), payload.c_str());
}

bool MQTT_publishResponse(const String& payload)
{
    if(!mqtt.connected()) return false;

    return mqtt.publish(MQTTTopic_resp().c_str(), payload.c_str());
}

// ======================
// subscribe（只提供接口）
// ======================
bool MQTT_subscribe(const String& topic)
{
    if(!mqtt.connected()) return false;

    return mqtt.subscribe(topic.c_str());
}

static void mqttCallback(char* topic, byte* payload, unsigned int length)
{
    String msg;
    for (unsigned int i = 0; i < length; i++)
    {
        msg += (char)payload[i];
    }

    CommandHandler_handle(msg);
}

// ======================
// 自动上传封装
// ======================
void MQTT_publishTelemetryAuto(const SystemState& state,const String& sensorType)
{
    if(!mqtt.connected())
        return;

    String payload = OutputFormatter_buildTelemetry(state, sensorType);

    mqtt.publish(
        MQTTTopic_telemetry().c_str(),
        payload.c_str()
    );
}