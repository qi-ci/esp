/**
 * ESP32 多参数环境监测仪 - 主程序
 * 
 * 功能: 监测温度、湿度、甲醛、CO2浓度
 * 功耗模式: B节能模式(15分钟采样间隔)
 * 传感器优化: 集成寿命管理和温度补偿
 * 
 * 硬件配置:
 * - HTU21D (I2C): GPIO 21(SDA), 22(SCL)
 * - ZE08-CH2O (UART2): GPIO 16(RX), 17(TX)
 * - S8 0053 (UART1): GPIO 4(RX), 5(TX)
 * 
 * 作者: IoT Expert Assistant
 * 版本: V1.0
 * 日期: 2024-01-16
 */

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

// ==================== 传感器库引用 ====================
#include "HTU21D.h"  // 温湿度传感器
#include "ZE08_CH2O.h"  // 甲醛传感器
#include "S8_CO2.h"  // CO2传感器

// ==================== 引脚定义 ====================
#define I2C_SDA_PIN     21
#define I2C_SCL_PIN     22
#define UART1_RX_PIN    4   // S8 CO2
#define UART1_TX_PIN    5
#define UART2_RX_PIN    16  // ZE08 CH2O
#define UART2_TX_PIN    17
#define LED_PIN         2   // 板载LED

// ==================== WiFi配置 ====================
const char* WIFI_SSID = "TPGuest_3BD4";
const char* WIFI_PASSWORD = "yxmy211609";
const uint16_t SERVER_PORT = 8080;

// 服务器配置（可根据实际情况修改）
// 选项1: 使用广播地址自动发现（推荐用于开发测试）
// 选项2: 指定具体的服务器 IP
#define USE_BROADCAST_DISCOVERY true  // 设置为 false 时使用固定 IP
const char* SERVER_IP = "192.168.0.100";  // 修改为您的 APP 服务器实际 IP

// ==================== 功耗模式配置 (方案B: 节能模式) ====================
#define NORMAL_INTERVAL     900   // 正常时段采样间隔: 15分钟(秒)
#define NIGHT_INTERVAL      3600  // 夜间时段采样间隔: 60分钟(秒)
#define NIGHT_START_HOUR    23    // 夜间模式开始时间
#define NIGHT_END_HOUR      7     // 夜间模式结束时间

// ==================== 传感器寿命管理配置 ====================
#define HCHO_SENSOR_LIFE_HOURS    15000  // ZE08预期寿命(小时)
#define S8_ABC_CALIBRATION_DAYS   7      // S8 ABC校准周期(天)
#define HTU21D_RESET_DAYS         30     // HTU21D软复位周期(天)
#define ZE08_CALIBRATION_DAYS     90     // ZE08零点校准周期(天)

// ==================== 温度补偿配置 ====================
#define TEMP_COMPENSATION_ENABLED true
#define HUMIDITY_COMP_FACTOR      0.001   // 湿度温度补偿系数
#define HCHO_TEMP_FACTOR          0.005   // 甲醛温度补偿系数
#define REFERENCE_TEMP            25.0    // 参考温度

// ==================== 数据结构定义 ====================
typedef struct {
    float temperature;      // 温度(°C)
    float humidity;         // 湿度(%RH)
    float formaldehyde;     // 甲醛(ppm)
    float co2;             // CO2(ppm)
    uint32_t timestamp;    // 时间戳
    float latitude;        // 纬度
    float longitude;       // 经度
    uint8_t battery_level; // 电池电量(%)
} SensorData;

typedef struct {
    uint32_t hcho_work_hours;     // ZE08累计工作时长
    uint32_t s8_work_hours;       // S8累计工作时长
    uint32_t last_abc_calibration; // 上次ABC校准时间戳
    uint32_t last_ze08_calib;     // 上次ZE08校准时间戳
    uint32_t last_htu21d_reset;   // 上次HTU21D复位时间戳
} SensorLifeInfo;

// ==================== 全局变量 ====================
HTU21D htu21d;
ZE08_CH2O ze08;
S8_CO2 s8;

Preferences prefs;
SensorData current_data;
SensorLifeInfo sensor_life;

QueueHandle_t data_queue;
EventGroupHandle_t event_group;

const int EVENT_WIFI_CONNECTED = BIT0;
const int EVENT_DATA_READY = BIT1;
const int EVENT_SENSOR_ERROR = BIT2;

