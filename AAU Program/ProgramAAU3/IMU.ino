float RateRoll, RatePitch, RateYaw;
float GyroRoll, GyroPitch;
float GyroX_bias = -1.22, GyroY_bias = 1.18, GyroZ_bias = 0.69;

float AccX, AccY, AccZ;
float AngleRoll, AnglePitch;
float AccX_bias = 0.01, AccY_bias = 0, AccZ_bias = -0.11;



#define IMU1_ADDR 0x68  //0x69 for IMU2
void InitIMUsNew() {
  Wire.setClock(400000);  //From MPU6050 product spec sheet
  Wire.begin();
  delay(250);
  //Power up the IMU
  Wire.beginTransmission(IMU1_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  //Enable 5 Hz bandwidth low-pass filter for the small vibrations and keep gyro at 1kHz
  Wire.beginTransmission(IMU1_ADDR);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  //Set the gyro sensitivity scale factor at 250 deg/s
  Wire.beginTransmission(IMU1_ADDR);
  Wire.write(0x1B);
  Wire.write(0x00);
  Wire.endTransmission();
  //Set the accel to +-2g
  Wire.beginTransmission(IMU1_ADDR);
  Wire.write(0x1C);
  Wire.write(0x00);
  Wire.endTransmission();
}


void updateIMUReadings() {

  //Write to the gyro measurement registers
  Wire.beginTransmission(IMU1_ADDR);
  Wire.write(0x43);
  Wire.endTransmission();
  //Request for 6 bytes
  Wire.requestFrom(IMU1_ADDR, 6);
  //Gyro is a 16-bit complement value
  int16_t raw_gyroX = Wire.read() << 8 | Wire.read();  // You have to merge the two gyro registers
  int16_t raw_gyroY = Wire.read() << 8 | Wire.read();
  int16_t raw_gyroZ = Wire.read() << 8 | Wire.read();
  //Convert the gyro values to deg/s
  RateRoll = (float)raw_gyroX / 131 - GyroX_bias;  // 131 is the LSB sensitivity for 250deg/s
  RatePitch = (float)raw_gyroY / 131 - GyroY_bias;
  RateYaw = (float)raw_gyroZ / 131 - GyroZ_bias;


  //Write to the accel measurement registers
  Wire.beginTransmission(IMU1_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission();
  //Request for 6 bytes
  Wire.requestFrom(IMU1_ADDR, 6);
  //Accel is a 16-bit complement value
  int16_t raw_accelX = Wire.read() << 8 | Wire.read();  // You have to merge the two accel registers
  int16_t raw_accelY = Wire.read() << 8 | Wire.read();
  int16_t raw_accelZ = Wire.read() << 8 | Wire.read();
  //Convert the accel values to g
  AccX = (float)raw_accelX / 16384 - AccX_bias;  // 16384 is the LSB sensitivity for 2g
  AccY = (float)raw_accelY / 16384 - AccY_bias;
  AccZ = (float)raw_accelZ / 16384 - AccZ_bias;

  kalman_filter.ang_speed = -RateYaw * PI / 180;
  kalman_filter.angle = atan(AccY / AccX) - 44.5 / 180 * PI;
}



float FrameAngleIMUs() {
  static float compAngle = 0;
  static float kalmanAngle = 0;

  if (FILTERING_MODE == KALMAN) {
    kalmanAngle = kalman(kalman_filter.angle, kalman_filter.ang_speed);
    return kalmanAngle;
  }
  if (FILTERING_MODE == COMPLEMENTARY) {
    compAngle = 0.02 * kalman_filter.angle + 0.98 * (compAngle + kalman_filter.ang_speed * kalman_filter.dt);
    return compAngle;
  }
  Serial.print("Incorrect filtering mode.\n\n\n");
  exit(0);
}

float kalman(float newAngle, float newRate) {
  static float P[2][2] = { { 0, 0 }, { 0, 0 } };
  static float rate = 0;
  static float bias = 0;   // gyro bias calculated by the Kalman filter
  static float angle = 0;  //angle calculated by the Kalman filter
  //equation 1:
  rate = newRate - bias;
  angle += kalman_filter.dt * rate;
  //equation 2:
  P[0][0] += kalman_filter.dt * (kalman_filter.dt * P[1][1] - P[0][1] - P[1][0] + kalman_filter.Q_angle);
  P[0][1] -= kalman_filter.dt * P[1][1];
  P[1][0] -= kalman_filter.dt * P[1][1];
  P[1][1] += kalman_filter.Q_gyroBias * kalman_filter.dt;
  //equation 3:
  float y = newAngle - angle;
  //equation 4:
  float S = P[0][0] + kalman_filter.R_measure;
  //equation 5:
  float K[2];
  K[0] = P[0][0] / S;
  K[1] = P[1][0] / S;
  //equation 6:
  angle += K[0] * y;
  bias += K[1] * y;
  //equation 7:
  float P00_temp = P[0][0];
  float P01_temp = P[0][1];

  P[0][0] -= K[0] * P00_temp;
  P[0][1] -= K[0] * P01_temp;
  P[1][0] -= K[1] * P00_temp;
  P[1][1] -= K[1] * P01_temp;

  return angle;
}
