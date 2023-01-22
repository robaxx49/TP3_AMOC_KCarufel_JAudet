#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>

class BME280
{
private:
    float m_temperature;
    float m_pression;
    float m_altitude;
    String m_message;

public:
    BME280();
    void tick();
};