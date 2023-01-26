#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

#include "BME280.h"
#include "Config.h"

BME280::BME280()
{
    m_temperature = 0.0f;
    m_altitude = 0.0f;
    m_pression = 0.0f;
    m_humidite = 0.0f;
    m_message1 = "";
    m_message2 = "";
}
//https://www.otip.com/Why-OTIP/News/Planning-to-build-a-backyard-ice-rink-Skate-smooth
void BME280::tick()
{
    bool status;
    status = m_bme.begin(0x76);
    if (!status)
    {
        Serial.println("Impossible de trouver le BME280 sur votre port, contactez le support!");
        while (1)
            ;
    }
    m_temperature = m_bme.readTemperature();
    m_altitude = m_bme.readAltitude(SEALEVELPRESSURE_HPA);
    m_pression = m_bme.readPressure();
    m_humidite = m_bme.readHumidity();
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