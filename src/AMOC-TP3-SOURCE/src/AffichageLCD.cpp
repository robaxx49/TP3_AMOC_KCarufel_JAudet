#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "Config.h"
#include "AffichageLCD.h"


AffichageLCD::AffichageLCD(String p_nomEquipeLocal, String p_nomEquipeVisiteur)
    :m_nomEquipeLocal(p_nomEquipeLocal),
    m_nomEquipeVisiteur(p_nomEquipeVisiteur)
{
    LiquidCrystal_I2C lcd(0x27, 16, 2);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print(m_nomEquipeVisiteur);
    lcd.setCursor(0,1);
    lcd.print(m_nomEquipeLocal);
         
     
};

void AffichageLCD::tick(String p_nomEquipeLocal, String p_nomEquipeVisiteur)
{
    if (p_nomEquipeLocal != m_nomEquipeLocal)
    {
        m_nomEquipeLocal = p_nomEquipeLocal;
        LiquidCrystal_I2C lcd(0x27, 16, 2);
        lcd.init();
        lcd.backlight();
        lcd.setCursor(0,0);
        lcd.print(m_nomEquipeVisiteur);
        lcd.setCursor(0,1);
        lcd.print(m_nomEquipeLocal);
    }
    if (p_nomEquipeLocal != m_nomEquipeLocal)
    {
        m_nomEquipeLocal = p_nomEquipeLocal;
        LiquidCrystal_I2C lcd(0x27, 16, 2);
        lcd.init();
        lcd.backlight();
        lcd.setCursor(0,0);
        lcd.print(m_nomEquipeVisiteur);
        lcd.setCursor(0,1);
        lcd.print(m_nomEquipeLocal);
    }
    
};