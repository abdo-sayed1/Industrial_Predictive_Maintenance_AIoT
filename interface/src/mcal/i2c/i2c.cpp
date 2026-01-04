#include "i2c.h"
void I2C_Init(void) {
    Wire.begin(I2C_PIN_SDA, I2C_PIN_SCL);
}
void I2C_beginTx(uint8_t address) {
    Wire.beginTransmission(address);
}
void sI2CTx(char* data)
{
    Wire.printf("%s", data);
}
void I2C_endTx(void) {
    Wire.endTransmission();
}
void I2C_beginRx(uint8_t address) {
    
    Wire.requestFrom(address, (uint8_t)32); // Request 32 bytes from the device
}
char I2C_Rx(void) {
    if (Wire.available()) {
        return Wire.read();
    }
    return -1; // Indicate no data available
}
