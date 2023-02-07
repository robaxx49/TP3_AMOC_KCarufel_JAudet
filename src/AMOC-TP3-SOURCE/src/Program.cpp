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


WiFiClient wifiClient;
PubSubClient client(wifiClient);

WiFiManagerParameter paramerePersonnalise("identifiant_unique_champ",
                                          "Nom du champ",
                                          "ValeurQuiEtaitSauvegardee", 40);

int period = 1000;
unsigned long time_now = 0;
const uint8_t PinSensorOneWire = 12;
void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

Program::Program()
{
    Serial.begin(115200);

    this->m_wifiManager = new WiFiManager();

    this->connexionReseau();

    client.setCallback(callback);

    this->m_bme280 = new BME280();
    this->m_ds18b20 = new DS18B20Sensor(&PinSensorOneWire,m_oneWire);
    this->m_affichageLCD = new AffichageLCD(m_ds18b20->m_message1,m_ds18b20->m_message2);
    client.setServer(mqtt_server, brokerPort);
}

void Program::loop()
{
    this->m_bme280->tick();
    this->m_affichageLCD->tick(m_ds18b20->m_message1,m_ds18b20->m_message2);
    this->m_ds18b20->tick(m_oneWire);
    if (!client.connected())
    {
        this->reconnect();
    }

    if (millis() >= time_now + period)
    {
        time_now += period;
        bool x = client.publish(brokerTopic, "Hello from ESP32");
        if (x)
        {
            Serial.println("Message sent");
        }
        else
        {
            Serial.println("Message not sent");
        }

        this->sendMQTTTemperatureDiscoveryMsg(m_bme280->m_temperature);
        this->sendMQTTHumiditeDiscoveryMsg(m_bme280->m_humidite);
        this->sendMQTTPressionDiscoveryMsg(m_bme280->m_pression);
        this->sendMQTTAltitudeDiscoveryMsg(m_bme280->m_altitude);

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

void Program::sendMQTTTemperatureDiscoveryMsg(float temperature)
{
    // This is the discovery topic for this specific sensor
    String discoveryTopic = "homeassistant/bme280/temperature";

    DynamicJsonDocument doc(256);
    char buffer[256];

    // doc["name"] = "ESP32_Temperature";
    // doc["state_topic"] = stateTopic;
    // doc["unit_of_meas"] = "°C";
    // doc["dev_cla"] = "temperature";
    // doc["frc_upd"] = true;
    // I'm sending a JSON object as the state of this MQTT device
    // so we'll need to unpack this JSON object to get a single value
    // for this specific sensor.
    doc["temperature"] = temperature;

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
void Program::sendMQTTHumiditeDiscoveryMsg(float humidite)
{
    // This is the discovery topic for this specific sensor
    String discoveryTopic = "homeassistant/bme280/humidite";

    DynamicJsonDocument doc(256);
    char buffer[256];


    doc["humidite"] = humidite;

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
void Program::sendMQTTPressionDiscoveryMsg(float pression)
{
    // This is the discovery topic for this specific sensor
    String discoveryTopic = "homeassistant/bme280/pression";

    DynamicJsonDocument doc(256);
    char buffer[256];


    doc["pression"] = pression;

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
void Program::sendMQTTAltitudeDiscoveryMsg(float altitude)
{
    // This is the discovery topic for this specific sensor
    String discoveryTopic = "homeassistant/bme280/altitude";

    DynamicJsonDocument doc(256);
    char buffer[256];

    // doc["name"] = "ESP32_Temperature";
    // doc["state_topic"] = stateTopic;
    // doc["unit_of_meas"] = "°C";
    // doc["dev_cla"] = "temperature";
    // doc["frc_upd"] = true;
    // I'm sending a JSON object as the state of this MQTT device
    // so we'll need to unpack this JSON object to get a single value
    // for this specific sensor.
    doc["altitude"] = altitude;

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
