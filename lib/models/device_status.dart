// models/device_status.dart
class DeviceStatus {
  String sensors;
  String wifi;
  String mqtt;
  String uptime;
  String lastUpdate;

  DeviceStatus({
    required this.sensors,
    required this.wifi,
    required this.mqtt,
    required this.uptime,
    required this.lastUpdate,
  });

  factory DeviceStatus.fromJson(Map<String, dynamic> json) {
    return DeviceStatus(
      sensors: json['sensors'] ?? 'Unknown',
      wifi: json['wifi'] ?? 'Disconnected',
      mqtt: json['mqtt'] ?? 'Disconnected',
      uptime: json['uptime'] ?? '0h 0m',
      lastUpdate: json['lastUpdate'] ?? '',
    );
  }
}