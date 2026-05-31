// #include <Arduino.h>
// #include "core/sensor_manager.h"
// #include "app/serial_reporter.h"
// #include "core/system_init.h"

// void setup()
// {
//     SystemInit();
//     SensorManager_begin();
//     SerialReporter_begin();
// }

// void loop()
// {
//     SensorManager_update();
//     SerialReporter_print();
//     delay(5000);
// }

#include <Arduino.h>

//#include "core/system_state.h"
#include "core/sensor_manager.h"
#include "core/system_init.h"
#include "app/serial_reporter.h"

void setup()
{
    SystemInit();
    SensorManager_begin();
}

void loop()
{
    SensorManager_update();

    // Serial.println();

    // Serial.println(
    //     "====================");

    // Serial.printf(
    //     "Temperature : %.2f C\n",
    //     g_systemState.temperature);

    // Serial.printf(
    //     "Humidity    : %.2f %%\n",
    //     g_systemState.humidity);

    // Serial.printf(
    //     "CO2         : %d ppm\n",
    //     g_systemState.co2);

    // Serial.printf(
    //     "HCHO        : %.3f mg/m3\n",
    //     g_systemState.hcho);

    // Serial.printf(
    //     "Air Quality : %s\n",
    //     g_systemState.airQuality);

    // Serial.println(
    //     "====================");

    SerialReporter_printHuman();

    delay(1000);
}