// models/sensor_data.dart
class SensorData {
  double temperature;
  double humidity;
  double co2;
  double hcho;

  SensorData({
    required this.temperature,
    required this.humidity,
    required this.co2,
    required this.hcho,
  });

  factory SensorData.fromJson(Map<String, dynamic> json) {
    return SensorData(
      temperature: (json['temperature'] ?? 0).toDouble(),
      humidity: (json['humidity'] ?? 0).toDouble(),
      co2: (json['co2'] ?? 0).toDouble(),
      hcho: (json['hcho'] ?? 0).toDouble(),
    );
  }
}