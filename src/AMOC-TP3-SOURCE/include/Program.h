#pragma once

class WiFiManager;
class Program
{
public:
    Program();
  
private:
    WiFiManager* wifiManager;       
    void setup();
    void loop();
};