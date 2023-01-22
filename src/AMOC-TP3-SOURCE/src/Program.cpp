#include <WiFiManager.h>
#include <uri/UriRegex.h>

// #include "BME280.h"
#include "Program.h"
#include "Arduino.h"

#include "configReseau.h"

WebServer *serveurWeb;

Program::Program()
{
    this->m_wifiManager = new WiFiManager();
    this->m_webServer = new WebServer();
    serveurWeb = new WebServer();
    this->connexionReseau();

    // this->m_bme280 = new BME280();
}

void Program::loop()
{
    // this->m_bme280->tick();
    // Serial.println(bme280->m_temperature);
    // Serial.println("test");

    if (WiFi.isConnected())
    {
        this->m_webServer->handleClient();
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

    this->m_wifiManager->setDebugOutput(false); // Mettre à true si vous avez des problèmes
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
    this->m_wifiManager->erase();

    // Essaie de se connecter au réseau WiFi. Si échec, il lance le portail de
    // configuration. L'appel est bloquant -> rend la main après le timeout
    this->m_wifiManager->autoConnect(SSIDPortail, motPasseAPPortail);

    // Pour lancer le portail manuellement
    this->m_wifiManager->startConfigPortal();

    this->m_webServer->on(UriRegex("/.*"), []()
                          { serveurWeb->send(200, "text/plain", "Bienvenue sur mon site web !"); });

    if (WiFi.isConnected())
    {
        this->m_webServer->begin();
        Serial.println("Connecté au réseau : " + WiFi.SSID() +
                       " avec l'adresse : " + WiFi.localIP().toString());
    }
}
