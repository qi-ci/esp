#include "protocol_json.h"
#include "../app/output_formatter.h"

// ======================
// telemetry JSON
// ======================
String JSON_buildTelemetry(const SystemState& state, const String& sensor)
{
    return OutputFormatter_buildTelemetry(state, sensor);
}