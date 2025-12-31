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
    
#if __cplusplus 
}
#endif
#endif