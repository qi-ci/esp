import 'package:flutter/material.dart';
import '../services/sensor_service.dart';
import '../models/sensor_data.dart';

/// 首页 - 实时数据显示
class HomeScreen extends StatefulWidget {
  const HomeScreen({super.key});

  @override
  State<HomeScreen> createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  final SensorService _sensorService = SensorService();
  SensorData? _currentData;
  bool _isLoading = false;

  @override
  void initState() {
    super.initState();
    
    // 监听数据流
    _sensorService.dataStream.listen((data) {
      if (mounted) {
        setState(() {
          _currentData = data;
          _isLoading = false;
        });
      }
    });
    
    // 初始加载
    _refreshData();
  }

  /// 刷新数据
  Future<void> _refreshData() async {
    setState(() => _isLoading = true);
    await _sensorService.refresh();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('环境监测仪'),
        actions: [
          IconButton(
            icon: const Icon(Icons.refresh),
            onPressed: _isLoading ? null : _refreshData,
            tooltip: '刷新数据',
          ),
        ],
      ),
      body: RefreshIndicator(
        onRefresh: _refreshData,
        child: SingleChildScrollView(
          physics: const AlwaysScrollableScrollPhysics(),
          padding: const EdgeInsets.all(16.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.stretch,
            children: [
              // 连接状态
              _buildConnectionStatus(),
              
              const SizedBox(height: 16),
              
              // 传感器数据卡片
              if (_currentData != null) ...[
                _buildSensorGrid(_currentData!),
              ] else if (_isLoading) ...[
                const Center(
                  child: Padding(
                    padding: EdgeInsets.all(32.0),
                    child: CircularProgressIndicator(),
                  ),
                ),
              ] else ...[
                _buildEmptyState(),
              ],
              
              const SizedBox(height: 16),
              
              // 数据质量指示
              if (_currentData != null)
                _buildDataQuality(_currentData!),
            ],
          ),
        ),
      ),
    );
  }

  /// 连接状态
  Widget _buildConnectionStatus() {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Row(
          children: [
            Icon(
              _currentData != null 
                  ? Icons.wifi 
                  : Icons.wifi_off,
              color: _currentData != null 
                  ? Colors.green 
                  : Colors.red,
            ),
            const SizedBox(width: 8),
            Text(
              _currentData != null 
                  ? '已连接' 
                  : '未连接',
              style: TextStyle(
                fontWeight: FontWeight.bold,
                color: _currentData != null 
                    ? Colors.green 
                    : Colors.red,
              ),
            ),
            const Spacer(),
            if (_currentData != null)
              Text(
                '最后更新: ${_formatTime(_currentData!.timestamp)}',
                style: Theme.of(context).textTheme.bodySmall,
              ),
          ],
        ),
      ),
    );
  }

  /// 传感器数据网格
  Widget _buildSensorGrid(SensorData data) {
    return GridView.count(
      shrinkWrap: true,
      physics: const NeverScrollableScrollPhysics(),
      crossAxisCount: 2,
      crossAxisSpacing: 12,
      mainAxisSpacing: 12,
      childAspectRatio: 1.2,
      children: [
        _buildSensorCard(
          icon: Icons.thermostat,
          title: '温度',
          value: '${data.temperature.toStringAsFixed(1)}°C',
          unit: '摄氏度',
          color: _getTemperatureColor(data.temperature),
        ),
        _buildSensorCard(
          icon: Icons.water_drop,
          title: '湿度',
          value: '${data.humidity.toStringAsFixed(1)}%',
          unit: '相对湿度',
          color: _getHumidityColor(data.humidity),
        ),
        _buildSensorCard(
          icon: Icons.air,
          title: '甲醛',
          value: '${data.formaldehyde.toStringAsFixed(3)} ppm',
          unit: 'HCHO',
          color: _getFormaldehydeColor(data.formaldehyde),
        ),
        _buildSensorCard(
          icon: Icons.co2,
          title: 'CO₂',
          value: '${data.co2.toStringAsFixed(0)} ppm',
          unit: '二氧化碳',
          color: _getCO2Color(data.co2),
        ),
      ],
    );
  }

  /// 传感器卡片
  Widget _buildSensorCard({
    required IconData icon,
    required String title,
    required String value,
    required String unit,
    required Color color,
  }) {
    return Card(
      elevation: 4,
      child: Container(
        decoration: BoxDecoration(
          borderRadius: BorderRadius.circular(12),
          gradient: LinearGradient(
            begin: Alignment.topLeft,
            end: Alignment.bottomRight,
            colors: [
              color.withOpacity(0.1),
              color.withOpacity(0.05),
            ],
          ),
        ),
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Icon(icon, size: 32, color: color),
              const SizedBox(height: 8),
              Text(
                title,
                style: Theme.of(context).textTheme.titleMedium?.copyWith(
                  fontWeight: FontWeight.bold,
                ),
              ),
              const SizedBox(height: 8),
              Text(
                value,
                style: Theme.of(context).textTheme.headlineSmall?.copyWith(
                  fontWeight: FontWeight.bold,
                  color: color,
                ),
              ),
              const SizedBox(height: 4),
              Text(
                unit,
                style: Theme.of(context).textTheme.bodySmall,
              ),
            ],
          ),
        ),
      ),
    );
  }

  /// 空状态
  Widget _buildEmptyState() {
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Icon(
            Icons.sensor_window,
            size: 64,
            color: Colors.grey[400],
          ),
          const SizedBox(height: 16),
          Text(
            '等待传感器数据...',
            style: Theme.of(context).textTheme.titleMedium?.copyWith(
              color: Colors.grey[600],
            ),
          ),
          const SizedBox(height: 8),
          Text(
            '请确保ESP32已连接并运行',
            style: Theme.of(context).textTheme.bodySmall?.copyWith(
              color: Colors.grey[500],
            ),
          ),
        ],
      ),
    );
  }

  /// 数据质量指示
  Widget _buildDataQuality(SensorData data) {
    double quality = _calculateDataQuality(data);
    
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              '数据质量',
              style: Theme.of(context).textTheme.titleMedium,
            ),
            const SizedBox(height: 8),
            LinearProgressIndicator(
              value: quality / 100,
              backgroundColor: Colors.grey[200],
              valueColor: AlwaysStoppedAnimation<Color>(
                quality > 80 ? Colors.green 
                    : quality > 60 ? Colors.orange 
                    : Colors.red,
              ),
            ),
            const SizedBox(height: 8),
            Text(
              '${quality.toStringAsFixed(0)}% - ${_getQualityText(quality)}',
              style: Theme.of(context).textTheme.bodySmall,
            ),
          ],
        ),
      ),
    );
  }

  /// 计算数据质量
  double _calculateDataQuality(SensorData data) {
    double quality = 100.0;
    
    if (data.temperature < 0 || data.temperature > 40) quality -= 20;
    if (data.humidity < 20 || data.humidity > 80) quality -= 15;
    if (data.formaldehyde > 1.0) quality -= 10;
    if (data.co2 > 2000) quality -= 10;
    
    return quality.clamp(0, 100);
  }

  /// 获取质量文本
  String _getQualityText(double quality) {
    if (quality > 90) return '优秀';
    if (quality > 70) return '良好';
    if (quality > 50) return '一般';
    return '较差';
  }

  /// 格式化时间
  String _formatTime(DateTime time) {
    return '${time.hour.toString().padLeft(2, '0')}:'
        '${time.minute.toString().padLeft(2, '0')}:'
        '${time.second.toString().padLeft(2, '0')}';
  }

  /// 颜色辅助方法
  Color _getTemperatureColor(double temp) {
    if (temp < 18) return Colors.blue;
    if (temp > 28) return Colors.red;
    return Colors.green;
  }

  Color _getHumidityColor(double hum) {
    if (hum < 30) return Colors.orange;
    if (hum > 70) return Colors.blue;
    return Colors.green;
  }

  Color _getFormaldehydeColor(double hcho) {
    if (hcho > 0.10) return Colors.red;
    if (hcho > 0.08) return Colors.orange;
    return Colors.green;
  }

  Color _getCO2Color(double co2) {
    if (co2 > 1000) return Colors.red;
    if (co2 > 800) return Colors.orange;
    return Colors.green;
  }
}
