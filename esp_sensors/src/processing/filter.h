#pragma once

// EMA滤波器

float Filter_applyEMA(
    float newValue,
    float* state,
    float alpha);