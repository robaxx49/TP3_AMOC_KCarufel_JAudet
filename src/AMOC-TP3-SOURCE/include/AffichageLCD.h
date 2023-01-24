#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

class AffichageLCD
{
private:
    long m_derniereDateChangement;
    String m_message1;
    String m_message2;

public:
    AffichageLCD(String p_message1, String p_message2);
    void tick(String p_message1, String p_message2);
};