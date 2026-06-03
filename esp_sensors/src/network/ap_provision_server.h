#pragma once

class APProvisionServer
{
public:

    static void begin();

    static void loop();

    static bool isConfigured();
};