// class SensorData {
//   double temperature;
//   double humidity;
//   double co2;
//   double hcho;
//   bool deviceOnline;
//
//   SensorData({
//     this.temperature = 0.0,
//     this.humidity = 0.0,
//     this.co2 = 0.0,
//     this.hcho = 0.0,
//     this.deviceOnline = false,
//   });
//
//   // ✅ 从 MQTT JSON 更新数据（关键）
//   void updateFromJson(Map<String, dynamic> json) {
//     temperature = (json['temp'] ?? temperature).toDouble();
//     humidity = (json['humi'] ?? humidity).toDouble();
//     co2 = (json['co2'] ?? co2).toDouble();
//     hcho = (json['hcho'] ?? hcho).toDouble();
//   }
//
//   // ✅ 可选：调试用
//   @override
//   String toString() {
//     return 'T:$temperature H:$humidity CO2:$co2 HCHO:$hcho';
//   }
// }