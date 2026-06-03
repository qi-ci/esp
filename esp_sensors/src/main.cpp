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

// #include <Arduino.h>

// //#include "core/system_state.h"
// #include "core/sensor_manager.h"
// #include "core/system_init.h"
// #include "app/serial_reporter.h"

// void setup()
// {
//     SystemInit();
//     SensorManager_begin();
// }

// void loop()
// {
//     SensorManager_update();

//     SerialReporter_printHuman();

//     delay(1000);
// }

#include "app/app_init.h"

// ======================
// Arduino setup
// ======================
void setup()
{
    App_init();
}

// ======================
// 主循环（调度器）
// ======================
void loop()
{
    App_loop();
}