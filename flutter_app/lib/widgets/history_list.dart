import 'package:flutter/material.dart';

/// ============================
/// 📜 历史数据列表
/// ============================
///
/// 显示时间 + 指标值
class HistoryList extends StatelessWidget {
  final List<Map<String, dynamic>> data;
  final String indicator;

  const HistoryList({
    super.key,
    required this.data,
    required this.indicator,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Column(
        children: data.map((item) {
          return ListTile(
            title: Text(item["time"]),
            trailing: Text("${item["value"]}"),
          );
        }).toList(),
      ),
    );
  }
}