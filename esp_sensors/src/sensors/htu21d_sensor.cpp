// #include <Wire.h>
#include "drivers/i2c_manager.h"
#include <HTU21D.h>

HTU21D htu;

// void HTU21D_begin()
// {
//     // Wire.begin(21, 22);
//     I2CManager_begin(21, 22);
//     htu.begin();
// }

// void HTU21D_read(float* temperature, float* humidity)
// {
//     *temperature = htu.readTemperature();
//     *humidity = htu.readHumidity();
// }

static bool g_ready = false;

bool HTU21D_begin()
{
    // I2CManager_begin(21, 22);

    g_ready = htu.begin();

    return g_ready;
}

bool HTU21D_isReady()
{
    return g_ready;
}

bool HTU21D_read(
    float* temperature,
    float* humidity)
{
    if(!g_ready)
    {
        return false;
    }

    float t = htu.readTemperature();
    float h = htu.readHumidity();

    *temperature = t;
    *humidity = h;

    return true;
}