// #include <Arduino.h>

// #include "sensors/htu21d_sensor.h"
// #include "sensors/ze08_sensor.h"
// #include "sensors/s8_sensor.h"

// // ================= 数据（从原main迁移） =================
// float temperature;
// float humidity;
// int co2ppm;
// float hcho;

// void SensorManager_begin()
// {
//     HTU21D_begin();
//     ZE08_begin();
//     S8_begin();
// }

// void SensorManager_update()
// {
//     // 原逻辑不变，只是搬到这里

//     HTU21D_read(&temperature, &humidity);

//     ZE08_read(&hcho);

//     S8_read(&co2ppm);
// }

#include "sensor_manager.h"
#include "system_state.h"

// ======================
// Sensors
// ======================
#include "../sensors/htu21d_sensor.h"
#include "../sensors/ze08_sensor.h"
#include "../sensors/s8_sensor.h"

// ======================
// Processing
// ======================
#include "../processing/data_validator.h"
#include "../processing/filter.h"

// ======================
// Arduino
// ======================
#include <Arduino.h>

// ======================
// EMA状态
// ======================

static float g_tempEMA = NAN;
static float g_humiEMA = NAN;
static float g_hchoEMA = NAN;
static float g_co2EMA  = NAN;

// ======================
// 定时器
// ======================

static uint32_t lastHTURead  = 0;
static uint32_t lastZE08Read = 0;
static uint32_t lastS8Read   = 0;

// ======================
// 初始化
// ======================
void SensorManager_begin()
{
    HTU21D_begin();
    ZE08_begin();
    S8_begin();
}

// ======================
// 主更新函数
// ======================
void SensorManager_update()
{
    uint32_t now = millis();

    // ======================
    // 1. HTU21D
    // ======================
    if (now - lastHTURead >= g_systemState.htu_interval)
    {
        lastHTURead = now;

        float temp;
        float humi;

        HTU21D_read(&temp, &humi);

        if (Validator_temperature(temp))
        {
            g_systemState.temperature =
                Filter_applyEMA(temp, &g_tempEMA, 0.2f);

        }

        if (Validator_humidity(humi))
        {
            g_systemState.humidity =
                Filter_applyEMA(humi, &g_humiEMA, 0.2f);
        }
    }

    // ======================
    // 2. ZE08（甲醛）
    // ======================
    if (now - lastZE08Read >= g_systemState.ze08_interval)
    {
        lastZE08Read = now;

        float hcho;

        ZE08_read(&hcho);

        if (Validator_hcho(hcho))
        {
            g_systemState.hcho =
                Filter_applyEMA(hcho, &g_hchoEMA, 0.3f);
        }
    }

    // ======================
    // 3. S8（CO2）
    // ======================
    if (now - lastS8Read >= g_systemState.s8_interval)
    {
        lastS8Read = now;

        int co2;

        S8_read(&co2);

        if (Validator_co2(co2))
        {
            float co2_f = (float)co2;

            g_systemState.co2 =
                (int)Filter_applyEMA(co2_f, &g_co2EMA, 0.2f);
        }
    }

    // ======================
    // 4. 时间戳
    // ======================
    g_systemState.timestamp = now;
    g_systemState.lastUpdateTime = now;
}

void SensorManager_HTU21Dupdate()
{
    uint32_t now = millis();

    // ======================
    // HTU21D
    // ======================
    if (now - lastHTURead >= g_systemState.htu_interval)
    {
        lastHTURead = now;

        float temp;
        float humi;

        HTU21D_read(&temp, &humi);

        if (Validator_temperature(temp))
        {
            g_systemState.temperature =
                Filter_applyEMA(temp, &g_tempEMA, 0.2f);

        }

        if (Validator_humidity(humi))
        {
            g_systemState.humidity =
                Filter_applyEMA(humi, &g_humiEMA, 0.2f);
        }
    }
    g_systemState.timestamp = now;
    g_systemState.lastUpdateTime = now;
}

void SensorManager_ZE08update()
{
    uint32_t now = millis();

    // ======================
    // 2. ZE08（甲醛）
    // ======================
    if (now - lastZE08Read >= g_systemState.ze08_interval)
    {
        lastZE08Read = now;

        float hcho;

        ZE08_read(&hcho);

        if (Validator_hcho(hcho))
        {
            g_systemState.hcho =
                Filter_applyEMA(hcho, &g_hchoEMA, 0.3f);
        }
    }
    g_systemState.timestamp = now;
    g_systemState.lastUpdateTime = now;
}

void SensorManager_S8update()
{
    uint32_t now = millis();

    // ======================
    // 3. S8（CO2）
    // ======================
    if (now - lastS8Read >= g_systemState.s8_interval)
    {
        lastS8Read = now;

        int co2;

        S8_read(&co2);

        if (Validator_co2(co2))
        {
            float co2_f = (float)co2;

            g_systemState.co2 =
                (int)Filter_applyEMA(co2_f, &g_co2EMA, 0.2f);
        }
    }
    g_systemState.timestamp = now;
    g_systemState.lastUpdateTime = now;
}