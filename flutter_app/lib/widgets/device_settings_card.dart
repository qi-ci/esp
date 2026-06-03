import 'package:flutter/material.dart';

/// ============================
/// 🔧 设备设置卡片
/// ============================
///
/// 包含：
/// - Device ID
/// - MQTT Broker
/// - Restart Device
/// - Factory Reset
class DeviceSettingsCard extends StatelessWidget {
  final String deviceId;
  final String mqttBroker;

  final ValueChanged<String> onDeviceIdChanged;
  final ValueChanged<String> onBrokerChanged;

  final VoidCallback onRestart;
  final VoidCallback onFactoryReset;

  const DeviceSettingsCard({
    super.key,
    required this.deviceId,
    required this.mqttBroker,
    required this.onDeviceIdChanged,
    required this.onBrokerChanged,
    required this.onRestart,
    required this.onFactoryReset,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(12),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [

            const Text(
              "Device Settings",
              style: TextStyle(fontSize: 16, fontWeight: FontWeight.bold),
            ),

            TextField(
              controller: TextEditingController(text: deviceId),
              decoration: const InputDecoration(labelText: "Device ID"),
              onChanged: onDeviceIdChanged,
            ),

            TextField(
              controller: TextEditingController(text: mqttBroker),
              decoration: const InputDecoration(labelText: "MQTT Broker"),
              onChanged: onBrokerChanged,
            ),

            const SizedBox(height: 10),

            Row(
              children: [

                ElevatedButton(
                  onPressed: onRestart,
                  child: const Text("Restart"),
                ),

                const SizedBox(width: 10),

                ElevatedButton(
                  onPressed: onFactoryReset,
                  child: const Text("Factory Reset"),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}