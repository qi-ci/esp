import 'package:flutter/material.dart';
import 'package:fl_chart/fl_chart.dart';
import '../services/database_service.dart';
import '../models/sensor_data.dart';

/// 历史数据页面
class HistoryScreen extends StatefulWidget {
  const HistoryScreen({super.key});

  @override
  State<HistoryScreen> createState() => _HistoryScreenState();
}

class _HistoryScreenState extends State<HistoryScreen> {
  final DatabaseService _dbService = DatabaseService();
  List<SensorData> _historyData = [];
  bool _isLoading = false;
  String _selectedPeriod = '今日';

  @override
  void initState() {
    super.initState();
    _loadHistoryData();
  }

  /// 加载历史数据
  Future<void> _loadHistoryData() async {
    setState(() => _isLoading = true);
    
    try {
      if (_selectedPeriod == '今日') {
        _historyData = await _dbService.getTodayData();
      } else if (_selectedPeriod == '最近7天') {
        final end = DateTime.now();
        final start = end.subtract(const Duration(days: 7));
        _historyData = await _dbService.getDataByRange(start: start, end: end);
      } else if (_selectedPeriod == '最近30天') {
        final end = DateTime.now();
        final start = end.subtract(const Duration(days: 30));
        _historyData = await _dbService.getDataByRange(start: start, end: end);
      }
      
      // 按时间排序
      _historyData.sort((a, b) => a.timestamp.compareTo(b.timestamp));
    } catch (e) {
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(content: Text('加载失败: $e')),
        );
      }
    } finally {
      if (mounted) {
        setState(() => _isLoading = false);
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('历史数据'),
        actions: [
          PopupMenuButton<String>(
            onSelected: (value) {
              setState(() => _selectedPeriod = value);
              _loadHistoryData();
            },
            itemBuilder: (context) => [
              const PopupMenuItem(value: '今日', child: Text('今日')),
              const PopupMenuItem(value: '最近7天', child: Text('最近7天')),
              const PopupMenuItem(value: '最近30天', child: Text('最近30天')),
            ],
          ),
        ],
      ),
      body: _isLoading
          ? const Center(child: CircularProgressIndicator())
          : _historyData.isEmpty
              ? _buildEmptyState()
              : _buildContent(),
    );
  }

  /// 内容区域
  Widget _buildContent() {
    return SingleChildScrollView(
      padding: const EdgeInsets.all(16.0),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          // 统计信息
          _buildStatistics(),
          
          const SizedBox(height: 24),
          
          // 温度趋势图
          _buildChart('温度趋势', '°C', 
            (data) => data.temperature, Colors.red),
          
          const SizedBox(height: 24),
          
          // 湿度趋势图
          _buildChart('湿度趋势', '%', 
            (data) => data.humidity, Colors.blue),
          
          const SizedBox(height: 24),
          
          // 甲醛趋势图
          _buildChart('甲醛趋势', 'ppm', 
            (data) => data.formaldehyde, Colors.orange),
          
          const SizedBox(height: 24),
          
          // CO2趋势图
          _buildChart('CO₂趋势', 'ppm', 
            (data) => data.co2, Colors.green),
        ],
      ),
    );
  }

  /// 统计信息卡片
  Widget _buildStatistics() {
    if (_historyData.isEmpty) return const SizedBox.shrink();
    
    final avgTemp = _average(_historyData.map((d) => d.temperature));
    final avgHum = _average(_historyData.map((d) => d.humidity));
    final avgHcho = _average(_historyData.map((d) => d.formaldehyde));
    final avgCo2 = _average(_historyData.map((d) => d.co2));
    
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              '$_selectedPeriod - 平均值',
              style: Theme.of(context).textTheme.titleMedium,
            ),
            const SizedBox(height: 12),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceAround,
              children: [
                _buildStatItem('温度', '${avgTemp.toStringAsFixed(1)}°C'),
                _buildStatItem('湿度', '${avgHum.toStringAsFixed(1)}%'),
                _buildStatItem('甲醛', '${avgHcho.toStringAsFixed(3)}'),
                _buildStatItem('CO₂', '${avgCo2.toStringAsFixed(0)}'),
              ],
            ),
          ],
        ),
      ),
    );
  }

  /// 统计项
  Widget _buildStatItem(String label, String value) {
    return Column(
      children: [
        Text(
          label,
          style: Theme.of(context).textTheme.bodySmall,
        ),
        const SizedBox(height: 4),
        Text(
          value,
          style: Theme.of(context).textTheme.titleMedium?.copyWith(
            fontWeight: FontWeight.bold,
          ),
        ),
      ],
    );
  }

  /// 图表
  Widget _buildChart(
    String title,
    String unit,
    double Function(SensorData) getValue,
    Color color,
  ) {
    if (_historyData.length < 2) {
      return const SizedBox.shrink();
    }
    
    final spots = <FlSpot>[];
    for (int i = 0; i < _historyData.length; i++) {
      spots.add(FlSpot(i.toDouble(), getValue(_historyData[i])));
    }
    
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              title,
              style: Theme.of(context).textTheme.titleMedium,
            ),
            const SizedBox(height: 16),
            SizedBox(
              height: 200,
              child: LineChart(
                LineChartData(
                  gridData: FlGridData(show: true),
                  titlesData: FlTitlesData(
                    leftTitles: AxisTitles(
                      sideTitles: SideTitles(showTitles: true),
                    ),
                    bottomTitles: AxisTitles(
                      sideTitles: SideTitles(showTitles: false),
                    ),
                  ),
                  borderData: FlBorderData(show: true),
                  lineBarsData: [
                    LineChartBarData(
                      spots: spots,
                      isCurved: true,
                      color: color,
                      barWidth: 2,
                      dotData: FlDotData(show: false),
                    ),
                  ],
                ),
              ),
            ),
          ],
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
            Icons.history,
            size: 64,
            color: Colors.grey[400],
          ),
          const SizedBox(height: 16),
          Text(
            '暂无历史数据',
            style: Theme.of(context).textTheme.titleMedium?.copyWith(
              color: Colors.grey[600],
            ),
          ),
        ],
      ),
    );
  }

  /// 计算平均值
  double _average(Iterable<double> values) {
    if (values.isEmpty) return 0.0;
    return values.reduce((a, b) => a + b) / values.length;
  }
}
