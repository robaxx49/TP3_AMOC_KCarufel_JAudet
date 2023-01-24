#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "Config.h"
#include "AffichageLCD.h"


AffichageLCD::AffichageLCD(String p_message1, String p_message2)
    :m_message1(p_message1),
    m_message2(p_message2)
{
    LiquidCrystal_I2C lcd(0x27, 16, 2);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print(m_message1);
    lcd.setCursor(0,1);
    lcd.print(m_message2);
         
     
};

void AffichageLCD::tick(String p_message1, String p_message2)
{
    if (p_message1 != m_message1 || p_message2 != m_message2)
    {
        m_message1 = p_message1;
        m_message2 = p_message2;
        LiquidCrystal_I2C lcd(0x27, 16, 2);
        lcd.init();
        lcd.backlight();
        lcd.setCursor(0,0);
        lcd.print(m_message1);
        lcd.setCursor(0,1);
        lcd.print(m_message2);
    }
};