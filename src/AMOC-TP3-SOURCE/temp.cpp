// This is the discovery topic for this specific sensor
String discoveryTopic = "homeassistant/sensor/ESP32_Temperature/config";

DynamicJsonDocument doc(256);
char buffer[256];

// JsonObject device;
// JsonArray identifiers;
// device = doc.createNestedObject("device");
// device["name"] = "ESP32_Temperature_capteur";

identifiers = device.createNestedArray("identifiers");
identifiers.add("esp32-1");

doc["name"] = "ESP32_Temperature";
doc["state_topic"] = stateTopic;
doc["unit_of_meas"] = "°C";
doc["dev_cla"] = "temperature";
doc["frc_upd"] = true;
// I'm sending a JSON object as the state of this MQTT device
// so we'll need to unpack this JSON object to get a single value
// for this specific sensor.
doc["value_template"] = "{{ value_json.temperature|default(0) }}";

size_t n = serializeJson(doc, buffer);

bool messageEnvoye = client.publish(discoveryTopic.c_str(), buffer, n);

if (messageEnvoye)
{
    Serial.println("Message envoyé : temperature");
}

else
{
    Serial.println("Message non envoyé : temperature");
}