#include "BoutonReinitialisationData.h"
#include "config.h"
#include "Network.h"

BoutonReinitialisationData::BoutonReinitialisationData(const unsigned char &pin, Network *p_network)
    : m_pin(pin),
      m_dernierEtatBouton(HIGH),
      m_dernierEtatStableBouton(HIGH),
      
      m_derniereDateChangement(0)
{
    this->m_network = p_network;
}

void BoutonReinitialisationData::tick()
{
    // Action se fait lors du relachement du bouton
    int etatBouton = digitalRead(this->m_pin);
    long dateActuelle = millis();
    if (etatBouton != m_dernierEtatBouton)
    {
        m_derniereDateChangement = dateActuelle;
        m_dernierEtatBouton = etatBouton;
    }
    if (dateActuelle - m_derniereDateChangement > DELAIS_PRESSION_CLICK)
    {
        if (m_dernierEtatStableBouton == LOW && etatBouton == HIGH)
        {
            m_network->resetWifiManager();
            
        }
        m_dernierEtatStableBouton = etatBouton;
    }
}