#pragma once

#include <Arduino.h>

class Adafruit_BME280;
class Adafruit_Sensor;
class BME280
{
private:
public:
    float m_temperature;
    float m_pression;
    float m_altitude;
    String m_message;
    Adafruit_BME280 m_bme;
    BME280();
    void tick();
};