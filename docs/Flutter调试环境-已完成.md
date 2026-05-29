# Flutter调试环境 - 已完成配置 ✅

**创建时间:** 2024-01-16  
**Flutter版本:** 3.44.0  
**Dart版本:** 3.12.0  
**状态:** ✅ 就绪,可以开始调试

---

## ✅ 已完成的配置工作

### 1. 项目结构创建

```
flutter_app/
├── lib/
│   ├── main.dart                    ✅ APP入口和主界面
│   ├── models/
│   │   └── sensor_data.dart         ✅ 数据模型(SensorData, AlertInfo)
│   ├── services/
│   │   ├── sensor_service.dart      ✅ ESP32 HTTP通信服务
│   │   └── database_service.dart    ✅ SQLite本地数据库
│   ├── screens/
│   │   ├── home_screen.dart         ✅ 首页(实时数据卡片)
│   │   ├── history_screen.dart      ✅ 历史数据图表
│   │   └── settings_screen.dart     ✅ 设置页面
│   ├── widgets/                     📁 预留目录
│   └── utils/                       📁 预留目录
├── android/                         ✅ Android完整配置
├── ios/                             ✅ iOS配置文件
├── assets/                          📁 资源目录
├── test/
│   └── widget_test.dart             ✅ 基础测试用例
├── pubspec.yaml                     ✅ 依赖配置(15个库)
├── debug.sh                         ✅ 交互式调试脚本
└── README.md                        ✅ 项目说明文档
```

### 2. 依赖安装

已成功安装的核心依赖:
- ✅ **http** (^1.1.0) - HTTP通信
- ✅ **dio** (^5.3.2) - 高级HTTP客户端
- ✅ **sqflite** (^2.3.0) - SQLite数据库
- ✅ **path_provider** (^2.1.1) - 文件路径
- ✅ **path** (^1.8.3) - 路径处理
- ✅ **shared_preferences** (^2.2.2) - 轻量存储
- ✅ **geolocator** (^10.1.0) - GPS定位
- ✅ **geocoding** (^2.1.1) - 地理编码
- ✅ **fl_chart** (^0.65.0) - 图表绘制
- ✅ **intl** (^0.18.1) - 国际化
- ✅ **flutter_local_notifications** (^16.1.0) - 通知
- ✅ **permission_handler** (^11.0.1) - 权限管理
- ✅ **provider** (^6.1.1) - 状态管理

总计: **101个包**已安装

### 3. 代码质量检查

```bash
flutter analyze
```

结果:
- ✅ **0个错误**
- ⚠️ **2个轻微警告** (deprecated的withOpacity方法,不影响运行)
- ✅ 无编译错误

### 4. 设备检测

检测到3个可用设备:
- 📱 **AMM AN00** (Android真机) - AXSCVB4B09020298 ← **推荐使用**
- 💻 **Linux桌面** - linux
- 🌐 **Chrome浏览器** - chrome

---

## 🚀 立即开始调试

### 方式1: 使用调试脚本(最简单)

```bash
cd /data/info/esp/flutter_app
./debug.sh
```

然后选择选项 `1` (Android真机调试)

### 方式2: 直接命令行

```bash
cd /data/info/esp/flutter_app
flutter run -d AXSCVB4B09020298
```

### 方式3: VSCode调试

1. 在VSCode中打开项目: `File → Open Folder → /data/info/esp/flutter_app`
2. 按 `F5` 启动调试
3. 或点击左侧"运行和调试"图标

---

## ⚙️ 调试前必须配置

### 🔴 重要: 修改ESP32服务器地址

编辑文件: `lib/services/sensor_service.dart`

找到第13行:
```dart
String _serverUrl = 'http://192.168.1.100:8080';
```

**将 `192.168.1.100` 改为您的ESP32实际IP地址**

#### 如何获取ESP32的IP?

**方法1: 查看ESP32串口输出**
```bash
cd /data/info/esp/esp32_firmware
pio device monitor
```

看到类似输出:
```
WiFi connected!
IP address: 192.168.1.XXX  ← 这就是ESP32的IP
```

**方法2: 路由器管理页面**
- 登录路由器管理界面
- 查看"已连接设备"列表
- 找到ESP32设备的IP地址

---

## 📱 运行时调试命令

启动 `flutter run` 后,可以使用以下命令:

| 按键 | 功能 | 说明 |
|------|------|------|
| **r** | 热重载 | ⚡ 快速更新UI,保持应用状态 |
| **R** | 热重启 | 🔄 完全重启应用 |
| **h** | 帮助 | 显示所有命令 |
| **w** | Widget树 | 打印当前UI结构 |
| **t** | 性能数据 | 显示FPS、内存等 |
| **l** | 日志 | 显示print输出 |
| **c** | 清屏 | 清空控制台 |
| **q** | 退出 | 停止调试 |

**推荐工作流:**
```
修改代码 → Ctrl+S保存 → 自动热重载(r) → 观察效果
```

---

## 🐛 常见问题快速解决

### ❌ 问题1: 应用显示"未连接"

**原因:** ESP32服务器地址配置错误

**解决:**
1. 确认ESP32已启动并连接到WiFi
2. 查看ESP32的IP地址(见上文)
3. 修改 `lib/services/sensor_service.dart` 中的 `_serverUrl`
4. 按 `R` 热重启APP

### ❌ 问题2: 数据不更新

