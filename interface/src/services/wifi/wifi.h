#ifndef OUR_WIIFIII_H
#define OUR_WIIFIII_H
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
     #include <WiFiNINA.h>
    #elif defined(ARDUINO_SAMD_MKR1000)
    #include <WiFi101.h>
    #elif defined(ARDUINO_ARCH_ESP8266)
    #include <ESP8266WiFi.h>
    #elif defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_NICLA_VISION) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_GIGA) || defined(ARDUINO_OPTA)
    #include <WiFi.h>
    #elif defined(ARDUINO_PORTENTA_C33)
    #include <WiFiC3.h>
    #elif defined(ARDUINO_UNOR4_WIFI)
    #include <WiFiS3.h>
#endif
#if __cplusplus 
extern "C" {
#endif
    #include <Arduino.h>
    
    #ifndef SSID
    #define SSID      "your_SSID"
    #endif
    #ifndef PASSWORD
    #define PASSWORD  "your_PASSWORD"
    #endif
    #define SERVER_M true
    #define CLIENT_M false
    #ifndef SERVERCLIENTMODE 
    #define SERVERCLIENTMODE CLIENT_M
    #endif
    String wifi_receive();
    void wifi_transmit(char*);
    void wifiSetup();
#if __cplusplus 
}
#endif
#endif