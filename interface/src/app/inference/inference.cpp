#include "inference.h"
#include "model_data.h"
#include "../../hal/ds18b20/ds18b20.h"
#include "../../hal/electrical_sensors/max471.h"
#include "../../hal/mpu6050/mpu6050.h"
#include "../../hal/encoder/encoder.h"
#include "../../hal/stepper/A4988.h"
#include "../../../src/config.h"
#include <Arduino.h>
xQueueHandle xDataQueue = xQueueCreate(20, sizeof(MachineData_t));
xQueueHandle xRawInferenceDataQueue= xQueueCreate(1, sizeof(MachineData_t));
xQueueHandle xRawFeatureDataQueue= xQueueCreate(1, sizeof(MachineData_t));
xQueueHandle xProcessedInferenceQueue= xQueueCreate(1, sizeof(MachineData_t));
xQueueHandle xProcessedFeatureQueue= xQueueCreate(1, sizeof(MachineData_t));
xQueueHandle get_raw_feature_queue()
{
    return xRawFeatureDataQueue;
}
xQueueHandle get_data_queue()
{
    return xDataQueue;
}
static portMUX_TYPE my_mutex = portMUX_INITIALIZER_UNLOCKED;
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
    MachineData_t NormalizedReadings;
    while (1) 
    {
        // 1. Data Acquisition from Queue
        if( xQueueReceive(get_feature_queue(), &NormalizedReadings, portMAX_DELAY) != pdPASS) 
        {
            continue; // Failed to receive data
        }
        // 2. Pre-processing / Feature Mapping
        // Manually map your sensor data into the TFLite input tensor
        input->data.f[0] = NormalizedReadings.gforce;
        input->data.f[1] = NormalizedReadings.temperature;
        input->data.f[2] = NormalizedReadings.current;
        input->data.f[3] = NormalizedReadings.voltage;
        input->data.f[4] = NormalizedReadings.speed;
        input->data.f[5] = NormalizedReadings.gforce_rms;

        // 3. Run Inference
        TfLiteStatus invoke_status = interpreter->Invoke();
        if (invoke_status != kTfLiteOk) {
            TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed!");
            continue;
        }

        // 4. Post-processing
        // Assuming your model output is a probability of failure (Softmax or Sigmoid)
        float failure_probability = output->data.f[0];
        
        NormalizedReadings.isAnomaly = (failure_probability > 0.8f); // Threshold 80%
        NormalizedReadings.faultType = (int)output->data.f[1];      // Example fault classification

        // 5. Send to Queue for the MQTT Task
        xQueueSend(xProcessedInferenceQueue, &NormalizedReadings, 0);
        // Task yield to allow others to run
        vTaskDelay(pdMS_TO_TICKS(50)); 
    }
}

void vSensorCollectionTask(void *pvParameters) 
{
    portENTER_CRITICAL(&my_mutex);
    MachineData_t sensorData;
    TickType_t xLastWakeTime;
    xSemaphoreHandle encoder_semaphore = get_encoder_semaphore();
    xQueueHandle encoder_queue = get_encoder_queue();
    DS18B20_Simple ds18b20(DS18B20_PIN);
    float previous_rpm = 0.0f;
    MAX471 max471;
    max471.init(MAX471_VOLTAGE_PIN,MAX471_CURRENT_PIN);
    portEXIT_CRITICAL(&my_mutex);
    while (1) 
    {
        // Read sensors
        /*
        float getxacceleration();
    float getyacceleration();
    float getzacceleration();
    float getxgyro();
    float getygyro();
    float getzgyro();
        */
       portENTER_CRITICAL(&my_mutex);
        sensorData.a_x =  getxacceleration();
        sensorData.a_y =  getyacceleration();
        sensorData.a_z =  getzacceleration();
        sensorData.g_x = getxgyro();
        sensorData.g_y = getygyro();
        sensorData.g_z = getzgyro();
        sensorData.gforce = get_total_gforce();
        sensorData.gforce_rms = get_rms_gforce();
        sensorData.temperature = ds18b20.getTemperature();  
        sensorData.current =max471.getCurrentRaw(); 
        sensorData.voltage = max471.getVoltageRaw(); 
        portEXIT_CRITICAL(&my_mutex);
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
        xQueueSend(xRawFeatureDataQueue, &sensorData, pdMS_TO_TICKS(10));
        // Send to Data Queue for Buffering/MQTT
        xQueueReceive(xProcessedInferenceQueue, &sensorData, pdMS_TO_TICKS(20)); // Ensure latest data

        xQueueSend(xDataQueue, &sensorData, portMAX_DELAY);
        // Sampling rate using vTaskDelayUntil
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(100)); // 10Hz sampling
    }
}