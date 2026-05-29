# Flutter 调试完全指南

**项目:** ESP32环境监测仪APP  
**Flutter版本:** 3.44.0  
**Dart版本:** 3.12.0

---

## ✅ 当前状态检查

### 依赖状态
```bash
✅ 所有依赖已安装 (101 packages)
✅ 无编译错误
⚠️ 2个轻微deprecated警告(不影响运行)
```

### 可用设备
```
📱 AMM AN00 (Android真机) - AXSCVB4B09020298
💻 Linux桌面应用
🌐 Chrome浏览器(Web)
```

---

## 🚀 快速开始调试

### 方式1: Android真机调试 (推荐) ⭐⭐⭐⭐⭐

**优势:** 最接近真实使用场景,GPS/通知功能完整

```bash
cd /data/info/esp/flutter_app
flutter run -d AXSCVB4B09020298
```

**调试命令:**
```bash
# 热重载 (修改UI代码后)
r

# 热重启 (修改逻辑代码后)
R

# 查看日志
l

# 退出
q
```

---

### 方式2: Linux桌面调试 ⭐⭐⭐⭐

**优势:** 快速迭代,无需真机

```bash
cd /data/info/esp/flutter_app
flutter run -d linux
```

**注意:** GPS和某些Android特性可能不可用

---

### 方式3: Chrome Web调试 ⭐⭐⭐

**优势:** 跨平台,易于分享测试

```bash
cd /data/info/esp/flutter_app
flutter run -d chrome
```

**注意:** 
- HTTP请求可能受CORS限制
- GPS需要HTTPS或localhost
- 部分原生功能不可用

---

## 🔧 VSCode调试配置

### 创建launch.json

在项目根目录创建 `.vscode/launch.json`:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Flutter (Android)",
            "request": "launch",
            "type": "dart",
            "deviceId": "AXSCVB4B09020298"
        },
        {
            "name": "Flutter (Linux)",
            "request": "launch",
            "type": "dart",
            "deviceId": "linux"
        },
        {
            "name": "Flutter (Chrome)",
            "request": "launch",
            "type": "dart",
            "deviceId": "chrome"
        }
    ]
}
```

### 使用VSCode调试

1. **设置断点**: 点击行号左侧
2. **启动调试**: F5 或 Run → Start Debugging
3. **单步执行**: F10 (跳过), F11 (进入)
4. **查看变量**: 左侧Variables面板
5. **热重载**: Ctrl+S保存时自动触发

---

## 🐛 常见问题调试

### 问题1: 连接ESP32失败

**症状:** APP显示"未连接"

**调试步骤:**

1. **检查服务器地址**
   ```dart
   // lib/services/sensor_service.dart
   String _serverUrl = 'http://YOUR_ESP32_IP:8080';
   ```

2. **查看网络日志**
   ```bash
   # Android真机
   adb logcat | grep SensorService
   
   # Linux/Chrome
   flutter run --verbose
   ```

3. **测试HTTP连接**
   ```bash
   # 从手机/电脑访问ESP32
   curl http://192.168.1.100:8080/api/data
   ```

4. **检查防火墙**
   ```bash
   # 确保ESP32和手机在同一WiFi
   # 关闭路由器AP隔离
   ```

---

### 问题2: 数据不更新

**症状:** 数据显示但不变动

**调试步骤:**

1. **检查Stream监听**
   ```dart
   // lib/screens/home_screen.dart
   _sensorService.dataStream.listen((data) {
     print('收到新数据: $data');  // 添加日志
   });
   ```

2. **验证定时器**
   ```dart
   // lib/services/sensor_service.dart
   Timer.periodic(Duration(seconds: 5), (_) {
     print('定时获取数据...');  // 确认定时器工作
     fetchData();
   });
   ```

3. **手动刷新测试**
   ```dart
   // 在APP中点击刷新按钮
   // 观察控制台输出
   ```

---

### 问题3: 图表不显示

**症状:** History页面空白

**调试步骤:**

1. **检查数据库**
   ```dart
   // lib/screens/history_screen.dart
   print('历史数据数量: ${_historyData.length}');
   ```

2. **验证数据存储**
   ```dart
   // lib/services/database_service.dart
   Future<int> insertSensorData(SensorData data) async {
     final id = await db.insert('sensor_readings', data.toJson());
     print('插入数据ID: $id');  // 确认插入成功
     return id;
   }
   ```

3. **检查fl_chart配置**
   ```dart
   // 确保spots不为空
   if (_historyData.length < 2) {
     print('数据不足,无法绘制图表');
     return const SizedBox.shrink();
   }
   ```

---

### 问题4: GPS定位失败

**症状:** 位置信息为空

**调试步骤:**

1. **检查权限**
   ```bash
   # Android
   adb shell dumpsys package com.environmentalmonitor.environmental_monitor | grep permission
   ```

2. **查看GPS日志**
   ```dart
   import 'package:geolocator/geolocator.dart';
   
   LocationPermission permission = await Geolocator.checkPermission();
   print('GPS权限状态: $permission');
   ```

3. **模拟位置(Android)**
   ```bash
   # 开发者选项中启用"模拟位置"
   # 或使用ADB设置位置
   adb emu geo fix 116.4074 39.9042
   ```

---

## 📊 性能调试

### 查看FPS

```bash
flutter run --profile
```

在APP顶部会显示FPS计数器。

### 内存分析

```bash
# 启动DevTools
flutter pub global activate devtools
devtools

