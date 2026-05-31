// #pragma once

// void S8_begin();
// void S8_read(int* co2ppm);

#pragma once

bool S8_begin();

bool S8_read(
    int* co2ppm);

bool S8_isReady();