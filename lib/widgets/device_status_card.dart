import 'package:flutter/material.dart';

/// ============================
/// 📡 设备状态卡片
/// ============================
///
/// 显示：
/// Sensors
/// WiFi
/// MQTT
/// Uptime
/// LastUpDate

class DeviceStatusCard extends StatelessWidget {
  final String sensors;
  final String wifi;
  final String mqtt;
  final String uptime;
  final String lastUpdate;

  const DeviceStatusCard({
    super.key,
    required this.sensors,
    required this.wifi,
    required this.mqtt,
    required this.uptime,
    required this.lastUpdate,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      // elevation: 3,
      // margin: const EdgeInsets.all(8),
      child: Padding(
        padding: const EdgeInsets.all(12),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            // 左边状态
            Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  "Sensors: $sensors",
                  style: TextStyle(fontSize: 16),
                ),
                Text(
                  "WiFi: $wifi",
                  style: TextStyle(fontSize: 16),
                ),
                Text(
                  "MQTT: $mqtt",
                  style: TextStyle(fontSize: 16),
                ),
                Text(
                  "Uptime: $uptime",
                  style: TextStyle(fontSize: 16),
                ),
                Text(
                  "LastUpdate: $lastUpdate",
                  style: TextStyle(fontSize: 16),
                )
              ],
            ),
            // // 右边时间信息
            // Column(
            //   crossAxisAlignment: CrossAxisAlignment.end,
            //   children: [
            //     Text(
            //       "Uptime: $uptime",
            //       style: TextStyle(fontSize: 18),
            //     ),
            //     Text(
            //       "LastUpdate: $lastUpdate",
            //       style: TextStyle(fontSize: 18),
            //     ),
            //   ],
            // ),
          ],
        ),
      ),
    );
  }
}