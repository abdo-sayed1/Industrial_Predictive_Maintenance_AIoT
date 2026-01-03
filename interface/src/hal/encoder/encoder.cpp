#include "encoder.h"
SemaphoreHandle_t pulseSemaphore;
TaskHandle_t processingTaskHandle = NULL;
volatile unsigned long pulseCount = 0;
xQueueHandle encoder_data_queue;
void encoder_setup()
{
    // Initialize encoder pins and settings
    pinMode(ENCODER_PIN_A, INPUT);
    // Additional setup code as needed
    pulseSemaphore = xSemaphoreCreateBinary();
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), handleEncoderPulse, RISING);    
    encoder_data_queue = xQueueCreate(1, sizeof(unsigned long));
    xTaskCreate(encoderProcessingTask, "Encoder Processing Task", 1<<8, NULL, 2, &processingTaskHandle);
}
long read_encoder_counts()
{
    // Read the encoder counts from hardware registers or variables
    long counts = 0;
    // Replace with actual reading logic
    counts = digitalRead(ENCODER_PIN_A);
    return counts;
}
void IRAM_ATTR handleEncoderPulse() 
{
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > 2) { // 2ms debounce
    pulseCount++;
    }
    last_interrupt_time = interrupt_time;
  // Signal the task that a pulse occurred
  xSemaphoreGiveFromISR(pulseSemaphore, NULL);
}
void encoderProcessingTask(void *pvParameters) 
{
  unsigned long lastCount = 0;
  while (1) 
  {
    // Wait for the semaphore (block for up to 100ms)
    if (xSemaphoreTake(pulseSemaphore, pdMS_TO_TICKS(100)) == pdTRUE) 
    {
      // You could do per-pulse logic here if needed
      lastCount = pulseCount;
      xQueueSend(encoder_data_queue, &lastCount,pdMS_TO_TICKS(50));
    }
  }
}
xQueueHandle get_encoder_queue()
{
    return encoder_data_queue;
}
