import 'package:flutter/material.dart';
import 'pages/dashboard_page.dart';

void main() {
  runApp(const AirMonitorApp());
}

class AirMonitorApp extends StatelessWidget {
  const AirMonitorApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'ESP32 Air Monitor',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        useMaterial3: true,
      ),
      home: const DashboardPage(),
    );
  }
}