#include "mqtt_client.h"

#include "../config/network_config.h"
#include "../core/system_state.h"

#include "../drivers/wifi_manager.h"

#include "../core/command_handler.h"

#include "mqtt_topics.h"

#include <WiFiClient.h>
#include <PubSubClient.h>

// ======================
// 底层MQTT驱动（只负责连接）
// ======================
static WiFiClient wifiClient;
static PubSubClient mqtt(wifiClient);

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
    msg.reserve(length);
    for (unsigned int i = 0; i < length; i++)
    {
        msg += (char)payload[i];
    }

    String topicStr(topic);

    if(topicStr == MQTTTopic_cmd())
    {
        CommandHandler_handle(msg);
    }
}

// ======================
// 初始化
// ======================
void MQTT_begin()
{
    mqtt.setServer(MQTT_HOST, MQTT_PORT);
    mqtt.setCallback(mqttCallback);
    mqtt.setBufferSize(512);
}

// ======================
// 内部连接（只做connect）
// ======================
bool MQTT_connect()
{
    if(!WiFiManager_isConnected())
    {
        Serial.println("[MQTT] WiFi not ready");
        return false;
    }

    if(mqtt.connected())
        return true;

    String clientId = g_systemState.device_id;

    bool ok = mqtt.connect(
        clientId.c_str(),
        MQTT_USERNAME,
        MQTT_PASSWORD
    );

    if(ok)
    {
        Serial.println("[MQTT] Connected");
        MQTT_subscribe(MQTTTopic_cmd());
    }
    else
    {
        Serial.printf("[MQTT] Connect Failed state=%d\n",mqtt.state());
    }

    return ok;
}

void MQTT_disconnect()
{
    if(mqtt.connected())
    {
        mqtt.disconnect();
        Serial.println("[MQTT] Disconnected");
    }
}

// ======================
// loop（只保留底层处理）
// ======================
void MQTT_loop()
{
    if(!mqtt.connected())
        return;
    
    mqtt.loop();
}

// ======================
// 状态
// ======================
bool MQTT_isConnected()
{
    return mqtt.connected();
}

int MQTT_lastError()
{
    return mqtt.state();
}

// ======================
// publish
// ======================
bool MQTT_publish(const String& topic,const String& payload)
{
    if(!mqtt.connected())
    {
        Serial.println("[MQTT] Publish Failed");
        return false;
    }

    return mqtt.publish(topic.c_str(),payload.c_str());
}

bool MQTT_publishTelemetry(const String& payload)
{
    return MQTT_publish(MQTTTopic_telemetry(),payload);
}

bool MQTT_publishResponse(const String& payload)
{
    return MQTT_publish(MQTTTopic_resp(),payload);
}





