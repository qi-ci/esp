// #include <Arduino.h>
#include "drivers/uart_manager.h"

// HardwareSerial ze08Serial(2);

void ZE08_begin()
{
    // ze08Serial.begin(9600, SERIAL_8N1, 16, 17);
    UARTManager_beginUART2(9600, 16, 17);
}

void ZE08_read(float* hcho)
{
    HardwareSerial& serial = UARTManager_getUART2();
    // while (ze08Serial.available() >= 9)
    while(serial.available() >= 9)
    {
        uint8_t buf[9];
        // ze08Serial.readBytes(buf, 9);
        serial.readBytes(buf, 9);

        if (buf[0] == 0xFF)
        {
            uint16_t value =
                ((uint16_t)buf[4] << 8) |
                buf[5];

            *hcho = value / 1000.0f;
        }
    }
}