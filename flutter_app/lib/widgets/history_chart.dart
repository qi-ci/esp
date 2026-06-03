import 'package:flutter/material.dart';

/// ============================
/// 📈 历史趋势图占位组件
/// ============================
///
/// TODO: 后续使用 fl_chart/echarts_flutter 实现真实折线图
class HistoryChart extends StatelessWidget {
  final List<Map<String, dynamic>> data;
  final String indicator;

  const HistoryChart({
    super.key,
    required this.data,
    required this.indicator,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      child: SizedBox(
        height: 200,
        child: Center(
          child: Text(
            "$indicator Trend Chart Placeholder",
            style: const TextStyle(fontSize: 16, color: Colors.grey),
          ),
        ),
      ),
    );
  }
}