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

void CommunicationMQTT::sendMQTTTemperatureDiscoveryMsg()
{
        // This is the discovery topic for this specific sensor
    String discoveryTopicTemp = "homeassistant/sensor/ESP32_Temperature/config";

    DynamicJsonDocument doc(1024);
    char strPayloadTemp[512];
    JsonObject device;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Temperature
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    doc["name"] = nomAppareil + "_temp";
    doc["unique_id"] = "esp32-1_temp";
    doc["state_topic"] = stateTopic;
    doc["dev_cla"] = "temperature";
    doc["unit_of_meas"] = "°C";
    doc["frc_upd"] = true;
    doc["value_template"] = "{{ value_json.temperature | default(0) }}";

    device = doc.createNestedObject("device");
    device["name"] = nomAppareil;
    device["model"] = modele;
    device["manufacturer"] = fabricant;

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
void CommunicationMQTT::sendTemperatureMsg(float p_temperatureInt,float p_temperatureExt)
{
    // This is the discovery topic for this specific sensor
    String discoveryTopic = "homeassistant/sensor/temperature";

    DynamicJsonDocument doc(256);
    char buffer[256];

    doc["temperatureInterieur"] = p_temperatureInt;
    doc["temperatureExterieur"] = p_temperatureExt;

    size_t n = serializeJson(doc, buffer);

    bool messageEnvoye = m_client->publish(discoveryTopic.c_str(), buffer, n);

    if (messageEnvoye)
    {
        Serial.println("Message envoyé");
    }
    else
    {
        Serial.println("Message non envoyé");
    }
}