// ==================== 函数声明 ====================
void setup_wifi();
void task_sensor_collect(void *pvParameters);
void task_data_process(void *pvParameters);
void task_wifi_communicate(void *pvParameters);
void task_power_manage(void *pvParameters);

bool read_htu21d_with_compensation(float &temp, float &hum);
bool read_ze08_with_compensation(float &hcho);
bool read_s8_co2(float &co2);

void save_sensor_life_info();
void load_sensor_life_info();
void check_sensor_maintenance();
float compensate_temperature(float value, float temp, float factor);
void execute_sensor_maintenance();

// ==================== Arduino Setup ====================
void setup() {
    Serial.begin(115200);
    
    // 等待串口稳定（重要！）
    delay(1000);
    
    Serial.println("\n========================================");
    Serial.println("ESP32 Environmental Monitor Starting...");
    Serial.println("Power Mode: B (Energy Saving)");
    Serial.println("========================================\n");

    // 初始化LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // ===== 第1步：优先初始化 WiFi（给予充足时间）=====
    Serial.println("[Step 1] Initializing WiFi module...");
    WiFi.mode(WIFI_MODE_NULL);  // 先设置为 NULL 模式确保干净状态
    delay(500);
    WiFi.mode(WIFI_STA);        // 再设置为 STA 模式
    delay(500);
    
    // 检查 WiFi 是否就绪
    int wifi_status = WiFi.status();
    Serial.printf("[WiFi] Initial status: %d\n", wifi_status);
    if (wifi_status == -1) {
        Serial.println("[WARN] WiFi not ready yet, waiting...");
        delay(1000);  // 额外等待
    }

    // ===== 第2步：初始化 I2C 总线 =====
    Serial.println("[Step 2] Initializing I2C bus...");
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    delay(100);

    // ===== 第3步：初始化 UART 串口 =====
    Serial.println("[Step 3] Initializing UART ports...");
    Serial1.begin(9600, SERIAL_8N1, UART1_RX_PIN, UART1_TX_PIN);  // S8
    Serial2.begin(9600, SERIAL_8N1, UART2_RX_PIN, UART2_TX_PIN);  // ZE08
    delay(100);

    // ===== 第4步：初始化传感器 =====
    Serial.println("[Step 4] Initializing sensors...");
    htu21d.begin();
    ze08.begin(Serial2);
    s8.begin(Serial1);
    delay(200);  // 给传感器预热时间
    
    // ZE08 需要较长的预热时间（通常3分钟）
    Serial.println("[INFO] ZE08 sensor needs 3 minutes warmup time");
    Serial.println("       First readings may be unavailable during warmup");

    // ===== 第5步：加载持久化数据 =====
    Serial.println("[Step 5] Loading sensor life info...");
    prefs.begin("sensor_life", false);
    load_sensor_life_info();

    // ===== 第6步：创建 RTOS 对象 =====
    Serial.println("[Step 6] Creating RTOS objects...");
    data_queue = xQueueCreate(5, sizeof(SensorData));
    event_group = xEventGroupCreate();

    if (data_queue == NULL || event_group == NULL) {
        Serial.println("ERROR: Failed to create RTOS objects!");
        ESP.restart();
    }

    // ===== 第7步：连接 WiFi（此时 WiFi 模块已充分初始化）=====
    Serial.println("[Step 7] Connecting to WiFi...");
    setup_wifi();

    // ===== 第8步：创建 FreeRTOS 任务 =====
    Serial.println("[Step 8] Creating FreeRTOS tasks...");
    xTaskCreate(task_sensor_collect, "SensorCollect", 4096, NULL, 1, NULL);
    xTaskCreate(task_data_process, "DataProcess", 4096, NULL, 2, NULL);
    xTaskCreate(task_wifi_communicate, "WiFiComm", 8192, NULL, 3, NULL);
    xTaskCreate(task_power_manage, "PowerManage", 2048, NULL, 0, NULL);

    Serial.println("\n✅ System initialized successfully!");
    Serial.println("========================================\n");
}

// ==================== Arduino Loop ====================
void loop() {
    // FreeRTOS接管,主循环空闲
    vTaskDelay(pdMS_TO_TICKS(1000));
}

