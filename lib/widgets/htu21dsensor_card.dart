import 'package:flutter/material.dart';
/// ============================
/// 🌡️ 通用传感器卡片
/// ============================
///
/// 用于：
/// HTU21D
///
/// 特点：
/// ✔ 标题
/// ✔ 数值
/// ✔ 单位
/// ✔ 刷新按钮
class HTU21DSensorCard extends StatelessWidget {
  final String title;
  final String value1;
  final String value2;
  final String unit1;
  final String unit2;
  final VoidCallback onRefresh;

  const HTU21DSensorCard({
    super.key,
    required this.title,
    required this.value1,
    required this.unit1,
    required this.value2,
    required this.unit2,
    required this.onRefresh,
  });

  @override
  Widget _buildValueRow(String value, String unit) {
    return Row(
      crossAxisAlignment: CrossAxisAlignment.end,
      children: [
        Text(
          value,
          style: TextStyle(fontSize: 18),
        ),
        const SizedBox(width: 8),
        Text(
          unit,
          style: TextStyle(fontSize: 18),
        ),
      ],
    );
  }

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
            Expanded(
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    title,
                    style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
                  ),
                  const SizedBox(height: 8),
                  _buildValueRow(value1, unit1),
                  const SizedBox(height: 8),
                  _buildValueRow(value2, unit2),
                ],
              ),
            ),
            // 右边刷新按钮
            IconButton(
              onPressed: onRefresh,
              icon: Icon(Icons.refresh),
            )
          ]
        ),
      ),
    );
  }
}