import 'package:flutter/material.dart';

/// ============================
/// 📊 数据设置卡片
/// ============================
///
/// 包含：
/// - 自动刷新开关
/// - 刷新间隔
/// - 历史数据保留时间
class DataSettingsCard extends StatelessWidget {
  final bool autoRefresh;
  final int refreshInterval;
  final int historyRetentionDays;

  final ValueChanged<bool> onAutoRefreshChanged;
  final ValueChanged<int> onIntervalChanged;
  final ValueChanged<int> onRetentionChanged;

  const DataSettingsCard({
    super.key,
    required this.autoRefresh,
    required this.refreshInterval,
    required this.historyRetentionDays,
    required this.onAutoRefreshChanged,
    required this.onIntervalChanged,
    required this.onRetentionChanged,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(12),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [

            const Text(
              "Data Settings",
              style: TextStyle(fontSize: 16, fontWeight: FontWeight.bold),
            ),

            SwitchListTile(
              title: const Text("Auto Refresh"),
              value: autoRefresh,
              onChanged: onAutoRefreshChanged,
            ),

            TextField(
              keyboardType: TextInputType.number,
              decoration: const InputDecoration(
                labelText: "Refresh Interval (s)",
              ),
              onChanged: (v) => onIntervalChanged(int.tryParse(v) ?? 10),
            ),

            TextField(
              keyboardType: TextInputType.number,
              decoration: const InputDecoration(
                labelText: "History Retention Days",
              ),
              onChanged: (v) => onRetentionChanged(int.tryParse(v) ?? 30),
            ),
          ],
        ),
      ),
    );
  }
}