# 根据上一条的所有优化建议，先给出完成这四个层面后的文件架构tree，以显示需要创建哪些h和cpp文件，好帮助全局理解有哪些优化模块

很好，这一步你是在做真正的**物联网工程架构设计**了，而不是写单文件脚本。

我根据你上一条提到的4个优化层面，帮你整理成一个**工业级最小可扩展架构（ESP32空气监测V2）**。

---

# 🧠 总体分层（你要优化的4层）

我们先把你的系统拆成4层：

```
① 驱动层（Driver）
② 数据采集层（Sensor Layer）
③ 数据处理层（Processing Layer）
④ 应用输出层（Application Layer）
```

---

# 📁 最终项目结构 Tree（推荐架构）

```text id="tree1"
air-monitor-v2/
├── platformio.ini
└── src/
    ├── main.cpp
    │
    ├── drivers/
    │   ├── i2c_manager.h
    │   ├── i2c_manager.cpp
    │   │
    │   ├── uart_manager.h
    │   └── uart_manager.cpp
    │
    ├── sensors/
    │   ├── htu21d_sensor.h
    │   ├── htu21d_sensor.cpp
    │   │
    │   ├── ze08_sensor.h
    │   ├── ze08_sensor.cpp
    │   │
    │   ├── s8_sensor.h
    │   └── s8_sensor.cpp
    │
    ├── processing/
    │   ├── filter.h
    │   ├── filter.cpp
    │   │
    │   ├── air_quality.h
    │   ├── air_quality.cpp
    │   │
    │   ├── data_validator.h
    │   └── data_validator.cpp
    │
    ├── core/
    │   ├── sensor_manager.h
    │   ├── sensor_manager.cpp
    │   │
    │   ├── system_state.h
    │   └── system_state.cpp
    │
    └── app/
        ├── output_formatter.h
        ├── output_formatter.cpp
        │
        ├── serial_reporter.h
        ├── serial_reporter.cpp
        │
        ├── mqtt_client.h        (可选扩展)
        └── mqtt_client.cpp      (可选扩展)
```

---

# 🧠 每一层的作用解释（很关键）

---

# ① drivers（底层通信抽象层）

## 📌 i2c_manager

```text id="i2c"
统一管理 I2C
```

作用：

* 初始化 I2C
* 扫描设备
* 统一读写接口

👉 防止 HTU21D 直接依赖 Wire

---

## 📌 uart_manager

```text id="uart"
统一管理 UART0/1/2
```

作用：

* S8 UART
* ZE08 UART
* 超时控制
* buffer管理

👉 解决你现在“available不可靠”的问题

---

# ② sensors（传感器驱动层）

## 📌 htu21d_sensor

封装：

* begin()
* readTemperature()
* readHumidity()
* status()

👉 解决：

* htu.begin不可靠
* 无状态问题

---

## 📌 ze08_sensor

封装：

* 帧同步（FF 17）
* CRC校验
* 数据解析
* 超时丢包

👉 解决：

* 丢帧
* 数据错位
* 无校验

---

## 📌 s8_sensor

封装：

* Modbus请求
* CRC16
* 响应等待
* timeout机制

👉 解决：

* RX=0问题无法判断原因
* blocking delay问题
* 响应不稳定

---

# ③ processing（数据处理层）

## 📌 filter

```text id="filter"
滑动平均 / EMA滤波
```

解决：

* 数据抖动（HTU21D）
* CO2跳变
* HCHO噪声

---

## 📌 air_quality

负责：

```text id="aq"
CO2 + HCHO → 空气质量等级
```

例如：

* 优
* 良
* 一般
* 差

👉 把“规则”从 main.cpp 拆出去

---

## 📌 data_validator

负责：

* NAN过滤
* 合法范围判断
* 异常值剔除

例如：

```cpp id="val"
CO2 < 3000
HCHO < 1.0
```

---

# ④ core（系统核心层）

