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
String OutputFormatter_buildFullTelemetry()
{
    unsigned long seconds = millis() / 1000;
    unsigned long hours = seconds / 3600;
    unsigned long minutes = (seconds % 3600) / 60;

    String j = "{";

    j += "\"temp\":" + String(g_systemState.temperature) + ",";
    j += "\"humi\":" + String(g_systemState.humidity) + ",";
    j += "\"co2\":"  + String(g_systemState.co2) + ",";
    j += "\"hcho\":" + String(g_systemState.hcho) + ",";

    j += "\"sensors\":\"" + String(g_systemState.sensor_ok ? "Running" : "Error") + "\",";
    j += "\"wifi\":\"" + String(g_systemState.wifi_connected ? "Connected" : "Disconnected") + "\",";
    j += "\"mqtt\":\"" + String(g_systemState.mqtt_connected ? "Connected" : "Disconnected") + "\",";

    j += "\"uptime\":\"" + String(hours) + "h " + String(minutes) + "m\"";

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
    
    t += "Sensor OK: ";
    t += g_systemState.sensor_ok ? "YES" : "NO";
    t += "\n";

    t += "WiFi Connected: ";
    t += g_systemState.wifi_connected ? "YES" : "NO";
    t += "\n";

    t += "MQTT Connected: ";
    t += g_systemState.mqtt_connected ? "YES" : "NO";
    t += "\n";

    t += "System OK: ";
    t += g_systemState.system_ok ? "YES" : "NO";

    t += "\n=====================\n";
    return t;
}

