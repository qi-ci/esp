import 'dart:convert';

class CommandModel {
  final String deviceId;
  final String cmd;
  final Map<String, dynamic>? params;

  CommandModel({
    required this.deviceId,
    required this.cmd,
    this.params,
  });

  String toJsonString() {
    final map = {
      'device_id': deviceId,
      'cmd': cmd,
      'params': params ?? {},
    };
    return jsonEncode(map);
  }
}

class CommandResponse {
  final String deviceId;
  final String cmd;
  final String status;
  final String message;

  CommandResponse({
    required this.deviceId,
    required this.cmd,
    required this.status,
    required this.message,
  });

  factory CommandResponse.fromJsonString(String jsonStr) {
    final map = jsonDecode(jsonStr);
    return CommandResponse(
      deviceId: map['device_id'],
      cmd: map['cmd'],
      status: map['status'],
      message: map['message'],
    );
  }
}