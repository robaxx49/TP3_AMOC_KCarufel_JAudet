#include <WiFiManager.h>
#include <uri/UriRegex.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "Program.h"
#include "Arduino.h"
#include "BME280.h"
#include "config.h"
#include "configMQTT.h"
#include "configReseau.h"
#include "AffichageLCD.h"
#include "DS18B20Sensor.h"

IPAddress adresseIPPortail(192, 168, 23, 1);
IPAddress passerellePortail(192, 168, 23, 1);
IPAddress masqueReseauPortail(255, 255, 255, 0);

int sensorNumber = 1;
String stateTopic = "homeassistant/sensor/status";
String nomAppareil = "ESP32_Temperature_capteur";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

WiFiManagerParameter paramerePersonnalise("identifiant_unique_champ",
                                          "Nom du champ",
                                          "ValeurQuiEtaitSauvegardee", 40);

int period = 1000;
unsigned long time_now = 0;
const uint8_t PinSensorOneWire = 12;

void mqttHomeAssistantDiscoverySetup()
{
    // This is the discovery topic for this specific sensor

    String discoveryTopic = "homeassistant/sensor/ESP32_Temperature/config";
    DynamicJsonDocument doc(1024);
    char strPayload[256];
    JsonObject device;
    JsonArray identifiers;

    // String payload = "{\"name\":\"Temperature\",\"state_topic\":\"homeassistant/sensor/status\",\"unit_of_measurement\":\"°C\",\"value_template\":\"{{ value_json.temperature }}\",\"device\":{\"identifiers\":[\"esp32-1\"],\"name\":\"ESP32\",\"model\":\"ESP32\",\"manufacturer\":\"Espressif\"}}";
    doc["name"] = "ESP32_Temperature";
    doc["unique_id"] = "esp32-1_temperature";
    doc["state_topic"] = stateTopic;
    doc["dev_cla"] = "temperature";
    doc["unit_of_measurement"] = "°C";
    doc["frc_upd"] = true;

    //doc["value_template"] = "{{ value_json.temperature|default(0) }}";

    // doc["name"] = "ESP32_Temperature";
    // doc["state_topic"] = stateTopic;+
    // doc["unit_of_meas"] = "°C";
    // doc["dev_cla"] = "temperature";
    // doc["frc_upd"] = true;
    // // I'm sending a JSON object as the state of this MQTT device
    // // so we'll need to unpack this JSON object to get a single value
    // // for this specific sensor.
    // doc["value_template"] = "{{ value_json.temperature|default(0) }}";

    // device = doc.createNestedObject("device");
    // device["name"] = "ESP32_Temperature_capteur";

    // identifiers = device.createNestedArray("identifiers");
    // identifiers.add("esp32-1");

    serializeJsonPretty(doc, Serial);
    Serial.println(" ");
    size_t n = serializeJson(doc, strPayload);

    bool messageEnvoye = client.publish(discoveryTopic.c_str(), strPayload);

    if (messageEnvoye)
    {
        Serial.println("Message envoyé : temperature");
    }

    else
    {
        Serial.println("Message non envoyé : temperature");
    }
}

Program::Program()
{
    Serial.begin(115200);

    this->m_wifiManager = new WiFiManager();

    this->connexionReseau();

    // this->m_bme280 = new BME280();
    // this->m_affichageLCD = new AffichageLCD(m_bme280->m_message1,m_bme280->m_message2);

    client.setServer(mqtt_server_home, brokerPort);

    if (!client.connected())
    {
        this->reconnect();
        mqttHomeAssistantDiscoverySetup();
        this->sendMQTTTemperatureDiscoveryMsg();
        // this->sendMQTTHumiditeDiscoveryMsg();
    }
}

bool sendTemperatureMsg(float temperature, float humidity)
{
    StaticJsonDocument<200> doc;
    doc["temperature"] = temperature;
    doc["humidite"] = humidity;
    char buffer[200];
    serializeJson(doc, buffer);
    if (client.publish(stateTopic.c_str(), buffer))
    {
        return true;
    }
    else
    {
        return false;
    }
    // this->m_bme280 = new BME280();
    // this->m_ds18b20 = new DS18B20Sensor(&PinSensorOneWire,m_oneWire);
    // this->m_affichageLCD = new AffichageLCD(m_ds18b20->m_message1,m_ds18b20->m_message2);
    client.setServer(mqtt_server_home, brokerPort);
}

// bool sendHumidityMsg(float humidity)
// {
//     StaticJsonDocument<200> doc;
//     doc["humidite"] = humidity;
//     char buffer[200];
//     serializeJson(doc, buffer);
//     if (client.publish(stateTopic.c_str(), buffer))
//     {
//         return true;
//     }
//     else
//     {
//         return false;
//     }
// }

void Program::loop()
{
    // this->m_bme280->tick();
    // this->m_affichageLCD->tick(m_bme280->m_message1,m_bme280->m_message2);

    // this->m_bme280->tick();
    // this->m_affichageLCD->tick(m_ds18b20->m_message1,m_ds18b20->m_message2);
    // this->m_ds18b20->tick(m_oneWire);

    if (!client.connected())
    {
        this->reconnect();
    }

    if (millis() >= time_now + period)
    {
        time_now += period;
        // bool temp = sendTemperatureMsg(25.5, 50.3);
        //  bool humid = sendHumidityMsg(50.5);

        // if (temp)
        // {
        //     Serial.println("Message sent");
        // }
        // else
        // {
        //     Serial.println("Message not sent");
        // }
    }

    client.loop();
}

