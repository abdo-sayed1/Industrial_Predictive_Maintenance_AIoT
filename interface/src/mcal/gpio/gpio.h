#ifndef GPIO_H
#define GPIO_H
 #include <Arduino.h>
#if __cplusplus 
extern "C" {
#endif
    typedef enum {
        GPIOINPUT,
        GPIOOUTPUT
    } GPIO_Mode;

    void GPIO_Init(void);
    void GPIO_SetPinMode(uint8_t pin, GPIO_Mode mode);
    void GPIO_WritePin(uint8_t pin, bool value);
    bool GPIO_ReadPin(uint8_t pin);
    void GPIO_TogglePin(uint8_t pin);
#if __cplusplus 
}
#endif
#endif