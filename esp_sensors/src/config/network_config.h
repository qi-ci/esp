#pragma once

// // ======================
// // WiFi配置
// // ======================
// #define WIFI_SSID      "YOUR_WIFI"
// #define WIFI_PASSWORD  "YOUR_PASSWORD"

// ======================
// MQTT Broker
// ======================
#define MQTT_HOST      "192.168.1.100"
#define MQTT_PORT      1883

#define MQTT_USERNAME  ""
#define MQTT_PASSWORD  ""


// ======================
// 网络基础配置
// ======================

#define WIFI_RETRY_LIMIT     20
#define WIFI_RETRY_DELAY_MS  500

#define MQTT_RETRY_INTERVAL  5000

#define MQTT_KEEPALIVE       60