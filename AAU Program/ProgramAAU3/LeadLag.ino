static bool once = false;
static float* outputArray;
static float* inputArray;
// The controller is designed to work with a sampling rate of 50Hz!!! If changed, alert Gustav.
float LeadLagController(float frameAngle) {

  if (once == false) {
    outputArray = InitSamplingArray(3);
    inputArray = InitSamplingArray(3);
    once = true;
  }


  AdvanceSamplingArray(outputArray, 3);  // Move array entrence 1 place
  AdvanceSamplingArray(inputArray, 3);   // Move array entrence 1 place
  inputArray[0] = frameAngle;            // Update with newest sample

  outputArray[0] = -3.768 * inputArray[0] + 7.107 * inputArray[1] - 3.339 * inputArray[2] + 1.768 * outputArray[1] - 0.7852 * outputArray[2];
  //Serial.println(outputArray[0]);

  static float Kt = 33.5 / 1000;                     // Nm/A
  static float limits = MAX_MOTOR_CURRENT_HIGH_RES;  // A
  static float max_torque = MAX_MOTOR_CURRENT_HIGH_RES * 33.5 / 1000;

  if (abs(outputArray[0]) > max_torque) {
    outputArray[0] = outputArray[0] / abs(outputArray[0]) * max_torque;
  }
  // Compute curernt to apply
  float current = -outputArray[0] / Kt;  //- is to keep consistent with directions
  // Limit to allowable values
  if (abs(current) > limits) {
    current = current / abs(current) * limits;
  }

  // map to duty
  float duty = -(current + limits) * (0.9 - 0.1) / (limits + limits) + 0.1;
  Serial.println(abs(duty));


  return abs(duty);
}


// Samme funktioner som i CM approximation, skal ikke kopieres hvis dette allerede er gjort.
float* InitSamplingArray(int size) {  // Initialize array for sampling
  float* sampArray = new float[size];
  for (int i = 0; i < size; i++) {
    sampArray[i] = 0;
  }
  return sampArray;
}

float* AdvanceSamplingArray(float* sampArray, int size) {  // Advances sampling entry n to n-1
  for (int i = size - 2; i >= 0; i--) {
    sampArray[i + 1] = sampArray[i];
  }
  return sampArray;
}