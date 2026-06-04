#include "wifi_ap_server.h"
#include "../drivers/wifi_manager.h"
#include "../config/wifi_storage.h"
#include "../core/system_state.h"
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

// =============================
// 处理提交 WiFi 表单
void handleRoot() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    if (ssid.length() > 0 && password.length() > 0) {
        // =========================
        // 1️⃣ 进入测试模式（关键）
        // =========================
        g_systemState.wifi_ssid = ssid;
        g_systemState.wifi_password = password;
        bool ok = WiFiManager_testConnect();

        g_systemState.wifi_test_result = ok; // 保存测试结果到全局状态

        if (ok)
        {
            // =========================
            // 2️⃣ 仅成功才保存
            // =========================
            WiFiStorage_saveWiFi();

            String html = "<h1>Saved! Device ID: " + g_systemState.device_id + "</h1>"
                        "<p>Rebooting...</p>";
            server.send(200, "text/html", html);

                delay(1500);
                ESP.restart();   // ✔ 只在成功时重启
        }
        else
        {
            // =========================
            // 3️⃣ 失败不重启（关键）
            // =========================
            server.send(200, "text/html",
                        "<h1>Connection Failed</h1><p>Please retry.</p>");
        }
    } 
    else {
        String html = "<h1>ESP32 WiFi AP Config</h1>"
                      "<p>Device ID: " + g_systemState.device_id + "</p>"
                      "<form method='get'>"
                      "SSID: <input name='ssid'><br>"
                      "Password: <input name='password'><br>"
                      "<input type='submit'></form>";
        server.send(200, "text/html", html);
    }
}

void WiFiAPServer_begin() {
    server.on("/", handleRoot);
    server.begin();
    Serial.print("Web server started at 192.168.4.1");
}

void WiFiAPServer_loop() {
    server.handleClient();
}