## 📌 sensor_manager（最重要）

👉 这是整个系统“中枢”

职责：

* 统一调度3个传感器
* 控制采样频率
* 管理状态
* 防止阻塞

---

### 它解决你现在最大问题：

❌ loop里直接读
✔ 改为统一调度器

---

## 📌 system_state

保存：

```text id="state"
系统运行状态
传感器状态
错误状态
重启计数
```

👉 用于长期运行稳定性

---

# ⑤ app（应用层）

## 📌 output_formatter

负责：

* JSON输出
* 人类可读输出
* MQTT payload

---

## 📌 serial_reporter

负责：

* 串口打印格式统一
* debug开关
* 日志等级

---

## 📌 mqtt_client（可选）

未来扩展：

* HomeAssistant
* 云平台
* Node-RED

---

# 🧠 main.cpp 会变成什么样？

最终 main.cpp 只做一件事：

```text id="main"
初始化系统
调用 sensor_manager.update()
调用 output_formatter.print()
```

👉 不再包含任何业务逻辑

---

# 🚨 你现在代码 vs 架构升级对比

| 维度    | 当前   | V2架构         |
| ----- | ---- | ------------ |
| 结构    | 单文件  | 分层模块         |
| UART  | 直接读  | driver管理     |
| 传感器   | 耦合   | 独立类          |
| 数据    | 全局变量 | system_state |
| 稳定性   | 弱    | 可恢复          |
| 扩展性   | 差    | 强            |
| IoT能力 | 无    | MQTT ready   |

---

# 🧠 一句话总结这个架构

> 👉 现在是“脚本式采集程序”
> 👉 V2是“可长期运行的物联网设备架构”

---

# 🚀 如果你下一步要继续

我可以帮你做三件更关键的升级：

### ① 直接帮你画“数据流图”（非常重要）

让你一眼看懂系统如何运行

### ② 给你实现 sensor_manager（核心文件）

这是整个架构的“大脑”

### ③ 或直接帮你写 V2完整代码骨架（可编译）

只要你说一句：
👉 “开始实现sensor_manager”

我可以直接带你进入工业级写法。

# 仅仅根据下面代码，把代码内功能分布到上一条所述架构中，给出需要改动的所有文件及代码，没有涉及改动的文件保持空白，以让我了解代码分层的方式，不进行上面提出的所有优化，看懂代码分层后我会每层优化分别添加进代码

很好，这一步是**真正的重构理解关键点**：你不是在“优化代码”，而是在做**结构迁移（refactor to architecture）**。

我只做一件事：

> ✅ 不改任何逻辑
> ✅ 不加任何优化
> ✅ 只是把你现有 main.cpp **拆到架构中对应文件**

---

# 🧠 你当前代码的真实结构（先对照）

你现在 main.cpp 本质只有4块：

```text id="srcmap"
1. HTU21D读取
2. ZE08读取
3. S8读取
4. 串口输出
```

👉 所以我们只是“搬家”，不改逻辑

---

# 📁 最终分层映射（核心）

| 原代码功能    | 新架构位置                   |
| -------- | ----------------------- |
| HTU21D读数 | sensors/htu21d_sensor   |
| ZE08解析   | sensors/ze08_sensor     |
| S8通信     | sensors/s8_sensor       |
| CRC函数    | drivers（或 s8_sensor 内部） |
| loop调度   | core/sensor_manager     |
| Serial输出 | app/serial_reporter     |
| main.cpp | 只保留调用                   |

---

# 📦 1️⃣ main.cpp（最终变“空调度器”）

```cpp id="main_cpp"
#include <Arduino.h>
#include "core/sensor_manager.h"
#include "app/serial_reporter.h"

void setup()
{
    Serial.begin(115200);
    delay(2000);

    SensorManager_begin();
    SerialReporter_begin();
}

void loop()
{
    SensorManager_update();
    SerialReporter_print();
    delay(5000);
}
```

---

