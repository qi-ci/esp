#include <Arduino.h>

#include "../drivers/i2c_manager.h"
#include "../drivers/uart_manager.h"

void SystemInit()
{
    Serial.begin(115200);

    I2CManager_begin(21,22);
    UARTManager_beginUART1(9600, 4, 5);
    UARTManager_beginUART2(9600, 16, 17);

    delay(2000);
}