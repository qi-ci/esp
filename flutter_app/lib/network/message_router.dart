import 'dart:convert';

import '../models/sensor_data.dart';
import '../data/cache_manager.dart';
import '../data/history_repository.dart';
import '../core/device_context.dart';
import '../logic/sensor_controller.dart';
import '../logic/command_controller.dart';

// ⚡ 单例模式
class MessageRouter {
  static final MessageRouter _instance = MessageRouter._internal();

  factory MessageRouter() => _instance;

  MessageRouter._internal();

  final CacheManager _cache = CacheManager();
  final HistoryRepository _history = HistoryRepository();

  /// 处理所有 ESP32 MQTT 消息
  void handle(String topic, String payload) {
    try {
      final Map<String, dynamic> json = jsonDecode(payload);

      // === 区分 Topic 类型 ===
      if (topic.endsWith("/tele")) {
        _handleTelemetry(json);
      } else if (topic.endsWith("/resp")) {
        _handleResponse(json);
      } else {
        print("[Router] Unknown topic: $topic");
      }
    } catch (e) {
      print("[Router] JSON parse error: $e, payload: $payload");
    }
  }

  /// 解析 telemetry 数据
  void _handleTelemetry(Map<String, dynamic> json) {
    final data = SensorData(
      deviceId: json['device_id'],
      temperature: (json['temp'] ?? 0).toDouble(),
      humidity: (json['humi'] ?? 0).toDouble(),
      co2: (json['co2'] ?? 0).toInt(),
      hcho: (json['hcho'] ?? 0).toDouble(),
      timestamp: json['timestamp'] ?? DateTime.now().millisecondsSinceEpoch,
    );

    // ⚡ 关键改动：统一入口
    SensorController().onNewData(data);
  }

  /// 处理 ESP32 响应消息
  void _handleResponse(Map<String, dynamic> json) {
    // ⚡ 推送给 CommandController
    final jsonStr = jsonEncode(json);
    CommandController().onResponseReceived(jsonStr);
  }
}