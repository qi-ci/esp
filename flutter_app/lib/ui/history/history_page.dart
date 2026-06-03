import 'package:flutter/material.dart';
import 'widgets/indicator_selector.dart';
import 'widgets/time_range_selector.dart';
import 'widgets/history_chart.dart';
import 'widgets/history_list.dart';

/// ==============================
/// 📜 History Page（历史记录）
/// ==============================
///
/// 功能：
— 1️⃣ 指标选择（温度/湿度/CO2/HCHO）
/// 2️⃣ 时间范围选择（24h/7d/30d/自定义）
/// 3️⃣ 历史数据列表
/// 4️⃣ 趋势图
class HistoryPage extends StatefulWidget {
  const HistoryPage({super.key});

  @override
  State<HistoryPage> createState() => _HistoryPageState();
}

class _HistoryPageState extends State<HistoryPage> {

  /// ==========================
  /// 📌 Mock 数据
  /// ==========================
  List<String> indicators = ["Temperature", "Humidity", "CO2", "HCHO"];
  String selectedIndicator = "Temperature";

  List<String> timeRanges = ["24h", "7d", "30d", "Custom"];
  String selectedTimeRange = "24h";

  /// Mock 历史数据（timestamp -> value）
  List<Map<String, dynamic>> historyData = [
    {"time": "10:00", "value": 25.3},
    {"time": "11:00", "value": 25.8},
    {"time": "12:00", "value": 26.0},
  ];

  void onIndicatorChange(String indicator) {
    setState(() {
      selectedIndicator = indicator;
      // TODO: 后续绑定 sensor_controller 更新数据
    });
  }

  void onTimeRangeChange(String range) {
    setState(() {
      selectedTimeRange = range;
      // TODO: 后续绑定 history_controller 查询数据
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("History"),
      ),
      body: SingleChildScrollView(
        padding: const EdgeInsets.all(12),
        child: Column(
          children: [

            /// ==========================
            /// 📌 ① 指标选择
            /// ==========================
            IndicatorSelector(
              indicators: indicators,
              selected: selectedIndicator,
              onChanged: onIndicatorChange,
            ),

            const SizedBox(height: 12),

            /// ==========================
            /// 📌 ② 时间范围选择
            /// ==========================
            TimeRangeSelector(
              ranges: timeRanges,
              selected: selectedTimeRange,
              onChanged: onTimeRangeChange,
            ),

            const SizedBox(height: 12),

            /// ==========================
            /// 📌 ③ 历史趋势图
            /// ==========================
            HistoryChart(
              data: historyData,
              indicator: selectedIndicator,
            ),

            const SizedBox(height: 12),

            /// ==========================
            /// 📌 ④ 历史列表
            /// ==========================
            HistoryList(
              data: historyData,
              indicator: selectedIndicator,
            ),
          ],
        ),
      ),
    );
  }
}