#ifndef MPU6050_H
#define MPU6050_H
#if __cplusplus 
extern "C" {
#endif
    #include <Adafruit_MPU6050.h>
    #include <Adafruit_Sensor.h>
    #include <Wire.h>
    // 
    void mpusetup();
    float getxacceleration();
    float getyacceleration();
    float getzacceleration();
    float getxgyro();
    float getygyro();
    float getzgyro();
    float gettemp();
    
#if __cplusplus 
}
#endif
#endif