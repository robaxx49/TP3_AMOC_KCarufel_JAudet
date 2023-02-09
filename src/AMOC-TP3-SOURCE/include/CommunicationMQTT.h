#include <PubSubClient.h>
#include <ArduinoJson.h>

class CommunicationMQTT
{
public:
    CommunicationMQTT(PubSubClient *client);
    void loop();
    void sendMQTTTemperatureExtDiscoveryMsg();
    void sendMQTTTemperatureIntDiscoveryMsg();
    void sendMQTTHumiditeDiscoveryMsg();
    void sendTemperatureExtMsg(float p_temperature);
    void sendTemperatureIntMsg(float p_temperature);
    void sendHumiditeMsg(float p_humidite);


private:
    PubSubClient *m_client;
};