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
    
}

void DS18B20Sensor::tick(OneWire *oneWire)
{
    this->m_ds18b20->requestTemperatures();
    
};