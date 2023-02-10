#include <WiFiManager.h>
#include <uri/UriRegex.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "configTransMQTT.h"
#include "Program.h"
#include "Arduino.h"
#include "BME280.h"
#include "config.h"
#include "configMQTT.h"
#include "configReseau.h"
#include "AffichageLCD.h"
#include "DS18B20Sensor.h"
#include "Network.h"
#include <SPIFFS.h>
#include "GetDataFromJson.h"
#include "CommunicationMQTT.h"
#include "BoutonReinitialisationData.h"

WiFiClient wifiClient;
PubSubClient client(wifiClient);

unsigned long time_now = 0;

OneWire oneWire(PinSensorOneWire);
Program::Program()
{
    SPIFFS.begin();
    Serial.begin(115200);

    this->m_network = new Network();
    
    this->m_bme280 = new BME280();
    this->m_ds18b20 = new DS18B20Sensor(&PinSensorOneWire,&oneWire);
    this->m_affichageLCD = new AffichageLCD(m_ds18b20->m_message1,m_ds18b20->m_message2);
    this->m_BoutonReset = new BoutonReinitialisationData(PinBouton,m_network);

    client.setServer(GetDataFromJson::getDataOneFromJsonStringReturned("adresseIP").c_str(), brokerPort);
    client.setBufferSize(1024);
    this->m_communicationMQTT = new CommunicationMQTT(&client);
    if (!client.connected())
    { 
         this->m_communicationMQTT->reconnect();
    }

    if(client.connected())
    {
         this->m_communicationMQTT->sendMQTTTemperatureExtDiscoveryMsg();
         this->m_communicationMQTT->sendMQTTTemperatureIntDiscoveryMsg();
         this->m_communicationMQTT->sendMQTTHumiditeDiscoveryMsg();
    }
}

void Program::loop()
{
    
    
    this->m_bme280->tick();
    this->m_affichageLCD->tick(m_ds18b20->m_message1,m_ds18b20->m_message2);
    this->m_ds18b20->tick();
    this->m_BoutonReset->tick();
    if (!client.connected())
    {
        //m_network->resetWifiManager();
        //m_network = new Network();    
        this->m_communicationMQTT->reconnect();
    }

    else
    {
         if (millis() >= time_now + period)
        {
            time_now += period;
            this->m_communicationMQTT->sendTemperatureExtMsg(this->m_ds18b20->m_temperature);
            this->m_communicationMQTT->sendTemperatureIntMsg(this->m_bme280->m_temperature);
            this->m_communicationMQTT->sendHumiditeMsg(this->m_bme280->m_humidite);
        }
    }

    client.loop();
}







