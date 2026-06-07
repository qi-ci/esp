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
      // elevation: 3,
      // margin: const EdgeInsets.all(8),
      child: Padding(
        padding: const EdgeInsets.all(12),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            // 左边状态
            Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  country,
                  style: TextStyle(fontSize: 18),
                ),

                Text(
                  city,
                  style: TextStyle(fontSize: 16),
                ),
              ],
            ),
            // 右边时间信息
            Column(
              crossAxisAlignment: CrossAxisAlignment.end,
              children: [
                Text(
                  time,
                  style: TextStyle(fontSize: 16),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}