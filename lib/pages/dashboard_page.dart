// pages/dashboard_page.dart
import 'package:flutter/material.dart';
import 'package:intl/intl.dart';

import '../widgets/location_card.dart';
import '../widgets/device_status_card.dart';
import '../widgets/sensor_card.dart';
import '../widgets/htu21dsensor_card.dart';
import '../services/mqtt_service.dart';
import '../models/sensor_data.dart';
import '../models/device_status.dart';

import 'package:provider/provider.dart';
import '../core/settings_manager.dart';

class DashboardPage extends StatefulWidget {
  const DashboardPage({super.key});

  @override
  State<DashboardPage> createState() => _DashboardPageState();
}

class _DashboardPageState extends State<DashboardPage> {
  late MQTTService mqttService;

  // String time = "Unknown";
  // String country = "Unknown";
  // String city = "Unknown";

  DeviceStatus deviceStatus = DeviceStatus(
    sensors: " ",
    wifi: " ",
    mqtt: " ",
    uptime: " ",
    lastUpdate: " ",
  );

  SensorData sensorData = SensorData(
    temperature: 0,
    humidity: 0,
    co2: 0,
    hcho: 0,
  );

  String formatTimeByTimezone(String timezone) {
    final now = DateTime.now().toUtc();

    int offset = 8;

    if (timezone.contains("+")) {
      offset = int.parse(timezone.split("+")[1]);
    } else if (timezone.contains("-")) {
      offset = -int.parse(timezone.split("-")[1]);
    }

    final localTime = now.add(Duration(hours: offset));

    return DateFormat("yyyy-MM-dd HH:mm:ss").format(localTime);
  }

  double parseNum(dynamic v) => (v as num).toDouble();

  @override
  void initState() {
    super.initState();

    mqttService = MQTTService(
        broker: "104.168.81.179", port: 8883, clientId: "flutter_app");

    // async 初始化
    (() async {
      mqttService.onSensorData = (json) {
        setState(() {
          sensorData = SensorData(
            temperature: parseNum(json["temp"]),
            humidity: parseNum(json["humi"]),
            co2: parseNum(json["co2"]),
            hcho: parseNum(json["hcho"]),
          );

          deviceStatus = DeviceStatus(
            sensors: "${json["sensors"]}",
            wifi: "${json["wifi"]}",
            mqtt: "${json["mqtt"]}",
            uptime: "${json["uptime"]}",
            lastUpdate: DateFormat("HH:mm:ss").format(DateTime.now()),
          );
        });
      };

      await mqttService.initTls("assets/certs/ca.crt");
      await mqttService.connect();

      // 每秒更新时间
      Future.doWhile(() async {
        await Future.delayed(const Duration(seconds: 1));
        if (!mounted) return false;

        Provider.of<SettingsManager>(context, listen: false)
            .updateTime(DateTime.now());

        return true;
      });
    })();
  }

  @override
  void dispose() {
    mqttService.disconnect();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final settings = Provider.of<SettingsManager>(context);
    return Scaffold(
      appBar: AppBar(title: const Text("Air Monitor"), centerTitle: true),
      body: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.all(12),
          child: Column(
            children: [
              // LocationCard(time: time, country: country, city: city),
              LocationCard(
                time: formatTimeByTimezone(settings.timezone),
                country: settings.country,
                city: settings.city,
              ),
              const SizedBox(height: 8),

              DeviceStatusCard(
                sensors: deviceStatus.sensors,
                wifi: deviceStatus.wifi,
                mqtt: deviceStatus.mqtt,
                uptime: deviceStatus.uptime,
                lastUpdate: deviceStatus.lastUpdate,
              ),
              const SizedBox(height: 8),

              HTU21DSensorCard(
                title: "HTU21D",
                value1: "Temp: ${sensorData.temperature}",
                unit1: "°C",
                value2: "Humi: ${sensorData.humidity}",
                unit2: "%RH",
                onRefresh: () {},
              ),
              const SizedBox(height: 8),
              SensorCard(
                title: "S8 0053",
                value: "CO2: ${sensorData.co2}",
                unit: "ppm",
                onRefresh: () {},
              ),
              const SizedBox(height: 8),
              SensorCard(
                title: "ZE08",
                value: "HCHO: ${sensorData.hcho}",
                unit: "mg/m3",
                onRefresh: () {},
              ),
            ],
          ),
        ),
      ),
    );
  }
}