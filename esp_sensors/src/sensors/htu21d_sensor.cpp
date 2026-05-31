// #include <Wire.h>
#include "drivers/i2c_manager.h"
#include <HTU21D.h>

HTU21D htu;

void HTU21D_begin()
{
    // Wire.begin(21, 22);
    I2CManager_begin(21, 22);
    htu.begin();
}

void HTU21D_read(float* temperature, float* humidity)
{
    *temperature = htu.readTemperature();
    *humidity = htu.readHumidity();
}