#pragma once

#include <Arduino.h>

// 统一命令入口（MQTT / Serial / App都走这里）
void CommandHandler_begin();

// 不再返回 String，而是直接处理 + 发布
void CommandHandler_handle(const String& jsonCmd);