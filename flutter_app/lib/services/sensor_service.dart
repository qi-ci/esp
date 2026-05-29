import 'dart:async';
import 'dart:convert';
import 'package:http/http.dart' as http;
import '../models/sensor_data.dart';

/// 传感器数据服务 - 负责与ESP32通信
class SensorService {
  static final SensorService _instance = SensorService._internal();
  factory SensorService() => _instance;
  SensorService._internal();

  // ESP32服务器地址(需要修改为实际IP)
  String _serverUrl = 'http://192.168.1.100:8080';
  
  // 最新传感器数据
  SensorData? _latestData;
  
  // 数据流控制器
  final StreamController<SensorData> _dataController = 
      StreamController<SensorData>.broadcast();
  
  Stream<SensorData> get dataStream => _dataController.stream;
  
  SensorData? get latestData => _latestData;

  /// 初始化服务
  void initialize({String? serverUrl}) {
    if (serverUrl != null) {
      _serverUrl = serverUrl;
    }
    
    // 启动定时获取数据
    Timer.periodic(const Duration(seconds: 5), (_) {
      fetchData();
    });
  }

  /// 从ESP32获取数据
  Future<SensorData?> fetchData() async {
    try {
      final response = await http.get(
        Uri.parse('$_serverUrl/api/data'),
      ).timeout(const Duration(seconds: 10));

      if (response.statusCode == 200) {
        final jsonData = json.decode(response.body);
        
        if (jsonData['status'] == 'success') {
          final data = SensorData.fromJson(jsonData['data']);
          _latestData = data;
          _dataController.add(data);
          
          print('[SensorService] Data received: $data');
          return data;
        }
      }
      
      print('[SensorService] Failed to fetch data: ${response.statusCode}');
      return null;
    } catch (e) {
      print('[SensorService] Error: $e');
      return null;
    }
  }

  /// 手动刷新数据
  Future<SensorData?> refresh() async {
    return await fetchData();
  }

  /// 更新服务器地址
  void updateServerUrl(String url) {
    _serverUrl = url;
    print('[SensorService] Server URL updated to: $_serverUrl');
  }

  /// 获取服务器地址
  String get serverUrl => _serverUrl;

  /// 释放资源
  void dispose() {
    _dataController.close();
  }
}
