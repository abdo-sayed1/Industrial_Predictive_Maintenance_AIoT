#include "features.h"
namespace {
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
    const tflite::Model* model = tflite::GetModel(g_model_data);
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
