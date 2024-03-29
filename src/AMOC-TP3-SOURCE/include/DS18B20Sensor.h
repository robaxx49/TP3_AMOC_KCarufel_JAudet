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
    String m_message1;
    String m_message2;
    const uint8_t *m_sensorAddress;
    DS18B20Sensor(const uint8_t *sensorAddress, OneWire *oneWire);
    void tick();
};