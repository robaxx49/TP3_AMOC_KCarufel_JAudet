#include <PubSubClient.h>
#include <ArduinoJson.h>

class Network;
class CommunicationMQTT
{
public:
    CommunicationMQTT(PubSubClient *client, Network *p_network);
    void loop();
    void sendMQTTTemperatureExtDiscoveryMsg();
    void sendMQTTTemperatureIntDiscoveryMsg();
    void sendMQTTHumiditeDiscoveryMsg();
    void sendTemperatureExtMsg(float p_temperature);
    void sendTemperatureIntMsg(float p_temperature);
    void sendHumiditeMsg(float p_humidite);
    void reconnect();

private:
    PubSubClient *m_client;
    Network *m_network;
};