# 📦 2️⃣ core/sensor_manager.h

```cpp id="sm_h"
#pragma once

void SensorManager_begin();
void SensorManager_update();
```

---

# 📦 3️⃣ core/sensor_manager.cpp

👉 负责“调度原逻辑”

```cpp id="sm_cpp"
#include <Arduino.h>

#include "sensors/htu21d_sensor.h"
#include "sensors/ze08_sensor.h"
#include "sensors/s8_sensor.h"

// ================= 数据（从原main迁移） =================
float temperature;
float humidity;
int co2ppm;
float hcho;

void SensorManager_begin()
{
    HTU21D_begin();
    ZE08_begin();
    S8_begin();
}

void SensorManager_update()
{
    // 原逻辑不变，只是搬到这里

    HTU21D_read(&temperature, &humidity);

    ZE08_read(&hcho);

    S8_read(&co2ppm);
}
```

---

# 📦 4️⃣ sensors/htu21d_sensor.h

```cpp id="htu_h"
#pragma once

void HTU21D_begin();
void HTU21D_read(float* temperature, float* humidity);
```

---

# 📦 5️⃣ sensors/htu21d_sensor.cpp

```cpp id="htu_cpp"
#include <Wire.h>
#include <HTU21D.h>

HTU21D htu;

void HTU21D_begin()
{
    Wire.begin(21, 22);
    htu.begin();
}

void HTU21D_read(float* temperature, float* humidity)
{
    *temperature = htu.readTemperature();
    *humidity = htu.readHumidity();
}
```

---

# 📦 6️⃣ sensors/ze08_sensor.h

```cpp id="ze_h"
#pragma once

void ZE08_begin();
void ZE08_read(float* hcho);
```

---

# 📦 7️⃣ sensors/ze08_sensor.cpp

```cpp id="ze_cpp"
#include <Arduino.h>

HardwareSerial ze08Serial(2);

void ZE08_begin()
{
    ze08Serial.begin(9600, SERIAL_8N1, 16, 17);
}

void ZE08_read(float* hcho)
{
    while (ze08Serial.available() >= 9)
    {
        uint8_t buf[9];
        ze08Serial.readBytes(buf, 9);

        if (buf[0] == 0xFF)
        {
            uint16_t value =
                ((uint16_t)buf[4] << 8) |
                buf[5];

            *hcho = value / 1000.0;
        }
    }
}
```

---

# 📦 8️⃣ sensors/s8_sensor.h

```cpp id="s8_h"
#pragma once

void S8_begin();
void S8_read(int* co2ppm);
```

---

# 📦 9️⃣ sensors/s8_sensor.cpp

```cpp id="s8_cpp"
#include <Arduino.h>

HardwareSerial s8Serial(1);

// ===== CRC（原样搬过来） =====
uint16_t modbusCRC(uint8_t *buf, uint8_t len)
{
    uint16_t crc = 0xFFFF;

    for (uint8_t pos = 0; pos < len; pos++)
    {
        crc ^= (uint16_t)buf[pos];

        for (int i = 8; i != 0; i--)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}

void S8_begin()
{
    s8Serial.begin(9600, SERIAL_8N1, 4, 5);
}

void S8_read(int* co2ppm)
{
    uint8_t cmd[8] =
    {
        0xFE, 0x04, 0x00, 0x03,
        0x00, 0x01, 0x00, 0x00
    };

    uint16_t crc = modbusCRC(cmd, 6);
    cmd[6] = crc & 0xFF;
    cmd[7] = crc >> 8;

    while (s8Serial.available())
        s8Serial.read();

    s8Serial.write(cmd, 8);

    delay(120);

    if (s8Serial.available() >= 7)
    {
        uint8_t buf[7];
        s8Serial.readBytes(buf, 7);

        *co2ppm =
            ((uint16_t)buf[3] << 8) |
            buf[4];
    }
    else
    {
        *co2ppm = -1;
    }
}
```

---

