#pragma once

class WiFiManager;
class Program
{
public:
    Program(WiFiManager* wifiManager);
  
private:
    WiFiManager* wifiManager;       
    void setup();
    void loop();
};