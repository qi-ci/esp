#include <stdio.h>

#include "output_formatter.h"

void OutputFormatter_toHuman(char* buffer, int size)
{
    snprintf(buffer, size,
        "Temperature : %.2f C\n"
        "Humidity    : %.2f %%\n"
        "CO2         : %d ppm\n"
        "HCHO        : %.3f mg/m3\n"
        "Air Quality : %s\n",
        g_systemState.temperature,
        g_systemState.humidity,
        g_systemState.co2,
        g_systemState.hcho,
        g_systemState.airQuality);
}

void OutputFormatter_toJSON(char* buffer, int size)
{
    snprintf(buffer, size,
        "{"
        "\"temp\":%.2f,"
        "\"humi\":%.2f,"
        "\"co2\":%d,"
        "\"hcho\":%.3f,"
        "\"aq\":\"%s\""
        "}",
        g_systemState.temperature,
        g_systemState.humidity,
        g_systemState.co2,
        g_systemState.hcho,
        g_systemState.airQuality);
}