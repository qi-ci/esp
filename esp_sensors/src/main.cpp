#include <Arduino.h>
#include "core/sensor_manager.h"
#include "app/serial_reporter.h"
#include "core/system_init.h"

void setup()
{
    SystemInit();
    SensorManager_begin();
    SerialReporter_begin();
}

void loop()
{
    SensorManager_update();
    SerialReporter_print();
    delay(5000);
}