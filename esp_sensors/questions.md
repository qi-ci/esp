<!-- device_config_h
network_config.h 
wifi_storage.cpp 
recovery_manager.cpp 
system_health_manager.cpp 
system_state.h 
wifi_manager.cpp 
mqtt_reconnect_manager.cpp 
network_state_machine.cpp 
wifi_ap_server.cpp
这十个文件的作用和联系分析，是否有哪些逻辑和代码可以优化，十个文件全部代码如下，判断哪些文件和代码需要修改，给出每个文件的修改后代码并与之前代码对比，并注释修改说明，按照“--- # 📦 4️⃣ sensors/htu21d_sensor.hcpp id="htu_h" #pragma once void HTU21D_begin(); void HTU21D_read(float* temperature, float* humidity); “格式

device_config_h
#pragma once

#define DEVICE_ID "airmon_home_001"

network_config.h:
#pragma once

// ======================
// MQTT Broker
// ======================
#define MQTT_HOST      "192.168.1.100"
#define MQTT_PORT      1883

#define MQTT_USERNAME  ""
#define MQTT_PASSWORD  ""

// ======================
// 网络基础配置
// ======================

#define WIFI_RETRY_LIMIT     20
#define WIFI_RETRY_DELAY_MS  500

#define MQTT_RETRY_INTERVAL  5000

#define MQTT_KEEPALIVE       60

wifi_storage.cpp:
#include "wifi_storage.h"
#include <Preferences.h>

static Preferences prefs;

// ======================
// 初始化
// ======================
void WiFiStorage_begin()
{
    prefs.begin("esp_config", false); // namespace "esp_config", RW模式
}

// ======================
// WiFi保存与读取
// ======================
void WiFiStorage_saveWiFi(const String& ssid, const String& password)
{
    prefs.putString("wifi_ssid", ssid);
    prefs.putString("wifi_pass", password);
}

bool WiFiStorage_loadWiFi(String& ssid, String& password)
{
    if (!prefs.isKey("wifi_ssid") || !prefs.isKey("wifi_pass"))
        return false;

    ssid = prefs.getString("wifi_ssid", "");
    password = prefs.getString("wifi_pass", "");
    return true;
}

// ======================
// Device ID保存与读取
// ======================
void WiFiStorage_saveDeviceID(const String& device_id)
{
    prefs.putString("device_id", device_id);
}

bool WiFiStorage_loadDeviceID(String& device_id)
{
    if (!prefs.isKey("device_id"))
        return false;

    device_id = prefs.getString("device_id", "");
    return true;
}

// ======================
// 传感器采样周期保存与读取
// ======================
void WiFiStorage_saveIntervals(uint32_t htu, uint32_t ze08, uint32_t s8)
{
    prefs.putUInt("htu_interval", htu);
    prefs.putUInt("ze08_interval", ze08);
    prefs.putUInt("s8_interval", s8);
}

bool WiFiStorage_loadIntervals(uint32_t& htu, uint32_t& ze08, uint32_t& s8)
{
    if (!prefs.isKey("htu_interval") || !prefs.isKey("ze08_interval") || !prefs.isKey("s8_interval"))
        return false;

    htu   = prefs.getUInt("htu_interval", 5000);
    ze08  = prefs.getUInt("ze08_interval", 5000);
    s8    = prefs.getUInt("s8_interval", 5000);
    return true;
}

recovery_manager.cpp:
#include "recovery_manager.h"
#include "system_state.h"

#include "../sensors/htu21d_sensor.h"
#include "../sensors/ze08_sensor.h"
#include "../sensors/s8_sensor.h"

#include "../network/mqtt_client.h"
#include <Arduino.h>

// ======================
// 传感器失败计数
// ======================
static int sensorFailCount = 0;
static int mqttFailCount   = 0;

// ======================
// 强制重启传感器
// ======================
void RecoveryManager_forceSensorReset()
{
    Serial.println("[RECOVERY] Restart sensors...");

    HTU21D_begin();
    ZE08_begin();
    S8_begin();

    sensorFailCount = 0;
}

// ======================
// 强制重连MQTT
// ======================
void RecoveryManager_forceMQTTReconnect()
{
    Serial.println("[RECOVERY] Reconnect MQTT...");

    MQTT_begin();

    mqttFailCount = 0;
}

