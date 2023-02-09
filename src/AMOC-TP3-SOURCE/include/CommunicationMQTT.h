#include <PubSubClient.h>
#include <ArduinoJson.h>

class CommunicationMQTT
{
public:
    CommunicationMQTT(PubSubClient *client);
    void loop();
    void sendMQTTTemperatureDiscoveryMsg();
    void sendTemperatureMsg(float p_temperatureInt,float p_temperatureExt);


private:
    PubSubClient *m_client;
};