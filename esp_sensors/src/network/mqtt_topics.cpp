#include "mqtt_topics.h"
#include "../config/network_config.h"
#include "../config/device_config.h"

// ======================
// Topic命名规则：
// esp32/{device_id}/{type}
// ======================

String MQTTTopic_telemetry()
{
    return String("esp32/") +
           DEVICE_ID +
           "/tele";
}

String MQTTTopic_cmd()
{
    return String("esp32/") +
           DEVICE_ID +
           "/cmd";
}

String MQTTTopic_resp()
{
    return String("esp32/") +
           DEVICE_ID +
           "/resp";
}