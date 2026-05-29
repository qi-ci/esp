#!/bin/bash

echo "========================================="
echo "Flutter项目初始化脚本"
echo "========================================="
echo ""

# 检查Flutter是否安装
if ! command -v flutter &> /dev/null; then
    echo "❌ Flutter未安装"
    echo ""
    echo "请先安装Flutter SDK:"
    echo "  Linux: sudo snap install flutter --classic"
    echo "  macOS: brew install --cask flutter"
    echo "  Windows: 从 https://flutter.dev 下载"
    echo ""
    exit 1
fi

echo "✅ Flutter已安装"
flutter --version
echo ""

# 进入项目目录
cd "$(dirname "$0")"

# 获取依赖
echo "📦 正在获取依赖..."
flutter pub get

if [ $? -eq 0 ]; then
    echo "✅ 依赖获取成功"
else
    echo "❌ 依赖获取失败"
    exit 1
fi

echo ""
echo "🔍 运行代码分析..."
flutter analyze

echo ""
echo "========================================="
echo "项目初始化完成!"
echo "========================================="
echo ""
echo "下一步:"
echo "  1. 修改 lib/services/sensor_service.dart 中的服务器地址"
echo "  2. 运行: flutter run"
echo "  3. 或打开Android Studio / VSCode进行开发"
echo ""
