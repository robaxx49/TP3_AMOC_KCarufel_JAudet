#pragma once

class WiFiManager;
class BME280;
class AffichageLCD;
class DS18B20Sensor;
class OneWire;
class CommunicationMQTT;
class Network;
class BoutonReinitialisationData;
class Program
{
public:
    Program();
    void loop();

private:
    
    BME280 *m_bme280;
    DS18B20Sensor *m_ds18b20;
    AffichageLCD *m_affichageLCD;
    CommunicationMQTT *m_communicationMQTT;
    Network *m_network;
    BoutonReinitialisationData *m_BoutonReset;
};
