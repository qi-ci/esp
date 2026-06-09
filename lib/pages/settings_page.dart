import 'package:flutter/material.dart';

import '../widgets/device_settings_card.dart';
import '../widgets/data_settings_card.dart';
import '../widgets/timelocation_settings_card.dart';

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

  String deviceId = "airmon_livingroom";
  String mqttBroker = "104.168.81.179";
  bool autoRefresh = true;
  String timezone = "UTC+8";

  final TextEditingController htuController = TextEditingController();
  final TextEditingController s8Controller = TextEditingController();
  final TextEditingController ze08Controller = TextEditingController();
  final TextEditingController ssidController = TextEditingController();
  final TextEditingController passwordController = TextEditingController();
  final TextEditingController countyController = TextEditingController();
  final TextEditingController cityController = TextEditingController();

  @override
  void initState() {
    super.initState();

    // 初始化默认值
    htuController.text = "1000";   // ms
    s8Controller.text = "2000";    // ms
    ze08Controller.text = "5000";  // ms

    ssidController.text = "MyWiFi";
    passwordController.text = "12345678";

    countyController.text = "China";
    cityController.text = "Henan";
  }

  // 回调
  void _saveHTU() {
    print("Save HTU: ${htuController.text}");
  }

  void _saveS8() {
    print("Save S8: ${s8Controller.text}");
  }

  void _saveZE08() {
    print("Save ZE08: ${ze08Controller.text}");
  }

  void _saveWifi() {
    print("Save WiFi SSID: ${ssidController.text}, Password: ${passwordController.text}");
  }

  void _saveLocation() {
    print("Save Location: ${countyController.text}, Password: ${cityController.text}");
  }


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
        centerTitle: true,
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

              ssidController: ssidController,
              passwordController: passwordController,
              onSaveWifi: _saveWifi,

              onRestart: () {
                print("restart");
              },
            ),

            const SizedBox(height: 8),

            /// ==========================
            /// ② Data Settings
            /// ==========================
            DataSettingsCard(
              autoRefresh: autoRefresh,
              onAutoRefreshChanged: (v) {
                setState(() => autoRefresh = v);
              },

              htuController: htuController,
              s8Controller: s8Controller,
              ze08Controller: ze08Controller,
              onSaveHTU: _saveHTU,
              onSaveS8: _saveS8,
              onSaveZE08: _saveZE08,
            ),

            const SizedBox(height: 8),

            /// ==========================
            /// ③ timeLocation Settings
            /// ==========================
            TimeLocationSettingsCard(
              timezone: timezone,
              onTimezoneChanged: (v) => setState(() => timezone = v),

              countyController: countyController,
              cityController: cityController,
              onSaveLocation: _saveLocation,
            ),
          ],
        ),
      ),
    );
  }
}