// import 'package:flutter/material.dart';
// import '../widgets/sensor_card.dart';
// import '../services/mqtt_service.dart';
// import '../models/sensor_data.dart';
//
// class DashboardPage extends StatefulWidget {
//   const DashboardPage({super.key});
//
//   @override
//   State<DashboardPage> createState() => _DashboardPageState();
// }
//
// class _DashboardPageState extends State<DashboardPage> {
//   final MqttService mqttService = MqttService(
//     broker: '192.168.1.100', // 替换为你的 broker
//     clientId: 'flutter_client_01',
//     topic: 'esp32/airmon_livingroom/tele',
//   );
//
//   SensorData sensorData = SensorData();
//
//   @override
//   void initState() {
//     super.initState();
//     mqttService.onSensorData = (data) {
//       setState(() {
//         sensorData = data;
//       });
//     };
//     mqttService.connect();
//   }
//
//   @override
//   void dispose() {
//     mqttService.disconnect();
//     super.dispose();
//   }
//
//   @override
//   Widget build(BuildContext context) {
//     return Scaffold(
//       appBar: AppBar(
//         title: Text(
//           'ESP32环境监测仪 - ${sensorData.deviceOnline ? "在线" : "离线"}',
//         ),
//         centerTitle: true,
//       ),
//       body: Padding(
//         padding: const EdgeInsets.all(12),
//         child: GridView.count(
//           crossAxisCount: 2,
//           crossAxisSpacing: 12,
//           mainAxisSpacing: 12,
//           children: [
//             SensorCard(
//               title: '温度',
//               value: sensorData.temperature.toStringAsFixed(1),
//               unit: '°C',
//               icon: Icons.thermostat,
//             ),
//             SensorCard(
//               title: '湿度',
//               value: sensorData.humidity.toStringAsFixed(1),
//               unit: '%RH',
//               icon: Icons.water_drop,
//             ),
//             SensorCard(
//               title: 'CO₂',
//               value: sensorData.co2.toStringAsFixed(0),
//               unit: 'ppm',
//               icon: Icons.co2,
//             ),
//             SensorCard(
//               title: 'HCHO',
//               value: sensorData.hcho.toStringAsFixed(3),
//               unit: 'mg/m³',
//               icon: Icons.air,
//             ),
//           ],
//         ),
//       ),
//     );
//   }
// }

import 'package:flutter/material.dart';

import '../widgets/location_card.dart';
import '../widgets/device_status_card.dart';
import '../widgets/sensor_card.dart';
import '../widgets/htu21dsensor_card.dart';

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

  String sensors = "OK";
  String wifi = "Connected";
  String mqtt = "Connected";
  String uptime = "12h 35m";
  String lastUpdate = "10:29:55";

  /// 传感器数据
  double temperature = 25.3;
  double humidity = 48.2;
  double co2 = 612;
  double hcho = 0.03;

  /// 更新时间


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
        title: const Text("Air Monitor"),
        centerTitle: true,
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

              const SizedBox(height: 8),

              /// ==========================
              /// 📡 ② 设备状态
              /// ==========================
              DeviceStatusCard(
                sensors: sensors,
                wifi: wifi,
                mqtt: mqtt,
                uptime: uptime,
                lastUpdate: lastUpdate,
              ),

              const SizedBox(height: 8),

              /// ==========================
              /// 🌡️ ③ HTU21D
              /// ==========================
              HTU21DSensorCard(
                title: "HTU21D",
                value1: "Temp: $temperature",
                unit1: "°C",
                value2: "Humi: $humidity",
                unit2: "%RH",
                onRefresh: () => refreshSensor("fresh"),
              ),

              const SizedBox(height: 8),

              /// ==========================
              /// 🌫️ ④ S8 CO2
              /// ==========================
              SensorCard(
                title: "S8 0053",
                value: "CO2: $co2",
                unit: "ppm",
                onRefresh: () => refreshSensor("fresh"),
              ),

              const SizedBox(height: 8),

              /// ==========================
              /// ☁️ ⑤ ZE08 HCHO
              /// ==========================
              SensorCard(
                title: "ZE08",
                value: "HCHO: $hcho",
                unit: "mg/m3",
                onRefresh: () => refreshSensor("fresh"),
              ),
            ],
          ),
        ),
      ),
    );
  }
}