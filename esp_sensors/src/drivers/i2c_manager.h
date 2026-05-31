#pragma once

#include <Wire.h>

void I2CManager_begin(
    uint8_t sdaPin,
    uint8_t sclPin);

TwoWire& I2CManager_getBus();