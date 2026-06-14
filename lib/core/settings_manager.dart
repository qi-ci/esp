import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

class SettingsManager extends ChangeNotifier {
  String country = "Unknown";
  String city = "Unknown";
  String timezone = "UTC+8";

  DateTime _currentTime = DateTime.now();

  DateTime get currentTime => _currentTime;

  SettingsManager() {
    loadSettings(); // App启动自动加载
  }

  /// ==========================
  /// 📥 读取本地存储
  /// ==========================
  Future<void> loadSettings() async {
    final prefs = await SharedPreferences.getInstance();

    timezone = prefs.getString('timezone') ?? "UTC+8";
    city = prefs.getString('city') ?? "Unknown";
    country = prefs.getString('country') ?? "Unknown";

    notifyListeners();
  }
  /// ==========================
  /// 📤 保存设置
  /// ==========================
  Future<void> updateLocation({
    required String country,
    required String city,
    required String timezone,
  }) async {

    this.country = country;
    this.city = city;
    this.timezone = timezone;

    final prefs = await SharedPreferences.getInstance();

    await prefs.setString('country', country);
    await prefs.setString('city', city);
    await prefs.setString('timezone', timezone);

    notifyListeners();
  }

  void updateTime(DateTime time) {
    _currentTime = time;
    notifyListeners();
  }
}