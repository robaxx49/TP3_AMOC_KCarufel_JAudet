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
uint16_t brokerPort = 8123;
const char *brokerTopic = "test/broker";

WebServer *serveurWeb;
WiFiClient espClient;
PubSubClient client(espClient);
long currentTime = 0;
long lastTime = 0;

Program::Program()
{
    this->m_wifiManager = new WiFiManager();
    this->m_webServer = new WebServer();
    serveurWeb = new WebServer();
    this->connexionReseau();
    Serial.begin(115200);

    // this->m_bme280 = new BME280();
    client.setServer(mqtt_server, brokerPort);
}

void Program::loop()
{
    //  this->m_bme280->tick();
    //  Serial.println(bme280->m_temperature);
    //  Serial.println("test");

    if (WiFi.isConnected())
    {
        // this->m_webServer->handleClient();
    }

    client.loop();

    currentTime = millis();
    if (currentTime - lastTime > 5000 && client.connect(mqtt_server, brokerUser, brokerPassword))
    {
        Serial1.println("test");
        lastTime = currentTime;
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
}

void Program::connexionReseau()
{
    IPAddress adresseIPPortail(192, 168, 23, 1);
    IPAddress passerellePortail(192, 168, 23, 1);
    IPAddress masqueReseauPortail(255, 255, 255, 0);

    WiFiManagerParameter paramerePersonnalise("identifiant_unique_champ",
                                              "Nom du champ",
                                              "ValeurQuiEtaitSauvegardee", 40);

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

    // Exemple d'actions...
    // Sauvegarde des données en JSON
    // Redémarrage : ESP.restart();
    // etc.
    //                                           });

    this->m_wifiManager->addParameter(&paramerePersonnalise);

    this->m_wifiManager->setSTAStaticIPConfig(adresseIPPortail, passerellePortail, masqueReseauPortail);

    this->m_wifiManager->setParamsPage(true);

    // Pour le débug, on peut forcer l'effacement de la configuration du WiFi
    this->m_wifiManager->erase();

    // Essaie de se connecter au réseau WiFi. Si échec, il lance le portail de
    // configuration. L'appel est bloquant -> rend la main après le timeout
    this->m_wifiManager->autoConnect(SSIDPortail, motPasseAPPortail);

    // Pour lancer le portail manuellement
    this->m_wifiManager->startConfigPortal();

    // this->m_webServer->on(UriRegex("/.*"), []()
    //                       { serveurWeb->send(200, "text/plain", "Bienvenue sur mon site web !"); });

    // if (WiFi.isConnected())
    // {
    //     this->m_webServer->begin();
    //     Serial.println("Connecté au réseau : " + WiFi.SSID() +
    //                    " avec l'adresse : " + WiFi.localIP().toString());
    // }
}
