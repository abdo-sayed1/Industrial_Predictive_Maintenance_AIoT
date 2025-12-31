#include "../src/config.h"
#include "./services/mqtt/mqtt.h"
#include "./services/wifi/wifi.h"
void setup()
{
    Serial.begin(115200);
    wifiSetup();
    mqttsetup();
    xTaskCreate
    (
        vloopmqtt,          // Task function
        "MQTT Loop",       // Name of the task
        (1<<12),           // Stack size (bytes)
        NULL,              // Task input parameter
        1,                 // Priority of the task
        NULL               // Task handle
    );
    vTaskStartScheduler();
}
void loop()
{
    
}
/*
*/