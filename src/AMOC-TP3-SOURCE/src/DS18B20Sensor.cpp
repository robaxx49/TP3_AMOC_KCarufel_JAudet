#include <Arduino.h>
#include <DallasTemperature.h>
#include <Arduino.h>
#include "Config.h"
#include "DS18B20Sensor.h"
#include "GuideTemperature.h"

DS18B20Sensor::DS18B20Sensor(const uint8_t *sensorAddress , OneWire *oneWire)
: m_sensorAddress(sensorAddress)
{
    
    DallasTemperature *ds18b20 = new DallasTemperature(oneWire);
    ds18b20->begin();
    m_message1 = "";
    m_message2 = "";
    ds18b20->requestTemperaturesByAddress(this->m_sensorAddress);
    m_temperature = ds18b20->getTempC(this->m_sensorAddress);
    this->m_ds18b20 = ds18b20;
}

void DS18B20Sensor::tick()
{
    this->m_ds18b20->requestTemperatures();
    m_temperature = m_ds18b20->getTempCByIndex(0);
    if (m_temperature > temperatureTropChaud)
    {
        m_message1 = "Trop Chaud!";
        m_message2 = "Fermer Patinoire";
    } 
    else if (m_temperature > temperatureIdeal)
    {
        m_message1 = "Chaud!";
        m_message2 = "Pat. a risque!";
    }
    else if (m_temperature > temperatureTropFroid)
    {
        m_message1 = "Froid!";
        m_message2 = "Temp. Ideal!";
    }
    else
    {
        m_message1 = "Trop Froid!";
        m_message2 = "Ne pas resurf.!";
    }
};