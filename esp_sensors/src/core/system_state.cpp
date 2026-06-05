// #include <Arduino.h>
// #include "system_state.h"

// SystemState g_systemState =
// {
//     .temperature = NAN,
//     .humidity = NAN,

//     .hcho = NAN,
//     .co2 = -1,

//     .airQuality = "未知",

//     .htuReady = false,
//     .ze08Ready = false,
//     .s8Ready = false,

//     .errorCount = 0,

//     .bootCount = 1

//     .timestamp = 0,
//     .sampleInterval = 5000
// };

#include "system_state.h"
#include <math.h>

// ======================
// 全局状态初始化
// ======================
SystemState g_systemState =
{
    "", "", 
    
    false, 
    
    "airmon_livingroom", 

    NAN, NAN,
    -1, -1,

    0,
    0,

    false, false, false,

    false, false, false, false, 

    5000,5000,5000,
};