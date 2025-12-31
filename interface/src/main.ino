#include "../src/config.h"
#include "./services/mqtt/mqtt.h"
#include "./services/wifi/wifi.h"
#include "./services/buffer/buffer.h"
#include "./hal/mpu6050/mpu6050.h"
void setup()
{
    Serial.begin(115200);
    wifiSetup();
    mqttsetup();
    mpusetup();
    xTaskCreate
    (
        vloopmqtt,          // Task function
        "MQTT Loop",       // Name of the task
        (1<<12),           // Stack size (bytes)
        NULL,              // Task input parameter
        1,                 // Priority of the task
        NULL               // Task handle
    );
    xTaskCreate
    (
        vbufferTask,       // Task function
        "Buffer Task",     // Name of the task
        (1<<12),           // Stack size (bytes)
        NULL,              // Task input parameter
        3,                 // Priority of the task
        NULL               // Task handle
    );
    vTaskStartScheduler();
}
void loop()
{
    
}
/*
*/