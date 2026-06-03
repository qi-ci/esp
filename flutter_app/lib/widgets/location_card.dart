import 'package:flutter/material.dart';

/// ============================
/// 📍 时间 + 国家 + 城市卡片
/// ============================
class LocationCard extends StatelessWidget {
  final String time;
  final String country;
  final String city;

  const LocationCard({
    super.key,
    required this.time,
    required this.country,
    required this.city,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      child: ListTile(
        title: Text(city),
        subtitle: Text(country),
        trailing: Text(time),
      ),
    );
  }
}