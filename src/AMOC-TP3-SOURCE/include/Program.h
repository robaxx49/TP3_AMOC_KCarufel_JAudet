#pragma once

class WiFiManager;
class WebServer;
class Program
{
public:
    Program();
  
private:
    WiFiManager* wifiManager;    
    WebServer* webServer;   
    void connexionReseau();
    void loop();
};