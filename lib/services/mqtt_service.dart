// // 新增文件：MQTT 连接和数据订阅
// import 'dart:async';
// import 'dart:convert';
// import 'package:mqtt_client/mqtt_client.dart';
// import 'package:mqtt_client/mqtt_server_client.dart';
// import '../models/sensor_data.dart';
//
// typedef OnSensorData = void Function(SensorData data);
//
// class MqttService {
//   final String broker;
//   final String clientId;
//   final String topic;
//   late MqttServerClient client;
//   final SensorData sensorData = SensorData();
//   OnSensorData? onSensorData;
//
//   MqttService({
//     required this.broker,
//     required this.clientId,
//     required this.topic,
//   });
//
//   Future<void> connect() async {
//     client = MqttServerClient(broker, clientId);
//     client.logging(on: false);
//     client.keepAlivePeriod = 20;
//     client.onDisconnected = _onDisconnected;
//
//     final connMess = MqttConnectMessage()
//         .withClientIdentifier(clientId)
//         .startClean()
//         .withWillQos(MqttQos.atLeastOnce);
//
//     client.connectionMessage = connMess;
//
//     try {
//       await client.connect();
//     } catch (e) {
//       client.disconnect();
//       return;
//     }
//
//     if (client.connectionStatus!.state == MqttConnectionState.connected) {
//       // 订阅主题
//       client.subscribe(topic, MqttQos.atLeastOnce);
//       client.updates!.listen(_onMessage);
//       sensorData.deviceOnline = true;
//       _notify();
//     } else {
//       sensorData.deviceOnline = false;
//       client.disconnect();
//     }
//   }
//
//   void _onDisconnected() {
//     sensorData.deviceOnline = false;
//     _notify();
//   }
//
//   void _onMessage(List<MqttReceivedMessage<MqttMessage?>>? event) {
//     final recMess = event![0].payload as MqttPublishMessage;
//     final payload =
//     MqttPublishPayload.bytesToStringAsString(recMess.payload.message);
//
//     try {
//       final jsonData = jsonDecode(payload) as Map<String, dynamic>;
//       sensorData.updateFromJson(jsonData);
//       _notify();
//     } catch (_) {}
//   }
//
//   void _notify() {
//     if (onSensorData != null) {
//       onSensorData!(sensorData);
//     }
//   }
//
//   void disconnect() {
//     client.disconnect();
//     sensorData.deviceOnline = false;
//     _notify();
//   }
// }