/**
 * HTU21D 温湿度传感器驱动
 * 
 * I2C地址: 0x40
 * 供电: 1.5V-3.6V (使用ESP32 3.3V)
 */

#ifndef HTU21D_H
#define HTU21D_H

#include <Wire.h>

#define HTU21D_ADDR         0x40
#define TRIG_TEMP_MEAS_HM   0xE3
#define TRIG_HUMID_MEAS_HM  0xE5
#define SOFT_RESET          0xFE
#define WRITE_USER_REG      0xE6
#define READ_USER_REG       0xE7

class HTU21D {
private:
    float temperature;
    float humidity;
    
public:
    HTU21D() : temperature(0), humidity(0) {}
    
    void begin() {
        Wire.begin();
        softReset();
        delay(15);  // 等待复位完成
    }
    
    bool read() {
        return readTemperature() && readHumidity();
    }
    
    float readTemperature() {
        Wire.beginTransmission(HTU21D_ADDR);
        Wire.write(TRIG_TEMP_MEAS_HM);
        Wire.endTransmission();
        
        delay(50);  // 等待测量完成
        
        Wire.requestFrom(HTU21D_ADDR, 3);
        if (Wire.available() != 3) return false;
        
        uint8_t msb = Wire.read();
        uint8_t lsb = Wire.read();
        uint8_t crc = Wire.read();
        
        uint16_t raw = (msb << 8) | lsb;
        raw &= 0xFFFC;  // 清除状态位
        
        // 温度计算公式: T = -46.85 + 175.72 * (RAW / 2^16)
        temperature = -46.85 + 175.72 * ((float)raw / 65536.0);
        
        return temperature;
    }
    
    float readHumidity() {
        Wire.beginTransmission(HTU21D_ADDR);
        Wire.write(TRIG_HUMID_MEAS_HM);
        Wire.endTransmission();
        
        delay(50);
        
        Wire.requestFrom(HTU21D_ADDR, 3);
        if (Wire.available() != 3) return false;
        
        uint8_t msb = Wire.read();
        uint8_t lsb = Wire.read();
        uint8_t crc = Wire.read();
        
        uint16_t raw = (msb << 8) | lsb;
        raw &= 0xFFFC;
        
        // 湿度计算公式: RH = -6 + 125 * (RAW / 2^16)
        humidity = -6.0 + 125.0 * ((float)raw / 65536.0);
        
        // 限制范围
        if (humidity < 0) humidity = 0;
        if (humidity > 100) humidity = 100;
        
        return humidity;
    }
    
    void softReset() {
        Wire.beginTransmission(HTU21D_ADDR);
        Wire.write(SOFT_RESET);
        Wire.endTransmission();
    }
    
    float getTemperature() { return temperature; }
    float getHumidity() { return humidity; }
};

#endif
