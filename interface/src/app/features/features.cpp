#include "features.h"

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

void setupTFLite() 
{
    // Set up logging
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;
    
    // Load model
    const tflite::Model* model = tflite::GetModel(fault_model);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        Serial.println("Model schema mismatch!");
        return;
    }
    
    // Load operations resolver
    // Micro Op Resolver

    static tflite::AllOpsResolver resolver;

    // Build interpreter
    static tflite::MicroInterpreter static_interpreter(model,resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;
    
    // Allocate tensors
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        Serial.println("Tensor allocation failed!");
        return;
    }
    
    // Get input/output tensors
    input = interpreter->input(0);
    output = interpreter->output(0);
    
    // Print tensor info
    Serial.println("Input tensor dims: " + String(input->dims->size));
    Serial.println("Output tensor dims: " + String(output->dims->size));
}
// Normalization Constants (Example values based on sensor specs)
const float VIB_MAX = 5.0f;  // Max G-force total 
const float TEMP_MAX = 120.0f; // Max Celsius
const float CURR_MAX = 20.0f;  // Max Amps

// Handle for a queue to pass features to Inference
QueueHandle_t xFeatureQueue= xQueueCreate(1, sizeof(MachineData_t));
xQueueHandle get_feature_queue()
{
    return xFeatureQueue;
}
// Helper to normalize raw data to 0.0 - 1.0 range
float normalize(float value, float max_val) 
{
    float norm = value / max_val;
    if (norm > 1.0f) return 1.0f;
    if (norm < 0.0f) return 0.0f;
    return norm;
}

void vFeaturesTask(void *pvParameters) 
{
    MachineData_t rawData;
    MachineData_t processedFeatures;
    while (1) {
        // 1. High-frequency Sampling
        // Vibration often needs higher sampling rates than temperature
        // get data from queue and process
        if (xQueueReceive(get_raw_feature_queue(), &rawData, portMAX_DELAY) != pdPASS) 
        {
            continue; // Failed to receive data
        }
        // 2. Feature Engineering / Normalization
        // ML models perform poorly if one input is 0.5 and another is 220.0
        processedFeatures.gforce = normalize(rawData.gforce, VIB_MAX);
        processedFeatures.gforce_rms = normalize(rawData.gforce_rms, VIB_MAX);
        processedFeatures.temperature = normalize(rawData.temperature, TEMP_MAX);
        processedFeatures.current = normalize(rawData.current, CURR_MAX);
        processedFeatures.voltage = normalize(rawData.voltage, 20.0f); // Assuming 240V mains
        processedFeatures.speed = normalize(rawData.speed,100.0f); // RPM can be used as-is or normalized if needed
        // 4. Send processed features to the Inference Task
        // We wait up to 10 ticks if the queue is full
        if (xQueueSend(xFeatureQueue, &processedFeatures,pdMS_TO_TICKS(100)) != pdPASS) 
        {
            // Handle buffer overflow (e.g., increment an error counter)
        }
        // Match the sampling rate of your model (e.g., 20Hz = 50ms)
    }
}