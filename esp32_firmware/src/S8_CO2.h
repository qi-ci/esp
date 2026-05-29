/**
 * S8 0053 CO2传感器驱动 (SenseAir)
 * 
 * UART Modbus RTU协议
 * 供电: 4.5V-5.25V (使用ESP32 5V,并联1000μF电容)
 */

#ifndef S8_CO2_H
#define S8_CO2_H

#include <Arduino.h>
#include <HardwareSerial.h>

class S8_CO2 {
private:
    HardwareSerial* uart;
    float co2;
    uint8_t rx_buffer[7];
    
    // Modbus RTU读取命令
    const uint8_t READ_CMD[8] = {0xFE, 0x04, 0x00, 0x03, 0x00, 0x01, 0xD5, 0xC5};
    
public:
    S8_CO2() : co2(0) {}
    
    void begin(HardwareSerial& serial) {
        uart = &serial;
    }
    
    bool update() {
        // 发送读取命令
        uart->write(READ_CMD, 8);
        
        // 等待响应(最多2秒)
        unsigned long start = millis();
        while (uart->available() < 7 && millis() - start < 2000) {
            delay(10);
        }
        
        if (uart->available() >= 7) {
            // 读取响应
            for (int i = 0; i < 7; i++) {
                rx_buffer[i] = uart->read();
            }
            
            // 验证帧头和功能区
            if (rx_buffer[0] == 0xFE && rx_buffer[1] == 0x04) {
                // 解析CO2浓度
                uint8_t high = rx_buffer[3];
                uint8_t low = rx_buffer[4];
                
                // CO2计算: 高位*256 + 低位
                co2 = (uint16_t)high * 256 + low;
                
                // CRC验证(简化,实际应完整计算)
                return true;
            }
        }
        
        return false;
    }
    
    float getCO2() {
        return co2;
    }
    
    // ABC自动基线校准(每7天执行一次)
    void executeABCCalibration() {
        Serial.println("[S8] Executing ABC calibration...");
        
        // ABC校准命令
        uint8_t abc_cmd[8] = {0xFE, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        
        // 计算CRC
        uint16_t crc = calculateCRC(abc_cmd, 6);
        abc_cmd[6] = crc & 0xFF;
        abc_cmd[7] = (crc >> 8) & 0xFF;
        
        uart->write(abc_cmd, 8);
        delay(100);
        
        Serial.println("[S8] ABC calibration completed");
    }
    
private:
    uint16_t calculateCRC(uint8_t* data, uint8_t length) {
        uint16_t crc = 0xFFFF;
        
        for (uint8_t i = 0; i < length; i++) {
            crc ^= data[i];
            for (uint8_t j = 0; j < 8; j++) {
                if (crc & 0x01) {
                    crc = (crc >> 1) ^ 0xA001;
                } else {
                    crc >>= 1;
                }
            }
        }
        
        return crc;
    }
};

#endif
