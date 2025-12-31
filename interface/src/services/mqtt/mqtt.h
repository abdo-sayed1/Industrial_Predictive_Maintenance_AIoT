#ifndef MQTT_H
#define MQTT_H
    #include <WiFi.h>
    #include <ArduinoMqttClient.h>
    #include "../../../src/config.h"
    #define SECRET_SSID WIFI_SSID
    #define SECRET_PASS WIFI_PASSWORD
    #define MQTT_IPADDRESS MQTT_BROKER
    void mqttsetup();
    void mqttpublish(const char* topic, const char* payload);
    void mqttsubscribe(const char* topic);
#endif