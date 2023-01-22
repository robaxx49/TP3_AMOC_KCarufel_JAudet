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
    String m_message;
    Adafruit_BME280 bme;
    BME280();
    void tick();
};