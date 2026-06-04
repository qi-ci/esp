#pragma once
#include <Arduino.h>

void MQTT_begin();

bool MQTT_connect();

void MQTT_disconnect();

void MQTT_loop();

bool MQTT_isConnected();

int MQTT_lastError();

bool MQTT_publish(const String& topic, const String& payload);

bool MQTT_publishTelemetry(const String& payload);

bool MQTT_publishResponse(const String& payload);

bool MQTT_subscribe(const String& topic);