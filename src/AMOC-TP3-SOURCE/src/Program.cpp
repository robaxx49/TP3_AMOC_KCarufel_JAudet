#include <WiFiManager.h>

#include "Program.h"
#include "Arduino.h"

#include "configReseau.h"

Program::Program()
{
   this->wifiManager = new WiFiManager();
   this->webServer = new WebServer();

}

void Program::loop()
{
   
}

void Program::connexionReseau()
{

}
