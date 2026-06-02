#pragma once

#include "../core/system_state.h"

// 初始化（可留空或打印信息）
void TelemetryManager_begin();

// 自动上传传感器数据
void TelemetryManager_update();