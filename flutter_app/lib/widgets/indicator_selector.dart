import 'package:flutter/material.dart';

/// ============================
/// 📊 指标选择组件
/// ============================
///
/// 用于选择 Temperature/Humidity/CO2/HCHO
class IndicatorSelector extends StatelessWidget {
  final List<String> indicators;
  final String selected;
  final ValueChanged<String> onChanged;

  const IndicatorSelector({
    super.key,
    required this.indicators,
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
          children: indicators.map((ind) {
            bool isSelected = ind == selected;
            return ChoiceChip(
              label: Text(ind),
              selected: isSelected,
              onSelected: (_) => onChanged(ind),
            );
          }).toList(),
        ),
      ),
    );
  }
}