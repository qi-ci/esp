class MqttTopics {
  static String telemetry(String deviceId) =>
      "iot/$deviceId/tele";

  static String command(String deviceId) =>
      "iot/$deviceId/cmd";

  static String response(String deviceId) =>
      "iot/$deviceId/resp";
}