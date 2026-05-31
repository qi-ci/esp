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


// =========================
// Buffer管理
// =========================

void UARTManager_clearBuffer(
    HardwareSerial& serial);


// =========================
// 超时读取
// =========================

size_t UARTManager_readBytesTimeout(
    HardwareSerial& serial,
    uint8_t* buffer,
    size_t length,
    uint32_t timeoutMs);


// =========================
// 帧读取
// =========================

bool UARTManager_readFrame(
    HardwareSerial& serial,
    uint8_t* buffer,
    size_t frameSize,
    uint32_t timeoutMs);


// =========================
// 串口状态
// =========================

bool UARTManager_hasData(
    HardwareSerial& serial);