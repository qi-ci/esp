import 'package:flutter/material.dart';
import 'input_card.dart';

final List<String> timezones = [
  "UTC-12", "UTC-11", "UTC-10", "UTC-9", "UTC-8",
  "UTC-7", "UTC-6", "UTC-5", "UTC-4", "UTC-3",
  "UTC-2", "UTC-1", "UTC+0", "UTC+1", "UTC+2",
  "UTC+3", "UTC+4", "UTC+5", "UTC+6", "UTC+7",
  "UTC+8", "UTC+9", "UTC+10", "UTC+11", "UTC+12"
];

/// ============================
/// 🌍 地区 & 语言设置卡片
/// ============================
///
/// 包含：
/// - 国家
/// - 城市
/// - 时区
/// - 语言
class TimeLocationSettingsCard extends StatelessWidget {
  final TextEditingController countyController;
  final TextEditingController cityController;
  final VoidCallback onSaveLocation;

  final String timezone;
  final ValueChanged<String> onTimezoneChanged;

  const TimeLocationSettingsCard({
    super.key,
    required this.timezone,
    required this.onTimezoneChanged,
    required this.countyController,
    required this.cityController,
    required this.onSaveLocation,
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
              "TimeLocation Settings",
              style: TextStyle(fontSize: 16, fontWeight: FontWeight.bold),
            ),

            const SizedBox(height: 8),

            InputCard(
              title: "Location",
              controller1: countyController,
              label1: "County",
              controller2: cityController,
              label2: "City",
              obscureText2: true,
              onSave: onSaveLocation,
            ),

            ListTile(
              title: const Text("Timezone"),
              subtitle: Text(timezone), // 显示当前选中的时区
              trailing: const Icon(Icons.arrow_drop_down),
              onTap: () async {
                // 弹出选择框
                final selected = await showDialog<String>(
                  context: context,
                  builder: (_) => SimpleDialog(
                    title: const Text("Select Timezone"),
                    children: timezones
                        .map((tz) => SimpleDialogOption(
                      child: Text(tz),
                      onPressed: () => Navigator.pop(context, tz), // 选择后关闭
                    ))
                        .toList(),
                  ),
                );
                if (selected != null) onTimezoneChanged(selected); // ✅ 回传给父组件
              },
            )
          ],
        ),
      ),
    );
  }
}