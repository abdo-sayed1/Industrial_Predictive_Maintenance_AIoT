#include "mqtt.h"
const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;
const char server[] = MQTT_IPADDRESS;
const int port = MQTT_PORT;
bool switchState = false;
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
void mqttsetup()
{
    if(Serial)
    {
        Serial.printf("Attempting to connect to WPA SSID: %s\n", ssid);
        while(WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
            delay(1000);
        }
        Serial.printf("You're connected to the network!\n\n");
        Serial.print("Attempting to connect to the MQTT broker: ");
      Serial.println(server);
      if (!mqttClient.connect(server, port)) 
      {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(mqttClient.connectError());
        while (1);
      }
      pinMode(14, INPUT);
      Serial.println("You're connected to the MQTT broker!");
      Serial.println();
    }       
}
void mqttpublish(const char* topic, const char* payload)
{
    mqttClient.beginMessage(topic);
    mqttClient.print(payload);
    mqttClient.endMessage();
}
