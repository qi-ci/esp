import 'package:flutter/material.dart';

/// ============================
/// 📡 设备状态卡片
/// ============================
///
/// 显示：
/// Device Online
/// WiFi
/// MQTT
/// Uptime
/// Firmware
class DeviceStatusCard extends StatelessWidget {
  final Map<String, String> status;

  const DeviceStatusCard({
    super.key,
    required this.status,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(12),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [

            Text(
              status["online"] ?? "",
              style: const TextStyle(
                fontSize: 16,
                fontWeight: FontWeight.bold,
              ),
            ),

            const SizedBox(height: 8),

            Text("WiFi: ${status["wifi"]}"),
            Text("MQTT: ${status["mqtt"]}"),
            Text("Uptime: ${status["uptime"]}"),
            Text("FW: ${status["fw"]}"),
          ],
        ),
      ),
    );
  }
}