// ======================
// 自动恢复调度器
// ======================
void RecoveryManager_update()
{
    // ======================
    // 1. 传感器异常处理
    // ======================
    if (!g_systemState.sensor_ok)
    {
        sensorFailCount++;
    }
    else
    {
        sensorFailCount = 0;
    }

    if (sensorFailCount > 5)
    {
        g_systemState.deviceStatus = "degraded";
        RecoveryManager_forceSensorReset();
    }

    // ======================
    // 2. MQTT异常处理
    // ======================
    if (!g_systemState.mqtt_connected)
    {
        mqttFailCount++;
    }
    else
    {
        mqttFailCount = 0;
    }

    if (mqttFailCount > 5)
    {
        g_systemState.deviceStatus = "degraded";
        RecoveryManager_forceMQTTReconnect();
    }

    // ======================
    // 3. 系统恢复正常
    // ======================
    if (g_systemState.sensor_ok &&
        g_systemState.mqtt_connected)
    {
        g_systemState.deviceStatus = "running";
    }
}

system_health_manager.cpp:
#include "system_health_manager.h"

#include "system_state.h"
#include "sensor_manager.h"

#include "../network/mqtt_reconnect_manager.h"
#include "../drivers/wifi_manager.h"

// ======================
// 健康状态
// ======================
static bool g_healthy = true;

// ======================
// 异常计数器
// ======================
static uint32_t sensorFailCount = 0;
static uint32_t mqttFailCount   = 0;

// ======================
// 配置阈值
// ======================
static const uint8_t SENSOR_FAIL_LIMIT = 5;
static const uint8_t MQTT_FAIL_LIMIT   = 5;

// ======================
// 初始化
// ======================
void SystemHealth_begin()
{
    g_healthy = true;
    sensorFailCount = 0;
    mqttFailCount = 0;
}

// ======================
// 检查传感器状态
// ======================
static void checkSensors()
{
    bool ok =
        g_systemState.htuReady &&
        g_systemState.ze08Ready &&
        g_systemState.s8Ready;

    if(!ok)
    {
        sensorFailCount++;
    }
    else
    {
        sensorFailCount = 0;
    }

    // ======================
    // 传感器恢复策略
    // ======================
    if(sensorFailCount >= SENSOR_FAIL_LIMIT)
    {
        Serial.println("[HEALTH] sensor recovery triggered");

        SensorManager_begin(); // 重新初始化传感器
        sensorFailCount = 0;
    }
}

// ======================
// 检查MQTT状态
// ======================
static void checkMQTT()
{
    bool ok = MQTTReconnect_isOnline();

    if(!ok)
    {
        mqttFailCount++;
    }
    else
    {
        mqttFailCount = 0;
    }

    // ======================
    // MQTT恢复策略
    // ======================
    if(mqttFailCount >= MQTT_FAIL_LIMIT)
    {
        Serial.println("[HEALTH] mqtt force reconnect");

        MQTTReconnect_forceReconnect();
        mqttFailCount = 0;
    }
}

// ======================
// 检查数据异常
// ======================
static void checkData()
{
    if(
        isnan(g_systemState.temperature) ||
        isnan(g_systemState.humidity) ||
        isnan(g_systemState.co2) ||
        isnan(g_systemState.hcho)
    )
    {
        Serial.println("[HEALTH] invalid sensor data detected");

        // 降级策略：重启传感器
        SensorManager_begin();
    }
}

// ======================
// 主更新
// ======================
void SystemHealth_update()
{
    checkSensors();
    checkMQTT();
    checkData();

    // ======================
    // 综合健康判断
    // ======================
    g_healthy =
        sensorFailCount < SENSOR_FAIL_LIMIT &&
        mqttFailCount < MQTT_FAIL_LIMIT;
}

// ======================
// 状态查询
// ======================
bool SystemHealth_isHealthy()
{
    return g_healthy;
}

system_state.h:
#pragma once
#include <stdint.h>
#include <Arduino.h>

// ======================
// 系统全局状态中心
// ======================
struct SystemState
{
    //新增保存 WiFi 配置
    String wifi_ssid;
    String wifi_password;
    bool wifi_connected;
    String device_id;

