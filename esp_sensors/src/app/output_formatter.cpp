#include <stdio.h>

#include "output_formatter.h"

// void OutputFormatter_toHuman(char* buffer, int size)
// {
//     snprintf(buffer, size,
//         "Temperature : %.2f C\n"
//         "Humidity    : %.2f %%\n"
//         "CO2         : %d ppm\n"
//         "HCHO        : %.3f mg/m3\n"
//         "Air Quality : %s\n",
//         g_systemState.temperature,
//         g_systemState.humidity,
//         g_systemState.co2,
//         g_systemState.hcho,
//         g_systemState.airQuality);
// }

// void OutputFormatter_toJSON(char* buffer, int size)
// {
//     snprintf(buffer, size,
//         "{"
//         "\"temp\":%.2f,"
//         "\"humi\":%.2f,"
//         "\"co2\":%d,"
//         "\"hcho\":%.3f,"
//         "\"aq\":\"%s\""
//         "}",
//         g_systemState.temperature,
//         g_systemState.humidity,
//         g_systemState.co2,
//         g_systemState.hcho,
//         g_systemState.airQuality);
// }



// ======================
// MQTT Telemetry JSON
// ======================
String OutputFormatter_buildTelemetry(const SystemState& s, const String& sensorType)
{
    String j = "{";
    j += "\"sensor\":\"" + sensorType + "\"";
    j += ",\"temp\":" + String(s.temperature);
    j += ",\"humi\":" + String(s.humidity);
    j += ",\"co2\":" + String(s.co2);
    j += ",\"hcho\":" + String(s.hcho);
    j += ",\"time\":" + String(s.timestamp);
    j += "}";
    return j;
}

// ======================
String OutputFormatter_buildResponse(
    const String& status,
    const String& msg)
{
    return "{\"status\":\"" + status +
           "\",\"message\":\"" + msg + "\"}";
}

// ======================
String OutputFormatter_buildDebug()
{
    String t;
    t += "\n=== DEVICE STATUS ===\n";
    t += "Temp: " + String(g_systemState.temperature) + " °C\n";
    t += "Humi: " + String(g_systemState.humidity) + " %RH\n";
    t += "CO2 : " + String(g_systemState.co2) + " ppm\n";
    t += "HCHO: " + String(g_systemState.hcho) + " mg/m3\n";
    t += "State: " + String(g_systemState.deviceStatus) + "\n";
    return t;
}

