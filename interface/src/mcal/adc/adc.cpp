#include "adc.h"
#include <Arduino.h>
static volatile float voltage_value;
static volatile float temperature_value;

void SensorInit(void)
{
  analogSetAttenuation(ADC_0db);
  analogReadResolution(12);
  voltage_value = 0.0;
  temperature_value = 0.0;
  map(ADC, 0, 4095, 0, 3300); // Map ADC values to millivolts
}
float ReadVoltage(void)
{   
    unsigned int raw_adc = analogReadMilliVolts(ADC);
    voltage_value = (float)raw_adc / 1000.0f; // Convert mV to V
    return voltage_value;
}
unsigned int ADC_Read(void)
{
    return analogRead(ADC);
}