# 📦 10️⃣ app/serial_reporter.h

```cpp id="sr_h"
#pragma once

void SerialReporter_begin();
void SerialReporter_print();
```

---

# 📦 11️⃣ app/serial_reporter.cpp

```cpp id="sr_cpp"
#include <Arduino.h>

extern float temperature;
extern float humidity;
extern int co2ppm;
extern float hcho;

void SerialReporter_begin()
{
    Serial.println("Reporter Ready");
}

void SerialReporter_print()
{
    Serial.println("\n==============================");

    Serial.printf("温度 : %.2f °C\n", temperature);
    Serial.printf("湿度 : %.2f %%\n", humidity);

    if (co2ppm >= 0)
        Serial.printf("CO2  : %d ppm\n", co2ppm);
    else
        Serial.println("CO2  : NO DATA");

    if (hcho >= 0)
        Serial.printf("甲醛 : %.3f mg/m³\n", hcho);
    else
        Serial.println("甲醛 : NO DATA");

    Serial.println("==============================");
}
```

---

# 🧠 你现在得到的“架构效果”

## ✔ main.cpp

👉 只剩“调度”

---

## ✔ sensor_manager

👉 统一控制采集

---

## ✔ sensors/*

👉 每个传感器独立

---

## ✔ app/*

👉 输出完全独立

---

# 🚨 重要：你现在没有做优化（刻意的）

你要求的是：

> ❗ 不优化
> ❗ 不改逻辑
> ❗ 只拆结构

✔ 所以：

* 没有滤波
* 没有CRC改进
* 没有状态机
* 没有异常处理

---

# 🚀 下一步（你后面一定会做）

当你理解这个结构后，优化将会变成：

### ① sensor层优化

→ 加 CRC / timeout

### ② manager层优化

→ 状态机调度

### ③ app层优化

→ JSON / MQTT

---

如果你下一步说：

> 👉 “开始优化sensor层（ZE08或S8）”

我可以带你一层一层升级成真正工业级 IoT 架构。

# system_init加入

更规范的做法

很多大型嵌入式项目会专门做一个：

core/
├── system_init.h
└── system_init.cpp

例如：

system_init.h
#pragma once

void SystemInit();
system_init.cpp
#include <Arduino.h>

void SystemInit()
{
    Serial.begin(115200);

    delay(2000);
}
main.cpp
#include "core/system_init.h"

void setup()
{
    SystemInit();

    SensorManager_begin();

    SerialReporter_begin();
}

这样更清晰：

SystemInit
    ↓
初始化所有硬件资源

SensorManager
    ↓
初始化业务模块

Reporter
    ↓
初始化输出模块

# 这是之前你给出四个层面优化，把它写入记忆中，后面我将一步一步提问，根据这四个层面内容完成优化项目，所以现在写入记忆，然后等待我的分层提问

已经记录。

后续我们讨论这个空气质量检测项目时，我会默认以你定义的 Air Monitor V2 分层架构 为基准：

① Driver Layer
② Sensor Layer
③ Processing Layer
④ Core / Application Layer

以及对应目录结构：

drivers/
sensors/
processing/
core/
app/

并遵循你当前的开发方式：

先理解架构
↓
再逐层优化
↓
每次只修改当前层
↓
不跨层引入复杂逻辑
↓
保持项目始终可编译


# 现在处理drivers层，判断哪些文件和代码需要修改，给出每个文件的修改后代码并与之前代码对比，并注释修改说明，按照“--- # 📦 4️⃣ sensors/htu21d_sensor.h
cpp id="htu_h"
#pragma once

void HTU21D_begin();
void HTU21D_read(float* temperature, float* humidity);
“格式 ---# ① drivers（底层通信抽象层） ## 📌 i2c_manager text id="i2c" 统一管理 I2C 作用： * 初始化 I2C * 扫描设备 * 统一读写接口 👉 防止 HTU21D 直接依赖 Wire --- ## 📌 uart_manager text id="uart" 统一管理 UART0/1/2 作用： * S8 UART * ZE08 UART * 超时控制 * buffer管理 👉 解决你现在“available不可靠”的问题

