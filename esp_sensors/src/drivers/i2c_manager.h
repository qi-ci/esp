#pragma once

#include <Arduino.h>
#include <Wire.h>

void I2CManager_begin(
    uint8_t sdaPin,
    uint8_t sclPin);

TwoWire& I2CManager_getBus();

// =========================
// 设备检测
// =========================

bool I2CManager_deviceExists(
    uint8_t address);


// =========================
// I2C扫描
// =========================

uint8_t I2CManager_scan(
    uint8_t* addresses,
    uint8_t maxCount);


// =========================
// 总线恢复
// =========================

bool I2CManager_recoverBus();