#include <WiFiManager.h>
#include <uri/UriRegex.h>
#include <PubSubClient.h>

// #include "BME280.h"
#include "Program.h"
#include "Arduino.h"

#include "configReseau.h"

const char *brokerUser = "admin";
const char *brokerPassword = "admin";
const char *mqtt_server = "10.0.0.156";
// const char* brokerPort = 1883;
uint16_t brokerPort = 1883;
const char *brokerTopic = "test/broker";

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

    // this->m_bme280 = new BME280();
    client.setServer(mqtt_server, brokerPort);
}

void Program::loop()
{
    //  this->m_bme280->tick();
    //  Serial.println(bme280->m_temperature);
    //  Serial.println("test");

    if (!client.connected())
    {
        this->reconnect();
    }
    // && client.connected() && WiFi.isConnected()

    if (millis() >= time_now + period)
    {
        time_now += period;
        Serial.println("test");
        bool x = client.publish(brokerTopic, "Hello from ESP32");
        if (x)
        {
            Serial.println("Message sent");
        }
        else
        {
            Serial.println("Message not sent");
        }
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

    this->m_wifiManager->erase();

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
