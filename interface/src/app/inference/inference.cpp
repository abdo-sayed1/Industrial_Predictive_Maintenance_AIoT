#include "inference.h"
#include "model_data.h"
#include "../../hal/ds18b20/ds18b20.h"
#include "../../hal/electrical_sensors/max471.h"
#include "../../hal/mpu6050/mpu6050.h"
#include "../../hal/encoder/encoder.h"
#include "../../hal/stepper/A4988.h"
#include "../../../src/config.h"
#include <Arduino.h>
xQueueHandle xDataQueue = xQueueCreate(5, sizeof(MachineData_t));
xQueueHandle get_data_queue()
{
    return xDataQueue;
}
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
    while (1) 
    {
        // 1. Data Acquisition from Queue
        if (xQueueReceive(xDataQueue, &currentReadings, portMAX_DELAY) != pdPASS) {
            continue; // Failed to receive data
        }
        // 2. Pre-processing / Feature Mapping
        // Manually map your sensor data into the TFLite input tensor
        input->data.f[0] = currentReadings.gforce;
        input->data.f[1] = currentReadings.temperature;
        input->data.f[2] = currentReadings.current;
        input->data.f[3] = currentReadings.voltage;
        input->data.f[4] = currentReadings.speed;
        input->data.f[5] = currentReadings.gforce_rms;

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
void vSensorCollectionTask(void *pvParameters) 
{
    MachineData_t sensorData;
    TickType_t xLastWakeTime;
    xSemaphoreHandle encoder_semaphore = get_encoder_semaphore();
    xQueueHandle encoder_queue = get_encoder_queue();
    DS18B20_Simple ds18b20(DS18B20_PIN);
    float previous_rpm = 0.0f;
    MAX471 max471;
    max471.init(MAX471_VOLTAGE_PIN,MAX471_CURRENT_PIN);
    while (1) 
    {
        // Read sensors
        sensorData.gforce = get_total_gforce();
        sensorData.gforce_rms = get_rms_gforce();
        sensorData.temperature = ds18b20.getTemperature();  // Placeholder for actual temperature reading
        sensorData.current =max471.getCurrentRaw(); // Placeholder for actual current reading
        sensorData.voltage = max471.getVoltageRaw(); // Placeholder for actual voltage reading

        sensorData.speed;
        // Read encoder counts
        if (xSemaphoreTake(encoder_semaphore, pdMS_TO_TICKS(100)) == pdTRUE) 
        {
            float rpm;
            xQueueReceive(encoder_queue, &rpm, 0);
            sensorData.speed= rpm;
            previous_rpm = rpm;
        }
        else 
        {
            sensorData.speed = previous_rpm; // No new counts
        }

        // Send to Inference Task
        xQueueSend(xDataQueue, &sensorData, portMAX_DELAY);
        // Sampling rate using vTaskDelayUntil
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(20)); // 50Hz sampling
    }
}