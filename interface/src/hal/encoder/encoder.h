#ifndef ENCODER_H
#define ENCODER_H
/*
    Encoder HAL functions
    WYC H2010
*/
    #include <Arduino.h>
    void encoder_setup();
    long read_encoder_counts();
    #define ENCODER_PIN_A 18
    void IRAM_ATTR handleEncoderPulse();
    void encoderProcessingTask(void *pvParameters);
    xQueueHandle get_encoder_queue();
#endif