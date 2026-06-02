#pragma once
#include <Arduino.h>

// ======================
// 统一Topic管理
// ======================
String MQTTTopic_telemetry();
String MQTTTopic_cmd();
String MQTTTopic_resp();