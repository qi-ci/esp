/// 传感器数据模型
class SensorData {
  final double temperature;    // 温度(°C)
  final double humidity;       // 湿度(%RH)
  final double formaldehyde;   // 甲醛(ppm)
  final double co2;           // CO2(ppm)
  final DateTime timestamp;   // 时间戳
  final double? latitude;     // 纬度
  final double? longitude;    // 经度
  final int batteryLevel;     // 电池电量(%)

  SensorData({
    required this.temperature,
    required this.humidity,
    required this.formaldehyde,
    required this.co2,
    required this.timestamp,
    this.latitude,
    this.longitude,
    this.batteryLevel = 100,
  });

  /// 从JSON解析
  factory SensorData.fromJson(Map<String, dynamic> json) {
    return SensorData(
      temperature: (json['temperature'] as num?)?.toDouble() ?? 0.0,
      humidity: (json['humidity'] as num?)?.toDouble() ?? 0.0,
      formaldehyde: (json['formaldehyde'] as num?)?.toDouble() ?? 0.0,
      co2: (json['co2'] as num?)?.toDouble() ?? 0.0,
      timestamp: DateTime.fromMillisecondsSinceEpoch(
        (json['timestamp'] as num?)?.toInt() ?? 0,
      ),
      latitude: (json['latitude'] as num?)?.toDouble(),
      longitude: (json['longitude'] as num?)?.toDouble(),
      batteryLevel: (json['battery_level'] as num?)?.toInt() ?? 100,
    );
  }

  /// 转换为JSON
  Map<String, dynamic> toJson() {
    return {
      'temperature': temperature,
      'humidity': humidity,
      'formaldehyde': formaldehyde,
      'co2': co2,
      'timestamp': timestamp.millisecondsSinceEpoch,
      'latitude': latitude,
      'longitude': longitude,
      'battery_level': batteryLevel,
    };
  }

  /// 复制到新实例
  SensorData copyWith({
    double? temperature,
    double? humidity,
    double? formaldehyde,
    double? co2,
    DateTime? timestamp,
    double? latitude,
    double? longitude,
    int? batteryLevel,
  }) {
    return SensorData(
      temperature: temperature ?? this.temperature,
      humidity: humidity ?? this.humidity,
      formaldehyde: formaldehyde ?? this.formaldehyde,
      co2: co2 ?? this.co2,
      timestamp: timestamp ?? this.timestamp,
      latitude: latitude ?? this.latitude,
      longitude: longitude ?? this.longitude,
      batteryLevel: batteryLevel ?? this.batteryLevel,
    );
  }

  @override
  String toString() {
    return 'SensorData(T: ${temperature.toStringAsFixed(1)}°C, '
        'H: ${humidity.toStringAsFixed(1)}%, '
        'HCHO: ${formaldehyde.toStringAsFixed(3)}ppm, '
        'CO2: ${co2.toStringAsFixed(0)}ppm)';
  }
}

/// 告警信息模型
class AlertInfo {
  final String title;
  final String message;
  final AlertLevel level;
  final DateTime timestamp;
  final bool isRead;

  AlertInfo({
    required this.title,
    required this.message,
    required this.level,
    required this.timestamp,
    this.isRead = false,
  });

  @override
  String toString() {
    return 'AlertInfo($title, $level, $timestamp)';
  }
}

/// 告警级别
enum AlertLevel {
  info,      // 信息
  warning,   // 警告
  critical,  // 严重
}

extension AlertLevelExtension on AlertLevel {
  String get displayName {
    switch (this) {
      case AlertLevel.info:
        return '信息';
      case AlertLevel.warning:
        return '警告';
      case AlertLevel.critical:
        return '严重';
    }
  }
}
