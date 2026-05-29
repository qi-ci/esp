// This is a basic Flutter widget test.

import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';

import 'package:environmental_monitor/main.dart';

void main() {
  testWidgets('APP启动测试', (WidgetTester tester) async {
    // Build our app and trigger a frame.
    await tester.pumpWidget(const EnvironmentalMonitorApp());

    // Verify that the app title is displayed
    expect(find.text('环境监测仪'), findsOneWidget);
    
    // Verify that sensor cards are present
    expect(find.byType(Card), findsWidgets);
  });
}
