#include "telemetry_manager.h"
#include "../app/output_formatter.h"
#include "../network/mqtt_client.h"
#include <Arduino.h>

// 上一次上传时间（毫秒）
static unsigned long lastUploadHTU  = 0;
static unsigned long lastUploadZE08 = 0;
static unsigned long lastUploadS8   = 0;

void TelemetryManager_begin()
{
    Serial.println("[TEL] Telemetry manager ready");
}

void TelemetryManager_update()
{
    unsigned long now = millis();

    // ======================
    // HTU21D上传
    // ======================
    if (now - lastUploadHTU >= g_systemState.htuInterval)
    {
        lastUploadHTU = now;

        String payload = OutputFormatter_buildTelemetry(g_systemState, "htu"); // 增加 sensor字段区分
        if (MQTT_isConnected() && !MQTT_publishTelemetry(payload))
        {
            Serial.println("[TEL] HTU telemetry publish failed");
        }
    }

    // ======================
    // ZE08上传
    // ======================
    if (now - lastUploadZE08 >= g_systemState.ze08Interval)
    {
        lastUploadZE08 = now;

        String payload = OutputFormatter_buildTelemetry(g_systemState, "ze08");
        if (MQTT_isConnected() && !MQTT_publishTelemetry(payload))
        {
            Serial.println("[TEL] ZE08 telemetry publish failed");
        }
    }

    // ======================
    // S8上传
    // ======================
    if (now - lastUploadS8 >= g_systemState.s8Interval)
    {
        lastUploadS8 = now;

        String payload = OutputFormatter_buildTelemetry(g_systemState, "s8");
        if (MQTT_isConnected() && !MQTT_publishTelemetry(payload))
        {
            Serial.println("[TEL] S8 telemetry publish failed");
        }
    }
}