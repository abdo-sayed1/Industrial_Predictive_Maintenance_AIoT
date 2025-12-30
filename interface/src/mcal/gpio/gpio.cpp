#include "gpio.h"
void GPIO_Init(void)
{
    // Initialize GPIO pins if needed

}
void GPIO_SetPinMode(uint8_t pin, GPIO_Mode mode)
{
    if (mode == GPIOINPUT)
    {
        pinMode(pin, INPUT);
    }
    else if (mode == GPIOOUTPUT)
    {
        pinMode(pin, OUTPUT);
    }
}
void GPIO_WritePin(uint8_t pin, bool value)
{
    digitalWrite(pin, value ? HIGH : LOW);
}
bool GPIO_ReadPin(uint8_t pin)
{
    return digitalRead(pin) == HIGH;
}
void GPIO_TogglePin(uint8_t pin)
{
    digitalWrite(pin, !digitalRead(pin));
}
