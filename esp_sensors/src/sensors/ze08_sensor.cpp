// #include <Arduino.h>
// #include "drivers/uart_manager.h"

// HardwareSerial ze08Serial(2);

// void ZE08_begin()
// {
//     // ze08Serial.begin(9600, SERIAL_8N1, 16, 17);
//     UARTManager_beginUART2(9600, 16, 17);
// }

// void ZE08_read(float* hcho)
// {
//     // HardwareSerial& serial = UARTManager_getUART2();
//     // // while (ze08Serial.available() >= 9)
//     // while(serial.available() >= 9)
//     // {
//     //     uint8_t buf[9];
//     //     // ze08Serial.readBytes(buf, 9);
//     //     serial.readBytes(buf, 9);

//     //     if (buf[0] == 0xFF)
//     //     {
//     //         uint16_t value =
//     //             ((uint16_t)buf[4] << 8) |
//     //             buf[5];

//     //         *hcho = value / 1000.0f;
//     //     }
//     // }
//     HardwareSerial& serial =
//         UARTManager_getUART2();

//     uint8_t buf[9];

//     while(UARTManager_hasData(serial))
//     {
//         if(
//             UARTManager_readFrame(
//                 serial,
//                 buf,
//                 9,
//                 50))
//         {
//             if(buf[0] == 0xFF)
//             {
//                 uint16_t value =
//                     ((uint16_t)buf[4] << 8)
//                     | buf[5];

//                 *hcho =
//                     value / 1000.0f;
//             }
//         }
//         else
//         {
//             break;
//         }
//     }
// }

#include "drivers/uart_manager.h"
#include "ze08_protocol.h"

static bool g_ready = false;

bool ZE08_begin()
{
    UARTManager_beginUART2(9600,16,17);

    g_ready = true; // UART可用即ready

    return g_ready;
}

bool ZE08_isReady()
{
    return g_ready;
}

// // ================= CRC8（ZE08） =================
// static uint8_t ZE08_crc(uint8_t *data)
// {
//     uint8_t sum = 0;

//     for(int i = 0; i < 8; i++)
//     {
//         sum += data[i];
//     }

//     return (~sum) + 1;
// }

bool ZE08_read(float* hcho)
{
    if(!g_ready)
    {
        return false;
    }

    HardwareSerial& serial =
        UARTManager_getUART2();

    uint8_t buf[9];

    if(!UARTManager_readFrame(
        serial,
        buf,
        9,
        100))
    {
        return false;
    }

    // // 帧头校验
    // if(buf[0] != 0xFF)
    // {
    //     return false;
    // }

    // // CRC校验（关键新增）
    // uint8_t crc = ZE08_crc(buf);

    // if(crc != buf[8])
    // {
    //     return false;
    // }

    // uint16_t value =
    //     ((uint16_t)buf[4] << 8)
    //     | buf[5];

    // *hcho = value / 1000.0f;

    // return true;

    ZE08Frame frame;

    if(!ZE08_parseFrame(buf, &frame))
    {
        return false;
    }

    *hcho = frame.hcho;

    return frame.valid;
}