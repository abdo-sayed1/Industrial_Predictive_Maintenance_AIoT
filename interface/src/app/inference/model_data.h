#ifndef MODEL_DATA_H
#define MODEL_DATA_H
    #include <Arduino.h>
    #include <TensorFlowLite_ESP32.h>
    #include "tensorflow/lite/builtin_ops.h"
    #include "tensorflow/lite/micro/kernels/micro_ops.h"
    #include "tensorflow/lite/micro/micro_error_reporter.h"
    #include "tensorflow/lite/micro/micro_interpreter.h"
    #include "tensorflow/lite/micro/all_ops_resolver.h"
    #include "../../../src/config.h"
    #include "../model/model.h"
    struct MachineData_s 
    {
        float gforce;
        float gforce_rms;
        float temperature;
        float current;
        float voltage;
        float speed;
        bool isAnomaly;
        float healthScore;
        int faultType;
    };
    typedef struct MachineData_s MachineData_t;
    void setupTFLite();
    xQueueHandle get_data_queue();    
    xQueueHandle get_feature_queue();
    xQueueHandle get_inference_queue();
    xQueueHandle get_raw_feature_queue();
#endif