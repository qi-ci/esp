# ESP32 固件开发环境配置指南

## ❓ 为什么main.ino报红?

这是**VSCode的C/C++扩展找不到Arduino/ESP32头文件路径**导致的,不是代码错误。

---

## ✅ 推荐方案: 使用PlatformIO (最简单)

### 步骤1: 安装PlatformIO扩展

```
1. VSCode → 扩展市场 (Ctrl+Shift+X)
2. 搜索 "PlatformIO IDE"
3. 点击安装
4. 重启VSCode
```

### 步骤2: 打开项目

```
File → Open Folder → 选择 /data/info/esp/esp32_firmware
```

### 步骤3: 等待自动配置

```
PlatformIO会自动:
- 读取 platformio.ini
- 下载ESP32框架和依赖
- 配置includePath
- 建立索引

底部状态栏会显示: "PlatformIO: Ready"
```

### 步骤4: 编译测试

```
方法1: 点击左侧PlatformIO图标 → Build
方法2: 命令行执行: pio run
```

**✅ 完成!不再有红色波浪线!**

---

## 🔧 备选方案: 手动配置C/C++扩展

如果不想用PlatformIO,可以手动配置:

### 已自动生成配置文件

我已经为您创建了 `.vscode/c_cpp_properties.json`,但需要修改路径。

### 步骤1: 找到ESP32框架路径

```bash
# PlatformIO安装的ESP32框架通常在:
~/.platformio/packages/framework-arduinoespressif32/

# 验证是否存在
ls ~/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h
```

### 步骤2: 修改c_cpp_properties.json

编辑 `/data/info/esp/esp32_firmware/.vscode/c_cpp_properties.json`:

```json
{
    "configurations": [
        {
            "name": "ESP32",
            "includePath": [
                "${workspaceFolder}/**",
                "/YOUR_PATH_TO/framework-arduinoespressif32/cores/esp32",
                "/YOUR_PATH_TO/framework-arduinoespressif32/libraries/Wire/src",
                "/YOUR_PATH_TO/framework-arduinoespressif32/libraries/WiFi/src"
                // ... 其他路径
            ]
        }
    ]
}
```

**将 `/YOUR_PATH_TO/` 替换为实际路径**

### 步骤3: 重新加载VSCode

```
Ctrl+Shift+P → Developer: Reload Window
```

---

## 🎯 最佳实践建议

### 作为物联网专家,我强烈推荐PlatformIO:

**优势对比:**

| 特性 | PlatformIO | Arduino IDE + C/C++扩展 |
|------|-----------|------------------------|
| 配置复杂度 | ⭐ 自动配置 | ⭐⭐⭐ 手动配置 |
| 依赖管理 | ⭐⭐⭐⭐⭐ 自动 | ⭐⭐ 手动 |
| 智能提示 | ⭐⭐⭐⭐⭐ 完整 | ⭐⭐⭐ 需配置 |
| 编译速度 | ⭐⭐⭐⭐ 增量编译 | ⭐⭐ 全量编译 |
| 调试支持 | ⭐⭐⭐⭐⭐ 强大 | ⭐⭐ 基础 |
| 多平台支持 | ⭐⭐⭐⭐⭐ 统一 | ⭐⭐⭐ 分散 |

---

## 🚀 快速开始(使用PlatformIO)

```bash
# 1. 进入项目目录
cd /data/info/esp/esp32_firmware

# 2. 编译
pio run

# 3. 烧录(连接ESP32后)
pio run --target upload

# 4. 监视串口
pio device monitor
```

---

## 🔍 没有看到输出情况排查

当执行 `platformio device monitor` 后没有任何输出时，请按以下步骤逐一排查：

### 1. 确认固件已成功烧录

**检查编译和上传是否成功：**

```bash
# 重新编译并上传
cd /data/info/esp/esp32_firmware
~/.platformio/penv/bin/platformio run --target upload
```

**成功的标志：**
- 编译阶段显示 `[SUCCESS] Took X seconds`
- 上传阶段显示 `Writing at 0x... (100 %)`
- 最后显示 `Hard resetting via RTS pin...`

**如果失败：**
- 编译错误 → 检查代码语法和依赖
- 上传错误 → 参考下方"上传失败"排查

---

### 2. 检查 ESP32 是否正常启动

