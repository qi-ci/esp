import 'package:flutter/material.dart';

/// ============================
/// ⏰ 时间范围选择组件
/// ============================
///
/// 选项：24h / 7d / 30d / 自定义
class TimeRangeSelector extends StatelessWidget {
  final List<String> ranges;
  final String selected;
  final ValueChanged<String> onChanged;

  const TimeRangeSelector({
    super.key,
    required this.ranges,
    required this.selected,
    required this.onChanged,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(8),
        child: Wrap(
          spacing: 12,
          children: ranges.map((range) {
            bool isSelected = range == selected;
            return ChoiceChip(
              label: Text(range),
              selected: isSelected,
              onSelected: (_) => onChanged(range),
            );
          }).toList(),
        ),
      ),
    );
  }
}