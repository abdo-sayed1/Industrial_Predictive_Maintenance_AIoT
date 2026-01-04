#include "scheduler.h"
#include <Arduino.h>
void setupScheduler()
{
    xTaskCreate
    (
        vSensorCollectionTask, // Task function
        "Sensor Task",         // Name of the task
        (1<<12),               // Stack size (bytes)
        NULL,                  // Task input parameter
        1,                     // Priority of the task
        NULL                   // Task handle
    );
    xTaskCreate
    (
        vInferenceTask,       // Task function
        "Inference Task",     // Name of the task
        (1<<12),             // Stack size (bytes)
        NULL,                // Task input parameter
        3,                   // Priority of the task
        NULL                 // Task handle
    );
    xTaskCreate
    (
        vFeaturesTask,       // Task function
        "Feature Task",      // Name of the task
        (1<<12),             // Stack size (bytes)
        NULL,                // Task input parameter
        2,                   // Priority of the task
        NULL                 // Task handle
    );
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
        (1<<15),           // Stack size (bytes)
        NULL,              // Task input parameter
        5,                 // Priority of the task
        NULL               // Task handle
    );
    xTaskCreate
    (
        vCalculateRPM,     // Task function
        "RPM Calc Task",   // Name of the task
        (1<<9),           // Stack size (bytes)
        NULL,              // Task input parameter
        1,                 // Priority of the task
        NULL               // Task handle
    );
}