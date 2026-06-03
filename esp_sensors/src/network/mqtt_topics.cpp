#include "mqtt_topics.h"
#include "../config/network_config.h"
#include "../config/device_config.h"
#include "../core/system_state.h"

// ======================
// Topic命名规则：
// esp32/{device_id}/{type}
// ======================

String MQTTTopic_telemetry()
{
    return String("esp32/") +
           "g_Systemstate.device_id" +
           "/tele";
}

String MQTTTopic_cmd()
{
    return String("esp32/") +
           "g_Systemstate.device_id" +
           "/cmd";
}

String MQTTTopic_resp()
{
    return String("esp32/") +
           "g_Systemstate.device_id" +
           "/resp";
}