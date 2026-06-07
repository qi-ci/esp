import 'package:flutter/material.dart';
import 'input_card.dart';

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

  final TextEditingController ssidController;
  final TextEditingController passwordController;
  final VoidCallback onSaveWifi;

  final VoidCallback onRestart;

  const DeviceSettingsCard({
    super.key,
    required this.deviceId,
    required this.mqttBroker,
    required this.ssidController,
    required this.passwordController,
    required this.onSaveWifi,
    required this.onRestart,
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
            const SizedBox(height: 8),

            Text(
              "Device ID: $deviceId",
              style: TextStyle(fontSize: 14),
            ),

            const SizedBox(height: 8),

            Text(
              "MQTT Brocker: $mqttBroker",
              style: TextStyle(fontSize: 14),
            ),

            const SizedBox(height: 8),

            InputCard(
              title: "WiFi Configuration",
              controller1: ssidController,
              label1: "SSID",
              controller2: passwordController,
              label2: "Password",
              obscureText2: true,
              onSave: onSaveWifi,
            ),

            const SizedBox(height: 8),

            Row(
              children: [
                ElevatedButton(
                  onPressed: onRestart,
                  child: const Text("Restart"),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}