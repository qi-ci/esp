明白了，你贴的这个 `AndroidManifest.xml` **缺少最关键的权限声明**：`INTERNET`。
Flutter 的 MQTT、HTTP、TCP 连接都必须有这个权限，否则就会出现你日志里的：

```
SocketException: No route to host
```

---

# 🔹 修改方案

在 `<manifest>` 下加：

```xml
<uses-permission android:name="android.permission.INTERNET"/>
```

同时，如果你的 MQTT 是局域网（明文 1883），建议在 `<application>` 下加：

```xml
android:usesCleartextTraffic="true"
```

完整示例：

```xml
<manifest xmlns:android="http://schemas.android.com/apk/res/android">

    <!-- 必须加这个 -->
    <uses-permission android:name="android.permission.INTERNET"/>

    <application
        android:label="AirMonitor"
        android:name="${applicationName}"
        android:icon="@mipmap/ic_launcher"
        android:usesCleartextTraffic="true">
        
        <activity
            android:name=".MainActivity"
            android:exported="true"
            android:launchMode="singleTop"
            android:taskAffinity=""
            android:theme="@style/LaunchTheme"
            android:configChanges="orientation|keyboardHidden|keyboard|screenSize|smallestScreenSize|locale|layoutDirection|fontScale|screenLayout|density|uiMode"
            android:hardwareAccelerated="true"
            android:windowSoftInputMode="adjustResize">
            
            <meta-data
                android:name="io.flutter.embedding.android.NormalTheme"
                android:resource="@style/NormalTheme"
            />
            
            <intent-filter>
                <action android:name="android.intent.action.MAIN"/>
                <category android:name="android.intent.category.LAUNCHER"/>
            </intent-filter>
        </activity>

        <meta-data
            android:name="flutterEmbedding"
            android:value="2" />
    </application>

    <queries>
        <intent>
            <action android:name="android.intent.action.PROCESS_TEXT"/>
            <data android:mimeType="text/plain"/>
        </intent>
    </queries>

</manifest>
```

---

# 🔹 小贴士

1. `usesCleartextTraffic="true"` 对局域网 MQTT 非 TLS 必须，否则 Android 9+ 默认拒绝明文 TCP。
2. 修改完记得 **clean + rebuild** Flutter 项目：

```bash
flutter clean
flutter pub get
flutter run
```

3. 如果还是连不上，可以用我之前说的 **TCP 测试函数**先检查手机能否 ping 到 ESP32。

---

我可以帮你改写一个 **Flutter 局域网网络诊断函数**，直接打印 MQTT/TCP 连接状态，非常直观。
你希望我直接写吗？
