#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

class AffichageLCD
{
private:
    long m_derniereDateChangement;
    String m_nomEquipeLocal;
    String m_nomEquipeVisiteur;

public:
    AffichageLCD(String p_nomEquipeLocal, String p_nomEquipeVisiteur);
    void tick(String p_nomEquipeLocal, String p_nomEquipeVisiteur);
};