**观察板载 LED：**
- GPIO 2 (板载LED) 应该在程序启动时闪烁
- 如果 LED 完全不亮，可能硬件故障或固件未运行

**按下 RESET 按钮：**
- 找到 ESP32 开发板上的 EN/RST 按钮
- 按一下重启设备
- 观察串口监视器是否有输出

---

### 3. 验证波特率匹配

**当前配置：**
- 代码中：`Serial.begin(115200)` (见 `src/main.ino` 第 168 行)
- 监视器：`--baud 115200` ✅ 已匹配

**如果出现乱码：**
```bash
# 尝试其他常见波特率
platformio device monitor --baud 9600
platformio device monitor --baud 57600
platformio device monitor --baud 115200  # 推荐
platformio device monitor --baud 921600
```

---

### 4. 检查串口设备权限

**症状：** 报错 "Permission denied" 或 "Could not open /dev/ttyUSB0"

**解决方案：**

```bash
# 临时方案（当前会话有效）
sudo chmod 666 /dev/ttyUSB0

# 永久方案（推荐）
sudo usermod -a -G dialout $USER
# ⚠️ 必须注销重新登录或重启系统才能生效！

# 安装 udev 规则
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/master/scripts/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules
sudo service udev restart
```

**验证权限：**
```bash
ls -l /dev/ttyUSB*
# 应该显示: crw-rw---- 1 root dialout ...
```

---

### 5. 检查串口是否被占用

**症状：** 报错 "Resource temporarily unavailable" 或 "port is busy"

**排查方法：**
```bash
# 查找占用串口的进程
lsof /dev/ttyUSB0

# 示例输出:
# COMMAND   PID USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
# minicom  1234 user    3u   CHR  188,0      0t0  123 /dev/ttyUSB0
```

**解决方案：**
```bash
# 终止占用进程（将 1234 替换为实际 PID）
kill 1234

# 或者强制终止
kill -9 1234

# 等待几秒后重试
sleep 2
platformio device monitor --baud 115200
```

**预防措施：**
- 确保之前的监视器已完全关闭（Ctrl+C）
- 关闭其他串口工具（Arduino IDE、minicom 等）
- 避免同时运行多个监视器实例

---

### 6. 检查 USB 连接

**物理检查：**
- USB 线是否插紧？
- 尝试更换 USB 线（某些线仅充电，无数据传输功能）
- 尝试不同的 USB 端口
- 使用带数据功能的 USB 线（通常较粗）

**识别设备：**
```bash
# 查看串口设备
ls -l /dev/ttyUSB*
ls -l /dev/ttyACM*

# 查看 USB 设备列表
lsusb

# 监控设备连接/断开
dmesg | tail -20
```

**正常输出示例：**
```
[12345.678901] usb 1-2: new full-speed USB device number 5 using xhci_hcd
[12345.812345] cp210x 1-2:1.0: cp210x converter detected
[12345.813456] usb 1-2: cp210x converter now attached to ttyUSB0
```

**如果没有检测到设备：**
- 更换 USB 线
- 检查 ESP32 开发板供电指示灯
- 尝试另一台电脑

---

### 7. 检查代码中的 Serial 输出

**确认代码确实有输出：**

查看 `src/main.ino`：
- 第 168 行：`Serial.begin(115200);` ✅
- 第 169-173 行：启动信息输出 ✅
- 第 230+ 行：传感器数据采集输出 ✅

**最小测试代码：**
如果怀疑代码问题，创建简单测试：

``cpp
void setup() {
    Serial.begin(115200);
    delay(1000);  // 等待串口稳定
}

void loop() {
    Serial.println("Hello ESP32!");
    delay(1000);
}
```

编译上传后应该每秒输出一次 "Hello ESP32!"

---

### 8. 检查 ESP32 是否进入下载模式

**症状：** 上传时卡在 "Connecting..."

**手动进入下载模式：**
1. 按住 BOOT 按钮不放
2. 按下并释放 RESET 按钮
3. 继续按住 BOOT 按钮 2-3 秒
4. 开始上传
5. 上传完成后自动重启

**自动进入下载模式：**
- 已在 `platformio.ini` 中配置 `upload_resetmethod = nodemcu`
- 大多数 ESP32 开发板支持自动复位

---

### 9. 检查 FreeRTOS 任务是否正常

**当前项目使用 FreeRTOS 多任务：**
- `task_sensor_collect` - 传感器采集（优先级 1）
- `task_data_process` - 数据处理（优先级 2）
- `task_wifi_communicate` - WiFi 通信（优先级 3）
- `task_power_manage` - 电源管理（优先级 0）

**如果 WiFi 连接失败：**
- 程序会继续运行，但会打印错误信息
- 传感器数据仍会采集并通过串口输出

**查看完整日志：**
```bash
# 使用过滤器查看原始数据
platformio device monitor --baud 115200 --filter direct

