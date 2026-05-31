#include <Arduino.h>
#include "system_state.h"

SystemState g_systemState =
{
    .temperature = NAN,
    .humidity = NAN,

    .hcho = NAN,
    .co2 = -1,

    .airQuality = "未知",

    .htuReady = false,
    .ze08Ready = false,
    .s8Ready = false,

    .errorCount = 0,

    .bootCount = 1
};