很好，现在我们严格遵循你的原则：

```text
只处理 Driver Layer
不处理 Sensor Layer
不处理 Processing Layer
不处理 Core Layer
不引入额外优化
只完成架构分层
```

因此本轮目标仅仅是：

```text
把 Wire 抽离到 i2c_manager
把 HardwareSerial 抽离到 uart_manager
```

不增加：

* CRC优化
* Timeout优化
* 状态机
* Buffer管理

这些后面再做。

---

# 本轮涉及哪些文件

新增：

```text
src/drivers/
├── i2c_manager.h
├── i2c_manager.cpp
├── uart_manager.h
└── uart_manager.cpp
```

修改：

```text
src/sensors/
├── htu21d_sensor.cpp
├── ze08_sensor.cpp
└── s8_sensor.cpp
```

其余文件：

```text
保持不变
```

---

# 📦 1️⃣ drivers/i2c_manager.h

新增文件

```cpp
#pragma once

#include <Wire.h>

void I2CManager_begin(
    uint8_t sdaPin,
    uint8_t sclPin);

TwoWire& I2CManager_getBus();
```

---

## 修改说明

以前：

```cpp
Wire.begin(...)
```

直接写在：

```cpp
htu21d_sensor.cpp
```

现在：

```cpp
I2CManager_begin(...)
```

统一管理。

---

# 📦 2️⃣ drivers/i2c_manager.cpp

新增文件

```cpp
#include "i2c_manager.h"

void I2CManager_begin(
    uint8_t sdaPin,
    uint8_t sclPin)
{
    Wire.begin(
        sdaPin,
        sclPin);
}

TwoWire& I2CManager_getBus()
{
    return Wire;
}
```

---

## 修改说明

目前只是简单封装：

```cpp
Wire.begin()
```

以后：

```cpp
I2C扫描
总线恢复
设备检测
```

都加在这里。

---

# 📦 3️⃣ drivers/uart_manager.h

新增文件

```cpp
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
```

---

## 修改说明

以后：

```text
S8
ZE08
```

不再自己创建：

```cpp
HardwareSerial serial(...)
```

全部统一从这里获取。

---

# 📦 4️⃣ drivers/uart_manager.cpp

新增文件

```cpp
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
```

---

## 修改说明

以前：

```cpp
HardwareSerial s8Serial(1);
HardwareSerial ze08Serial(2);
```

分别存在于：

```text
s8_sensor.cpp
ze08_sensor.cpp
```

现在：

```text
drivers统一管理UART资源
```

---

# 📦 5️⃣ sensors/htu21d_sensor.cpp

修改前

```cpp
#include <Wire.h>
#include <HTU21D.h>

HTU21D htu;

void HTU21D_begin()
{
    Wire.begin(21,22);

    htu.begin();
}
```

---

修改后

```cpp
#include <HTU21D.h>

#include "drivers/i2c_manager.h"

HTU21D htu;

void HTU21D_begin()
{
    I2CManager_begin(
        21,
        22);

    htu.begin();
}

void HTU21D_read(
    float* temperature,
    float* humidity)
{
    *temperature =
        htu.readTemperature();

    *humidity =
        htu.readHumidity();
}
```

---

## 修改说明

删除：

```cpp
Wire.begin(...)
```

改为：

```cpp
I2CManager_begin(...)
```

实现：

```text
HTU21D不直接依赖Wire
```

---

# 📦 6️⃣ sensors/ze08_sensor.cpp

修改前

```cpp
HardwareSerial ze08Serial(2);

void ZE08_begin()
{
    ze08Serial.begin(
        9600,
        SERIAL_8N1,
        16,
        17);
}
```

---

修改后

