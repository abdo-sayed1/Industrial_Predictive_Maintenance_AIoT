#include "max471.h"
//
MAX471::MAX471() {
    _voltagePin = 0;
    _currentPin = 0;
    _voltageRatio = 5.0;  // MAX471: 25V → 5V (5:1 ratio)
    _currentRatio = 1.0;  // MAX471: 3A → 3V (1:1 ratio)
    _vref = 3.3;          // ESP32/ESP8266 ADC reference
    _voltageZero = 0;
    _currentZero = 0;
}

void MAX471::init(uint8_t voltagePin, uint8_t currentPin) {
    _voltagePin = voltagePin;
    _currentPin = currentPin;
    
    pinMode(_voltagePin, INPUT);
    pinMode(_currentPin, INPUT);
    
    // Auto-calibrate zero points
    calibrate();
}

void MAX471::calibrate() {
    // Calibrate voltage zero (no load)
    uint32_t voltageSum = 0;
    uint32_t currentSum = 0;
    
    for (int i = 0; i < 100; i++) {
        voltageSum += analogRead(_voltagePin);
        currentSum += analogRead(_currentPin);
        delay(5);
    }
    
    _voltageZero = voltageSum / 100;
    _currentZero = currentSum / 100;
}

float MAX471::readVoltage() {
    uint16_t adcValue = analogRead(_voltagePin);
    
    // Convert ADC to voltage
    // ESP32: 12-bit ADC (0-4095) → 0-3.3V
    // ESP8266: 10-bit ADC (0-1023) → 0-3.3V
    
    #ifdef ESP32
        float sensorVoltage = (adcValue / 4095.0) * _vref;
    #else
        float sensorVoltage = (adcValue / 1023.0) * _vref;
    #endif
    
    // MAX471: 25V max → 5V output
    // Actual voltage = sensorVoltage * 5
    float actualVoltage = sensorVoltage * _voltageRatio;
    
    return actualVoltage;
}

float MAX471::getVoltageRaw() {
    return analogRead(_voltagePin);
}

float MAX471::readCurrent() {
    uint16_t adcValue = analogRead(_currentPin);
    
    // Convert ADC to voltage
    #ifdef ESP32
        float sensorVoltage = (adcValue / 4095.0) * _vref;
    #else
        float sensorVoltage = (adcValue / 1023.0) * _vref;
    #endif
    
    // MAX471: 3A max → 3V output (1V per Ampere)
    // Current = sensorVoltage * 1
    float current = sensorVoltage * _currentRatio;
    
    return current;
}

float MAX471::readCurrentRMS(uint16_t samples) {
    float sum = 0;
    
    for (uint16_t i = 0; i < samples; i++) {
        float current = readCurrent();
        sum += current * current;
        delayMicroseconds(100);
    }
    
    return sqrt(sum / samples);
}

float MAX471::getCurrentRaw() {
    return analogRead(_currentPin);
}

float MAX471::readPower() {
    float voltage = readVoltage();
    float current = readCurrent();
    return voltage * current;
}

void MAX471::setVoltageCalibration(float ratio) {
    _voltageRatio = ratio;
}

void MAX471::setCurrentCalibration(float ratio) {
    _currentRatio = ratio;
}
