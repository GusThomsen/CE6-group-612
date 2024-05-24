
void UpdateMeasurements(float* measured_values) {
  // updateIMUReadings();
  float extra = FrameAngleIMUs();


  updateIMUReadings();
  measured_values[0] = getFrameAngle();
  measured_values[1] = kalman_filter.ang_speed;
  measured_values[2] = getWheelSpeed();
}




float potOffset = 0;
float totalAddedAngles = 0;
int samplingSize = 0;

float getFrameAngle() {
  int value = analogRead(POT_PIN);
  float angle = PI / 4.0 * (1 - 2.0 * value / 1024.0);

  if (abs(angle) < 0.0872664626) {

    samplingSize++;
    totalAddedAngles += 0.0 - angle;

    potOffset = totalAddedAngles / samplingSize;
    angle += potOffset;
    return angle;
  } else {
    return angle;
  }
}

float getWheelSpeed() {
  float measure = analogRead(SPEED_PIN);
  return -(measure / 1024 - 0.5) * 12000 * PI / 30;
}



//Under here is unused but could work in theory

/*static int samplingFreq = 50;         // Sampling frequency for the system
static float PIZero = 0.01;           // Integral controller added zero
static float PIkp = 9.5 / 10000;      // Proportional gain for PI controller
float* TustinPI(float z, float kp) {  // Discretize PI controller with the Tustin method
  float* numCoefficientsAfterDiscretization = new float[2];

  numCoefficientsAfterDiscretization[0] = kp * (samplingFreq * z / 2 + 1);
  numCoefficientsAfterDiscretization[1] = kp * (samplingFreq * z / 2 - 1);
  return numCoefficientsAfterDiscretization;
}


float CMAngle(float meassured_w_wheel) {                         // PI controller to approximate centre of mass of the AAU^3
  static float* w_wheel = InitSamplingArray(2);                  // Reaction wheel angular velocity sampling array
  static float* theta_CM = InitSamplingArray(2);                 // Centre of mass approximation array
  static float* numeratorCoefficients = TustinPI(PIZero, PIkp);  // Calculate discrete numerator coefficients

  AdvanceSamplingArray(w_wheel, 2);   // Move array entrence 1 place
  AdvanceSamplingArray(theta_CM, 2);  // Move array entrence 1 place

  w_wheel[0] = meassured_w_wheel;  // Newest sampling at top of array

  theta_CM[0] = w_wheel[0] * numeratorCoefficients[0] + w_wheel[1] * numeratorCoefficients[1] + theta_CM[0];

  return theta_CM[0];
}




float* discretizePI(float z, float kp) {
  float* arr = new float[2];

  arr[0] = kp * (z / 2.0 / LOOP_FREQ + 1);
  arr[1] = kp * (z / 2.0 / LOOP_FREQ - 1);
  return arr;
}


float EstimatorPointFallingTorque(float measured_w) {  // Controller is discretized automatically following tustin's method. It requires a pole (s+z) and a Kp
  static float* w_err = initTimeArray(2);
  static float* theta_cm = initTimeArray(2);
  static float* num_coeffs = discretizePI(0.01, 9.4406 / 10000);
  static float den_coeffs[2] = { 0, 1 };


  advanceTimeArray(theta_cm, 2);
  advanceTimeArray(w_err, 2);


  w_err[0] = measured_w;

  theta_cm[0] = inner_product(w_err, num_coeffs, 2) + inner_product(theta_cm, den_coeffs, 2);

  return theta_cm[0];
}*/