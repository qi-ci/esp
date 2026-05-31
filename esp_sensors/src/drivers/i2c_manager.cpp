#include "i2c_manager.h"

void I2CManager_begin(
    uint8_t sdaPin,
    uint8_t sclPin)
{
    Wire.begin(
        sdaPin,
        sclPin);
}

TwoWire& I2CManager_getBus()
{
    return Wire;
}