    // ---- 传感器数据 ----
    float temperature;
    float humidity;
    int   co2;
    float hcho;

    // ---- 时间信息 ----
    uint32_t timestamp;        // 当前时间戳
    uint32_t lastUpdateTime;   // 最近更新时间

    // ---- 设备状态 ----
    bool htuReady;
    bool ze08Ready;
    bool s8Ready;

    bool mqtt_connected;
    bool sensor_ok;
    

    const char* deviceStatus;  // running / booting / error / degraded

    // ---- 采样间隔(ms) ----
    uint32_t htuInterval;
    uint32_t ze08Interval;
    uint32_t s8Interval;
};

extern SystemState g_systemState;

wifi_manager.cpp:
#include "wifi_manager.h"
#include "../config/wifi_storage.h"
#include <WiFi.h>

static bool g_connected = false;

void WiFiManager_begin()
{
    WiFi.mode(WIFI_STA);
}

// 在连接 WiFi 成功后自动保存
bool WiFiManager_connect(const String& ssid, const String& password)
{
    WiFi.disconnect(true);
    delay(200);
    WiFi.begin(ssid.c_str(), password.c_str());

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 20)
    {
        delay(500);
        retry++;
    }

    bool ok = (WiFi.status() == WL_CONNECTED);

    // 成功连接则写入 NVS
    if (ok)
    {
        WiFiStorage_saveWiFi(ssid, password);
    }

    return ok;
}

bool WiFiManager_isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager_disconnect()
{
    WiFi.disconnect();
    g_connected = false;
}

