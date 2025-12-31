#include "mpu6050.h"
Adafruit_MPU6050 mpu;
sensors_event_t a,g,temp;
void mpusetup()
{
    if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
}
float getxacceleration()
{
    mpu.getEvent(&a,&g,&temp);
    return a.acceleration.x;
}
float getyacceleration()
{
    mpu.getEvent(&a,&g,&temp);
    return a.acceleration.y;
}
float getzacceleration()
{
    mpu.getEvent(&a,&g,&temp);
    return a.acceleration.z;
}
float getxgyro()
{
    mpu.getEvent(&a,&g,&temp);
    return g.gyro.x;
}
// one for y and z 
float getygyro()
{
    mpu.getEvent(&a,&g,&temp);
    return g.gyro.y;
}
float getzgyro()
{
    mpu.getEvent(&a,&g,&temp);
    return g.gyro.z;
}
float gettemp()
{
    mpu.getEvent(&a,&g,&temp);
    return temp.temperature;
}