**检查清单:**
- [ ] ESP32固件正在运行
- [ ] ESP32和手机在同一WiFi
- [ ] 服务器地址正确
- [ ] 查看控制台日志是否有错误

**调试方法:**
```bash
# 查看详细日志
flutter run -d AXSCVB4B09020298 --verbose
```

### ❌ 问题3: 编译失败

**解决:**
```bash
flutter clean
flutter pub get
flutter run
```

### ❌ 问题4: 找不到设备

**解决:**
```bash
# 检查ADB连接
adb devices

# 如果未显示,检查:
# 1. USB线是否连接好
# 2. 手机是否开启USB调试
# 3. 是否授权了这台电脑
```

---

## 🔍 高级调试技巧

### 1. 查看实时日志

```bash
# 新终端窗口执行
adb logcat | grep flutter
```

可以看到所有Flutter输出的日志。

### 2. 网络请求调试

在 `sensor_service.dart` 中添加调试日志:

```dart
Future<SensorData?> fetchData() async {
  print('🔍 [DEBUG] 请求URL: $_serverUrl/api/data');
  
  try {
    final response = await http.get(
      Uri.parse('$_serverUrl/api/data'),
    ).timeout(const Duration(seconds: 10));
    
    print('📥 [DEBUG] 响应码: ${response.statusCode}');
    print('📦 [DEBUG] 响应内容: ${response.body}');
    
    // ... 其余代码
  } catch (e) {
    print('❌ [ERROR] 请求失败: $e');
    return null;
  }
}
```

### 3. 断点调试(VSCode)

1. **设置断点**: 点击行号左侧
2. **启动调试**: F5
3. **单步执行**: 
   - F10: 跳过
   - F11: 进入函数
4. **查看变量**: 左侧Variables面板自动显示

### 4. 性能分析

```bash
# 启动性能模式
flutter run --profile

# 打开DevTools
flutter pub global run devtools
```

在浏览器中查看:
- CPU使用率
- 内存占用
- Widget重建次数
- 网络请求详情

---

## 📊 测试清单

启动APP后,逐项测试:

### 基础功能
- [ ] APP能正常启动
- [ ] 首页显示4个传感器卡片
- [ ] 连接状态显示正常
- [ ] 数据能正常显示

### 交互功能
- [ ] 下拉刷新能更新数据
- [ ] 底部Tab切换正常
- [ ] 历史数据页面有图表
- [ ] 设置页面可以修改配置

### 数据验证
- [ ] 用手触摸HTU21D,温度数值上升
- [ ] 对着ZE08哈气,甲醛数值变化
- [ ] 用嘴吹S8,CO2数值上升到1000+
- [ ] 历史数据能正常保存和查询

---

## 📚 相关文档

- 📘 [Flutter快速调试指南](./Flutter快速调试指南.md) - **详细教程**
- 📗 [Flutter APP编译打包指南](./Flutter_APP_编译打包指南.md)
- 📕 [ESP32编译烧录指南](./ESP32_编译烧录指南.md)
- 📙 [项目总览](./项目总览_README.md)
- 📔 [产品需求说明书](../ESP32_环境监测仪产品需求说明书_V2.0_修正版.md)

---

## 💡 下一步建议

### 立即可做:

1. **修改服务器地址** (必须!)
   ```
   编辑: lib/services/sensor_service.dart
   修改: _serverUrl 为ESP32实际IP
   ```

2. **启动调试**
   ```bash
   ./debug.sh
   # 或
   flutter run -d AXSCVB4B09020298
   ```

3. **测试功能**
   - 观察数据是否正常显示
   - 测试各个交互功能
   - 验证数据准确性

### 后续优化:

1. **添加WebSocket支持** - 实现真正的实时数据推送
2. **完善错误处理** - 更友好的错误提示
3. **添加单元测试** - 提高代码质量
4. **性能优化** - 减少不必要的Widget重建
5. **UI美化** - 添加动画和过渡效果

---

## 🎯 快速参考

### 常用命令

```bash
# 启动调试
flutter run -d AXSCVB4B09020298

# 热重载
# (在运行时按 r)

# 查看设备
flutter devices

# 代码分析
flutter analyze

# 格式化代码
flutter format .

# 清理项目
flutter clean && flutter pub get

# 编译APK
flutter build apk --release
```

### 文件位置速查

```
服务器地址配置: lib/services/sensor_service.dart (第13行)
主界面代码:     lib/screens/home_screen.dart
历史数据代码:   lib/screens/history_screen.dart
设置页面代码:   lib/screens/settings_screen.dart
数据模型:       lib/models/sensor_data.dart
依赖配置:       pubspec.yaml
```

---

## ✅ 检查清单

开始调试前,请确认:

- [x] Flutter SDK已安装 (v3.44.0)
- [x] 项目依赖已获取 (101个包)
- [x] 代码无编译错误
- [x] Android设备已连接 (AXSCVB4B09020298)
- [ ] **ESP32服务器地址已修改** ← 需要您手动完成
- [ ] ESP32固件已烧录并运行
- [ ] ESP32和手机在同一WiFi

---

**一切准备就绪!现在您可以:**

```bash
cd /data/info/esp/flutter_app
./debug.sh
```

**祝您调试顺利! 🎉**

*如有问题,请查看 [Flutter快速调试指南](./Flutter快速调试指南.md)*
