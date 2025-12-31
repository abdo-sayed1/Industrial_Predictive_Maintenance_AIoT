#ifndef MAX471_H
#define MAX471_H

#include <Arduino.h>

class MAX471 {
public:
    MAX471();
    
    void init(uint8_t voltagePin, uint8_t currentPin);
    void calibrate();
    
    // Voltage measurement
    float readVoltage();
    float getVoltageRaw();
    
    // Current measurement
    float readCurrent();
    float readCurrentRMS(uint16_t samples = 100);
    float getCurrentRaw();
    
    // Power calculation
    float readPower();
    
    // Calibration
    void setVoltageCalibration(float ratio);
    void setCurrentCalibration(float ratio);
    
private:
    uint8_t _voltagePin;
    uint8_t _currentPin;
    
    float _voltageRatio;
    float _currentRatio;
    float _vref;
    
    uint16_t _voltageZero;
    uint16_t _currentZero;
};

#endif
