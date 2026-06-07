import 'package:flutter/material.dart';

import 'widgets/location_card.dart';
import 'widgets/device_status_card.dart';
import 'widgets/sensor_card.dart';

/// ==============================
/// 📊 Dashboard Page（主页面）
/// ==============================
///
/// 结构：
///
/// ┌──────────────┐
/// │ LocationCard │  时间 / 国家 / 城市
/// └──────────────┘
///
/// ┌──────────────┐
/// │ DeviceStatus │  ESP32状态
/// └──────────────┘
///
/// ┌──────────────┐
/// │ HTU21D Card  │
/// └──────────────┘
///
/// ┌──────────────┐
/// │ S8 CO2 Card  │
/// └──────────────┘
///
/// ┌──────────────┐
/// │ ZE08 HCHO    │
/// └──────────────┘
///
/// ⚠️ 当前全部使用 Mock 数据（后续接 MQTT）
class DashboardPage extends StatefulWidget {
  const DashboardPage({super.key});

  @override
  State<DashboardPage> createState() => _DashboardPageState();
}

class _DashboardPageState extends State<DashboardPage> {
  /// ==========================
  /// 📌 Mock 数据（后面替换成 Controller）
  /// ==========================

  String time = "2026-06-02 10:30";
  String country = "USA";
  String city = "Buffalo";

  /// 设备状态
  Map<String, String> deviceStatus = {
    "online": "Device Online",
    "wifi": "Connected",
    "mqtt": "Connected",
    "uptime": "12h 35m",
    // "fw": "v1.0.3",
  };

  /// 传感器数据
  double temperature = 25.3;
  double humidity = 48.2;
  double co2 = 612;
  double hcho = 0.03;

  /// 更新时间
  String lastUpdate = "10:29:55";

  /// ==========================
  /// 📌 UI刷新（模拟按钮）
  /// ==========================
  void refreshSensor(String sensor) {
    setState(() {
      lastUpdate = DateTime.now().toString().substring(11, 19);
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Environment Monitor"),
      ),

      /// ==========================
      /// 📌 页面主体
      /// ==========================
      body: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.all(12),
          child: Column(
            children: [

              /// ==========================
              /// 📍 ① 时间 / 国家 / 城市
              /// ==========================
              LocationCard(
                time: time,
                country: country,
                city: city,
              ),

              const SizedBox(height: 12),

              /// ==========================
              /// 📡 ② 设备状态
              /// ==========================
              DeviceStatusCard(
                status: deviceStatus,
              ),

              const SizedBox(height: 12),

              /// ==========================
              /// 🌡️ ③ HTU21D
              /// ==========================
              SensorCard(
                title: "HTU21D",
                value: "$temperature / $humidity",
                unit: "°C / %RH",
                updateTime: lastUpdate,
                onRefresh: () => refreshSensor("htu21d"),
              ),

              const SizedBox(height: 12),

              /// ==========================
              /// 🌫️ ④ S8 CO2
              /// ==========================
              SensorCard(
                title: "S8 CO2",
                value: "$co2",
                unit: "ppm",
                updateTime: lastUpdate,
                onRefresh: () => refreshSensor("co2"),
              ),

              const SizedBox(height: 12),

              /// ==========================
              /// ☁️ ⑤ ZE08 HCHO
              /// ==========================
              SensorCard(
                title: "ZE08 HCHO",
                value: "$hcho",
                unit: "mg/m3",
                updateTime: lastUpdate,
                onRefresh: () => refreshSensor("hcho"),
              ),
            ],
          ),
        ),
      ),
    );
  }
}