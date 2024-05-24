#include "IMU_lib.h"
#include <Arduino.h>
#include <Wire.h>


void InitIMU(uint32_t clkSpeed, uint16_t SDA, uint16_t SCL, uint16_t IMUAddr) {
  Wire.begin(SDA, SCL);  // Initialize I2C communication
  Wire.setClock(clkSpeed);
  Wire.beginTransmission(IMUAddr);  // Begin transmission to the MPU6050
  Wire.write(0x6B);                 // PWR_MGMT_1 register
  Wire.write(0);                    // Set to 0 to wake up MPU6050
  Wire.endTransmission();
  Wire.beginTransmission(IMUAddr);
  Wire.write(0x1C);  // ACCEL_CONFIG register
  Wire.write(0x00);  // Set the accelerometer to 2g
  Wire.endTransmission();
  Wire.beginTransmission(IMUAddr);
  Wire.write(0x1B);        // GYRO_CONFIG register
  Wire.write(0x00);        // Set to 0x00 to set the gyro to 250 degrees/second
  Wire.endTransmission();  // End transmission
  Wire.beginTransmission(IMUAddr);
  Wire.write(0x1A);   // lowpass filter config register
  Wire.write(0x05); // set to 5Hz
  Wire.endTransmission();
  if (IMUAddr = 0x68) {
    IMUCalibrationData1 = CalibrateIMU(IMUAddr);
  } else {
    IMUCalibrationData2 = CalibrateIMU(IMUAddr);
  }
}
IMUData ReadIMU(uint16_t imuAddress) {
  struct IMUData imuDataStruct;
  int16_t accXTemp, accYTemp, accZTemp = 0;
  int16_t gyroXTemp, gyroYTemp, gyroZTemp = 0;
  int16_t temperature = 0;
  Wire.beginTransmission(imuAddress);  // Begin transmission to the MPU6050
  Wire.write(0x3B);                    // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.requestFrom(imuAddress, 14);    // Request 14 bytes from the MPU6050

  // Print the values
  accXTemp = (Wire.read() << 8 | Wire.read());     // Combine high and low bytes for X
  accYTemp = (Wire.read() << 8 | Wire.read());     // Combine high and low bytes for Y
  accZTemp = (Wire.read() << 8 | Wire.read());     // Combine high and low bytes for Z
  temperature = (Wire.read() << 8 | Wire.read());  // Skip temperature
  gyroXTemp = (Wire.read() << 8 | Wire.read());    // Combine high and low bytes for X
  gyroYTemp = (Wire.read() << 8 | Wire.read());    // Combine high and low bytes for Y
  gyroZTemp = (Wire.read() << 8 | Wire.read());    // Combine high and low bytes for Z

  if (imuAddress == 0x68) {
    // Convert the values to g and degrees/sec
    imuDataStruct.accX = (float)(accXTemp / accSensitivity);
    imuDataStruct.accY = (float)(accYTemp / accSensitivity);
    imuDataStruct.accZ = (float)(accZTemp / accSensitivity);
    imuDataStruct.gyroX = (float)((gyroXTemp - IMUCalibrationData1.gyroX) / gyroSensitivity) * 0.01745329;
    imuDataStruct.gyroY = (float)((gyroYTemp - IMUCalibrationData1.gyroY) / gyroSensitivity) * 0.01745329;
    imuDataStruct.gyroZ = (float)((gyroZTemp - IMUCalibrationData1.gyroZ) / gyroSensitivity) * 0.01745329;
  } else {
    imuDataStruct.accX = (float)(accXTemp / accSensitivity);
    imuDataStruct.accY = (float)(accYTemp / accSensitivity);
    imuDataStruct.accZ = (float)(accZTemp / accSensitivity);
    imuDataStruct.gyroX = (float)((gyroXTemp - IMUCalibrationData2.gyroX) / gyroSensitivity) * 0.01745329;
    imuDataStruct.gyroY = (float)((gyroYTemp - IMUCalibrationData2.gyroY) / gyroSensitivity) * 0.01745329;
    imuDataStruct.gyroZ = (float)((gyroZTemp - IMUCalibrationData2.gyroZ) / gyroSensitivity) * 0.01745329;
  }

Wire.endTransmission();  // End transmission
return imuDataStruct;    // return the values read from IMU
}

IMUData CalibrateIMU(uint16_t imuAddress) {
  IMUData tempStruct;
  IMUData sumStruct;
  for (uint16_t i = 0; i < 1000; i++) {
    tempStruct = ReadIMU(imuAddress);
    sumStruct.gyroX += tempStruct.gyroX;
    sumStruct.gyroY += tempStruct.gyroY;
    sumStruct.gyroZ += tempStruct.gyroZ;
  }
  sumStruct.gyroX = (float)sumStruct.gyroX / 1000;
  sumStruct.gyroY = (float)sumStruct.gyroY / 1000;
  sumStruct.gyroZ = (float)sumStruct.gyroZ / 1000;
  return sumStruct;
}

float CalculateAccAngles(IMUData imuData1, IMUData imuData2) {
  // Calculate angle
  float angle1 = atan(imuData1.accY / imuData1.accX) - 1.570796;  //minus pi/2
  float angle2 = atan(imuData2.accY / imuData2.accX);
  float tempAngle = ((angle1+angle2) / 2) - 0.7853982;  //minus 45deg to get zero at equilibrium
  return tempAngle;
}

float CalculateGyroAngles(IMUData imuData1, IMUData imuData2, float elapsedTime) {
  float gyroAngleChange = ((imuData1.gyroZ + imuData2.gyroZ) / 2) * elapsedTime;
  return gyroAngleChange;
}

float ComplementaryFilter(IMUData imuData1, IMUData imuData2, float elapsedTime, float gyroWeight) {
  float elapsedTimeMillis = elapsedTime * 100;
  static float timeLast;
  static float lastAngle;
  float accAngle = CalculateAccAngles(imuData1, imuData2);
  float gyroAngle = CalculateGyroAngles(imuData1, imuData2, elapsedTime);

  float tempAngle = gyroWeight * (gyroAngle + lastAngle) + (1 - gyroWeight) * accAngle;

  lastAngle = tempAngle;
  return tempAngle;
}
