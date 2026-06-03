#include "ap_provision_server.h"

#include <WiFi.h>
#include <WebServer.h>

#include "../config/wifi_storage.h"

static WebServer server(80);

static bool configured = false;

void handleConfig()
{
    String ssid =
        server.arg("ssid");

    String password =
        server.arg("password");

    if(ssid.isEmpty())
    {
        server.send(400,
                    "text/plain",
                    "ssid required");
        return;
    }

    WifiStorage::save(
        ssid,
        password);

    configured = true;

    server.send(
        200,
        "text/plain",
        "ok");
}

void APProvisionServer::begin()
{
    WiFi.mode(WIFI_AP);

    WiFi.softAP(
        "ESP32_SETUP");

    server.on(
        "/config",
        HTTP_POST,
        handleConfig);

    server.begin();
}

void APProvisionServer::loop()
{
    server.handleClient();
}

bool APProvisionServer::isConfigured()
{
    return configured;
}