// ==============================
// 新增函数：启动 AP 配置模式
void WiFiManager_beginAPMode()
{
    WiFi.mode(WIFI_AP_STA);
    const char* ap_ssid = "ESP32_Setup";
    const char* ap_password = "12345678";
    WiFi.softAP(ap_ssid, ap_password);

    Serial.println("AP Mode started. Connect to WiFi 'ESP32_Setup' and configure network.");

mqtt_reconnect_manager.cpp:
#include "mqtt_reconnect_manager.h"

#include "mqtt_client.h"
#include "mqtt_topics.h"
#include "../drivers/wifi_manager.h"

// ======================
// 重连控制参数
// ======================
static uint32_t lastReconnectTime = 0;
static const uint32_t RECONNECT_INTERVAL = 5000; // 5秒节流

static bool g_online = false;

// ======================
// 初始化
// ======================
void MQTTReconnect_begin()
{
    MQTT_begin();
}

// ======================
// 内部：执行重连
// ======================
static bool reconnect()
{
    if(!WiFiManager_isConnected())
        return false;

    bool ok = MQTT_connect();

    if(ok)
    {
        // ======================
        // 订阅恢复（关键）
        // ======================
        MQTT_subscribe(MQTTTopic_cmd().c_str());

        g_online = true;
    }
    else
    {
        g_online = false;
    }

    return ok;
}

// ======================
// 主循环
// ======================
void MQTTReconnect_update()
{
    uint32_t now = millis();

    // WiFi不在线，MQTT直接失败
    if(!WiFiManager_isConnected())
    {
        g_online = false;
        return;
    }

    // MQTT正常
    if(MQTT_isConnected())
    {
        g_online = true;
        return;
    }

    // ======================
    // 节流重连
    // ======================
    if(now - lastReconnectTime > RECONNECT_INTERVAL)
    {
        lastReconnectTime = now;
        reconnect();
    }
}

// ======================
// 状态
// ======================
bool MQTTReconnect_isOnline()
{
    return g_online;
}

// ======================
// 强制重连
// ======================
void MQTTReconnect_forceReconnect()
{
    lastReconnectTime = 0;
    reconnect();
}

network_state_machine.cpp:
#include "network_state_machine.h"

#include "../drivers/wifi_manager.h"
#include "../network/mqtt_client.h"
#include "../network/mqtt_reconnect_manager.h"
#include "../network/wifi_ap_server.h"

#include "../config/wifi_storage.h"
#include "../config/network_config.h"

// ======================
// 当前状态
// ======================
static NetworkState g_state = NET_BOOT;

// ======================
// 上次重连时间
// ======================
static uint32_t lastReconnectTime = 0;

// ======================
// 状态字符串
// ======================
String NetworkStateMachine_getStateString()
{
    switch(g_state)
    {
        case NET_BOOT: return "BOOT";
        case NET_WIFI_CONNECTING: return "WIFI_CONNECTING";
        case NET_WIFI_CONNECTED: return "WIFI_CONNECTED";
        case NET_MQTT_CONNECTING: return "MQTT_CONNECTING";
        case NET_ONLINE: return "ONLINE";
        case NET_WIFI_LOST: return "WIFI_LOST";
        case NET_MQTT_LOST: return "MQTT_LOST";
        case NET_AP_MODE: return "AP_MODE";
        case NET_RECOVERY: return "RECOVERY";
    }
    return "UNKNOWN";
}

NetworkState NetworkStateMachine_getState()
{
    return g_state;
}

// ======================
// 初始化
// ======================
void NetworkStateMachine_begin()
{
    WiFiManager_begin();
    MQTT_begin();

    g_state = NET_BOOT;
}

// ======================
// 启动 AP
// ======================
void NetworkStateMachine_beginAP()
{
    WiFiManager_beginAPMode();   // 启动 AP 热点
    WiFiAPServer_begin();        // 启动内置 Web 配置服务器
}

// ======================
// AP 循环处理
// ======================
void NetworkStateMachine_APLoop()
{
    if(g_state == NET_AP_MODE)
        WiFiAPServer_loop();     // 处理网页请求
}

// ======================
// 状态机核心
// ======================
void NetworkStateMachine_update()
{
    uint32_t now = millis();

    switch(g_state)
    {
        // ======================
        case NET_BOOT:
        {
            String ssid, pass;
            WiFiStorage_loadWiFi(ssid, pass);
            if(ssid.length() > 0)
                g_state = NET_WIFI_CONNECTING;
            else
                g_state = NET_AP_MODE;   // 没保存 WiFi → AP 配置
            break;
        }

        // ======================
        case NET_WIFI_CONNECTING:
        {
            String ssid, pass;
            WiFiStorage_loadWiFi(ssid, pass);
            bool ok = WiFiManager_connect(ssid, pass);
            if(ok)
                g_state = NET_WIFI_CONNECTED;
            else
                g_state = NET_AP_MODE;       // WiFi 失败 → AP 配置
            break;
        }

        // ======================
        case NET_WIFI_CONNECTED:
        {
            if(!WiFiManager_isConnected())
            {
                g_state = NET_WIFI_LOST;
                break;
            }
            g_state = NET_MQTT_CONNECTING;
            break;
        }

        // ======================
        case NET_MQTT_CONNECTING:
        {
            MQTTReconnect_update();
            if(MQTTReconnect_isOnline())
                g_state = NET_ONLINE;
            break;
        }

        // ======================
        case NET_ONLINE:
        {
            if(!WiFiManager_isConnected())
                g_state = NET_WIFI_LOST;
            else if(!MQTT_isConnected())
                g_state = NET_MQTT_LOST;
            else
                MQTT_loop();   // 发布数据、处理订阅消息
            break;
        }

        // ======================
        case NET_WIFI_LOST:
        {
            if(now - lastReconnectTime > MQTT_RETRY_INTERVAL)
            {
                lastReconnectTime = now;
                g_state = NET_AP_MODE;   // WiFi 长时间掉线 → AP 配置
            }
            break;
        }

        // ======================
        case NET_MQTT_LOST:
        {
            if(now - lastReconnectTime > MQTT_RETRY_INTERVAL)
            {
                lastReconnectTime = now;
                g_state = NET_MQTT_CONNECTING;
            }
            break;
        }

        // ======================
        case NET_AP_MODE:
        {
            // AP 循环在 NetworkStateMachine_APLoop() 中处理
            break;
        }

        // ======================
        case NET_RECOVERY:
        {
            g_state = NET_WIFI_CONNECTING;
            break;
        }
    }
}

wifi_ap_server.cpp:
#include "wifi_ap_server.h"
#include "../drivers/wifi_manager.h"
#include "../config/wifi_storage.h"
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

// =============================
// 处理提交 WiFi 表单
void handleRoot() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    if (ssid.length() > 0 && password.length() > 0) {
        WiFiManager_connect(ssid, password);   // 保存到 NVS 并尝试连接
        server.send(200, "text/html", "<h1>Saved! Rebooting...</h1>");
        delay(2000);
        ESP.restart();
    } else {
        String html = "<form method='get'>"
                      "SSID: <input name='ssid'><br>"
                      "Password: <input name='password'><br>"
                      "<input type='submit'></form>";
        server.send(200, "text/html", html);
    }
}

