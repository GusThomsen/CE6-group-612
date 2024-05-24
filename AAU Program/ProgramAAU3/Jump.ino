void PerformJump() {
  
  SetServorAngle(RESET_ANG);
  TurnOnMotor();  
  float wheel_speed = getWheelSpeed();

  if (abs(wheel_speed) < 2) {
  }
  // Set the motor direction based on the fallen side
  if (currentState == FallenLeft) {  // If fallen to the left
    setAAU(5, 380);
  } else if (currentState == FallenRight) {  // If fallen to the right
    setAAU(5, 620);
  }
  if (abs(getWheelSpeed()) > 160.0) {
    SetServorAngle(BRAKE_ANG);
    TurnOffMotor();
    setAAU(5, 500);  // Braking action/
    delay(200);
    SetServorAngle(RESET_ANG);
    TurnOnMotor();
  }
}
