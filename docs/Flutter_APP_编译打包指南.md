# Flutter APP 编译与打包教程

## 📱 目录
1. [环境准备](#1-环境准备)
2. [运行调试](#2-运行调试)
3. [Android打包](#3-android打包)
4. [iOS打包](#4-ios打包)
5. [常见问题](#5-常见问题)

---

## 1. 环境准备

### 1.1 安装Flutter SDK

**Linux:**
```bash
# 下载Flutter
cd ~
git clone https://github.com/flutter/flutter.git -b stable

# 添加到环境变量
echo 'export PATH="$PATH:$HOME/flutter/bin"' >> ~/.bashrc
source ~/.bashrc

# 验证安装
flutter doctor
```

**macOS:**
```bash
brew install --cask flutter
flutter doctor
```

**Windows:**
```powershell
# 从 https://flutter.dev/docs/get-started/install/windows 下载安装包
# 解压后添加到PATH环境变量
```

### 1.2 安装Android Studio (Android开发)

```bash
# Ubuntu
sudo snap install android-studio --classic

# macOS
brew install --cask android-studio

# Windows: 从官网下载安装
```

**配置Android SDK:**
```
1. 打开Android Studio
2. Tools → SDK Manager
3. 安装:
   - Android SDK Platform (API 33+)
   - Android SDK Build-Tools
   - Android SDK Command-line Tools
```

### 1.3 安装Xcode (iOS开发,仅macOS)

```bash
# Mac App Store安装Xcode
# 命令行工具
sudo xcode-select --reset
sudo xcodebuild -runFirstLaunch
```

---

## 2. 运行调试

### 2.1 连接设备

**Android真机:**
```bash
# 启用开发者选项
设置 → 关于手机 → 连续点击版本号7次

# 启用USB调试
设置 → 开发者选项 → USB调试

# 连接电脑
flutter devices
```

**iOS模拟器(macOS):**
```bash
open -a Simulator
```

**Android模拟器:**
```bash
# Android Studio中创建AVD
# 或者命令行启动
emulator -avd <device_name>
```

### 2.2 运行APP

```bash
# 进入项目目录
cd /data/info/esp/flutter_app

# 获取依赖
flutter pub get

# 运行(自动检测可用设备)
flutter run

# 指定设备
flutter run -d <device_id>
```

**热重载:**
```
运行后按 r 键进行热重载
按 R 键完全重启
```

---

## 3. Android打包

### 3.1 配置签名

**创建key.properties:**
```properties
# flutter_app/android/key.properties
storePassword=your_store_password
keyPassword=your_key_password
keyAlias=upload
storeFile=/path/to/upload-keystore.jks
```

**生成密钥:**
```bash
keytool -genkey -v \
  -keystore ~/upload-keystore.jks \
  -keyalg RSA \
  -keysize 2048 \
  -validity 10000 \
  -alias upload
```

**修改build.gradle:**
```groovy
// flutter_app/android/app/build.gradle
def keystoreProperties = new Properties()
def keystorePropertiesFile = rootProject.file('key.properties')
if (keystorePropertiesFile.exists()) {
    keystoreProperties.load(new FileInputStream(keystorePropertiesFile))
}

android {
    signingConfigs {
        release {
            keyAlias keystoreProperties['keyAlias']
            keyPassword keystoreProperties['keyPassword']
            storeFile file(keystoreProperties['storeFile'])
            storePassword keystoreProperties['storePassword']
        }
    }
    buildTypes {
        release {
            signingConfig signingConfigs.release
        }
    }
}
```

### 3.2 构建APK

```bash
# 测试版APK
flutter build apk --debug

# 发布版APK(已签名)
flutter build apk --release

# 分包APK(减小体积)
flutter build apk --split-per-abi
```

**输出位置:**
```
build/app/outputs/flutter-apk/
├── app-release.apk          # 通用APK
└── app-arm64-v8a-release.apk # ARM64专用
```

### 3.3 构建AAB (Google Play)

```bash
flutter build appbundle --release
```

**输出位置:**
```
build/app/outputs/bundle/release/app-release.aab
```

---

## 4. iOS打包

### 4.1 配置Xcode

```
1. 用Xcode打开 ios/Runner.xcworkspace
2. 选择 Runner Target
3. Signing & Capabilities:
   - Team: 选择你的Apple ID
   - Bundle Identifier: com.yourname.environmentalmonitor
   - Automatically manage signing: ✓
```

### 4.2 构建IPA

```bash
# 清理项目
flutter clean
flutter pub get

# 构建
flutter build ios --release
```

**Archive导出:**
```
1. Xcode: Product → Archive
2. Distribute App
3. 选择分发方式:
   - App Store Connect
   - Ad Hoc (测试)
   - Development (开发)
```

---

## 5. 常见问题

### 5.1 编译错误

**问题: Gradle同步失败**
```
解决方案:
1. File → Invalidate Caches / Restart
2. 删除 .gradle 文件夹
3. flutter clean && flutter pub get
```

**问题: CocoaPods失败 (iOS)**
```bash
cd ios
pod install
pod repo update
```

### 5.2 运行时错误

**问题: 权限被拒绝**
```xml
<!-- Android: android/app/src/main/AndroidManifest.xml -->
<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
<uses-permission android:name="android.permission.INTERNET" />
```

```xml
<!-- iOS: ios/Runner/Info.plist -->
<key>NSLocationWhenInUseUsageDescription</key>
<string>需要定位权限获取位置信息</string>
```

### 5.3 性能优化

**减少APK体积:**
```bash
# 代码压缩
flutter build apk --release --split-per-abi

# 资源压缩
shrinkResources true
minifyEnabled true
```

**提高启动速度:**
```dart
// main.dart
void main() {
  // 减少main函数中的初始化工作
  runApp(MyApp());
}
```

---

## 6. 发布指南

### 6.1 Android发布

**检查清单:**
- [ ] 版本号递增 (pubspec.yaml)
- [ ] 应用图标替换
- [ ] 启动画面配置
- [ ] 隐私政策链接
- [ ] 签名配置

**发布到应用商店:**
1. Google Play Console上传AAB
2. 填写应用信息
3. 提交审核

### 6.2 iOS发布

**检查清单:**
- [ ] Apple Developer账号
- [ ] App Store Connect配置
- [ ] 截图和描述
- [ ] 隐私政策URL

**提交流程:**
1. Xcode Archive
2. Upload to App Store
3. TestFlight测试
4. 提交审核

---

**祝您打包顺利! 🎉**
