#pragma once

#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>

class BME280
{
private:
public:
    float m_temperature;
    float m_pression;
    float m_altitude;
    float m_humidite;
    String m_message1;
    String m_message2;
    Adafruit_BME280 m_bme;
    BME280();
    void tick();
};