#pragma once

#include <Arduino.h>

HardwareSerial& UARTManager_getUART1();

HardwareSerial& UARTManager_getUART2();

void UARTManager_beginUART1(
    uint32_t baudrate,
    int rxPin,
    int txPin);

void UARTManager_beginUART2(
    uint32_t baudrate,
    int rxPin,
    int txPin);