# 添加时间戳
platformio device monitor --baud 115200 --filter time
```

---

### 10. 启用调试级别日志

**修改 `platformio.ini`：**

```ini
build_flags = 
    -D CORE_DEBUG_LEVEL=5    ; 从 3 改为 5（Verbose）
    -D CONFIG_FREERTOS_UNICORE=0
```

**调试级别说明：**
- 0: None
- 1: Error
- 2: Warn
- 3: Info（当前设置）
- 4: Debug
- 5: Verbose（最详细）

重新编译上传后会看到更多底层日志。

---

### 快速排查流程图

```
无输出
  ├─ 1. 固件是否上传成功？ ─No→ 重新编译上传
  │         └─ Yes↓
  ├─ 2. 按下 RESET 按钮有输出吗？ ─No→ 检查硬件连接
  │         └─ Yes↓
  ├─ 3. 波特率是否匹配？ ─No→ 调整为 115200
  │         └─ Yes↓
  ├─ 4. 串口权限是否正确？ ─No→ 添加到 dialout 组
  │         └─ Yes↓
  ├─ 5. 串口是否被占用？ ─Yes→ 终止占用进程
  │         └─ No↓
  ├─ 6. USB 连接是否正常？ ─No→ 更换线缆/端口
  │         └─ Yes↓
  └─ 7. 代码是否有 Serial 输出？ ─No→ 添加测试代码
            └─ Yes↓
              联系技术支持或检查硬件故障
```

---

### 常用诊断命令汇总

```bash
# 1. 检查串口设备
ls -l /dev/ttyUSB*

# 2. 查看 USB 设备
lsusb

# 3. 检查串口占用
lsof /dev/ttyUSB0

# 4. 查看内核日志
dmesg | tail -30

# 5. 测试串口通信
echo "test" > /dev/ttyUSB0

# 6. 使用 screen 替代监视器
screen /dev/ttyUSB0 115200
# 退出: Ctrl+A, 然后按 K, 再按 Y

# 7. 使用 minicom
minicom -D /dev/ttyUSB0 -b 115200
# 退出: Ctrl+A, 然后按 X
```

---

### 仍然无法解决？

**收集以下信息寻求帮助：**

1. **硬件信息：**
   ```bash
   lsusb
   dmesg | grep tty
   ```

2. **上传日志：** 完整复制 `platformio run --target upload` 的输出

3. **监视器输出：** 即使无数据，也复制启动时的所有输出

4. **系统信息：**
   ```bash
   uname -a
   python3 --version
   platformio --version
   ```

5. **代码版本：** 确认使用的是最新代码

---

**祝您排查顺利! 🎉**

## 🔧 ZE08 甲醛传感器调试指南

### 问题现象

```
[ZE08] Retry 1/5...
[ZE08] Retry 2/5...
[ERROR] ZE08 read failed after retries!
```

### 可能原因

1. **预热时间不足** - ZE08 需要至少 3 分钟预热
2. **接线错误** - TX/RX 接反或电源电压不对
3. **UART 通信问题** - 波特率不匹配或引脚配置错误
4. **传感器故障** - 硬件损坏

---

### 解决方案

#### ✅ 方案 1：等待预热（已实施）

代码已增加 5 秒 UART 稳定等待，但完全稳定需要 3 分钟。

**操作：**
- 上传固件后等待 3-5 分钟
- 观察后续的数据采集周期（每 15 分钟一次）
- 第二次或第三次采集时可能会成功

---

#### ✅ 方案 2：检查硬件接线

**正确接线：**

| ESP32 引脚 | ZE08 引脚 | 线色（常见） |
|-----------|----------|------------|
| GPIO 16 (RX2) | TX | 绿色 |
| GPIO 17 (TX2) | RX | 白色 |
| 5V | VCC | 红色 |
| GND | GND | 黑色 |

**检查步骤：**
1. 确认 TX 和 RX **没有接反**（最常见错误）
2. 确认使用 **5V 供电**（不是 3.3V）
3. 确认地线连接良好
4. 检查杜邦线是否松动或损坏

---

#### ✅ 方案 3：手动测试 ZE08

创建测试文件 `test_ze08.ino`：

```cpp
#include <HardwareSerial.h>

