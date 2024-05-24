#ifndef IMU_lib_h
#define IMU_lib_h


#include "Wire.h"
#include "Arduino.h"
struct IMUData {
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;
};
typedef struct IMUData IMUData;
static IMUData IMUCalibrationData1;
static IMUData IMUCalibrationData2;



void InitIMU(uint32_t clkSpeed, uint16_t SDA, uint16_t SCL, uint16_t IMUAddr);


IMUData CalibrateIMU(uint16_t imuAddress);
IMUData ReadIMU(uint16_t imuAddress);
float CalculateAccAngles(IMUData imuData1,IMUData imuData2);
float CalculateGyroAngles(IMUData imuData1,IMUData imuData2, float elapsedTime);
float ComplementaryFilter(IMUData imuData1,IMUData imuData2, float elapsedTime, float gyroWeight);


const float accSensitivity = 8192.0;
const float gyroSensitivity = 131;



#endif
