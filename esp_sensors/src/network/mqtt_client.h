#pragma once
#include <Arduino.h>

void MQTT_begin();

bool MQTT_connect();

void MQTT_loop();

bool MQTT_isConnected();

bool MQTT_publishTelemetry(const String& payload);

bool MQTT_publishResponse(const String& payload);

bool MQTT_subscribe(const String& topic);

static void mqttCallback(char* topic, byte* payload, unsigned int length);

void MQTT_publishTelemetryAuto();