import 'dart:async';

import 'package:flutter/material.dart';
import '../widgets/sensor_card.dart';

class DashboardPage extends StatefulWidget {
  const DashboardPage({super.key});

  @override
  State<DashboardPage> createState() => _DashboardPageState();
}

class _DashboardPageState extends State<DashboardPage> {

  double temperature = 25.3;
  double humidity = 52.1;
  double co2 = 486;
  double hcho = 0.031;

  Timer? timer;

  @override
  void initState() {
    super.initState();

    timer = Timer.periodic(
      const Duration(seconds: 2),
          (_) {
        setState(() {
          temperature += 0.1;
          humidity += 0.1;
          co2 += 1;
          hcho += 0.001;
        });
      },
    );
  }

  @override
  void dispose() {
    timer?.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {

    return Scaffold(
      appBar: AppBar(
        title: const Text('ESP32环境监测仪'),
        centerTitle: true,
      ),
      body: Padding(
        padding: const EdgeInsets.all(12),
        child: GridView.count(
          crossAxisCount: 2,
          crossAxisSpacing: 12,
          mainAxisSpacing: 12,
          children: [

            SensorCard(
              title: '温度',
              value: temperature.toStringAsFixed(1),
              unit: '°C',
              icon: Icons.thermostat,
            ),

            SensorCard(
              title: '湿度',
              value: humidity.toStringAsFixed(1),
              unit: '%RH',
              icon: Icons.water_drop,
            ),

            SensorCard(
              title: 'CO₂',
              value: co2.toStringAsFixed(0),
              unit: 'ppm',
              icon: Icons.co2,
            ),

            SensorCard(
              title: 'HCHO',
              value: hcho.toStringAsFixed(3),
              unit: 'mg/m³',
              icon: Icons.air,
            ),
          ],
        ),
      ),
    );
  }
}