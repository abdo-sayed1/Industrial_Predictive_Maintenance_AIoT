#include "encoder.h"
SemaphoreHandle_t pulseSemaphore;
TaskHandle_t processingTaskHandle = NULL;
volatile unsigned long pulseCount = 0;
void encoder_setup()
{
    // Initialize encoder pins and settings
    pinMode(ENCODER_PIN_A, INPUT);
    // Additional setup code as needed
}
long read_encoder_counts()
{
    // Read the encoder counts from hardware registers or variables
    long counts = 0;
    // Replace with actual reading logic
    counts = digitalRead(ENCODER_PIN_A);

    return counts;
}
void IRAM_ATTR handleEncoderPulse() {
  pulseCount++;
  // Signal the task that a pulse occurred
  xSemaphoreGiveFromISR(pulseSemaphore, NULL);
}