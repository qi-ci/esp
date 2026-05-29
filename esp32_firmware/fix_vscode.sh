#!/bin/bash

echo "========================================="
echo "ESP32 VSCode 配置修复脚本"
echo "========================================="
echo ""

# 检查PlatformIO是否安装
if command -v pio &> /dev/null; then
    echo "✅ PlatformIO已安装"
    echo ""
    echo "正在编译项目..."
    cd "$(dirname "$0")"
    pio run
    
    if [ $? -eq 0 ]; then
        echo ""
        echo "✅ 编译成功!"
        echo "✅ VSCode应该不再报红"
    else
        echo ""
        echo "❌ 编译失败,请检查错误信息"
    fi
else
    echo "❌ PlatformIO未安装"
    echo ""
    echo "请选择安装方式:"
    echo ""
    echo "方式1 (推荐): 在VSCode中安装PlatformIO扩展"
    echo "  1. Ctrl+Shift+X 打开扩展市场"
    echo "  2. 搜索 'PlatformIO IDE'"
    echo "  3. 点击安装"
    echo "  4. 重启VSCode"
    echo ""
    echo "方式2: 命令行安装PlatformIO Core"
    echo "  pip install platformio"
    echo ""
fi

echo ""
echo "========================================="
echo "详细配置指南请查看: README.md"
echo "========================================="
