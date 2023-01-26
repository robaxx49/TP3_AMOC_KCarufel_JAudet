#include <PubSubClient.h>

class CommunicationMQTT
{
public:
    CommunicationMQTT(PubSubClient *client);
    void loop();

private:
    PubSubClient *m_client;
};