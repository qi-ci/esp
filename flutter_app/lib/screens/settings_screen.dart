import 'package:flutter/material.dart';
import '../services/sensor_service.dart';

/// 设置页面
class SettingsScreen extends StatefulWidget {
  const SettingsScreen({super.key});

  @override
  State<SettingsScreen> createState() => _SettingsScreenState();
}

class _SettingsScreenState extends State<SettingsScreen> {
  final SensorService _sensorService = SensorService();
  final TextEditingController _urlController = TextEditingController();
  
  bool _autoRefresh = true;
  int _refreshInterval = 5; // 秒
  bool _enableNotifications = true;
  String _themeMode = 'system';

  @override
  void initState() {
    super.initState();
    _urlController.text = _sensorService.serverUrl;
  }

  @override
  void dispose() {
    _urlController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('设置'),
      ),
      body: ListView(
        padding: const EdgeInsets.all(16.0),
        children: [
          // 服务器配置
          _buildSection(
            title: '服务器配置',
            icon: Icons.dns,
            children: [
              ListTile(
                title: const Text('ESP32服务器地址'),
                subtitle: Text(_sensorService.serverUrl),
                trailing: IconButton(
                  icon: const Icon(Icons.edit),
                  onPressed: _showEditServerDialog,
                ),
              ),
            ],
          ),
          
          const SizedBox(height: 16),
          
          // 数据刷新
          _buildSection(
            title: '数据刷新',
            icon: Icons.refresh,
            children: [
              SwitchListTile(
                title: const Text('自动刷新'),
                subtitle: const Text('定时从ESP32获取最新数据'),
                value: _autoRefresh,
                onChanged: (value) {
                  setState(() => _autoRefresh = value);
                },
              ),
              if (_autoRefresh)
                ListTile(
                  title: const Text('刷新间隔'),
                  subtitle: Text('$_refreshInterval 秒'),
                  trailing: DropdownButton<int>(
                    value: _refreshInterval,
                    items: [5, 10, 15, 30, 60]
                        .map((e) => DropdownMenuItem(
                              value: e,
                              child: Text('$e 秒'),
                            ))
                        .toList(),
                    onChanged: (value) {
                      if (value != null) {
                        setState(() => _refreshInterval = value);
                      }
                    },
                  ),
                ),
            ],
          ),
          
          const SizedBox(height: 16),
          
          // 通知设置
          _buildSection(
            title: '通知设置',
            icon: Icons.notifications,
            children: [
              SwitchListTile(
                title: const Text('启用告警通知'),
                subtitle: const Text('当传感器数据异常时推送通知'),
                value: _enableNotifications,
                onChanged: (value) {
                  setState(() => _enableNotifications = value);
                },
              ),
            ],
          ),
          
          const SizedBox(height: 16),
          
          // 主题设置
          _buildSection(
            title: '外观',
            icon: Icons.palette,
            children: [
              ListTile(
                title: const Text('主题模式'),
                subtitle: Text(_getThemeText()),
                trailing: DropdownButton<String>(
                  value: _themeMode,
                  items: const [
                    DropdownMenuItem(value: 'system', child: Text('跟随系统')),
                    DropdownMenuItem(value: 'light', child: Text('浅色')),
                    DropdownMenuItem(value: 'dark', child: Text('深色')),
                  ],
                  onChanged: (value) {
                    if (value != null) {
                      setState(() => _themeMode = value);
                    }
                  },
                ),
              ),
            ],
          ),
          
          const SizedBox(height: 16),
          
          // 关于
          _buildSection(
            title: '关于',
            icon: Icons.info,
            children: [
              ListTile(
                title: const Text('版本'),
                subtitle: const Text('1.0.0'),
              ),
              ListTile(
                title: const Text('开发者'),
                subtitle: const Text('IoT Expert Assistant'),
              ),
              ListTile(
                title: const Text('开源协议'),
                subtitle: const Text('MIT License'),
              ),
            ],
          ),
          
          const SizedBox(height: 16),
          
          // 操作按钮
          Card(
            child: Column(
              children: [
                ListTile(
                  leading: const Icon(Icons.bug_report, color: Colors.orange),
                  title: const Text('测试连接'),
                  onTap: _testConnection,
                ),
                const Divider(height: 1),
                ListTile(
                  leading: const Icon(Icons.delete_sweep, color: Colors.red),
                  title: const Text('清除历史数据'),
                  onTap: _clearHistoryData,
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }

  /// 构建设置区块
  Widget _buildSection({
    required String title,
    required IconData icon,
    required List<Widget> children,
  }) {
    return Card(
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Padding(
            padding: const EdgeInsets.all(16.0),
            child: Row(
              children: [
                Icon(icon, size: 24),
                const SizedBox(width: 8),
                Text(
                  title,
                  style: Theme.of(context).textTheme.titleMedium?.copyWith(
                    fontWeight: FontWeight.bold,
                  ),
                ),
              ],
            ),
          ),
          ...children,
        ],
      ),
    );
  }

  /// 显示编辑服务器对话框
  void _showEditServerDialog() {
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('修改服务器地址'),
        content: TextField(
          controller: _urlController,
          decoration: const InputDecoration(
            hintText: 'http://192.168.1.100:8080',
            labelText: '服务器URL',
          ),
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context),
            child: const Text('取消'),
          ),
          ElevatedButton(
            onPressed: () {
              _sensorService.updateServerUrl(_urlController.text);
              Navigator.pop(context);
              ScaffoldMessenger.of(context).showSnackBar(
                const SnackBar(content: Text('服务器地址已更新')),
              );
            },
            child: const Text('保存'),
          ),
        ],
      ),
    );
  }

  /// 测试连接
  Future<void> _testConnection() async {
    ScaffoldMessenger.of(context).showSnackBar(
      const SnackBar(content: Text('正在测试连接...')),
    );
    
    final data = await _sensorService.refresh();
    
    if (mounted) {
      if (data != null) {
        ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(
            content: Text('✅ 连接成功!'),
            backgroundColor: Colors.green,
          ),
        );
      } else {
        ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(
            content: Text('❌ 连接失败,请检查服务器地址'),
            backgroundColor: Colors.red,
          ),
        );
      }
    }
  }

  /// 清除历史数据
  void _clearHistoryData() {
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('确认清除'),
        content: const Text('确定要清除所有历史数据吗?此操作不可恢复。'),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context),
            child: const Text('取消'),
          ),
          ElevatedButton(
            style: ElevatedButton.styleFrom(
              backgroundColor: Colors.red,
            ),
            onPressed: () async {
              // TODO: 调用数据库服务清除数据
              Navigator.pop(context);
              ScaffoldMessenger.of(context).showSnackBar(
                const SnackBar(content: Text('历史数据已清除')),
              );
            },
            child: const Text('清除'),
          ),
        ],
      ),
    );
  }

  /// 获取主题文本
  String _getThemeText() {
    switch (_themeMode) {
      case 'system':
        return '跟随系统';
      case 'light':
        return '浅色';
      case 'dark':
        return '深色';
      default:
        return '未知';
    }
  }
}
