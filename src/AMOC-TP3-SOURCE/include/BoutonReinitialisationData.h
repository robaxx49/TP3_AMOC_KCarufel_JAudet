#pragma once

#include "Bouton.h"

class Network;
class BoutonReinitialisationData : public Bouton
{
private:
    unsigned char m_pin;
    int m_dernierEtatBouton;
    int m_dernierEtatStableBouton;
    long m_derniereDateChangement;
    Network *m_network;

public:
    BoutonReinitialisationData(const unsigned char &pin, Network *p_network);
    virtual void tick();
};