# 或在运行时按 p 键打开DevTools
```

### 网络请求监控

```bash
# 查看详细HTTP日志
flutter run --verbose 2>&1 | grep http
```

---

## 🔍 高级调试技巧

### 1. 条件断点

在VSCode中右键断点 → Edit Breakpoint → 添加条件:
```dart
data.temperature > 30  // 只在温度>30时中断
```

### 2. 异常断点

在VSCode调试面板勾选"All Exceptions",捕获所有异常。

### 3. 日志过滤

```bash
# 只看ERROR级别
flutter run 2>&1 | grep ERROR

# 只看特定标签
flutter run 2>&1 | grep SensorService
```

### 4. 远程调试Android

```bash
# 开启USB调试
adb devices

# 转发端口
adb forward tcp:8080 tcp:8080

# 查看实时日志
adb logcat -s flutter
```

---

## 🎯 调试最佳实践

### 作为物联网专家的建议:

1. **分层调试**
   ```
   UI层 → Service层 → Network层 → Hardware层
   ```

2. **日志规范**
   ```dart
   print('[模块名] 消息内容');
   // 例如:
   print('[SensorService] Data received: $data');
   ```

3. **错误处理**
   ```dart
   try {
     await fetchData();
   } catch (e, stackTrace) {
     print('[ERROR] $e');
     print(stackTrace);  // 打印堆栈追踪
   }
   ```

4. **性能监控**
   ```dart
   final stopwatch = Stopwatch()..start();
   await someOperation();
   print('耗时: ${stopwatch.elapsedMilliseconds}ms');
   ```

---

## 📱 真机调试完整流程

### 步骤1: 连接设备

```bash
# 确认设备连接
flutter devices

# 如果未检测到
adb devices
```

### 步骤2: 修改服务器地址

编辑 `lib/services/sensor_service.dart`:
```dart
String _serverUrl = 'http://192.168.1.XXX:8080';  // 改为ESP32实际IP
```

### 步骤3: 启动调试

```bash
flutter run -d AXSCVB4B09020298
```

### 步骤4: 观察日志

```
I/flutter (12345): [SensorService] Server URL updated to: http://192.168.1.100:8080
I/flutter (12345): [SensorService] Data received: SensorData(T: 23.5°C, H: 45.2%, ...)
```

### 步骤5: 交互测试

- 下拉刷新
- 切换Tab
- 修改设置
- 观察数据变化

---

## 🛠️ 常用调试命令速查

```bash
# 清理重新构建
flutter clean
flutter pub get
flutter run

# 查看设备
flutter devices

# 分析代码
flutter analyze

# 格式化代码
flutter format .

# 打包测试
flutter build apk --debug

# 查看依赖树
flutter pub deps

# 更新依赖
flutter pub upgrade
```

---

## 📞 获取帮助

### Flutter官方资源
- [Flutter调试文档](https://docs.flutter.dev/testing/debugging)
- [DevTools指南](https://docs.flutter.dev/development/tools/devtools/overview)
- [常见问题](https://docs.flutter.dev/testing/common-problems)

### 本项目相关
- [产品需求说明书](../ESP32_环境监测仪产品需求说明书_V2.0_修正版.md)
- [ESP32固件代码](../esp32_firmware/)
- [项目总览](./项目总览_README.md)

---

**祝您调试顺利! 🎉**

*最后更新: 2024-01-16*