// ==================== WiFi连接 ====================
void setup_wifi() {
    Serial.print("Connecting to WiFi: ");
    Serial.println(WIFI_SSID);

    // 第一次尝试：正常连接
    int attempt = 0;
    const int max_attempts = 3;  // 最多尝试3次完整的连接流程
    
    while (attempt < max_attempts) {
        attempt++;
        Serial.printf("\n[WiFi] Connection attempt %d/%d\n", attempt, max_attempts);
        
        // 确保 WiFi 处于正确模式
        WiFi.mode(WIFI_STA);
        delay(300);
        
        // 断开之前的连接（如果有）
        WiFi.disconnect(true);
        delay(500);
        
        // 开始连接
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        
        int retry = 0;
        const int max_retries = 40;  // 每次尝试最多40次（20秒）
        
        while (WiFi.status() != WL_CONNECTED && retry < max_retries) {
            delay(500);
            if (retry % 10 == 0) {
                Serial.print(".");
            }
            retry++;
            
            // 每10次打印一次详细状态
            if (retry % 10 == 0) {
                int status = WiFi.status();
                Serial.printf("\n[WiFi] Progress: %d/%d, Status: %d", retry, max_retries, status);
                
                // 输出状态含义
                switch(status) {
                    case 0: Serial.print(" (WL_IDLE_STATUS)"); break;
                    case 1: Serial.print(" (WL_NO_SSID_AVAIL)"); break;
                    case 3: Serial.print(" (WL_CONNECTED ✓)"); break;
                    case 4: Serial.print(" (WL_CONNECT_FAILED)"); break;
                    case 6: Serial.print(" (WL_DISCONNECTED)"); break;
                    case -1: Serial.print(" (WL_NO_SHIELD)"); break;
                    default: Serial.printf(" (Unknown: %d)", status); break;
                }
                Serial.println();
                
                // 如果状态是 -1，额外等待让 WiFi 模块就绪
                if (status == -1) {
                    Serial.println("[WiFi] Module not ready, waiting...");
                    delay(2000);
                }
            }
        }

        // 检查是否连接成功
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\n✅ WiFi connected successfully!");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
            Serial.print("Signal strength (RSSI): ");
            Serial.print(WiFi.RSSI());
            Serial.println(" dBm");
            Serial.print("MAC Address: ");
            Serial.println(WiFi.macAddress());
            xEventGroupSetBits(event_group, EVENT_WIFI_CONNECTED);
            return;  // 成功，退出函数
        }
        
        // 连接失败，记录原因
        Serial.printf("\n❌ Attempt %d failed. Status: %d\n", attempt, WiFi.status());
        
        // 如果还有重试机会，等待后继续
        if (attempt < max_attempts) {
            Serial.println("[WiFi] Waiting before retry...");
            delay(3000);  // 等待3秒再试
        }
    }

    // 所有尝试都失败
    Serial.println("\n❌ WiFi connection failed after all attempts!");
    Serial.print("Final status code: ");
    int final_status = WiFi.status();
    Serial.println(final_status);
    
    // 提供详细的诊断信息
    Serial.println("\n===== Diagnostic Information =====");
    Serial.print("- MAC Address: ");
    Serial.println(WiFi.macAddress());
    Serial.print("- Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("- Subnet Mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("- Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("- DNS Server: ");
    Serial.println(WiFi.dnsIP());
    
    Serial.println("\n===== Failure Analysis =====");
    if (final_status == -1) {
        Serial.println("⚠️  CRITICAL: WiFi module not initialized (WL_NO_SHIELD)");
        Serial.println("   Possible causes:");
        Serial.println("   1. Insufficient power supply");
        Serial.println("   2. Hardware fault");
        Serial.println("   3. Firmware issue");
        Serial.println("   Solutions:");
        Serial.println("   → Press RESET button on ESP32");
        Serial.println("   → Use stable 5V/2A power adapter");
        Serial.println("   → Check USB cable quality");
    } else if (final_status == 4) {
        Serial.println("⚠️  Authentication failed");
        Serial.println("   → Verify WiFi password is correct");
    } else if (final_status == 1) {
        Serial.println("⚠️  SSID not found");
        Serial.println("   → Check router is powered on");
        Serial.println("   → Verify SSID name: " + String(WIFI_SSID));
    } else if (final_status == 6) {
        Serial.println("⚠️  Connection disconnected");
        Serial.println("   → Router may have rejected connection");
        Serial.println("   → Check MAC filtering settings");
    } else {
        Serial.println("⚠️  Unknown error");
        Serial.println("   → Try resetting ESP32");
    }
    
    Serial.println("\n[WARN] System will continue without WiFi connectivity");
    Serial.println("       Sensor data will be collected but not transmitted\n");
}

// ==================== 传感器采集任务 ====================
void task_sensor_collect(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    int ze08_retry_count = 0;
    const int ZE08_MAX_RETRIES = 5;  // ZE08 最大重试次数
    
    while (1) {
        SensorData data;
        bool success = true;

        Serial.println("\n[Sensor] Starting data collection...");

        // 读取HTU21D(带温度补偿)
        if (!read_htu21d_with_compensation(data.temperature, data.humidity)) {
            Serial.println("[ERROR] HTU21D read failed!");
            success = false;
        } else {
            Serial.printf("[OK] Temperature: %.1f°C, Humidity: %.1f%%\n", 
                         data.temperature, data.humidity);
        }

        // 读取ZE08(带温度补偿) - 增加重试机制
        bool ze08_success = false;
        for (int retry = 0; retry < ZE08_MAX_RETRIES && !ze08_success; retry++) {
            if (retry > 0) {
                Serial.printf("[ZE08] Retry %d/%d...\n", retry, ZE08_MAX_RETRIES);
                delay(100);  // 等待100ms再试
            }
            ze08_success = read_ze08_with_compensation(data.formaldehyde);
        }
        
        if (!ze08_success) {
            Serial.println("[ERROR] ZE08 read failed after retries!");
            Serial.println("   Possible causes:");
            Serial.println("   - Sensor not connected (check GPIO 16/17)");
            Serial.println("   - Sensor needs warmup (wait 3 minutes)");
            Serial.println("   - UART communication error");
            data.formaldehyde = 0.0;  // 设置默认值
            success = false;
        } else {
            Serial.printf("[OK] Formaldehyde: %.3f ppm\n", data.formaldehyde);
            ze08_retry_count = 0;  // 重置重试计数
        }

        // 读取S8
        if (!read_s8_co2(data.co2)) {
            Serial.println("[ERROR] S8 read failed!");
            Serial.println("   Possible causes:");
            Serial.println("   - Sensor not connected (check GPIO 4/5)");
            Serial.println("   - Sensor needs calibration");
            data.co2 = 0.0;  // 设置默认值
            success = false;
        } else {
            Serial.printf("[OK] CO2: %.0f ppm\n", data.co2);
        }

        if (success) {
            data.timestamp = millis() / 1000;
            data.latitude = 0.0;   // APP获取
            data.longitude = 0.0;  // APP获取
            data.battery_level = 100; // USB供电,假设100%

            // 发送到队列
            if (xQueueSend(data_queue, &data, pdMS_TO_TICKS(100)) != pdTRUE) {
                Serial.println("[ERROR] Failed to send data to queue");
            } else {
                xEventGroupSetBits(event_group, EVENT_DATA_READY);
                Serial.println("[Sensor] Data collection completed successfully");
            }
        } else {
            xEventGroupSetBits(event_group, EVENT_SENSOR_ERROR);
            Serial.println("[WARN] Some sensors failed, continuing with partial data");
        }

        // 更新传感器工作时长
        sensor_life.hcho_work_hours += 1;  // 每次采集约1小时(节能模式)
        sensor_life.s8_work_hours += 1;
        save_sensor_life_info();

        // 检查维护需求
        check_sensor_maintenance();

        // 动态调整采样间隔(节能模式)
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);
        int hour = timeinfo.tm_hour;
        bool is_night = (hour >= NIGHT_START_HOUR || hour < NIGHT_END_HOUR);
        TickType_t interval = is_night ? NIGHT_INTERVAL : NORMAL_INTERVAL;

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(interval * 1000));
    }
}