void WiFiAPServer_begin() {
    server.on("/", handleRoot);
    server.begin();
    Serial.println("Web server started at 192.168.4.1");
}

void WiFiAPServer_loop() {
    server.handleClient();
}

我看完这 10 个文件后，整体评价是：

> **架构方向是正确的（状态机 + NVS + MQTT重连 + AP配网），但目前存在职责重叠、状态源重复、恢复机制重复、配置硬编码等问题。**

如果继续扩展 Flutter、MQTT 控制、OTA、历史存储，会越来越难维护。

---

# 一、十个文件关系图

```text
device_config.h
        │
        ▼
system_state.h
        │
        ▼
wifi_storage.cpp
        │
        ▼
wifi_manager.cpp
        │
        ▼
network_state_machine.cpp
        │
        ├─────────────► mqtt_reconnect_manager.cpp
        │                       │
        │                       ▼
        │                 mqtt_client.cpp
        │
        ▼
wifi_ap_server.cpp

system_health_manager.cpp
        │
        ▼
recovery_manager.cpp
        │
        ▼
system_state.h
```

---

# 二、存在的主要问题

## 问题1

device_id出现两个来源

### device_config.h

```cpp
#define DEVICE_ID "airmon_home_001"
```

### system_state.h

```cpp
String device_id;
```

### wifi_storage.cpp

```cpp
WiFiStorage_saveDeviceID()
```

---

现在有三套设备ID机制：

```text
DEVICE_ID宏
device_id变量
NVS中的device_id
```

会冲突。

---

建议：

删除：

```cpp
#define DEVICE_ID
```

改成：

```cpp
g_systemState.device_id
```

统一来源。

---

# 问题2

RecoveryManager 和 SystemHealthManager 重复

两个模块都在干：

```text
检测传感器异常
检测MQTT异常
触发恢复
```

例如：

RecoveryManager：

```cpp
RecoveryManager_forceSensorReset();
RecoveryManager_forceMQTTReconnect();
```

SystemHealth：

```cpp
SensorManager_begin();
MQTTReconnect_forceReconnect();
```

完全重叠。

---

建议：

保留：

```cpp
SystemHealthManager
```

删除：

```cpp
RecoveryManager
```

或者：

```cpp
SystemHealthManager
只检测

RecoveryManager
只恢复
```

---

# 问题3

WiFi状态重复

wifi_manager：

```cpp
static bool g_connected
```

network_state_machine：

```cpp
NET_WIFI_CONNECTED
```

system_state：

```cpp
wifi_connected
```

三份状态。

---

建议保留：

```cpp
g_systemState.wifi_connected
```

统一状态源。

---

# 问题4

MQTT状态重复

mqtt_reconnect_manager：

```cpp
g_online
```

system_state：

```cpp
mqtt_connected
```

network_state_machine：

```cpp
NET_ONLINE
NET_MQTT_LOST
```

三份状态。

---

建议：

统一：

```cpp
g_systemState.mqtt_connected
```

---

# 问题5

WiFiManager_connect阻塞

这里：

```cpp
while (
   WiFi.status()!=WL_CONNECTED
   && retry<20
)
{
    delay(500);
}
```

最大阻塞：

```text
20 × 500ms

=10秒
```

状态机失去意义。

---

工业级改法：

```cpp
WiFi.begin()

直接返回

状态机轮询
```

非阻塞。

---

# 问题6

AP模式会无限重启

wifi_ap_server：

```cpp
ESP.restart();
```

用户输错密码：

```text
重启
连接失败
AP
重启
连接失败
AP
```

死循环。

---

建议：

增加：

```cpp
wifi_test_result
```

成功才保存。

---

# 问题7

NetworkStateMachine没有真正使用SystemState

例如：

```cpp
g_state = NET_ONLINE;
```

