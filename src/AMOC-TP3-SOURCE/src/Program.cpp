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

// WebServer *serveurWeb;
WiFiClient wifiClient;
PubSubClient client(wifiClient);

WiFiManagerParameter paramerePersonnalise("identifiant_unique_champ",
                                          "Nom du champ",
                                          "ValeurQuiEtaitSauvegardee", 40);

long currentTime = 0;
long lastTime = 0;


Program::Program()
{
    Serial.begin(115200);

    this->m_wifiManager = new WiFiManager();
    // this->m_webServer = new WebServer();
    // serveurWeb = new WebServer();
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

    if (WiFi.isConnected())
    {
        // this->m_webServer->handleClient();
    }

    currentTime = millis();

    if (!client.connected())
    {
        reconnect();
    }

    if (currentTime - lastTime > 5000 && client.connected())
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
    else
    {
        Serial.println("not connected");
    }

    client.loop();
}


void Program::connexionReseau()
{
    Serial1.println("Connexion au réseau WiFi en cours...");

    IPAddress adresseIPPortail(192, 168, 23, 1);
    IPAddress passerellePortail(192, 168, 23, 1);
    IPAddress masqueReseauPortail(255, 255, 255, 0);

    this->m_wifiManager->setDebugOutput(true); // Mettre à true si vous avez des problèmes
    this->m_wifiManager->setAPCallback([](WiFiManager *p_wiFiManager)
                                       { Serial.println("Connexion au réseau WiFi échouée, on lance le portail !"); });
    this->m_wifiManager->setConfigPortalTimeout(180);

    this->m_wifiManager->setSaveParamsCallback([&paramerePersonnalise]()
                                               {
                                                   Serial.println(
                                                       "Sauvegarde de la configuration effectuée par l'utilisateur dans le "
                                                       "portail...");
                                                   Serial.println(String("Nouvelle valeur du paramètre : ") +
                                                                  paramerePersonnalise.getValue());
                                                                  

                                                   // Exemple d'actions...
                                                   // Sauvegarde des données en JSON
                                                   // Redémarrage : ESP.restart();
                                                   // etc.
                                               });

    this->m_wifiManager->addParameter(&paramerePersonnalise);

    this->m_wifiManager->setSTAStaticIPConfig(adresseIPPortail, passerellePortail, masqueReseauPortail);

    this->m_wifiManager->setParamsPage(true);

    // Pour le débug, on peut forcer l'effacement de la configuration du WiFi
    // this->m_wifiManager->erase();

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

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("Esp32Arduino"))
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

Program::Program()
{
    Serial.begin(115200);
    // this->m_webServer = new WebServer();
    // serveurWeb = new WebServer();

    // WiFi.begin(SSIDWifi, motPasseWifi);
    // Serial.println("Connecting to Wi-Fi");

    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     delay(500);
    //     Serial.print(".");
    // }

    // WiFi.setAutoReconnect(true);
    // WiFi.persistent(true);

    // Serial.println("Connected to Wi-Fi" + WiFi.SSID() + " with IP address: " + WiFi.localIP().toString());

    this->m_wifiManager = new WiFiManager();
    this->connexionReseau();

    // this->m_bme280 = new BME280();
    client.setServer(mqtt_server, brokerPort);
    client.setCallback(callback);
}

void Program::loop()
{
    //  this->m_bme280->tick();
    //  Serial.println(bme280->m_temperature);
    //  Serial.println("test");

    if (WiFi.isConnected())
    {
        Serial.println("Wifi connected");

        if (!client.connected())
        {
            reconnect();
        }

        if (currentTime - lastTime > 5000 && client.connected())
        {
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
            currentTime = millis();
        }

        client.loop();
    }
}
