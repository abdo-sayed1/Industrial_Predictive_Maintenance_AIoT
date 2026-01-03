# 1 "C:\\Users\\kimot\\AppData\\Local\\Temp\\tmpjfr84qy6"
#include <Arduino.h>
# 1 "C:/Users/kimot/Desktop/ES/AIoT_NTI_SEITECH/Hard_Skills/project/Industrial_Predictive_Maintenance_AIoT/interface/src/main.ino"
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
    constexpr int kTensorArenaSize = 8 * 1024;
    uint8_t tensor_arena[kTensorArenaSize];
}
void setup();
void loop();
#line 20 "C:/Users/kimot/Desktop/ES/AIoT_NTI_SEITECH/Hard_Skills/project/Industrial_Predictive_Maintenance_AIoT/interface/src/main.ino"
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
        vSensorCollectionTask,
        "Sensor Task",
        (1<<12),
        NULL,
        1,
        NULL
    );
    xTaskCreate
    (
        vInferenceTask,
        "Inference Task",
        (1<<12),
        NULL,
        3,
        NULL
    );
    xTaskCreate
    (
        vFeaturesTask,
        "Feature Task",
        (1<<12),
        NULL,
        2,
        NULL
    );
    xTaskCreate
    (
        vloopmqtt,
        "MQTT Loop",
        (1<<12),
        NULL,
        1,
        NULL
    );
    xTaskCreate
    (
        vbufferTask,
        "Buffer Task",
        (1<<15),
        NULL,
        5,
        NULL
    );
    xTaskCreate
    (
        vCalculateRPM,
        "RPM Calc Task",
        (1<<12),
        NULL,
        1,
        NULL
    );
    vTaskStartScheduler();
}
void loop(){}