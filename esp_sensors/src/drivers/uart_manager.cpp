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