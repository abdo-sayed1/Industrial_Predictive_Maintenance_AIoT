#include "../src/config.h"
#include "./services/mqtt/mqtt.h"
#include "./services/wifi/wifi.h"
#include "./services/buffer/buffer.h"
#include "./hal/mpu6050/mpu6050.h"
#include "./app/features/features.h"
#include "./app/inference/inference.h"
#include "./hal/encoder/encoder.h"
namespace 
{
    tflite::ErrorReporter* error_reporter = nullptr;
    const tflite::Model* model = nullptr;
    tflite::MicroInterpreter* interpreter = nullptr;
    TfLiteTensor* input = nullptr;
    TfLiteTensor* output = nullptr;
    constexpr int kTensorArenaSize = 8 * 1024;  // 8KB
    uint8_t tensor_arena[kTensorArenaSize];
}

void setup()
{
    Serial.begin(115200);
    wifiSetup();
    mqttsetup();
    mpusetup();
    setupTFLite();
    encoder_setup();
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
    vTaskStartScheduler();
}
void loop(){}
/*

*/