但：

```cpp
g_systemState.wifi_connected
```

没同步。

Flutter未来无法显示设备状态。

---

建议每次状态切换：

```cpp
g_systemState.wifi_connected=true;
g_systemState.mqtt_connected=true;
```

---

# 问题8

WiFiStorage_begin永不释放

现在：

```cpp
prefs.begin(...)
```

一次打开永久不关闭。

ESP32可以运行。

但长期OTA后可能出问题。

---

建议：

改成：

```cpp
每次读写
begin()
end()
```

---

# 三、建议最终结构

---

## 📦 device_config.h

保留：

```cpp
#pragma once

#define DEVICE_MODEL "AirMonitor-V1"
#define FW_VERSION "1.0.0"
```

删除：

```cpp
DEVICE_ID
```

---

## 📦 system_state.h

新增：

```cpp
enum DeviceStatus
{
    DEVICE_BOOTING,
    DEVICE_AP_MODE,
    DEVICE_WIFI_CONNECTING,
    DEVICE_WIFI_CONNECTED,
    DEVICE_MQTT_CONNECTING,
    DEVICE_ONLINE,
    DEVICE_DEGRADED,
    DEVICE_ERROR
};
```

替换：

```cpp
const char* deviceStatus;
```

为：

```cpp
DeviceStatus deviceStatus;
```

避免字符串比较。

---

## 📦 network_config.h

新增：

```cpp
#define AP_SSID_PREFIX "ESP32_AIR_"

#define WIFI_CONNECT_TIMEOUT 10000

#define WIFI_RETRY_INTERVAL 30000

#define MQTT_RETRY_INTERVAL 5000

#define AP_AUTO_EXIT_TIMEOUT 300000
```

---

## 📦 wifi_manager.cpp

建议改成：

### 旧版

```cpp
bool WiFiManager_connect(...)
{
   while(...)
}
```

---

### 新版

```cpp
void WiFiManager_connectAsync(
    const String& ssid,
    const String& pass
)
{
    WiFi.begin(
       ssid.c_str(),
       pass.c_str()
    );
}
```

状态机负责检查：

```cpp
WiFi.status()
```

---

## 📦 mqtt_reconnect_manager.cpp

删除：

```cpp
g_online
```

改为：

```cpp
g_systemState.mqtt_connected
```

---

## 📦 network_state_machine.cpp

新增：

```cpp
NET_AP_MODE
```

已经有了。

继续增强：

```cpp
NET_WIFI_RETRY
```

```cpp
NET_MQTT_RETRY
```

```cpp
NET_FACTORY_RESET
```

---

并同步：

```cpp
g_systemState.deviceStatus
```

例如：

```cpp
g_systemState.deviceStatus=
DEVICE_ONLINE;
```

---

## 📦 wifi_ap_server.cpp

建议增加：

```cpp
/config
```

返回：

```json
{
  "device_id":"xxx",
  "fw":"1.0.0",
  "wifi":"connected"
}
```

Flutter配网会非常方便。

---

## 📦 system_health_manager.cpp

保留：

```cpp
检测
```

删除：

```cpp
SensorManager_begin();
MQTTReconnect_forceReconnect();
```

改为：

```cpp
RecoveryManager_requestSensorRecovery();
RecoveryManager_requestMQTTRecovery();
```

---

## 📦 recovery_manager.cpp

专门负责：

```cpp
恢复传感器
恢复MQTT
恢复WiFi
恢复NVS
```

不要做健康检测。

---

# 我最推荐的下一步

对于你的 ESP32 环境监测仪项目，优先级应该是：

```text
① 消灭重复状态源
② WiFi改为非阻塞连接
③ Recovery与Health解耦
④ AP配置增加HTTP API
⑤ 状态机同步SystemState
```

这五项做完后，你的架构就会从目前的「能运行」提升到比较成熟的 IoT 产品架构，后面接 Flutter、MQTT 指令控制、OTA、历史数据存储都会顺畅很多。

device_config_h
network_config.h 
wifi_storage.cpp 
system_health_manager.cpp 
system_state.h 
wifi_manager.cpp 
mqtt_reconnect_manager.cpp 
network_state_machine.cpp 
wifi_ap_server.cpp
app_init.cpp -->
