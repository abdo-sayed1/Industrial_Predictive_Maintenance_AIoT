#ifndef MQTT_H
#define MQTT_H
    #include <WiFi.h>
    #include <ArduinoMqttClient.h>
    #define SECRET_SSID " "
    #define SECRET_PASS " "
    #define MQTT_IPADDRESS "8.8.8.8"
    #define MQTT_PORT 1883
    void mqttsetup();
    void mqttpublish(const char* topic, const char* payload);
    void mqttsubscribe(const char* topic);
#endif