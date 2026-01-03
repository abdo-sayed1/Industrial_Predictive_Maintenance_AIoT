#include "encoder.h"
SemaphoreHandle_t pulseSemaphore;
TaskHandle_t processingTaskHandle = NULL;
volatile unsigned long pulseCount = 0;
void encoder_setup()
{
    // Initialize encoder pins and settings
    pinMode(ENCODER_PIN_A, INPUT);
    // Additional setup code as needed
    pulseSemaphore = xSemaphoreCreateBinary();
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), handleEncoderPulse, RISING);    
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
  pulseCount++;
  // Signal the task that a pulse occurred
  xSemaphoreGiveFromISR(pulseSemaphore, NULL);
}

/*
void encoderProcessingTask(void *pvParameters) {
  unsigned long lastReportTime = 0;
  unsigned long lastCount = 0;

  for (;;) {
    // Wait for the semaphore (block for up to 100ms)
    if (xSemaphoreTake(pulseSemaphore, pdMS_TO_TICKS(100)) == pdTRUE) 
    {
      // You could do per-pulse logic here if needed
    }

    // Report RPM or Count every 1 second
    if (millis() - lastReportTime > 1000) {
      unsigned long currentCount = pulseCount;
      unsigned long pulsesPerSecond = currentCount - lastCount;
      
      Serial.printf("Total Pulses: %lu | Pulses/Sec: %lu\n", currentCount, pulsesPerSecond);
      
      lastCount = currentCount;
      lastReportTime = millis();
    }
  }
}
*/