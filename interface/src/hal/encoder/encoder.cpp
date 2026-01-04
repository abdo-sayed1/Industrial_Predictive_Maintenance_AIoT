#include "encoder.h"
SemaphoreHandle_t pulseSemaphore;
TaskHandle_t processingTaskHandle = NULL;
volatile unsigned long pulseCount = 0;
xQueueHandle encoder_data_queue;
xSemaphoreHandle get_encoder_semaphore()
{
    return pulseSemaphore;
}
void encoder_setup()
{
    // Initialize encoder pins and settings
    pinMode(ENCODER_PIN_A, INPUT);
    // Additional setup code as needed
    pulseSemaphore = xSemaphoreCreateBinary();
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), handleEncoderPulse, RISING);    
    encoder_data_queue = xQueueCreate(1, sizeof(float));
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
void vCalculateRPM(void *pvParameters) 
{
    unsigned long lastCount = 0;
    const TickType_t xDelay = pdMS_TO_TICKS(50); // Hz update rate
    while (1) 
    {
        vTaskDelay(xDelay);
        xSemaphoreTake(pulseSemaphore, portMAX_DELAY);
        unsigned long currentCount = pulseCount;
        unsigned long deltaCount = currentCount - lastCount;
        float rpm = (deltaCount / (float)STEPS_PER_REVOLUTION) * 60.0f; // Convert to RPM
        xQueueSend(encoder_data_queue, &rpm, 0);
        lastCount = currentCount;
        // You can send rpm to a queue or process it further
    }
}
xQueueHandle get_encoder_queue()
{
    return encoder_data_queue;
}
