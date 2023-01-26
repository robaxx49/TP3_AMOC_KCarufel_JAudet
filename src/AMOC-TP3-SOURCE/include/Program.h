#pragma once

class WiFiManager;
class WebServer;
class BME280;
class AffichageLCD;
class Program
{
public:
    Program();
    void loop();

private:
    WiFiManager *m_wifiManager;
    WebServer *m_webServer;
    BME280 *m_bme280;
    AffichageLCD *m_affichageLCD;
    void connexionReseau();
    void reconnect();
    void sendMQTTTemperatureDiscoveryMsg(int temperature);
};
