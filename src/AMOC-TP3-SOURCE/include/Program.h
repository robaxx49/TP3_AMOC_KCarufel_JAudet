#pragma once

class WiFiManager;
class WebServer;
class BME280;
class AffichageLCD;
class DS18B20Sensor;
class OneWire;
class Program
{
public:
    Program();
    void loop();

private:
    WiFiManager *m_wifiManager;
    WebServer *m_webServer;
    BME280 *m_bme280;
    OneWire *m_oneWire;
    DS18B20Sensor *m_ds18b20;
    AffichageLCD *m_affichageLCD;
    void connexionReseau();
    void reconnect();
    void sendMQTTTemperatureDiscoveryMsg();
    void sendMQTTHumiditeDiscoveryMsg();
    void sendMQTTPressionDiscoveryMsg();
    void sendMQTTAltitudeDiscoveryMsg();
};