```cpp
#include "drivers/uart_manager.h"

void ZE08_begin()
{
    UARTManager_beginUART2(
        9600,
        16,
        17);
}

void ZE08_read(float* hcho)
{
    HardwareSerial& serial =
        UARTManager_getUART2();

    while (serial.available() >= 9)
    {
        uint8_t buf[9];

        serial.readBytes(
            buf,
            9);

        if (buf[0] == 0xFF)
        {
            uint16_t value =
                ((uint16_t)buf[4] << 8)
                | buf[5];

            *hcho =
                value / 1000.0f;
        }
    }
}
```

---

## 修改说明

删除：

```cpp
HardwareSerial ze08Serial(2);
```

改成：

```cpp
UARTManager_getUART2()
```

---

# 📦 7️⃣ sensors/s8_sensor.cpp

修改前

```cpp
HardwareSerial s8Serial(1);
```

以及：

```cpp
s8Serial.begin(...)
```

---

修改后

```cpp
#include "drivers/uart_manager.h"
```

---

初始化

```cpp
void S8_begin()
{
    UARTManager_beginUART1(
        9600,
        4,
        5);
}
```

---

读取

```cpp
void S8_read(int* co2ppm)
{
    HardwareSerial& serial =
        UARTManager_getUART1();

    uint8_t cmd[8] =
    {
        0xFE,0x04,0x00,0x03,
        0x00,0x01,0x00,0x00
    };

    uint16_t crc =
        modbusCRC(cmd,6);

    cmd[6] = crc & 0xFF;
    cmd[7] = crc >> 8;

    while(serial.available())
    {
        serial.read();
    }

    serial.write(cmd,8);

    delay(120);

    if(serial.available() >= 7)
    {
        uint8_t buf[7];

        serial.readBytes(
            buf,
            7);

        *co2ppm =
            ((uint16_t)buf[3] << 8)
            | buf[4];
    }
    else
    {
        *co2ppm = -1;
    }
}
```

---

## 修改说明

删除：

```cpp
HardwareSerial s8Serial(1);
```

改为：

```cpp
UARTManager_getUART1()
```

---

# Driver层完成后的效果

当前分层关系变成：

```text
main
 ↓

sensor_manager
 ↓

htu21d_sensor
ze08_sensor
s8_sensor

 ↓

i2c_manager
uart_manager

 ↓

Wire
HardwareSerial
```

这就是 Driver Layer 第一阶段完成后的样子。

此时：

* 没有增加任何功能
* 没有改变任何通信逻辑
* 没有增加CRC校验
* 没有增加timeout
* 没有增加扫描功能

只是完成了：

```text
传感器层
    ↓
驱动层
    ↓
硬件接口
```

的依赖分离。

下一步如果继续优化 Driver Layer，我们再考虑：

```text
i2c_manager
├── I2C扫描
├── 设备检测
└── 总线恢复

uart_manager
├── 超时读取
├── 帧读取
├── 缓冲区管理
└── 串口状态检测
```

而不是直接进入 Sensor Layer。

# 下一步如果继续优化 Driver Layer考虑： i2c_manager ├── I2C扫描 ├── 设备检测 └── 总线恢复 uart_manager ├── 超时读取 ├── 帧读取 ├── 缓冲区管理 └── 串口状态检测判断哪些文件和代码需要修改，给出每个文件的修改后代码并与之前代码对比，并注释修改说明，按照“--- # 📦 4️⃣ sensors/htu21d_sensor.h cpp id="htu_h" #pragma once void HTU21D_begin(); void HTU21D_read(float* temperature, float* humidity); “格式

很好，这一步仍然只属于：

```text
Driver Layer V2
```

与之前不同的是：

```text
Driver Layer V1
↓
仅完成资源抽象

Driver Layer V2
↓
增加驱动能力
```

这时开始真正体现 Driver 层价值。

---

# 本轮涉及文件

修改：

```text
src/drivers/
├── i2c_manager.h
├── i2c_manager.cpp
├── uart_manager.h
└── uart_manager.cpp
```

