#ifndef BUFFER_H
#define BUFFER_H
    #include <Arduino.h>
/*
Always read data, collect it and transmit it every 2 seconds 
use freertos task for this
*/
    #include "../mqtt/mqtt.h"
    #define BUFFER_SIZE 512
    void vbufferTask(void* pvParameters);
#endif