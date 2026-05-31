#include "filter.h"

float Filter_applyEMA(
    float newValue,
    float* state,
    float alpha)
{
    if(*state != *state)
    {
        *state = newValue;
        return newValue;
    }

    *state =
        alpha * newValue
        + (1.0f - alpha) * (*state);

    return *state;
}