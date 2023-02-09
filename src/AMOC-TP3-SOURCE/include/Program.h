#pragma once

class WiFiManager;
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
    BME280 *m_bme280;
    DS18B20Sensor *m_ds18b20;
    AffichageLCD *m_affichageLCD;

    void connexionReseau();
    void reconnect();
    void sendMQTTTemperatureDiscoveryMsg();
    void sendMQTTHumiditeDiscoveryMsg();
    void sendMQTTPressionDiscoveryMsg();
    void sendMQTTAltitudeDiscoveryMsg();
};
