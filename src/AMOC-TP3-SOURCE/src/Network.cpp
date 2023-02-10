#include "Network.h"
#include "configReseau.h"
#include "WiFiManager.h"
#include <SPIFFS.h>

Network::Network()
{
    //m_wifiManager->erase();
    WiFiManagerParameter parametreAdresseIP("IPADRESSE",
                                          "Adresse Ip du server MQTT",
                                          "", 40);
    WiFiManagerParameter parametreMQTTUser("MQTTUSER",
                                          "nom utilisateur MQTT",
                                          "", 40);
    WiFiManagerParameter parametreMQTTPassword("MQTTPASSWORD",
                                          "Mot De Passe MQTT",
                                          "", 40);
    this->m_wifiManager = new WiFiManager();
    Serial1.println("Connexion au réseau WiFi en cours...");
    this->m_wifiManager->setDebugOutput(true); // Mettre à true si vous avez des problèmes
    this->m_wifiManager->setAPCallback([](WiFiManager *p_wiFiManager)
                                       { Serial.println("Connexion au réseau WiFi échouée, on lance le portail !"); });
    this->m_wifiManager->setSaveParamsCallback([&parametreAdresseIP,&parametreMQTTUser,&parametreMQTTPassword]()
    {
        Serial.println("Sauvegarde de la configuration effectuée par l'utilisateur dans le portail...");
       

        DynamicJsonDocument doc(1024);
        
        File file = SPIFFS.open("/settings.json","w");
        doc["adresseIP"] = parametreAdresseIP.getValue();  
        doc["username"] = parametreMQTTUser.getValue();
        doc["password"] = parametreMQTTPassword.getValue();
        serializeJson(doc, file);
        file.close();                                          
    });

    this->m_wifiManager->addParameter(&parametreAdresseIP);
    this->m_wifiManager->addParameter(&parametreMQTTUser);
    this->m_wifiManager->addParameter(&parametreMQTTPassword);


    //this->m_wifiManager->setSTAStaticIPConfig(adresseIPPortail, passerellePortail, masqueReseauPortail);

    this->m_wifiManager->setParamsPage(true);

    // Essaie de se connecter au réseau WiFi. Si échec, il lance le portail de
    // configuration. L'appel est bloquant -> rend la main après le timeout

    // this->m_wifiManager->erase();

    this->m_wifiManager->setConfigPortalTimeout(180);
    this->m_wifiManager->autoConnect(SSIDPortail, motPasseAPPortail);
}
void Network::resetWifiManager()
{
    m_wifiManager->erase();
}