#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

class WiFiManager;
class Network
{
public:
    Network();
    void resetWifiManager();
    WiFiManager *m_wifiManager;
};