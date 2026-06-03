import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

import '../core/mqtt_config.dart';
import 'mqtt_topics.dart';

class MqttService {
  late MqttServerClient client;

  void init() {
    client = MqttServerClient(
      MqttConfig.broker,
      MqttConfig.deviceId,
    );

    client.port = MqttConfig.port;
    client.keepAlivePeriod = 20;

    client.onConnected = onConnected;
    client.onDisconnected = onDisconnected;
    client.onSubscribed = onSubscribed;
  }

  Future<void> connect() async {
    await client.connect(
      MqttConfig.username,
      MqttConfig.password,
    );

    // ⭐订阅 response + telemetry
    client.subscribe(
      MqttTopics.telemetry(MqttConfig.deviceId),
      MqttQos.atMostOnce,
    );

    client.subscribe(
      MqttTopics.response(MqttConfig.deviceId),
      MqttQos.atMostOnce,
    );

    client.updates!.listen((event) {
      final recMess = event[0].payload as MqttPublishMessage;
      final payload =
          MqttPublishPayload.bytesToStringAsString(recMess.payload.message);
      MessageRouter().handle(event[0].topic, payload);
    });

  }

  void publishCommand(String json) {
    final builder = MqttClientPayloadBuilder();
    builder.addString(json);

    client.publishMessage(
      MqttTopics.command(MqttConfig.deviceId),
      MqttQos.atMostOnce,
      builder.payload!,
    );
  }

  void onMessage(List<MqttReceivedMessage<MqttMessage>> event) {
    final recMess = event[0].payload as MqttPublishMessage;
    final payload =
        MqttPublishPayload.bytesToStringAsString(recMess.payload.message);

    print("[MQTT] $payload");

    // 👉 后面会接 message_router
  }

  void onConnected() {
    print("[MQTT] connected");
  }

  void onDisconnected() {
    print("[MQTT] disconnected");
  }

  void onSubscribed(String topic) {
    print("[MQTT] subscribed: $topic");
  }
}