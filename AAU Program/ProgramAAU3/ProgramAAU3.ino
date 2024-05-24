
#define INIT_ANGLE_GUESS 0 / 180 * 3.1415  // Initial center of mass estimation. Stored in untis
#define BRAKE_ANG 136  // º
#define RESET_ANG 104  // º


enum SystemState {
  FallenLeft,
  FallenRight,
  CascadeControl,
  StateSpaceControl,
  LeadLagControl
};



SystemState currentState;
SystemState lastState;
float fallenAngleThreshold = 0.610865238;  //35 deg


float measured_values[3] = { 0, 0, 0 };  // Frame Angle, Frame Speed, Wheel Speed
float duty = 0;
float stable_point = INIT_ANGLE_GUESS;

float timer1;
float timer2;

float gns_theta_r = 0;
void setup() {

  Serial.begin(115200);
  // Initialize the neded stuff
  InitSystem();

  //Make sure wheel isnt spinning
  TurnOffMotor();
  SetServorAngle(BRAKE_ANG);
  delay(300);
  SetServorAngle(RESET_ANG);
  TurnOnMotor();
}

void loop() {

  timer1 = micros();

  if (timer1 >= timer2 + 2000) {


    UpdateMeasurements(measured_values);
    DetectState();
    SwitchState();

    if (lastState != currentState) {
      lastState = currentState;
    }
    timer2 = micros();
  }
}





//Detects what state the system should be in.
//This is based on the angle of the frame and the state of the switch.
void DetectState() {

  int ControllerChoice = Switch();

  float angle = getFrameAngle();

  if (angle > fallenAngleThreshold) {


    currentState = FallenRight;
  }

  else if (angle < -fallenAngleThreshold) {


    currentState = FallenLeft;
  } else {


    switch (ControllerChoice) {
      case 1:
        //Cascade
        currentState = CascadeControl;
        break;

      case 2:
        //statespace
        currentState = StateSpaceControl;
        break;

      case 3:
        //leadlag
        currentState = LeadLagControl;
        break;

      default:
        break;
    }
  }
}






//Tells the system what to do in each state.
void SwitchState() {

  switch (currentState) {
    case FallenLeft:

      if (lastState != currentState) {
        SlowDownMotor();
      }

      PerformJump();
      break;

    case FallenRight:


      if (lastState != currentState) {
        SlowDownMotor();
      }

      PerformJump();
      break;

    case CascadeControl:

      duty = CascadeController(measured_values);
      setAAU(5, (int)floor(duty * 1000));
      break;

    case StateSpaceControl:
      duty = StateSpaceController(measured_values);
      setAAU(5, (int)floor(duty * 1000));

      break;

    case LeadLagControl:
      duty = LeadLagController(measured_values[0]);
      setAAU(5, (int)floor(duty * 1000));
      break;

    default:
      break;
  }
}
