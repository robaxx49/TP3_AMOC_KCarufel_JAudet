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
    WiFiManager *m_wifiManager;
    WebServer *m_webServer;
};
