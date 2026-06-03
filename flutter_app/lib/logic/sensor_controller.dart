import 'dart:async';

import '../models/sensor_data.dart';
import '../data/cache_manager.dart';

class SensorController {
  static final SensorController _instance =
      SensorController._internal();

  factory SensorController() => _instance;

  SensorController._internal();

  final CacheManager _cache = CacheManager();

  /// 🌊 UI监听流（核心）
  final StreamController<SensorData> _streamController =
      StreamController<SensorData>.broadcast();

  Stream<SensorData> get stream => _streamController.stream;

  /// ⚡ 当 MessageRouter 更新数据后调用
  void onNewData(SensorData data) {
    // 1. 更新缓存
    _cache.updateSensorData(data);

    // 2. 推送给UI
    _streamController.add(data);
  }

  /// 📦 获取当前最新数据
  SensorData? getLatest(String deviceId) {
    return _cache.getLatest(deviceId);
  }

  void dispose() {
    _streamController.close();
  }
}