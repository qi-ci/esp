#include <Arduino.h>

#include "data_validator.h"

bool Validator_temperature(
    float value)
{
    if(isnan(value))
        return false;

    return
        value >= -40.0f
        &&
        value <= 125.0f;
}

bool Validator_humidity(
    float value)
{
    if(isnan(value))
        return false;

    return
        value >= 0.0f
        &&
        value <= 100.0f;
}

bool Validator_hcho(
    float value)
{
    if(isnan(value))
        return false;

    return
        value >= 0.0f
        &&
        value <= 5.0f;
}

bool Validator_co2(
    int value)
{
    return
        value >= 350
        &&
        value <= 10000;
}