#include "air_quality.h"

const char* AirQuality_getLevel(
    int co2,
    float hcho)
{
    if(co2 < 800 &&
       hcho < 0.08f)
    {
        return "优";
    }

    if(co2 < 1200 &&
       hcho < 0.10f)
    {
        return "良";
    }

    if(co2 < 2000 &&
       hcho < 0.20f)
    {
        return "一般";
    }

    return "差";
}