#include "protocol_json.h"
#include "../app/output_formatter.h"

// ======================
// telemetry JSON
// ======================
String JSON_buildTelemetry(const String& sensorType, float value1, float value2, unsigned long timestamp)
{
    return OutputFormatter_buildTelemetry(sensorType, value1, value2, timestamp);
}