void Program::connexionReseau()
{
    Serial1.println("Connexion au réseau WiFi en cours...");

    this->m_wifiManager->setDebugOutput(true); // Mettre à true si vous avez des problèmes
    this->m_wifiManager->setAPCallback([](WiFiManager *p_wiFiManager)
                                       { Serial.println("Connexion au réseau WiFi échouée, on lance le portail !"); });

    this->m_wifiManager->setConfigPortalTimeout(180);

    // this->m_wifiManager->setSaveParamsCallback([&paramerePersonnalise]()
    //                                            {
    //                                                Serial.println(
    //                                                    "Sauvegarde de la configuration effectuée par l'utilisateur dans le "
    //                                                    "portail...");
    //                                                Serial.println(String("Nouvelle valeur du paramètre : ") +
    //                                                               paramerePersonnalise.getValue());

    //                                                // Exemple d'actions...
    //                                                // Sauvegarde des données en JSON
    //                                                // Redémarrage : ESP.restart();
    //                                                // etc.
    //                                            });

    // this->m_wifiManager->addParameter(&paramerePersonnalise);

    // this->m_wifiManager->setSTAStaticIPConfig(adresseIPPortail, passerellePortail, masqueReseauPortail);

    // this->m_wifiManager->setParamsPage(true);

    // Essaie de se connecter au réseau WiFi. Si échec, il lance le portail de
    // configuration. L'appel est bloquant -> rend la main après le timeout

    // this->m_wifiManager->erase();

    this->m_wifiManager->autoConnect(SSIDPortail, motPasseAPPortail);
}

void Program::reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("Esp32Arduino", brokerUser, brokerPassword))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish("test/broker", "hello world");
            // ... and resubscribe
            client.subscribe("inTopic");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void Program::sendMQTTTemperatureDiscoveryMsg()
{
    // This is the discovery topic for this specific sensor
    String discoveryTopic = "homeassistant/sensor/ESP32_Temperature/config";

    DynamicJsonDocument doc(256);
    char buffer[256];

    doc["name"] = "ESP32_Temperature";
    doc["state_topic"] = stateTopic;
    doc["unit_of_meas"] = "°C";
    doc["dev_cla"] = "temperature";
    doc["frc_upd"] = true;
    // I'm sending a JSON object as the state of this MQTT device
    // so we'll need to unpack this JSON object to get a single value
    // for this specific sensor.
    doc["value_template"] = "{{ value_json.temperature|default(0) }}";

    size_t n = serializeJson(doc, buffer);

    bool messageEnvoye = client.publish(discoveryTopic.c_str(), buffer, n);

    if (messageEnvoye)
    {
        Serial.println("Message envoyé : temperature");
    }

    else
    {
        Serial.println("Message non envoyé : temperature");
    }
}

void Program::sendMQTTHumiditeDiscoveryMsg()
{
    // This is the discovery topic for this specific sensor
    String discoveryTopic = "homeassistant/sensor/ESP32_Humidite/config";

    DynamicJsonDocument doc(256);
    char buffer[256];

    doc["name"] = "ESP32_Humidite";
    doc["state_topic"] = stateTopic;
    doc["unit_of_meas"] = " %";
    doc["dev_cla"] = "humidite";
    doc["frc_upd"] = true;
    // I'm sending a JSON object as the state of this MQTT device
    // so we'll need to unpack this JSON object to get a single value
    // for this specific sensor.
    doc["value_template"] = "{{ value_json.humidite|default(0) }}";

    size_t n = serializeJson(doc, buffer);

    bool messageEnvoye = client.publish(discoveryTopic.c_str(), buffer, n);

    if (messageEnvoye)
    {
        Serial.println("Message envoyé : humidite");
    }

    else
    {
        Serial.println("Message non envoyé : humidite");
    }
}

void Program::sendMQTTPressionDiscoveryMsg()
{
    // This is the discovery topic for this specific sensor
    String discoveryTopic = "homeassistant/bme280/pression";

    DynamicJsonDocument doc(256);
    char buffer[256];

    // doc["pression"] = pression;

    size_t n = serializeJson(doc, buffer);

    bool messageEnvoye = client.publish(discoveryTopic.c_str(), buffer, n);

    if (messageEnvoye)
    {
        Serial.println("Message envoyé");
    }
    else
    {
        Serial.println("Message non envoyé");
    }
}

void Program::sendMQTTAltitudeDiscoveryMsg()
{
    // This is the discovery topic for this specific sensor
    String discoveryTopic = "homeassistant/bme280/altitude";

    DynamicJsonDocument doc(256);
    char buffer[256];

    doc["name"] = "ESP32_Temperature";
    doc["state_topic"] = stateTopic;
    doc["unit_of_meas"] = "°C";
    doc["dev_cla"] = "temperature";
    doc["frc_upd"] = true;
    // I'm sending a JSON object as the state of this MQTT device
    // so we'll need to unpack this JSON object to get a single value
    // for this specific sensor.
    // doc["altitude"] = altitude;

    size_t n = serializeJson(doc, buffer);

    bool messageEnvoye = client.publish(discoveryTopic.c_str(), buffer, n);

    if (messageEnvoye)
    {
        Serial.println("Message envoyé");
    }
    else
    {
        Serial.println("Message non envoyé");
    }
}
