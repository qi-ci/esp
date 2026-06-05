#include "command_handler.h"
#include "system_state.h"
#include "sensor_manager.h"

#include "../app/output_formatter.h"
#include "../network/mqtt_client.h"
#include "../drivers/wifi_manager.h"
#include "../config/wifi_storage.h"

#include <ArduinoJson.h>

// ======================
// INIT
// ======================
void CommandHandler_begin()
{
    Serial.println("[CMD] ready (ArduinoJson mode)");
}

// ======================
// CORE HANDLER
// ======================
void CommandHandler_handle(const String& jsonCmd)
{
    // ======================
    // JSON解析（工业标准）
    // ======================
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, jsonCmd);

    if (error)
    {
        String resp = OutputFormatter_buildResponse(
            "fail",
            "json parse error"
        );

        MQTT_publishResponse(resp);
        return;
    }

    String cmd = doc["cmd"] | "";

    String response;


    //wifi_set
    if (cmd == "wifi_set")
    {
        String ssid = doc["ssid"] | "";
        String password = doc["password"] | "";

        if (ssid.length() == 0)
        {
            response = OutputFormatter_buildResponse("fail", "ssid empty");
        }
        else
        {
            g_systemState.wifi_ssid = ssid;
            g_systemState.wifi_password = password;

            WiFiManager_connect();

            bool ok = WiFiManager_testConnect();

            // ✅ 保存到 NVS
            if(true) WiFiStorage_saveWiFi();

            response = OutputFormatter_buildResponse(
                ok ? "ok" : "fail",
                ok ? "wifi connected" : "wifi failed"
            );
        }
    }

    // ======================
    // 1. 获取全部数据
    // ======================
    else if (cmd == "get_all")
    {
        SensorManager_update();
        
        response = OutputFormatter_buildResponse(
            "ok",
            "Temp=" + String(g_systemState.temperature) +
            ",Humi=" + String(g_systemState.humidity) +
            ",CO2=" + String(g_systemState.co2) +
            ",HCHO=" + String(g_systemState.hcho) +
            ",State=" + String(g_systemState.System_OK)
        );
    }

    // ======================
    // 2. 单传感器查询
    // ======================
    else if (cmd == "get_htu21d")
    {
        SensorManager_HTU21Dupdate(); // 主动更新温湿度数据
        response = OutputFormatter_buildResponse("ok",
            "Temp=" + String(g_systemState.temperature) +",Humi=" + String(g_systemState.humidity));
    }

    else if (cmd == "get_ze08")
    {
        SensorManager_ZE08update(); // 主动更新甲醛数据
        response = OutputFormatter_buildResponse("ok",
            "HCHO=" + String(g_systemState.hcho));
    }

    else if (cmd == "get_s8")
    {
        SensorManager_S8update(); // 主动更新CO2数据
        response = OutputFormatter_buildResponse("ok",
            "CO2=" + String(g_systemState.co2));
    }

    // ======================
    // 3. 设置采样周期
    // ======================
    else if (cmd == "set_interval")
    {
        String sensor = doc["sensor"] | "";
        uint32_t value = doc["value"] | 0;

        if (sensor == "htu")
        {
            g_systemState.htu_interval = value;
            WiFiStorage_saveIntervals(); // ✅ 持久化
            response = OutputFormatter_buildResponse("ok", "htu interval updated");
        }
        else if (sensor == "ze08")
        {
            g_systemState.ze08_interval = value;
            WiFiStorage_saveIntervals(); // ✅ 持久化
            response = OutputFormatter_buildResponse("ok", "ze08 interval updated");
        }
        else if (sensor == "s8")
        {
            g_systemState.s8_interval = value;
            WiFiStorage_saveIntervals(); // ✅ 持久化
            response = OutputFormatter_buildResponse("ok", "s8 interval updated");
        }
        else
        {
            response = OutputFormatter_buildResponse("fail", "unknown sensor");
        }
    }

    // ======================
    // 4. 重置采样周期
    // ======================
    else if (cmd == "reset_interval")
    {
        g_systemState.htu_interval  = 5000;
        g_systemState.ze08_interval = 5000;
        g_systemState.s8_interval   = 5000;

        // ✅ 持久化
        WiFiStorage_saveIntervals();

        response = OutputFormatter_buildResponse("ok", "interval reset");
    }

    // ======================
    // 5. 重启设备
    // ======================
    else if (cmd == "reboot")
    {
        response = OutputFormatter_buildResponse("ok", "rebooting");

        MQTT_publishResponse(response);
        delay(300);
        ESP.restart();
        return;
    }



    // ======================
    // unknown command
    // ======================
    else
    {
        response = OutputFormatter_buildResponse(
            "fail",
            "unknown command"
        );
    }

    // ======================
    // MQTT统一回复
    // ======================
    MQTT_publishResponse(response);
}