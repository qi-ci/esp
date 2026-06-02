#pragma once
#include <Arduino.h>
#include "../core/system_state.h"

String JSON_buildTelemetry(const SystemState& state, const String& sensor);