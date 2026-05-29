# ESP32 环境监测仪固件编译与烧录指南

## 📋 目录
1. [环境准备](#1-环境准备)
2. [编译步骤](#2-编译步骤)
3. [烧录步骤](#3-烧录步骤)
4. [常见问题](#4-常见问题)

---

## 1. 环境准备

### 1.1 安装PlatformIO (推荐)

**方式A: VSCode扩展(最简单)**
```bash
1. 打开VSCode
2. 安装扩展: PlatformIO IDE
3. 重启VSCode
```

**方式B: 命令行安装**
```bash
# 安装Python (需要3.7+)
# 安装PlatformIO Core
pip install platformio
```

### 1.2 安装Arduino IDE (备选方案)

```bash
# Ubuntu/Debian
sudo apt install arduino

# macOS
brew install --cask arduino

# Windows: 从官网下载安装包
```

**添加ESP32支持:**
```
1. Arduino IDE → 文件 → 首选项
2. 附加开发板管理器URL添加:
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
3. 工具 → 开发板 → 开发板管理器
4. 搜索"ESP32",安装"esp32 by Espressif Systems"
```

---

## 2. 编译步骤

### 2.1 使用PlatformIO编译(推荐)

```bash
# 进入项目目录
cd /data/info/esp/esp32_firmware

# 编译项目
pio run

# 如果成功,会显示:
# ========================= [SUCCESS] Took X.XX seconds =========================
```

**编译产物位置:**
```
esp32_firmware/.pio/build/esp32dev/
├── firmware.bin      # 可烧录的二进制文件
└── firmware.elf      # ELF调试文件
```

### 2.2 使用Arduino IDE编译

```bash
1. 打开 esp32_firmware/main.ino
2. 选择开发板: 工具 → 开发板 → ESP32 Dev Module
3. 选择端口: 工具 → 端口 → /dev/ttyUSB0 (Linux) 或 COM3 (Windows)
4. 点击 ✓ 编译按钮
```

---

## 3. 烧录步骤

### 3.1 硬件连接

```
ESP32开发板 ←→ 电脑
─────────────────────
Micro USB    ←→ USB线
```

**注意事项:**
- ✅ 使用数据线而非仅充电线
- ✅ 确保USB线支持数据传输
- ✅ Linux用户可能需要添加udev规则

### 3.2 烧录固件

**PlatformIO方式:**
```bash
# 一键烧录
pio run --target upload

# 或者指定端口
pio run --target upload --upload-port /dev/ttyUSB0
```

**Arduino IDE方式:**
```
1. 点击 → 上传按钮
2. 等待编译和烧录完成
3. 看到"Hard resetting via RTS pin..."表示成功
```

### 3.3 验证烧录

**打开串口监视器:**
```bash
# PlatformIO
pio device monitor

# Arduino IDE
工具 → 串口监视器 (波特率: 115200)
```

**预期输出:**
```
========================================
ESP32 Environmental Monitor Starting...
Power Mode: B (Energy Saving)
========================================

Connecting to WiFi: Your_WiFi_SSID
........
WiFi connected!
IP address: 192.168.1.100
System initialized successfully!
========================================

[Sensor] Starting data collection...
[HTU21D] Compensated humidity: 45.2%
[Process] Data: T=23.5 H=45.2 HCHO=0.030 CO2=650
[Sensor] Data collection completed
```

---

## 4. 常见问题

### 4.1 编译错误

**问题1: 找不到头文件**
```
fatal error: HTU21D.h: No such file or directory
```
**解决:**
```
确保所有.h文件在同一个目录下:
esp32_firmware/
├── main.ino
├── HTU21D.h
├── ZE08_CH2O.h
└── S8_CO2.h
```

**问题2: 库依赖缺失**
```
Library not found: Wire
```
**解决:**
```
Wire是Arduino内置库,检查ESP32支持包是否正确安装
```

### 4.2 烧录失败

**问题1: 无法打开端口**
```
Serial port /dev/ttyUSB0 could not be opened
```
**解决(Linux):**
```bash
# 添加用户到dialout组
sudo usermod -a -G dialout $USER
# 重新登录

# 或者设置权限
sudo chmod 666 /dev/ttyUSB0
```

**解决(Windows):**
```
1. 安装CP2102驱动
   下载: https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers
2. 设备管理器确认端口号
```

**问题2: 烧录超时**
```
A fatal error occurred: Failed to connect to ESP32
```
**解决:**
```
1. 按住ESP32的BOOT按钮
2. 点击烧录
3. 看到连接提示后释放BOOT
4. 或者短接GND和IO0引脚进入下载模式
```

### 4.3 运行时问题

**问题: WiFi连接失败**
```
WiFi connection failed!
```
**解决:**
```
1. 修改main.ino中的WiFi名称和密码:
   const char* WIFI_SSID = "你的WiFi名称";
   const char* WIFI_PASSWORD = "你的WiFi密码";
2. 重新编译烧录
```

**问题: 传感器读取失败**
```
[ERROR] HTU21D read failed!
```
**解决:**
```
1. 检查接线是否正确
2. 用i2cdetect检测I2C设备(Linux):
   sudo i2cdetect -y 1
3. 确认HTU21D地址0x40被检测到
```

---

## 5. 高级调试

### 5.1 启用调试日志

修改`platformio.ini`:
```ini
build_flags = 
    -D CORE_DEBUG_LEVEL=5  ; 最详细日志
```

### 5.2 查看内存使用

在代码中添加:
```cpp
Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
Serial.printf("Min free heap: %d bytes\n", ESP.getMinFreeHeap());
```

### 5.3 OTA无线升级(后续版本)

```bash
# 首次通过USB烧录后,可配置OTA
pio run --target upload --upload-port 192.168.1.100
```

---

## 6. 性能优化建议

### 6.1 降低功耗
- 已实现: 节能模式(15分钟采样)
- 可进一步优化: Deep Sleep模式

### 6.2 提高稳定性
- 增加数据滤波算法
- 添加看门狗定时器

### 6.3 延长传感器寿命
- 已集成: 5.4节寿命管理措施
- 已集成: 5.5节温度补偿算法

---

**祝您编译顺利! 🚀**
