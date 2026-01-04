#include "../src/config.h"
#include "./services/mqtt/mqtt.h"
#include "./services/wifi/wifi.h"
#include "./services/buffer/buffer.h"
#include "./hal/mpu6050/mpu6050.h"
#include "./app/features/features.h"
#include "./app/inference/inference.h"
#include "./hal/encoder/encoder.h"
#include "./app/scheduler/scheduler.h"
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
    setupScheduler();
    vTaskStartScheduler();
}
void loop(){}
/*

*/
