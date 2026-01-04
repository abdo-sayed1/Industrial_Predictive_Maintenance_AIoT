#ifndef I2C_H
#define I2C_H
#include <Arduino.h>
#include <Wire.h>
#if __cplusplus 
extern "C" {
#endif
    void I2C_Init(void);
    // I2C Pin definition for ESP32
    #define I2C_PIN_SDA 21
    #define I2C_PIN_SCL 22
    // I2C Clock Speed
    #define I2C_CLOCK_SPEED 100000 // 100kHz
    void I2C_beginTx(uint8_t address);
    void sI2CTx(char* data);
    void I2C_endTx(void);
    void I2C_beginRx(uint8_t address);
    char I2C_Rx(void);

#if __cplusplus 
}
#endif

#endif