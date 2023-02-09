#include "CommunicationMQTT.h"
#include "configTransMQTT.h"

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

    // doc["availability_topic"] = "homeassistant/sensor/ds18b20/status",
    // device = doc.createNestedObject("device");
    // device["identifiers"] = "a1c6f55e";
    // device["name"] = nomAppareil;
    // device["model"] = modele;
    // device["manufacturer"] = fabricant;
    // device["sw_version"] = "f76f4640798cf77257362636b100103d";
    doc["platform"] = "mqtt";
    doc["unique_id"] = "esp32-1_tempExt";
    doc["name"] = nomAppareil + "_tempExt";
    doc["state_topic"] = stateTopicExt;
    doc["unit_of_meas"] = "°C";
    doc["value_template"] = "{{ value_json.temperature | default(0) }}";

    serializeJsonPretty(doc, Serial);
    Serial.println(" ");
    serializeJson(doc, strPayloadTemp);

    bool messageEnvoyeTemp = m_client->publish(discoveryTopicTemp.c_str(), strPayloadTemp);

    if (messageEnvoyeTemp)
    {
        Serial.println("Message envoyé : temperature");
    }

    else
    {
        Serial.println("Message non envoyé : temperature");
    }
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

    // doc["availability_topic"] = "homeassistant/sensor/ds18b20/status",
    // device = doc.createNestedObject("device");
    // device["identifiers"] = "a1c6f55e";
    // device["name"] = nomAppareil;
    // device["model"] = modele;
    // device["manufacturer"] = fabricant;
    // device["sw_version"] = "f76f4640798cf77257362636b100103d";
    doc["platform"] = "mqtt";
    doc["unique_id"] = "esp32-1_tempInt";
    doc["name"] = nomAppareil + "_tempInt";
    doc["state_topic"] = stateTopicInt;
    doc["unit_of_meas"] = "°C";
    doc["value_template"] = "{{ value_json.temperature | default(0) }}";

    serializeJsonPretty(doc, Serial);
    Serial.println(" ");
    serializeJson(doc, strPayloadTemp);

    bool messageEnvoyeTemp = m_client->publish(discoveryTopicTemp.c_str(), strPayloadTemp);

    if (messageEnvoyeTemp)
    {
        Serial.println("Message envoyé : temperature");
    }

    else
    {
        Serial.println("Message non envoyé : temperature");
    }
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

    // doc["availability_topic"] = "homeassistant/sensor/ds18b20/status",
    // device = doc.createNestedObject("device");
    // device["identifiers"] = "a1c6f55e";
    // device["name"] = nomAppareil;
    // device["model"] = modele;
    // device["manufacturer"] = fabricant;
    // device["sw_version"] = "f76f4640798cf77257362636b100103d";
    doc["platform"] = "mqtt";
    doc["unique_id"] = "esp32-1_hum";
    doc["name"] = nomAppareil + "_hum";
    doc["state_topic"] = stateTopicHum;
    doc["unit_of_meas"] = "%";
    doc["value_template"] = "{{ value_json.humidite | default(0) }}";

    serializeJsonPretty(doc, Serial);
    Serial.println(" ");
    serializeJson(doc, strPayloadTemp);

    bool messageEnvoyeTemp = m_client->publish(discoveryTopicTemp.c_str(), strPayloadTemp);

    if (messageEnvoyeTemp)
    {
        Serial.println("Message envoyé : temperature");
    }

    else
    {
        Serial.println("Message non envoyé : temperature");
    }
}
void CommunicationMQTT::sendTemperatureExtMsg(float p_temperature)
{

    String Topic = stateTopicExt;

    DynamicJsonDocument doc(256);
    char buffer[256];

    doc["temperature"] = p_temperature;

    size_t n = serializeJson(doc, buffer);

    bool messageEnvoye = m_client->publish(Topic.c_str(), buffer, n);

    if (messageEnvoye)
    {
        Serial.println("Message envoyé");
    }
    else
    {
        Serial.println("Message non envoyé");
    }
}
void CommunicationMQTT::sendTemperatureIntMsg(float p_temperature)
{
    String Topic = stateTopicInt;

    DynamicJsonDocument doc(256);
    char buffer[256];

    doc["temperature"] = p_temperature;

    size_t n = serializeJson(doc, buffer);

    bool messageEnvoye = m_client->publish(Topic.c_str(), buffer, n);

    if (messageEnvoye)
    {
        Serial.println("Message envoyé");
    }
    else
    {
        Serial.println("Message non envoyé");
    }
}
void CommunicationMQTT::sendHumiditeMsg(float p_humidite)
{
    String Topic = stateTopicHum;

    DynamicJsonDocument doc(256);
    char buffer[256];

    doc["humidite"] = p_humidite;

    size_t n = serializeJson(doc, buffer);

    bool messageEnvoye = m_client->publish(Topic.c_str(), buffer, n);

    if (messageEnvoye)
    {
        Serial.println("Message envoyé");
    }
    else
    {
        Serial.println("Message non envoyé");
    }
}
