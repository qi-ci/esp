import 'dart:async';

import '../network/mqtt_service.dart';
import '../network/mqtt_topics.dart';
import '../models/command_model.dart';

class CommandController {
  static final CommandController _instance = CommandController._internal();
  factory CommandController() => _instance;
  CommandController._internal();

  final MQTTService _mqtt = MQTTService();

  /// 🚀 用于UI监听命令响应
  final StreamController<CommandResponse> _responseController =
      StreamController<CommandResponse>.broadcast();

  Stream<CommandResponse> get responses => _responseController.stream;

  /// ======================
  /// 核心：发送命令
  /// ======================
  void sendCommand(CommandModel cmd) {
    final topic = MQTTTopic_cmd(cmd.deviceId);
    final payload = cmd.toJsonString();

    _mqtt.publish(topic, payload);
  }

  /// ======================
  /// 处理响应
  /// ======================
  void onResponseReceived(String jsonStr) {
    try {
      final resp = CommandResponse.fromJsonString(jsonStr);
      _responseController.add(resp);
    } catch (e) {
      // 可以考虑打印日志或UI提示
      print("[CommandController] parse error: $e");
    }
  }

  void dispose() {
    _responseController.close();
  }
}