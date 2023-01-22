#include <Arduino.h>
#include "BME280.h"
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include "Config.h"

BME280::BME280()
{
    m_temperature = 0.0f;
    m_altitude = 0.0f;
    m_pression = 0.0f;
}

void BME280::tick()
{
    
    bool status;
    status = m_bme.begin(0x76); 
    if (!status) {
    Serial.println("Impossible de trouver un BME280, contactez le support!");
    while (1);
    }
    m_temperature = m_bme.readTemperature();
    m_altitude = m_bme.readAltitude(SEALEVELPRESSURE_HPA);
    m_pression = m_bme.readPressure();
    if (m_temperature > 0)
    {
        m_message = "Plus haut que le point de congelation";
    }
    else if (m_temperature > -5)
    {
        m_message = "Temperature Ideal";
    }
    else
    {
        m_message = "Temperature Froide";
    }
    
    

};