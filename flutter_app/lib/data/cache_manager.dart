import '../models/sensor_data.dart';

class CacheManager {
  final Map<String, SensorData> _latestData = {};

  void updateSensorData(SensorData data) {
    _latestData[data.deviceId] = data;
  }

  SensorData? getLatest(String deviceId) => _latestData[deviceId];
}