新增：

```text
无
```

其它文件：

```text
保持不变
```

原因：

```text
Driver层优化
不应该影响Sensor层接口
```

这也是分层架构的意义。

---

# 📦 1️⃣ drivers/i2c_manager.h

---

## 修改前

```cpp
#pragma once

#include <Wire.h>

void I2CManager_begin(
    uint8_t sdaPin,
    uint8_t sclPin);

TwoWire& I2CManager_getBus();
```

---

## 修改后

```cpp
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
```

---

## 修改说明

新增：

```cpp
I2CManager_deviceExists()
```

用于：

```text
检测0x40是否存在
```

---

新增：

```cpp
I2CManager_scan()
```

用于：

```text
扫描全部I2C设备
```

---

新增：

```cpp
I2CManager_recoverBus()
```

用于：

```text
SDA被拉低
总线锁死
```

恢复。

---

# 📦 2️⃣ drivers/i2c_manager.cpp

---

## 修改前

```cpp
#include "i2c_manager.h"

void I2CManager_begin(
    uint8_t sdaPin,
    uint8_t sclPin)
{
    Wire.begin(
        sdaPin,
        sclPin);
}

TwoWire& I2CManager_getBus()
{
    return Wire;
}
```

---

## 修改后

```cpp
#include "i2c_manager.h"

static uint8_t g_sdaPin;
static uint8_t g_sclPin;

void I2CManager_begin(
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
```

---

## 修改说明

新增能力：

```text
设备检测
设备扫描
总线恢复
```

但：

```text
没有任何Sensor层代码改动
```

---

# 📦 3️⃣ drivers/uart_manager.h

---

## 修改前

```cpp
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
```

---

## 修改后

```cpp
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
```

---

## 修改说明

增加统一UART工具。

以后：

```cpp
serial.readBytes(...)
```

逐步迁移成：

```cpp
UARTManager_readFrame(...)
```

---

# 📦 4️⃣ drivers/uart_manager.cpp

---

## 修改前

```cpp
#include "uart_manager.h"

static HardwareSerial uart1(1);
static HardwareSerial uart2(2);

...
```

---

## 修改后

```cpp
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
```

---

## 修改说明

新增：

### Buffer管理

统一替代：

```cpp
while(serial.available())
{
    serial.read();
}
```

---

### 超时读取

统一替代：

```cpp
delay(120);

if(serial.available() >= 7)
```

这种写法。

---

### 帧读取

统一读取：

```text
S8固定7字节
ZE08固定9字节
```

---

### 状态检测

统一判断：

```cpp
UARTManager_hasData(...)
```

---

# Driver Layer V2 完成后的架构

```text
main
 ↓

sensor_manager
 ↓

htu21d_sensor
ze08_sensor
s8_sensor

 ↓

i2c_manager
├── begin
├── scan
├── deviceExists
└── recoverBus

uart_manager
├── begin
├── clearBuffer
├── readBytesTimeout
├── readFrame
└── hasData

 ↓

Wire
HardwareSerial
```

---

# 此阶段获得的能力

## i2c_manager

```text
✓ 初始化
✓ 扫描
✓ 设备检测
✓ 总线恢复
```

---

## uart_manager

```text
✓ UART统一管理
✓ Buffer管理
✓ 超时读取
✓ 固定帧读取
✓ 串口状态检测
```

---

注意：

```text
此时 Sensor Layer 还没有使用这些新能力
```

例如：

```cpp
S8_read()
```

仍然可能保留：

```cpp
delay(120);

if(serial.available() >= 7)
```

下一阶段才会进入 Sensor Layer，把这些新Driver能力真正接入到：

```text
htu21d_sensor
ze08_sensor
s8_sensor
```

内部。这样你可以清晰看到：

```text
Driver层负责“提供能力”
Sensor层负责“使用能力”
```

这是后续优化时最重要的分层边界。


