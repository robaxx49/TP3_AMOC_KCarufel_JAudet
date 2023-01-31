#pragma once
#include <Arduino.h>
#include <DallasTemperature.h>
#include <Arduino.h>

class DS18B20Sensor
{
private:
DallasTemperature *m_ds18b20;
public:
    float m_temperature;
    DS18B20Sensor(OneWire *oneWire);
    void tick(OneWire *oneWire);
};