#pragma once
#include <Arduino.h>
#include "../core/system_state.h"

String JSON_buildTelemetry(const String& sensorType, float value1, float value2, unsigned long timestamp);