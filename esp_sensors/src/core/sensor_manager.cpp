#include <Arduino.h>

#include "sensors/htu21d_sensor.h"
#include "sensors/ze08_sensor.h"
#include "sensors/s8_sensor.h"

// ================= 数据（从原main迁移） =================
float temperature;
float humidity;
int co2ppm;
float hcho;

void SensorManager_begin()
{
    HTU21D_begin();
    ZE08_begin();
    S8_begin();
}

void SensorManager_update()
{
    // 原逻辑不变，只是搬到这里

    HTU21D_read(&temperature, &humidity);

    ZE08_read(&hcho);

    S8_read(&co2ppm);
}