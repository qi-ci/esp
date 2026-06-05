#include "i2c_manager.h"

static uint8_t g_sdaPin;
static uint8_t g_sclPin;

void I2CManager_beginI2C1(
    uint8_t sdaPin,
    uint8_t sclPin)
{
    g_sdaPin = sdaPin;
    g_sclPin = sclPin;

    Wire.begin(
        sdaPin,
        sclPin);
}

TwoWire& I2CManager_getBus()
{
    return Wire;
}


// =========================
// 设备检测
// =========================

bool I2CManager_deviceExists(
    uint8_t address)
{
    Wire.beginTransmission(address);

    return
        Wire.endTransmission()
        == 0;
}


// =========================
// I2C扫描
// =========================

uint8_t I2CManager_scan(
    uint8_t* addresses,
    uint8_t maxCount)
{
    uint8_t count = 0;

    for(uint8_t addr = 1;
        addr < 127;
        addr++)
    {
        if(I2CManager_deviceExists(addr))
        {
            if(count < maxCount)
            {
                addresses[count] = addr;
            }

            count++;
        }
    }

    return count;
}


// =========================
// 总线恢复
// =========================

bool I2CManager_recoverBus()
{
    pinMode(
        g_sclPin,
        OUTPUT_OPEN_DRAIN);

    pinMode(
        g_sdaPin,
        INPUT_PULLUP);

    for(int i = 0; i < 9; i++)
    {
        digitalWrite(g_sclPin, LOW);
        delayMicroseconds(5);

        digitalWrite(g_sclPin, HIGH);
        delayMicroseconds(5);
    }

    Wire.begin(
        g_sdaPin,
        g_sclPin);

    return true;
}