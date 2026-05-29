# Flutter 快速调试指南

## 🚀 一键启动调试

### 方式1: 使用交互式脚本(推荐)

```bash
cd /data/info/esp/flutter_app
./debug.sh
```

脚本会显示可用设备并让您选择运行模式。

### 方式2: 直接命令行

```bash
cd /data/info/esp/flutter_app

# Android真机调试
flutter run -d AXSCVB4B09020298

# Linux桌面调试
flutter run -d linux

# Chrome浏览器调试
flutter run -d chrome
```

---

## 🔧 调试前准备

### 1. 修改ESP32服务器地址

编辑 `lib/services/sensor_service.dart`:

```dart
String _serverUrl = 'http://192.168.1.XXX:8080';  // 改为您的ESP32实际IP
```

**如何获取ESP32的IP地址:**
- ESP32连接WiFi后,在串口监视器中会显示IP地址
- 或在路由器管理页面查看

### 2. 确保ESP32已启动

```bash
# 在另一个终端监控ESP32日志
cd /data/info/esp/esp32_firmware
pio device monitor
```

---

## 📱 调试命令速查

### 运行时命令 (在flutter run运行时输入)

| 按键 | 功能 | 说明 |
|------|------|------|
| `r` | 热重载 | 快速更新UI代码,保持状态 |
| `R` | 热重启 | 完全重启应用 |
| `h` | 帮助 | 显示所有可用命令 |
| `w` | 打印widget树 | 调试UI布局 |
| `t` | 显示性能数据 | 查看FPS等信息 |
| `l` | 显示日志 | 查看print输出 |
| `c` | 清屏 | 清空控制台 |
| `q` | 退出 | 停止调试 |

### 常用开发命令

```bash
# 获取依赖
flutter pub get

# 代码分析(检查错误和警告)
flutter analyze

# 格式化代码
flutter format .

# 清理项目
flutter clean
flutter pub get

# 编译APK(测试版)
flutter build apk --debug

# 编译APK(发布版)
flutter build apk --release

# 编译iOS
flutter build ios --release

# 查看设备列表
flutter devices

# 创建新页面
flutter create --template=module new_feature
```

---

## 🐛 常见问题排查

### 问题1: 找不到设备

```bash
# 检查ADB连接
adb devices

# 如果没有显示设备,重新连接USB
# 确保手机开启了USB调试模式
```

### 问题2: 编译失败

```bash
# 清理并重新构建
flutter clean
flutter pub get
flutter run
```

### 问题3: 热重载不生效

某些修改需要热重启(R)而不是热重载(r):
- 修改了main()函数
- 修改了类的构造函数
- 添加/删除了资源文件
- 修改了全局变量

### 问题4: 无法连接ESP32

检查清单:
- [ ] ESP32和手机在同一WiFi网络
- [ ] 服务器地址配置正确
- [ ] ESP32防火墙允许8080端口
- [ ] 路由器未开启AP隔离

测试连接:
```bash
# 从电脑测试
curl http://192.168.1.XXX:8080/api/data

# 从Android设备测试(需要ADB)
adb shell curl http://192.168.1.XXX:8080/api/data
```

### 问题5: GPS定位失败

Android需要在设置中手动授予位置权限:
```
设置 → 应用 → 环境监测仪 → 权限 → 位置 → 允许
```

---

## 🔍 高级调试技巧

### 1. 查看完整日志

```bash
# Android详细日志
adb logcat | grep flutter

# 只看ERROR级别
adb logcat | grep -E "flutter.*ERROR"

# 保存日志到文件
adb logcat > debug_log.txt
```

### 2. 网络请求调试

在 `lib/services/sensor_service.dart` 中添加日志:

```dart
Future<SensorData?> fetchData() async {
  print('[DEBUG] 请求URL: $_serverUrl/api/data');  // 添加这行
  
  try {
    final response = await http.get(
      Uri.parse('$_serverUrl/api/data'),
    ).timeout(const Duration(seconds: 10));
    
    print('[DEBUG] 响应状态码: ${response.statusCode}');  // 添加这行
    print('[DEBUG] 响应内容: ${response.body}');  // 添加这行
    
    // ... 其余代码
  }
}
```

### 3. 数据库调试

```dart
// 在database_service.dart中添加
Future<void> printAllData() async {
  final db = await database;
  final result = await db.query('sensor_readings');
  print('数据库中共有 ${result.length} 条记录');
  for (var row in result) {
    print(row);
  }
}
```

### 4. 性能分析

```bash
# 启动性能分析模式
flutter run --profile

# 打开DevTools
flutter pub global run devtools
```

然后在浏览器中打开显示的URL,可以查看:
- CPU使用率
- 内存使用情况
- Widget重建次数
- 网络请求详情

---

## 📊 VSCode调试配置

### 创建 launch.json

在项目根目录创建 `.vscode/launch.json`:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Flutter Debug (Android)",
            "request": "launch",
            "type": "dart",
            "deviceId": "AXSCVB4B09020298",
            "program": "lib/main.dart",
            "args": []
        },
        {
            "name": "Flutter Debug (Linux)",
            "request": "launch",
            "type": "dart",
            "deviceId": "linux",
            "program": "lib/main.dart",
            "args": []
        },
        {
            "name": "Flutter Debug (Chrome)",
            "request": "launch",
            "type": "dart",
            "deviceId": "chrome",
            "program": "lib/main.dart",
            "args": []
        }
    ]
}
```

### 使用断点调试

1. **设置断点**: 点击行号左侧的红点区域
2. **启动调试**: 按F5或点击"运行"→"启动调试"
3. **单步执行**: 
   - F10: 跳过(不进入函数)
   - F11: 进入函数
   - Shift+F11: 跳出函数
4. **查看变量**: 左侧"变量"面板自动显示当前作用域的变量
5. **条件断点**: 右键断点 → "编辑断点" → 输入条件表达式

---

## 🎯 调试工作流建议

### 日常开发流程

```
1. 修改代码
   ↓
2. 保存文件 (Ctrl+S)
   ↓
3. 自动热重载 (或按 r)
   ↓
4. 观察效果
   ↓
5. 如有问题,查看日志或使用断点
   ↓
6. 重复步骤1-5
```

### 测试完整功能

```
1. 启动ESP32固件
   ↓
2. 启动Flutter APP调试
   ↓
3. 观察数据是否正常显示
   ↓
4. 测试各个功能:
   - 下拉刷新
   - 切换Tab
   - 查看历史数据
   - 修改设置
   ↓
5. 模拟异常情况:
   - 断开ESP32
   - 关闭WiFi
   - 输入无效数据
   ↓
6. 验证错误处理是否正确
```

---

## 📚 相关文档

- [Flutter官方调试文档](https://docs.flutter.dev/testing/debugging)
- [DevTools使用指南](https://docs.flutter.dev/development/tools/devtools/overview)
- [ESP32固件调试](../docs/ESP32_编译烧录指南.md)
- [项目总览](../docs/项目总览_README.md)

---

## 💡 专业提示

作为物联网专家,建议您:

1. **分层调试**: 先确保ESP32正常工作,再调试APP
2. **日志规范**: 使用 `[模块名] 消息` 格式,便于过滤
3. **异常处理**: 所有异步操作都要有try-catch
4. **性能监控**: 定期检查FPS和内存使用
5. **真实环境测试**: 经常在真机上测试,不要只依赖模拟器

---

**祝您调试顺利! 🎉**

*最后更新: 2024-01-16*
