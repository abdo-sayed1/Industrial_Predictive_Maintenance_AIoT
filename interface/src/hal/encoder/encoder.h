#ifndef ENCODER_H
#define ENCODER_H
/*
    Encoder HAL functions
    WYC H2010
*/
    #include <Arduino.h>
    #include "../../config.h"
    void encoder_setup();
    #define ENCODER_PIN_A ENCODER_A_PIN
    void IRAM_ATTR handleEncoderPulse();
    //void encoderProcessingTask(void *pvParameters);
    xQueueHandle get_encoder_queue();
    xSemaphoreHandle get_encoder_semaphore();
    void vCalculateRPM(void *pvParameters);
#endif