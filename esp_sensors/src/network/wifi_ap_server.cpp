#include "wifi_ap_server.h"

#include "../drivers/wifi_manager.h"
#include "../config/system_storage.h"
#include "../core/system_state.h"

#include <WiFi.h>
#include <WebServer.h>

static WebServer server(80);

// =============================
// HTML 页面
// =============================
static const char* html_page =
R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>ESP32 WiFi Setup</title>
</head>
<body>
    <h2>ESP32 WiFi Configuration</h2>
    <p>Device ID: %s</p>

    <form method="POST" action="/">
        SSID:<br>
        <input name="ssid"><br><br>

        Password:<br>
        <input name="password" type="password"><br><br>

        <input type="submit" value="Save">
    </form>
</body>
</html>
)rawliteral";

// =============================
// 处理 GET：显示页面
// =============================
static void handleRootGET()
{
    char buffer[512];
    snprintf(buffer, sizeof(buffer), html_page, g_systemState.device_id.c_str());
    server.send(200, "text/html", buffer);
}

// =============================
// 处理 POST：接收 WiFi
// =============================
static void handleRootPOST()
{
    // 🔥 强制打印调试信息
    Serial.println("\n===== WiFi Config Received =====");
    Serial.println("Args count: " + String(server.args()));

    for (int i = 0; i < server.args(); i++)
    {
        Serial.println(server.argName(i) + " = " + server.arg(i));
    }

    String ssid = server.arg("ssid");
    String password = server.arg("password");

    Serial.println("Parsed SSID = " + ssid);
    Serial.println("Parsed PASS = " + password);

    // =============================
    // 参数检查
    // =============================
    if (ssid.length() == 0 || password.length() == 0)
    {
        server.send(400, "text/plain", "Missing SSID or Password");
        return;
    }

    // =============================
    // 更新全局变量
    // =============================
    g_systemState.wifi_ssid = ssid;
    g_systemState.wifi_password = password;

    Serial.println("[AP] Saved to system_state");
    Serial.println("SSID => " + g_systemState.wifi_ssid);
    Serial.println("PASS => " + g_systemState.wifi_password);

    // =============================
    // 测试连接（阻塞5秒）
    // =============================
    bool ok = WiFiManager_testConnect(ssid, password);
    g_systemState.wifi_test_result = ok;

    // =============================
    // 成功：保存 + 重启
    // =============================
    if (ok)
    {
        SystemStorage_saveWiFi();

        server.send(200, "text/html",
            "<h1>WiFi Saved Successfully</h1><p>Device rebooting...</p>");

        delay(1500);
        ESP.restart();
    }
    else
    {
        // =============================
        // 失败：不保存
        // =============================
        server.send(200, "text/html",
            "<h1>Connection Failed</h1><p>Please retry.</p>");
    }
}

// =============================
// 启动 WebServer
// =============================
void WiFiAPServer_begin()
{
    server.on("/", HTTP_GET, handleRootGET);
    server.on("/", HTTP_POST, handleRootPOST);

    server.begin();

    Serial.println("\n[AP SERVER] Started");
    Serial.print("[AP SERVER] Open: http://");
    Serial.println(WiFi.softAPIP());

    // -------------------------------
    // 🔥 Fallback handler
    // -------------------------------
    server.onNotFound([]() {
        Serial.println("⚠️ Unknown request URI: " + server.uri());
        Serial.println("Method: " + String(server.method()));
        // 可以选择直接返回主页
        handleRootGET();
    });
}

// =============================
// loop 必须调用
// =============================
void WiFiAPServer_loop()
{
    server.handleClient();
}