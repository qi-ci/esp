import 'package:flutter/material.dart';

import 'widgets/device_settings_card.dart';
import 'widgets/data_settings_card.dart';
import 'widgets/location_settings_card.dart';

/// ==============================
/// ⚙️ Settings Page（设置页面）
/// ==============================
///
/// 三大模块：
///
/// ① 设备设置（Device）
/// ② 数据设置（Data）
/// ③ 地区设置（Location）
///
/// ⚠️ 当前为 UI 架构层（Mock数据）
/// 后续接：
/// - device_controller
/// - mqtt_service
/// - app_config
class SettingsPage extends StatefulWidget {
  const SettingsPage({super.key});

  @override
  State<SettingsPage> createState() => _SettingsPageState();
}

class _SettingsPageState extends State<SettingsPage> {

  /// ==========================
  /// 📌 Mock 配置数据
  /// ==========================

  String deviceId = "ESP32-001";
  String mqttBroker = "192.168.1.100";

  bool autoRefresh = true;
  int refreshInterval = 10;
  int historyRetentionDays = 30;

  String country = "USA";
  String city = "Buffalo";
  String timezone = "UTC-5";
  String language = "en";

  /// ==========================
  /// 📌 UI更新（Mock）
  /// ==========================
  void updateState() {
    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Settings"),
      ),

      body: SingleChildScrollView(
        padding: const EdgeInsets.all(12),
        child: Column(
          children: [

            /// ==========================
            /// ① Device Settings
            /// ==========================
            DeviceSettingsCard(
              deviceId: deviceId,
              mqttBroker: mqttBroker,
              onDeviceIdChanged: (v) {
                setState(() => deviceId = v);
              },
              onBrokerChanged: (v) {
                setState(() => mqttBroker = v);
              },
              onRestart: () {
                /// TODO: command_controller -> restart ESP32
              },
              onFactoryReset: () {
                /// TODO: MQTT command -> reset device
              },
            ),

            const SizedBox(height: 12),

            /// ==========================
            /// ② Data Settings
            /// ==========================
            DataSettingsCard(
              autoRefresh: autoRefresh,
              refreshInterval: refreshInterval,
              historyRetentionDays: historyRetentionDays,
              onAutoRefreshChanged: (v) {
                setState(() => autoRefresh = v);
              },
              onIntervalChanged: (v) {
                setState(() => refreshInterval = v);
              },
              onRetentionChanged: (v) {
                setState(() => historyRetentionDays = v);
              },
            ),

            const SizedBox(height: 12),

            /// ==========================
            /// ③ Location Settings
            /// ==========================
            LocationSettingsCard(
              country: country,
              city: city,
              timezone: timezone,
              language: language,
              onCountryChanged: (v) => setState(() => country = v),
              onCityChanged: (v) => setState(() => city = v),
              onTimezoneChanged: (v) => setState(() => timezone = v),
              onLanguageChanged: (v) => setState(() => language = v),
            ),
          ],
        ),
      ),
    );
  }
}