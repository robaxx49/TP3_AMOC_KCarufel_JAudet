#pragma once

#include <Arduino.h>

class Bouton
{
private:
    unsigned char m_pin;
    int m_dernierEtatBouton;
    int m_dernierEtatStableBouton;
    long m_derniereDateChangement;

public:
    virtual void tick();
};
