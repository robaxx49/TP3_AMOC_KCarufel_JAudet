#include <Arduino.h>
#include "BME280.h"
#include <Adafruit_BMP280.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include "Config.h"

BME280::BME280()
{
    m_temperature = bme.readTemperature();
    m_altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    m_pression = bme.readPressure();
}

void BME280::tick()
{
    Adafruit_BME280 bme;
    m_temperature = bme.readTemperature();
    m_altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    m_pression = bme.readPressure();
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