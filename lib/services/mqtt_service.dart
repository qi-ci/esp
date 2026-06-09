// network/mqtt_service.dart
import 'dart:convert';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

import 'dart:io';                    // SecurityContext, X509Certificate
import 'package:flutter/services.dart';    // rootBundle 读取 assets

typedef OnSensorDataReceived = void Function(Map<String, dynamic> data);
typedef OnDeviceStatusReceived = void Function(Map<String, dynamic> data);

class MQTTService {
  final String broker;
  final int port;
  final String clientId;

  late MqttServerClient client;

  OnSensorDataReceived? onSensorData;
  OnDeviceStatusReceived? onDeviceStatus;

  MQTTService({required this.broker, required this.port, required this.clientId}) {
    client = MqttServerClient(broker, clientId);
    client.port = port;
    client.secure = true;
    client.logging(on: true);
    client.keepAlivePeriod = 30;
    client.onDisconnected = _onDisconnected;
    client.onConnected = _onConnected;
    client.onSubscribed = _onSubscribed;

    client.connectionMessage = MqttConnectMessage()
        .authenticateAs('acorus', 'ichbinacorus')
        .startClean();
  }


  /// 初始化 TLS 并加载 CA 证书
  Future<void> initTls(String caAssetPath) async {
    final context = SecurityContext.defaultContext;

    try {
      // 加载 assets 下的自签 CA
      final data = await rootBundle.load(caAssetPath);
      context.setTrustedCertificatesBytes(data.buffer.asUint8List());

      client.securityContext = context;
      client.secure = true;

      // 开发阶段临时允许自签证书（生产环境不要用）
      client.onBadCertificate = (Object cert) {
        if (cert is X509Certificate) {
          print('Accepting self-signed certificate: ${cert.subject}');
        } else {
          print('Accepting self-signed certificate: $cert');
        }
        return true;
      };
    } catch (e) {
      print('TLS init error: $e');
      rethrow;
    }
  }

  /// 连接 broker
  Future<void> connect() async {
    try {
      await client.connect();
    } catch (e) {
      print('MQTT connect exception: $e');
      client.disconnect();
      rethrow;
    }

    // 订阅主题
    client.subscribe('esp32/airmon_livingroom/tele', MqttQos.atMostOnce);
    // client.subscribe('esp32/airmon_livingroom/tele', MqttQos.atMostOnce);

    client.updates!.listen(_onMessage);
  }

  /// 单 topic 同步传感器和设备状态
  void _onMessage(List<MqttReceivedMessage<MqttMessage?>> event) {
    final recMess = event[0].payload as MqttPublishMessage;
    final message =
    MqttPublishPayload.bytesToStringAsString(recMess.payload.message);

    try {
      final jsonData = jsonDecode(message) as Map<String, dynamic>;
      onSensorData?.call(jsonData);      // 更新传感器
      onDeviceStatus?.call(jsonData);    // 更新设备状态
    } catch (e) {
      print('MQTT parsing error: $e\nmessage: $message');
    }
  }

  void _onDisconnected() => print('MQTT disconnected');
  void _onConnected() => print('MQTT connected');
  void _onSubscribed(String topic) => print('Subscribed to $topic');

  /// 断开连接
  void disconnect() => client.disconnect();
}