import 'package:flutter/material.dart';

/// ============================
/// 🌍 地区 & 语言设置卡片
/// ============================
///
/// 包含：
/// - 国家
/// - 城市
/// - 时区
/// - 语言
class LocationSettingsCard extends StatelessWidget {
  final String country;
  final String city;
  final String timezone;
  final String language;

  final ValueChanged<String> onCountryChanged;
  final ValueChanged<String> onCityChanged;
  final ValueChanged<String> onTimezoneChanged;
  final ValueChanged<String> onLanguageChanged;

  const LocationSettingsCard({
    super.key,
    required this.country,
    required this.city,
    required this.timezone,
    required this.language,
    required this.onCountryChanged,
    required this.onCityChanged,
    required this.onTimezoneChanged,
    required this.onLanguageChanged,
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
              "Location Settings",
              style: TextStyle(fontSize: 16, fontWeight: FontWeight.bold),
            ),

            DropdownButton<String>(
              value: country,
              items: const [
                DropdownMenuItem(value: "USA", child: Text("USA")),
                DropdownMenuItem(value: "China", child: Text("China")),
              ],
              onChanged: (v) => onCountryChanged(v ?? country),
            ),

            TextField(
              decoration: const InputDecoration(labelText: "City"),
              controller: TextEditingController(text: city),
              onChanged: onCityChanged,
            ),

            DropdownButton<String>(
              value: timezone,
              items: const [
                DropdownMenuItem(value: "UTC-5", child: Text("UTC-5")),
                DropdownMenuItem(value: "UTC+8", child: Text("UTC+8")),
              ],
              onChanged: (v) => onTimezoneChanged(v ?? timezone),
            ),

            DropdownButton<String>(
              value: language,
              items: const [
                DropdownMenuItem(value: "en", child: Text("English")),
                DropdownMenuItem(value: "zh", child: Text("中文")),
              ],
              onChanged: (v) => onLanguageChanged(v ?? language),
            ),
          ],
        ),
      ),
    );
  }
}