// ==================== 数据处理任务 ====================
void task_data_process(void *pvParameters) {
    while (1) {
        SensorData data;
        
        // 等待数据
        if (xQueueReceive(data_queue, &data, portMAX_DELAY) == pdTRUE) {
            Serial.printf("[Process] Data: T=%.1f H=%.1f HCHO=%.3f CO2=%.0f\n",
                         data.temperature, data.humidity, 
                         data.formaldehyde, data.co2);

            // 数据验证
            if (data.temperature < -20 || data.temperature > 60) {
                Serial.println("[WARN] Temperature out of range");
            }
            if (data.humidity < 0 || data.humidity > 100) {
                Serial.println("[WARN] Humidity out of range");
            }
            if (data.formaldehyde > 5.0) {
                Serial.println("[WARN] Formaldehyde out of range");
            }
            if (data.co2 > 5000) {
                Serial.println("[WARN] CO2 out of range");
            }

            // 数据质量评估
            float quality = evaluate_data_quality(data);
            Serial.printf("[Process] Data quality: %.1f%%\n", quality);
        }
    }
}

// ==================== WiFi通信任务 ====================
void task_wifi_communicate(void *pvParameters) {
    int connection_fail_count = 0;
    const int MAX_FAIL_COUNT = 10;  // 连续失败10次后停止尝试
    
    while (1) {
        // 等待WiFi连接和数据就绪
        xEventGroupWaitBits(event_group, 
                           EVENT_WIFI_CONNECTED | EVENT_DATA_READY,
                           pdTRUE, pdFALSE, portMAX_DELAY);

        if (WiFi.status() == WL_CONNECTED) {
            // 如果连续失败次数过多，降低尝试频率以节能
            if (connection_fail_count >= MAX_FAIL_COUNT) {
                Serial.println("[WiFi] Server unreachable, reducing attempt frequency");
                vTaskDelay(pdMS_TO_TICKS(60000));  // 等待1分钟再试
                continue;
            }
            
            HTTPClient http;
            
            // 构建JSON数据
            String json = build_json_data(current_data);
            
            // 确定服务器地址
            String server_addr;
            #if USE_BROADCAST_DISCOVERY
                // 使用网关地址作为服务器（常见配置）
                IPAddress gateway = WiFi.gatewayIP();
                server_addr = "http://" + gateway.toString() + ":" + String(SERVER_PORT) + "/api/data";
            #else
                server_addr = "http://" + String(SERVER_IP) + ":" + String(SERVER_PORT) + "/api/data";
            #endif
            
            Serial.printf("[WiFi] Attempting to send data to: %s\n", server_addr.c_str());
            
            http.begin(server_addr);
            http.addHeader("Content-Type", "application/json");
            http.setTimeout(5000);  // 设置5秒超时
            
            int httpResponseCode = http.POST(json);
            
            if (httpResponseCode > 0) {
                String response = http.getString();
                Serial.printf("[WiFi] ✅ Data sent successfully! Response: %d\n", httpResponseCode);
                Serial.printf("[WiFi] Server response: %s\n", response.c_str());
                connection_fail_count = 0;  // 重置失败计数
            } else {
                connection_fail_count++;
                Serial.printf("[WiFi] ❌ HTTP POST failed: %d (fail count: %d/%d)\n", 
                             httpResponseCode, connection_fail_count, MAX_FAIL_COUNT);
                
                // 提供详细的错误诊断
                if (httpResponseCode == -1) {
                    Serial.println("   → Connection refused or timeout");
                    Serial.println("   → Check if server is running and accessible");
                    Serial.println("   → Verify firewall settings");
                } else if (httpResponseCode == -2) {
                    Serial.println("   → Send header failed");
                } else if (httpResponseCode == -3) {
                    Serial.println("   → Send payload failed");
                } else if (httpResponseCode == -4) {
                    Serial.println("   → Not connected");
                } else if (httpResponseCode == -5) {
                    Serial.println("   → Connection lost");
                } else if (httpResponseCode == -11) {
                    Serial.println("   → Read timeout");
                }
            }
            
            http.end();
        }

        // 正常情况每5秒尝试一次
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

// ==================== 电源管理任务 ====================
void task_power_manage(void *pvParameters) {
    while (1) {
        // 检查是否进入夜间模式
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);

        int hour = timeinfo.tm_hour;
        bool is_night = (hour >= NIGHT_START_HOUR || hour < NIGHT_END_HOUR);

        if (is_night) {
            Serial.println("[Power] Entering night mode (low frequency)");
            // 降低CPU频率
            setCpuFrequencyMhz(80);
        } else {
            Serial.println("[Power] Normal day mode");
            setCpuFrequencyMhz(240);
        }

        // 每小时检查一次
        vTaskDelay(pdMS_TO_TICKS(3600000));
    }
}

// ==================== HTU21D读取(带温度补偿) ====================
bool read_htu21d_with_compensation(float &temp, float &hum) {
    if (!htu21d.read()) {
        return false;
    }

    temp = htu21d.readTemperature();
    hum = htu21d.readHumidity();

    // 温度补偿
    if (TEMP_COMPENSATION_ENABLED) {
        hum = compensate_temperature(hum, temp, HUMIDITY_COMP_FACTOR);
        Serial.printf("[HTU21D] Compensated humidity: %.1f%%\n", hum);
    }

    return true;
}

// ==================== ZE08读取(带温度补偿) ====================
bool read_ze08_with_compensation(float &hcho) {
    if (!ze08.update()) {
        return false;
    }

    hcho = ze08.getFormaldehyde();

    // 温度补偿
    if (TEMP_COMPENSATION_ENABLED && current_data.temperature != 0) {
        hcho = compensate_temperature(hcho, current_data.temperature, HCHO_TEMP_FACTOR);
        Serial.printf("[ZE08] Compensated HCHO: %.3f ppm\n", hcho);
    }

    return true;
}

// ==================== S8读取 ====================
bool read_s8_co2(float &co2) {
    if (!s8.update()) {
        return false;
    }

    co2 = s8.getCO2();
    return true;
}

// ==================== 温度补偿算法 ====================
float compensate_temperature(float value, float temp, float factor) {
    // 补偿公式: value_compensated = value * (1 + factor * (temp - reference_temp))
    float compensation = 1.0 + factor * (temp - REFERENCE_TEMP);
    return value * compensation;
}

// ==================== 传感器寿命信息管理 ====================
void save_sensor_life_info() {
    prefs.putUInt("hcho_hours", sensor_life.hcho_work_hours);
    prefs.putUInt("s8_hours", sensor_life.s8_work_hours);
    prefs.putUInt("last_abc", sensor_life.last_abc_calibration);
    prefs.putUInt("last_ze08", sensor_life.last_ze08_calib);
    prefs.putUInt("last_htu", sensor_life.last_htu21d_reset);
}

void load_sensor_life_info() {
    sensor_life.hcho_work_hours = prefs.getUInt("hcho_hours", 0);
    sensor_life.s8_work_hours = prefs.getUInt("s8_hours", 0);
    sensor_life.last_abc_calibration = prefs.getUInt("last_abc", 0);
    sensor_life.last_ze08_calib = prefs.getUInt("last_ze08", 0);
    sensor_life.last_htu21d_reset = prefs.getUInt("last_htu", 0);
}

// ==================== 传感器维护检查 ====================
void check_sensor_maintenance() {
    uint32_t current_time = millis() / 1000;

    // ZE08寿命检查
    if (sensor_life.hcho_work_hours >= HCHO_SENSOR_LIFE_HOURS) {
        Serial.println("[ALERT] ZE08 sensor end of life! Please replace.");
        // TODO: 发送告警到APP
    }

    // S8 ABC校准检查(每7天)
    uint32_t days_since_abc = (current_time - sensor_life.last_abc_calibration) / 86400;
    if (days_since_abc >= S8_ABC_CALIBRATION_DAYS) {
        Serial.println("[MAINT] Executing S8 ABC calibration...");
        s8.executeABCCalibration();
        sensor_life.last_abc_calibration = current_time;
        save_sensor_life_info();
    }

    // HTU21D软复位检查(每30天)
    uint32_t days_since_reset = (current_time - sensor_life.last_htu21d_reset) / 86400;
    if (days_since_reset >= HTU21D_RESET_DAYS) {
        Serial.println("[MAINT] Executing HTU21D soft reset...");
        htu21d.softReset();
        sensor_life.last_htu21d_reset = current_time;
        save_sensor_life_info();
    }

    // ZE08零点校准检查(每90天)
    uint32_t days_since_ze08 = (current_time - sensor_life.last_ze08_calib) / 86400;
    if (days_since_ze08 >= ZE08_CALIBRATION_DAYS) {
        Serial.println("[MAINT] ZE08 zero calibration needed (in clean air)");
        // TODO: 提醒用户在洁净空气中执行校准
    }
}

// ==================== 辅助函数 ====================
String build_json_data(SensorData &data) {
    String json = "{";
    json += "\"status\":\"success\",";
    json += "\"data\":{";
    json += "\"temperature\":" + String(data.temperature, 1) + ",";
    json += "\"humidity\":" + String(data.humidity, 1) + ",";
    json += "\"formaldehyde\":" + String(data.formaldehyde, 3) + ",";
    json += "\"co2\":" + String(data.co2, 0) + ",";
    json += "\"timestamp\":" + String(data.timestamp) + ",";
    json += "\"battery_level\":" + String(data.battery_level);
    json += "}}";
    return json;
}

float evaluate_data_quality(SensorData &data) {
    // 简化的数据质量评估
    float quality = 100.0;
    
    // 根据数值范围调整质量评分
    if (data.temperature < 0 || data.temperature > 40) quality -= 20;
    if (data.humidity < 20 || data.humidity > 80) quality -= 15;
    if (data.formaldehyde > 1.0) quality -= 10;
    if (data.co2 > 2000) quality -= 10;
    
    return quality;
}
