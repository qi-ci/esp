import 'package:flutter/material.dart';
import 'input_card.dart';

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
  final ValueChanged<bool> onAutoRefreshChanged;

  final TextEditingController htuController;
  final TextEditingController s8Controller;
  final TextEditingController ze08Controller;

  final VoidCallback onSaveHTU;
  final VoidCallback onSaveS8;
  final VoidCallback onSaveZE08;

  const DataSettingsCard({
    super.key,
    required this.autoRefresh,
    required this.onAutoRefreshChanged,
    required this.htuController,
    required this.s8Controller,
    required this.ze08Controller,
    required this.onSaveHTU,
    required this.onSaveS8,
    required this.onSaveZE08,
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
            const SizedBox(height: 8),

            InputCard(
              title: "HTU21D Interval",
              controller1: htuController,
              label1: "Interval (ms)",
              onSave: onSaveHTU,
            ),

            InputCard(
              title: "S8 Interval",
              controller1: s8Controller,
              label1: "Interval (ms)",
              onSave: onSaveS8,
            ),

            InputCard(
              title: "ZE08 Interval",
              controller1: ze08Controller,
              label1: "Interval (ms)",
              onSave: onSaveZE08,
            ),
          ],
        ),
      ),
    );
  }
}