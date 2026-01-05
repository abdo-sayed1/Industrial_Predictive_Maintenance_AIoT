#include "A4988.h"
void MotorRunningTask(void *pvParameters) 
{
    A4988* stepper = (A4988*)pvParameters;
    stepper->begin();
    stepper->enable(true); // Enable the motor
    while (1) 
    {
        // Example: Rotate 200 steps clockwise with 1000us delay between steps
        stepper->setDirection(CLOCKWISE);
        stepper->step(200, 1000);
        vTaskDelay(pdMS_TO_TICKS(500)); // Wait for 500ms

        // Rotate 200 steps anti-clockwise
        stepper->setDirection(ANTI_CLOCKWISE);
        stepper->step(200, 1000);
        vTaskDelay(pdMS_TO_TICKS(500)); // Wait for 500ms
    }
}
