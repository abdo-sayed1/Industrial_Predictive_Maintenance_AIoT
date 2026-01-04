#ifndef BUFFER_H
#define BUFFER_H
    #include <Arduino.h>
/*
Always read data, collect it and transmit it every 2 seconds 
use freertos task for this
*/
    #include "../mqtt/mqtt.h"
    #include "../../../src/config.h"
    #define BUFFER_SIZE 512
    #include "../../app/inference/model_data.h"
    #include "../../app/features/features.h"
    void vbufferTask(void* pvParameters);
#endif