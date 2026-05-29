#!/bin/bash

echo "========================================="
echo "ESP32环境监测仪 - Flutter调试工具"
echo "========================================="
echo ""

# 检查Flutter是否安装
if ! command -v flutter &> /dev/null; then
    echo "❌ 错误: Flutter未安装"
    echo ""
    echo "请先安装Flutter SDK:"
    echo "  Linux: sudo snap install flutter --classic"
    echo "  macOS: brew install --cask flutter"
    echo "  Windows: 从 https://flutter.dev 下载"
    exit 1
fi

echo "✅ Flutter版本:"
flutter --version | head -1
echo ""

# 进入项目目录
cd "$(dirname "$0")"

# 显示可用设备
echo "📱 可用设备:"
flutter devices
echo ""

# 选择运行模式
echo "请选择运行模式:"
echo "1. Android真机调试 (推荐)"
echo "2. Linux桌面调试"
echo "3. Chrome浏览器调试"
echo "4. 仅编译不运行"
echo "5. 运行代码分析"
echo "6. 清理并重新构建"
echo ""
read -p "请输入选项 (1-6): " choice

case $choice in
    1)
        echo ""
        echo "🚀 启动Android真机调试..."
        echo ""
        echo "提示: 按 'r' 热重载, 按 'R' 热重启, 按 'q' 退出"
        echo ""
        flutter run -d android
        ;;
    2)
        echo ""
        echo "🚀 启动Linux桌面调试..."
        echo ""
        flutter run -d linux
        ;;
    3)
        echo ""
        echo "🚀 启动Chrome浏览器调试..."
        echo ""
        flutter run -d chrome
        ;;
    4)
        echo ""
        echo "🔨 编译APK..."
        flutter build apk --debug
        echo ""
        echo "✅ APK生成位置: build/app/outputs/flutter-apk/app-debug.apk"
        ;;
    5)
        echo ""
        echo "🔍 运行代码分析..."
        flutter analyze
        ;;
    6)
        echo ""
        echo "🧹 清理项目..."
        flutter clean
        echo ""
        echo "📦 重新获取依赖..."
        flutter pub get
        echo ""
        echo "✅ 清理完成!"
        ;;
    *)
        echo "❌ 无效选项"
        exit 1
        ;;
esac

echo ""
echo "========================================="
echo "调试会话结束"
echo "========================================="
