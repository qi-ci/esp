// #include <Arduino.h>
#include "drivers/uart_manager.h"

// HardwareSerial s8Serial(1);

// ===== CRC（原样搬过来） =====
uint16_t modbusCRC(uint8_t *buf, uint8_t len)
{
    uint16_t crc = 0xFFFF;

    for (uint8_t pos = 0; pos < len; pos++)
    {
        crc ^= (uint16_t)buf[pos];

        for (int i = 8; i != 0; i--)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}

void S8_begin()
{
    // s8Serial.begin(9600, SERIAL_8N1, 4, 5);
    UARTManager_beginUART1(9600, 4, 5);
}

void S8_read(int* co2ppm)
{
    HardwareSerial& serial = UARTManager_getUART1();

    uint8_t cmd[8] =
    {
        0xFE, 0x04, 0x00, 0x03,
        0x00, 0x01, 0x00, 0x00
    };

    uint16_t crc = modbusCRC(cmd, 6);
    cmd[6] = crc & 0xFF;
    cmd[7] = crc >> 8;

    // // while (s8Serial.available())
    // //     s8Serial.read();

    // // s8Serial.write(cmd, 8);
    // while(serial.available())
    // {
    //     serial.read();
    // }

    // serial.write(cmd,8);

    // delay(120);

    // // if (s8Serial.available() >= 7)
    // // {
    // //     uint8_t buf[7];
    // //     s8Serial.readBytes(buf, 7);

    // //     *co2ppm =
    // //         ((uint16_t)buf[3] << 8) |
    // //         buf[4];
    // // }
    // if (serial.available() >= 7)
    // {
    //     uint8_t buf[7];
    //     serial.readBytes(buf, 7);
    
    //     *co2ppm =
    //         ((uint16_t)buf[3] << 8) |
    //         buf[4];
    // }
    // else
    // {
    //     *co2ppm = -1;
    // }
    UARTManager_clearBuffer(
        serial);

    serial.write(
        cmd,
        8);

    uint8_t buf[7];

    if(
        UARTManager_readFrame(
            serial,
            buf,
            7,
            200))
    {
        *co2ppm =
            ((uint16_t)buf[3] << 8)
            | buf[4];
    }
    else
    {
        *co2ppm = -1;
    }
}