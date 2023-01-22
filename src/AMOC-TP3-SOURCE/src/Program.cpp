#include <WiFiManager.h>
#include "BME280.h"
#include "Program.h"
#include "Arduino.h"

#include "configReseau.h"

Program::Program()
{
   //this->wifiManager = new WiFiManager();
   //this->webServer = new WebServer();
   this->bme280 = new BME280();
}

void Program::loop()
{
   //bme280->tick();
   //Serial.println(bme280->m_temperature);
   Serial.println("test");
}

void Program::connexionReseau()
{

}
