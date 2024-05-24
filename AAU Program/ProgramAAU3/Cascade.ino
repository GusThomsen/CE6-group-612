float CascadeController(float* measured_values) {
  static float controller_constants[3] = { -2.14, -0.23, -0.003};  // angle err, speed frame, speed wheel
  static float max_torque = MAX_MOTOR_CURRENT_HIGH_RES * 33.5 / 1000;  // Chooses corrrect max torque. Kt = 0.0335

  float torque_to_apply = -inner_product(measured_values, controller_constants, 3);  // - to keep consistent with the directions
  if (abs(torque_to_apply) > max_torque) {
    torque_to_apply = torque_to_apply / abs(torque_to_apply) * max_torque;
  }


  static float Kt = 33.5 / 1000;                     // Nm/A
  static float limits = MAX_MOTOR_CURRENT_HIGH_RES;  // A

  // Compute curernt to apply
  float current = -torque_to_apply / Kt;  //- is to keep consistent with directions
  // Limit to allowable values
  if (abs(current) > limits) {
    current = current / abs(current) * limits;
  }

  // map to duty
  float duty = (current + limits) * (0.9 - 0.1) / (limits + limits) + 0.1;
  return duty;
}
