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
#include "CommunicationMQTT.h"

IPAddress adresseIPPortail(192, 168, 23, 1);
IPAddress passerellePortail(192, 168, 23, 1);
IPAddress masqueReseauPortail(255, 255, 255, 0);

int sensorNumber = 1;


WiFiClient wifiClient;
PubSubClient client(wifiClient);

WiFiManagerParameter paramerePersonnalise("identifiant_unique_champ",
                                          "Nom du champ",
                                          "ValeurQuiEtaitSauvegardee", 40);

int period = 2000;
unsigned long time_now = 0;
const uint8_t PinSensorOneWire = 14;
OneWire oneWire(PinSensorOneWire);
Program::Program()
{
    Serial.begin(115200);

    this->m_wifiManager = new WiFiManager();

    this->connexionReseau();
    
    this->m_bme280 = new BME280();
    this->m_ds18b20 = new DS18B20Sensor(&PinSensorOneWire,&oneWire);
    this->m_affichageLCD = new AffichageLCD(m_ds18b20->m_message1,m_ds18b20->m_message2);
    client.setServer(mqtt_server_cegep, brokerPort);
    client.setBufferSize(1024);
    this->m_communicationMQTT = new CommunicationMQTT(&client);
    if (!client.connected())
    {
         this->reconnect();
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

    if (!client.connected())
    {
        this->reconnect();
    }

    else
    {
         if (millis() >= time_now + period)
        {
            time_now += period;
            //bool temp = sendTemperatureMsg(25.5, 50.3);

            // if (temp)
            // {
            //     Serial.println("Message sent");
            // }
            // else
            // {
            //     Serial.println("Message not sent");
            // }
            this->m_communicationMQTT->sendTemperatureExtMsg(this->m_ds18b20->m_temperature);
            this->m_communicationMQTT->sendTemperatureIntMsg(this->m_bme280->m_temperature);
            this->m_communicationMQTT->sendHumiditeMsg(this->m_bme280->m_humidite);
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



