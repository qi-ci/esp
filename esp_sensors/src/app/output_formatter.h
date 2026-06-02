#pragma once

#include "../core/system_state.h"
#include <Arduino.h>

// void OutputFormatter_toHuman(char* buffer, int size);

// void OutputFormatter_toJSON(char* buffer, int size);


String OutputFormatter_buildTelemetry(const SystemState& s, const String& sensorType);
String OutputFormatter_buildResponse(const String& status,
                                     const String& msg);


String OutputFormatter_buildDebug();
