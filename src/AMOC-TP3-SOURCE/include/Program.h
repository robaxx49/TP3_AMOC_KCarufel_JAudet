#pragma once

class WiFiManager;
class WebServer;
class Program
{
public:
    Program();
    void connexionReseau();
    void loop();

private:
    WiFiManager *wifiManager;
    WebServer *webServer;
};