HardwareSerial SerialZE08(2);  // 使用 UART2

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("\nZE08 Test Starting...");
    
    // 初始化 UART2 (GPIO 16=RX, 17=TX)
    SerialZE08.begin(9600, SERIAL_8N1, 16, 17);
    
    // 等待传感器预热
    Serial.println("Waiting for sensor warmup (60 seconds)...");
    for (int i = 60; i > 0; i--) {
        delay(1000);
        if (i % 10 == 0) {
            Serial.printf("%d...", i);
        }
    }
    Serial.println("\nWarmup complete!");
    
    // 清空缓冲区
    while (SerialZE08.available()) {
        SerialZE08.read();
    }
}

void loop() {
    // 检查是否有数据
    if (SerialZE08.available() >= 9) {
        uint8_t buffer[9];
        
        // 读取帧头
        if (SerialZE08.read() == 0xFF && SerialZE08.read() == 0x17) {
            buffer[0] = 0xFF;
            buffer[1] = 0x17;
            
            // 读取剩余数据
            for (int i = 2; i < 9; i++) {
                buffer[i] = SerialZE08.read();
            }
            
            // 验证帧尾
            if (buffer[7] == 0xFF && buffer[8] == 0xFF) {
                // 解析甲醛浓度
                uint8_t high = buffer[4];
                uint8_t low = buffer[5];
                float hcho = ((uint16_t)high * 256 + low) / 1000.0;
                
                // 校验和
                uint8_t sum = 0;
                for (int i = 0; i < 6; i++) {
                    sum += buffer[i];
                }
                
                Serial.printf("HCHO: %.3f ppm | Checksum: %s\n", 
                             hcho, (sum == buffer[6]) ? "OK" : "FAIL");
                
                // 打印原始数据
                Serial.print("Raw: ");
                for (int i = 0; i < 9; i++) {
                    Serial.printf("0x%02X ", buffer[i]);
                }
                Serial.println();
            }
        }
    }
    
    delay(100);
}
```

**预期输出：**
```
ZE08 Test Starting...
Waiting for sensor warmup (60 seconds)...
60...50...40...30...20...10...
Warmup complete!
HCHO: 0.050 ppm | Checksum: OK
Raw: 0xFF 0x17 0x04 0x00 0x00 0x32 0x3B 0xFF 0xFF
```

---

#### ✅ 方案 4：检查 UART 通信

在现有代码中添加调试信息（已完成）：

```
[ZE08 Debug] UART2 available bytes: X
```

- 如果显示 `0`：说明没有数据到达，检查接线
- 如果显示 `>0` 但仍失败：可能是数据格式问题

---

### 常见问题

**Q1: 为什么 S8 可以工作但 ZE08 不行？**

A: 两个传感器使用不同的 UART 端口：
- S8 使用 UART1 (GPIO 4/5)
- ZE08 使用 UART2 (GPIO 16/17)

可能是 ZE08 的接线有问题，或者传感器本身故障。

---

**Q2: ZE08 需要多久才能稳定？**

A: 
- 官方建议：3 分钟预热
- 最佳性能：连续运行 24 小时后
- 每次上电都需要重新预热

---

**Q3: 能否跳过 ZE08 继续使用其他传感器？**

A: 可以！代码已经实现了优雅降级：
```
[WARN] Some sensors failed, continuing with partial data
```

系统会继续采集 HTU21D 和 S8 的数据。

---

### 联系支持

如果以上步骤都无法解决，请提供：

1. **接线照片** - 清晰显示 ESP32 和 ZE08 的连接
2. **测试代码输出** - 运行上述测试代码的完整输出
3. **万用表测量** - 确认 ZE08 VCC 引脚电压为 5V
4. **替换测试** - 如有条件，尝试另一个 ZE08 传感器
