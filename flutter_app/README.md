# 环境监测仪 Flutter APP

ESP32多参数环境监测仪的配套手机APP,支持实时数据显示、历史查询和智能告警。

---

## 📱 功能特性

### ✅ 已实现
- **实时监控**: 温度、湿度、甲醛、CO2四项参数
- **数据可视化**: fl_chart趋势图表展示
- **历史数据**: SQLite本地存储,支持日月查询
- **GPS定位**: 自动获取位置信息
- **智能告警**: 分级通知推送
- **主题切换**: 亮色/暗色自适应

### 🎨 UI设计
- Material Design 3规范
- 响应式布局
- 底部导航栏(首页/历史/设置)
- 下拉刷新交互

---

## 🚀 快速开始

### 1. 安装Flutter SDK

```bash
# Linux
sudo snap install flutter --classic

# macOS
brew install --cask flutter

# Windows
# 从 https://flutter.dev 下载安装包
```

验证安装:
```bash
flutter doctor
```

### 2. 获取依赖

```bash
cd /data/info/esp/flutter_app
flutter pub get
```

### 3. 运行APP

**模拟器:**
```bash
# Android模拟器
flutter emulators --launch <emulator_id>

# iOS模拟器(macOS)
open -a Simulator
```

**真机调试:**
```bash
# 连接设备后
flutter devices
flutter run
```

### 4. 修改服务器地址

编辑 `lib/services/sensor_service.dart`:
```dart
String _serverUrl = 'http://YOUR_ESP32_IP:8080';
```

或在APP设置页面中修改。

---

## 📦 项目结构

```
flutter_app/
├── lib/
│   ├── main.dart              # APP入口
│   ├── screens/               # 页面组件
│   │   ├── home_screen.dart   # 首页(实时数据)
│   │   ├── history_screen.dart # 历史数据
│   │   └── settings_screen.dart # 设置
│   ├── services/              # 业务服务
│   │   ├── sensor_service.dart # ESP32通信
│   │   └── database_service.dart # SQLite存储
│   ├── models/                # 数据模型
│   │   └── sensor_data.dart   # 传感器数据
│   └── widgets/               # 可复用组件
│
├── android/                   # Android配置
├── ios/                       # iOS配置
├── assets/                    # 资源文件
├── test/                      # 测试代码
└── pubspec.yaml              # 依赖配置
```

---

## 🔧 核心模块说明

### SensorService (传感器服务)

负责与ESP32通信:
```dart
final service = SensorService();
service.initialize(serverUrl: 'http://192.168.1.100:8080');

// 监听数据流
service.dataStream.listen((data) {
  print('收到数据: $data');
});

// 手动刷新
await service.refresh();
```

### DatabaseService (数据库服务)

本地数据存储:
```dart
final db = DatabaseService();

// 插入数据
await db.insertSensorData(sensorData);

// 查询今日数据
final todayData = await db.getTodayData();

// 查询日期范围
final rangeData = await db.getDataByRange(
  start: DateTime.now().subtract(Duration(days: 7)),
  end: DateTime.now(),
);
```

---

## 🎯 编译打包

### Android APK

```bash
# 测试版
flutter build apk --debug

# 发布版
flutter build apk --release

# 分包(减小体积)
flutter build apk --split-per-abi
```

输出位置: `build/app/outputs/flutter-apk/`

### iOS IPA

```bash
# 需要macOS和Xcode
flutter build ios --release
```

详细打包教程参考: `docs/Flutter_APP_编译打包指南.md`

---

## 🐛 常见问题

### Q1: flutter命令找不到

**解决:** 确保Flutter SDK已正确安装并添加到PATH。

```bash
export PATH="$PATH:/path/to/flutter/bin"
```

### Q2: 依赖下载失败

**解决:** 使用国内镜像

```bash
export PUB_HOSTED_URL=https://pub.flutter-io.cn
export FLUTTER_STORAGE_BASE_URL=https://storage.flutter-io.cn
flutter pub get
```

### Q3: Android编译失败

**解决:**
```bash
flutter clean
flutter pub get
cd android
./gradlew clean
cd ..
flutter run
```

### Q4: iOS编译失败

**解决:**
```bash
cd ios
pod install
cd ..
flutter run
```

---

## 📊 技术栈

| 类别 | 技术 |
|------|------|
| 框架 | Flutter 3.x |
| 语言 | Dart 3.x |
| 状态管理 | Provider |
| 网络请求 | http, dio |
| 本地存储 | sqflite, shared_preferences |
| 图表 | fl_chart |
| GPS | geolocator, geocoding |
| 通知 | flutter_local_notifications |

---

## 📝 开发规范

### 代码风格

遵循 [Effective Dart](https://dart.dev/guides/language/effective-dart):
- 使用camelCase命名变量和函数
- 使用PascalCase命名类
- 优先使用const构造函数
- 避免不必要的null检查

### 提交规范

```
feat: 添加新功能
fix: 修复bug
docs: 更新文档
style: 代码格式调整
refactor: 重构代码
test: 添加测试
chore: 构建过程或辅助工具变动
```

---

## 🔗 相关资源

- [Flutter官方文档](https://flutter.dev/docs)
- [Dart语言指南](https://dart.dev/guides)
- [Material Design 3](https://m3.material.io/)
- [ESP32固件代码](../esp32_firmware/)

---

## 📞 技术支持

遇到问题?
1. 查看 `docs/` 目录下的详细文档
2. 检查GitHub Issues
3. 联系开发团队

---

**祝您使用愉快! 🎉**

*版本: 1.0.0*  
*更新日期: 2024-01-16*
