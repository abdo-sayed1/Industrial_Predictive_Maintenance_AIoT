#ifndef MODEL_DATA_H
#define MODEL_DATA_H
    #include <Arduino.h>
    #include <TensorFlowLite_ESP32.h>
    #include "tensorflow/lite/builtin_ops.h"
    #include "tensorflow/lite/micro/kernels/micro_ops.h"
    #include "tensorflow/lite/micro/micro_error_reporter.h"
    #include "tensorflow/lite/micro/micro_interpreter.h"
    #include "tensorflow/lite/micro/all_ops_resolver.h"
    unsigned char g_model_data[] = 
    {
    // TFLite model bytes go here (from Python conversion)
    // Example: 0x1c, 0x00, 0x00, 0x00, 0x54, 0x46, 0x4c, 0x33...
    };
    
    void setupTFLite();
#endif