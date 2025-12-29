#ifndef MQTT_H
#define MQTT_H
#if __cplusplus 
extern "C" {
#endif
    #include <ArduinoMqttClient.h>
    #define SECRET_SSID " "
    #define SECRET_PASS " "
    #define MQTT_IPADDRESS "1"
    #define MQTT_PORT 1883
    #include <WiFi.h>
#if __cplusplus 
}
#endif
#endif