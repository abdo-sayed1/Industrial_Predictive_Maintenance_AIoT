#include "inference.h"
#include "model_data.h"
#include <Arduino.h>
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
// TFLM Globals
void vInferenceTask(void *pvParameters) 
{
    setupTFLite();
    MachineData_t currentReadings;

    for (;;) 
    {
        // 1. Read Raw Data
        currentReadings.vibration = GetVibration();
        currentReadings.temperature = HAL_GetTemperature();
        currentReadings.current = HAL_GetCurrent();
        currentReadings.voltage = HAL_GetVoltage();

        // 2. Pre-processing / Feature Mapping
        // Manually map your sensor data into the TFLite input tensor
        input->data.f[0] = currentReadings.vibration;
        input->data.f[1] = currentReadings.temperature;
        input->data.f[2] = currentReadings.current;
        input->data.f[3] = currentReadings.voltage;

        // 3. Run Inference
        TfLiteStatus invoke_status = interpreter->Invoke();
        if (invoke_status != kTfLiteOk) {
            TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed!");
            continue;
        }

        // 4. Post-processing
        // Assuming your model output is a probability of failure (Softmax or Sigmoid)
        float failure_probability = output->data.f[0];
        
        currentReadings.isAnomaly = (failure_probability > 0.8f); // Threshold 80%
        currentReadings.faultType = (int)output->data.f[1];      // Example fault classification

        // 5. Send to Queue for the MQTT Task
        xQueueSend(xDataQueue, &currentReadings, 0);

        // Task yield to allow others to run
        vTaskDelay(pdMS_TO_TICKS(50)); 
    }
}