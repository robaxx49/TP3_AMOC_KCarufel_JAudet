#include "CommunicationMQTT.h"

CommunicationMQTT::CommunicationMQTT(PubSubClient *client)
{
    this->m_client = client;
}

void CommunicationMQTT::loop()
{
    this->m_client->loop();
}