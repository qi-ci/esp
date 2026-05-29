/**
 * ZE08-CH2O 甲醛传感器驱动
 * 
 * UART通信,主动上传模式
 * 供电: 3.7V-5.5V (使用ESP32 5V)
 */

#ifndef ZE08_CH2O_H
#define ZE08_CH2O_H

#include <Arduino.h>
#include <HardwareSerial.h>

class ZE08_CH2O {
private:
    HardwareSerial* uart;
    float formaldehyde;
    uint8_t buffer[9];
    bool dataReady;
    
public:
    ZE08_CH2O() : formaldehyde(0), dataReady(false) {}
    
    void begin(HardwareSerial& serial) {
        uart = &serial;
        formaldehyde = 0;
        dataReady = false;
    }
    
    bool update() {
        // 读取9字节数据帧
        if (uart->available() >= 9) {
            if (uart->read() == 0xFF) {  // 帧头1
                buffer[0] = 0xFF;
                
                if (uart->read() == 0x17) {  // 帧头2
                    buffer[1] = 0x17;
                    
                    // 读取剩余7字节
                    for (int i = 2; i < 9; i++) {
                        buffer[i] = uart->read();
                    }
                    
                    // 验证帧尾
                    if (buffer[7] == 0xFF && buffer[8] == 0xFF) {
                        // 解析甲醛浓度
                        uint8_t high = buffer[4];
                        uint8_t low = buffer[5];
                        
                        // 浓度计算: (高位*256 + 低位) / 1000.0
                        formaldehyde = ((uint16_t)high * 256 + low) / 1000.0;
                        
                        // 校验和验证
                        uint8_t sum = 0;
                        for (int i = 0; i < 6; i++) {
                            sum += buffer[i];
                        }
                        
                        if (sum == buffer[6]) {
                            dataReady = true;
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }
    
    float getFormaldehyde() {
        return formaldehyde;
    }
    
    bool isDataReady() {
        return dataReady;
    }
};

#endif
