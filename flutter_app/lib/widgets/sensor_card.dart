import 'package:flutter/material.dart';

/// ============================
/// 🌡️ 通用传感器卡片
/// ============================
///
/// 用于：
/// HTU21D / CO2 / HCHO
///
/// 特点：
/// ✔ 标题
/// ✔ 数值
/// ✔ 单位
/// ✔ 更新时间
/// ✔ 刷新按钮
class SensorCard extends StatelessWidget {
  final String title;
  final String value;
  final String unit;
  final String updateTime;
  final VoidCallback onRefresh;

  const SensorCard({
    super.key,
    required this.title,
    required this.value,
    required this.unit,
    required this.updateTime,
    required this.onRefresh,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(12),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [

            /// 标题
            Text(
              title,
              style: const TextStyle(
                fontSize: 16,
                fontWeight: FontWeight.bold,
              ),
            ),

            const SizedBox(height: 8),

            /// 数值
            Text(
              value,
              style: const TextStyle(
                fontSize: 22,
              ),
            ),

            /// 单位
            Text(unit),

            const SizedBox(height: 8),

            /// 更新时间
            Text("Updated: $updateTime"),

            const SizedBox(height: 8),

            /// 刷新按钮
            ElevatedButton(
              onPressed: onRefresh,
              child: const Text("Refresh"),
            ),
          ],
        ),
      ),
    );
  }
}