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

#include <Arduino.h>

#include "sensor_manager.h"
#include "system_state.h"

#include "../sensors/htu21d_sensor.h"
#include "../sensors/ze08_sensor.h"
#include "../sensors/s8_sensor.h"

#include "../processing/filter.h"
#include "../processing/data_validator.h"
#include "../processing/air_quality.h"

#include "../drivers/i2c_manager.h"
#include "../drivers/uart_manager.h"

// EMA状态

static float tempFilterState = NAN;
static float humFilterState = NAN;
static float hchoFilterState = NAN;
static float co2FilterState = NAN;

// 定时器

static uint32_t lastSampleTime = 0;

static const uint32_t SAMPLE_INTERVAL = 5000;

// ============================

void SensorManager_begin()
{
    I2CManager_begin(21,22);
    UARTManager_beginUART1(9600, 4, 5);
    UARTManager_beginUART2(9600, 16, 17);
    
    HTU21D_begin();
    ZE08_begin();
    S8_begin();

    g_systemState.htuReady =
        HTU21D_isReady();

    g_systemState.ze08Ready =
        ZE08_isReady();

    g_systemState.s8Ready =
        S8_isReady();
}

// ============================

void SensorManager_update()
{
    if(millis() - lastSampleTime
        < SAMPLE_INTERVAL)
    {
        return;
    }

    lastSampleTime = millis();

    float temp;
    float hum;

    float hcho;

    int co2;

    // HTU21D

    if(HTU21D_read(&temp,&hum))
    {
        if(Validator_temperature(temp))
        {
            g_systemState.temperature =
                Filter_applyEMA(
                    temp,
                    &tempFilterState,
                    0.3f);
        }

        if(Validator_humidity(hum))
        {
            g_systemState.humidity =
                Filter_applyEMA(
                    hum,
                    &humFilterState,
                    0.3f);
        }
    }

    // ZE08

    if(ZE08_read(&hcho))
    {
        if(Validator_hcho(hcho))
        {
            g_systemState.hcho =
                Filter_applyEMA(
                    hcho,
                    &hchoFilterState,
                    0.2f);
        }
    }

    // S8

    if(S8_read(&co2))
    {
        if(Validator_co2(co2))
        {
            g_systemState.co2 =
                (int)Filter_applyEMA(
                    (float)co2,
                    &co2FilterState,
                    0.2f);
        }
    }

    g_systemState.airQuality =
        AirQuality_getLevel(
            g_systemState.co2,
            g_systemState.hcho);
}