#pragma once

class WiFiManager;
class WebServer;
class BME280;
class Program
{
public:
    Program();
    void loop();
private:
    WiFiManager* wifiManager;    
    WebServer* webServer;   
    BME280* bme280;
    void connexionReseau();
    
};