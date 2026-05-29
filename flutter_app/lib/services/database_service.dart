import 'package:sqflite/sqflite.dart';
import 'package:path/path.dart';
import '../models/sensor_data.dart';

/// 本地数据库服务 - 存储历史数据
class DatabaseService {
  static final DatabaseService _instance = DatabaseService._internal();
  factory DatabaseService() => _instance;
  DatabaseService._internal();

  Database? _database;

  /// 获取数据库实例
  Future<Database> get database async {
    if (_database != null) return _database!;
    _database = await _initDatabase();
    return _database!;
  }

  /// 初始化数据库
  Future<Database> _initDatabase() async {
    final dbPath = await getDatabasesPath();
    final path = join(dbPath, 'sensor_data.db');

    return await openDatabase(
      path,
      version: 1,
      onCreate: _onCreate,
    );
  }

  /// 创建表
  Future<void> _onCreate(Database db, int version) async {
    await db.execute('''
      CREATE TABLE sensor_readings (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        temperature REAL NOT NULL,
        humidity REAL NOT NULL,
        formaldehyde REAL NOT NULL,
        co2 REAL NOT NULL,
        timestamp INTEGER NOT NULL,
        latitude REAL,
        longitude REAL,
        battery_level INTEGER DEFAULT 100
      )
    ''');

    // 创建索引以加速查询
    await db.execute(
      'CREATE INDEX idx_timestamp ON sensor_readings(timestamp)'
    );
  }

  /// 插入传感器数据
  Future<int> insertSensorData(SensorData data) async {
    final db = await database;
    return await db.insert('sensor_readings', data.toJson());
  }

  /// 批量插入数据
  Future<void> insertBatch(List<SensorData> dataList) async {
    final db = await database;
    final batch = db.batch();
    
    for (var data in dataList) {
      batch.insert('sensor_readings', data.toJson());
    }
    
    await batch.commit(noResult: true);
  }

  /// 获取最近N条记录
  Future<List<SensorData>> getRecentData({int limit = 100}) async {
    final db = await database;
    final List<Map<String, dynamic>> maps = await db.query(
      'sensor_readings',
      orderBy: 'timestamp DESC',
      limit: limit,
    );

    return List.generate(maps.length, (i) {
      return SensorData.fromJson(maps[i]);
    });
  }

  /// 获取指定日期范围的数据
  Future<List<SensorData>> getDataByRange({
    required DateTime start,
    required DateTime end,
  }) async {
    final db = await database;
    final List<Map<String, dynamic>> maps = await db.query(
      'sensor_readings',
      where: 'timestamp >= ? AND timestamp <= ?',
      whereArgs: [
        start.millisecondsSinceEpoch,
        end.millisecondsSinceEpoch,
      ],
      orderBy: 'timestamp ASC',
    );

    return List.generate(maps.length, (i) {
      return SensorData.fromJson(maps[i]);
    });
  }

  /// 获取今日数据
  Future<List<SensorData>> getTodayData() async {
    final now = DateTime.now();
    final startOfDay = DateTime(now.year, now.month, now.day);
    final endOfDay = startOfDay.add(const Duration(days: 1));
    
    return await getDataByRange(start: startOfDay, end: endOfDay);
  }

  /// 删除旧数据(保留最近30天)
  Future<int> deleteOldData({int daysToKeep = 30}) async {
    final db = await database;
    final cutoffDate = DateTime.now().subtract(Duration(days: daysToKeep));
    
    return await db.delete(
      'sensor_readings',
      where: 'timestamp < ?',
      whereArgs: [cutoffDate.millisecondsSinceEpoch],
    );
  }

  /// 清空所有数据
  Future<void> clearAllData() async {
    final db = await database;
    await db.delete('sensor_readings');
  }

  /// 获取数据统计信息
  Future<Map<String, dynamic>> getStats() async {
    final db = await database;
    
    final countResult = await db.rawQuery(
      'SELECT COUNT(*) as count FROM sensor_readings'
    );
    
    final avgResult = await db.rawQuery('''
      SELECT 
        AVG(temperature) as avg_temp,
        AVG(humidity) as avg_hum,
        AVG(formaldehyde) as avg_hcho,
        AVG(co2) as avg_co2
      FROM sensor_readings
    ''');

    return {
      'total_records': countResult.first['count'],
      'averages': avgResult.first,
    };
  }
}
