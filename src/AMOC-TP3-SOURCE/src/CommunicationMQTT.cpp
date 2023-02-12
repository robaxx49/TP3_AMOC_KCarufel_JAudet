#include "CommunicationMQTT.h"
#include "configTransMQTT.h"
#include "configMQTT.h"
#include "GetDataFromJson.h"

CommunicationMQTT::CommunicationMQTT(PubSubClient *client)
{
    this->m_client = client;
}

void CommunicationMQTT::loop()
{
    this->m_client->loop();
}
void CommunicationMQTT::sendMQTTTemperatureExtDiscoveryMsg()
{
    String discoveryTopicTemp = "homeassistant/sensor/sensor1/config";
    
    DynamicJsonDocument doc(1024);
    char strPayloadTemp[512];
    JsonObject device;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Temperature Exterieur
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    doc["availability_topic"] = "homeassistant/sensor/ds18b20/status",
    device = doc.createNestedObject("device");
    device["identifiers"] = "a1c6f55e1";
    device["name"] = nomAppareil+"_capteur_exterieur";
    device["model"] = modele;
    device["manufacturer"] = fabricant;
    device["sw_version"] = "f76f4640798cf77257362636b100103d";
    doc["platform"] = "mqtt";
    doc["unique_id"] = "esp32-1_tempExt";
    doc["name"] = nomAppareil + "_tempExt";
    doc["state_topic"] = stateTopicExt;
    doc["unit_of_meas"] = "°C";
    doc["value_template"] = "{{ value_json.temperature | default(0) }}";

    Serial.println(" ");
    serializeJson(doc, strPayloadTemp);

    m_client->publish(discoveryTopicTemp.c_str(), strPayloadTemp);
   
}
void CommunicationMQTT::sendMQTTTemperatureIntDiscoveryMsg()
{
    String discoveryTopicTemp = "homeassistant/sensor/sensor2/config";

    DynamicJsonDocument doc(1024);
    char strPayloadTemp[512];
    JsonObject device;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Temperature Interieur
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    doc["availability_topic"] = "homeassistant/sensor/ds18b20/status",
    device = doc.createNestedObject("device");
    device["identifiers"] = "a1c6f55e2";
    device["name"] = nomAppareil+"_capteur_interieur_temperature";
    device["model"] = modele;
    device["manufacturer"] = fabricant;
    device["sw_version"] = "f76f4640798cf77257362636b100103d";
    doc["platform"] = "mqtt";
    doc["unique_id"] = "esp32-1_tempInt";
    doc["name"] = nomAppareil + "_tempInt";
    doc["state_topic"] = stateTopicInt;
    doc["unit_of_meas"] = "°C";
    doc["value_template"] = "{{ value_json.temperature | default(0) }}";

    Serial.println(" ");
    serializeJson(doc, strPayloadTemp);

    m_client->publish(discoveryTopicTemp.c_str(), strPayloadTemp);

}
void CommunicationMQTT::sendMQTTHumiditeDiscoveryMsg()
{
    String discoveryTopicTemp = "homeassistant/sensor/sensor3/config";

    DynamicJsonDocument doc(1024);
    char strPayloadTemp[512];
    JsonObject device;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Humidite
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    doc["availability_topic"] = "homeassistant/sensor/ds18b20/status",
    device = doc.createNestedObject("device");
    device["identifiers"] = "a1c6f55e3";
    device["name"] = nomAppareil+"_capteur_interieur_humidite";
    device["model"] = modele;
    device["manufacturer"] = fabricant;
    device["sw_version"] = "f76f4640798cf77257362636b100103d";
    doc["platform"] = "mqtt";
    doc["unique_id"] = "esp32-1_hum";
    doc["name"] = nomAppareil + "_hum";
    doc["state_topic"] = stateTopicHum;
    doc["unit_of_meas"] = "%";
    doc["value_template"] = "{{ value_json.humidite | default(0) }}";

    Serial.println(" ");
    serializeJson(doc, strPayloadTemp);

    m_client->publish(discoveryTopicTemp.c_str(), strPayloadTemp);

}
void CommunicationMQTT::sendTemperatureExtMsg(float p_temperature)
{

    String Topic = stateTopicExt;

    DynamicJsonDocument doc(256);
    char buffer[256];

    doc["temperature"] = p_temperature;

    size_t n = serializeJson(doc, buffer);

    m_client->publish(Topic.c_str(), buffer, n);

}
void CommunicationMQTT::sendTemperatureIntMsg(float p_temperature)
{
    String Topic = stateTopicInt;

    DynamicJsonDocument doc(256);
    char buffer[256];

    doc["temperature"] = p_temperature;

    size_t n = serializeJson(doc, buffer);

    m_client->publish(Topic.c_str(), buffer, n);

}
void CommunicationMQTT::sendHumiditeMsg(float p_humidite)
{
    String Topic = stateTopicHum;

    DynamicJsonDocument doc(256);
    char buffer[256];

    doc["humidite"] = p_humidite;

    size_t n = serializeJson(doc, buffer);

    m_client->publish(Topic.c_str(), buffer, n);

}
void CommunicationMQTT::reconnect()
{
    String id = "a1c6f55e1";
    String WillTopic = "homeassistant/sensor/ds18b20/status";

    // Loop until we're reconnected
    while (!m_client->connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (m_client->connect(id.c_str(), GetDataFromJson::getDataOneFromJsonStringReturned("username").c_str(), GetDataFromJson::getDataOneFromJsonStringReturned("password").c_str(),WillTopic.c_str(),0,true,"offline"))
        {
            m_client->publish(WillTopic.c_str(),"online");                               
            Serial.println("connected");
            // Once connected, publish an announcement...
            m_client->publish("test/broker", "hello world");
            // ... and resubscribe
            m_client->subscribe("inTopic");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(m_client->state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }

}
