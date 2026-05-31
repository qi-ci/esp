#include "uart_manager.h"

static HardwareSerial uart1(1);
static HardwareSerial uart2(2);

HardwareSerial& UARTManager_getUART1()
{
    return uart1;
}

HardwareSerial& UARTManager_getUART2()
{
    return uart2;
}

void UARTManager_beginUART1(
    uint32_t baudrate,
    int rxPin,
    int txPin)
{
    uart1.begin(
        baudrate,
        SERIAL_8N1,
        rxPin,
        txPin);
}

void UARTManager_beginUART2(
    uint32_t baudrate,
    int rxPin,
    int txPin)
{
    uart2.begin(
        baudrate,
        SERIAL_8N1,
        rxPin,
        txPin);
}


// =========================
// Buffer管理
// =========================

void UARTManager_clearBuffer(
    HardwareSerial& serial)
{
    while(serial.available())
    {
        serial.read();
    }
}


// =========================
// 超时读取
// =========================

size_t UARTManager_readBytesTimeout(
    HardwareSerial& serial,
    uint8_t* buffer,
    size_t length,
    uint32_t timeoutMs)
{
    uint32_t start =
        millis();

    size_t received = 0;

    while(received < length)
    {
        while(serial.available())
        {
            buffer[received++] =
                serial.read();

            if(received >= length)
            {
                return received;
            }
        }

        if(millis() - start > timeoutMs)
        {
            break;
        }
    }

    return received;
}


// =========================
// 帧读取
// =========================

bool UARTManager_readFrame(
    HardwareSerial& serial,
    uint8_t* buffer,
    size_t frameSize,
    uint32_t timeoutMs)
{
    return
        UARTManager_readBytesTimeout(
            serial,
            buffer,
            frameSize,
            timeoutMs)
        == frameSize;
}


// =========================
// 串口状态
// =========================

bool UARTManager_hasData(
    HardwareSerial& serial)
{
    return serial.available() > 0;
}