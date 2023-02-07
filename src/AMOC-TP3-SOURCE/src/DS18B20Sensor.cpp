#include <Arduino.h>
#include <DallasTemperature.h>
#include <Arduino.h>
#include "Config.h"
#include "DS18B20Sensor.h"

DS18B20Sensor::DS18B20Sensor(OneWire *oneWire)
{
    m_temperature = 0.0f;
    DallasTemperature *m_ds18b20 = new DallasTemperature(oneWire);
    m_ds18b20->begin();
    m_message1 = "";
    m_message2 = "";
    
}

void DS18B20Sensor::tick(OneWire *oneWire)
{
    this->m_ds18b20->requestTemperatures();
    
    if (m_temperature > 5)
    {
        m_message1 = "Trop Chaud!";
        m_message2 = "Fermer Patinoire";
    } 
    else if (m_temperature > -7)
    {
        m_message1 = "Chaud!";
        m_message2 = "Pat. a risque!";
    }
    else if (m_temperature > -20)
    {
        m_message1 = "Froid!";
        m_message2 = "Temp. Ideal!";
    }
    else
    {
        m_message1 = "Très Froid!";
        m_message2 = "Ne pas resurf.!";
    }
};