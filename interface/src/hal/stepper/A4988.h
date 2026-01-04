#ifndef A4988_H
#define A4988_H

#define CLOCKWISE true
#define ANTI_CLOCKWISE false 
#include <Arduino.h>

class A4988 
{
  private:
    uint8_t stepPin, dirPin, enablePin;
    long currentStep;
    
  public:
    A4988(uint8_t step, uint8_t dir, uint8_t enable = 255) : 
      stepPin(step), dirPin(dir), enablePin(enable), currentStep(0) {}

    void begin() {
      pinMode(stepPin, OUTPUT);
      pinMode(dirPin, OUTPUT);
      if (enablePin != 255) {
        pinMode(enablePin, OUTPUT);
        digitalWrite(enablePin, LOW); // Default to enabled
      }
    }

    void setDirection(bool clockwise) {
      digitalWrite(dirPin, clockwise ? HIGH : LOW);
    }

    // Move a specific number of steps with a delay (in microseconds)
    void step(int steps, uint32_t stepDelayUs = 1000) {
      for (int i = 0; i < steps; i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(stepDelayUs / 2);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(stepDelayUs / 2);
        currentStep += (digitalRead(dirPin) == HIGH) ? 1 : -1;
      }
    }

    void enable(bool state) {
      if (enablePin != 255) digitalWrite(enablePin, state ? LOW : HIGH);
    }
};
#endif
