#pragma once

// ======================
// MQTT Default Config
// ======================
#define MQTT_HOST      "192.168.1.100"
#define MQTT_PORT      1883

#define MQTT_USERNAME  ""
#define MQTT_PASSWORD  ""

// ======================
// WiFi Retry Policy
// ======================
#define WIFI_RETRY_LIMIT     20
#define WIFI_RETRY_DELAY_MS  500

// ======================
// MQTT Retry Policy
// ======================
#define MQTT_RETRY_INTERVAL  5000
#define MQTT_KEEPALIVE       60