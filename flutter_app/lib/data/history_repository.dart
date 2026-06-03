import '../models/sensor_data.dart';

class HistoryRepository {
  // ⚡ 这里用 SQLite / Hive 替代
  final List<SensorData> _history = [];

  void insertSensorData(SensorData data) {
    _history.add(data);
    // TODO: 持久化到数据库
  }

  List<SensorData> queryByDay(String deviceId, DateTime day) {
    return _history
        .where((e) =>
            e.deviceId == deviceId &&
            DateTime.fromMillisecondsSinceEpoch(e.timestamp).day == day.day)
        .toList();
  }
}