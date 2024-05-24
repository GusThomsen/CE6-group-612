#include <SAMD21turboPWM.h>  // PWM generator for high resolution motor
#include <Servo.h>           // Library for directly controlling servo

#define SPEED_PIN A5
#define SERVO_PIN 2
#define WRONG_RESOLUTION_MODE 2
#define RESOLUTION_MODE HIGH_RES  // Can be 1 (HIGH_RES-7.5A) or 2(LOW_RES-4A). The driver limits must be changed as well (if incorrect mode is chosen, nothing will blow up but it wont work nicely)
#define HIGH_RES 1
#define LOW_RES 2
struct motor_pins {
  int PWM_PIN = 5;     // PWM output pin
  int ENB_PIN = 1;     // ENABLE pin
} MOTOR_LOW_RES_DATA;  // Motor pin layout
#define MAX_MOTOR_CURRENT 4
#define MAX_MOTOR_CURRENT_HIGH_RES 7.5
Servo servo;


// High resolution motor stuff
struct timerStruct {
  const int enbPin = 1;        // Pin used for enabling motor (HIGH active)
  const int PWMPin = 5;        // Pin used for PWM signal
  const int TIMR = 0;          // Specific timer needed for the desired pin (needed by used library PWM.h)
  const int CLKDiv_1 = 15;     // Dividers needed for frequency specification :
  const int CLKDiv_2 = 15;     // PWM frequency is 3.2kHz
  const int aauMinCyc = 0;     //  (duty cycle: 0) This and next one needed for definition of duty cycle, do not touch
  const int aauMaxCyc = 1000;  //  (duty cycle: 1000*20)
  TurboPWM aau;                // Object from PWM.h
} MOTOR_HIGH_RES_DATA;






void InitMotor() {
  //Set enable pin
  pinMode(MOTOR_LOW_RES_DATA.ENB_PIN, OUTPUT);
  TurnOffMotor();
  // Set PWM pins
  pinMode(MOTOR_LOW_RES_DATA.PWM_PIN, OUTPUT);
  pinMode(SPEED_PIN, INPUT);
  // Set PWM data
  MOTOR_HIGH_RES_DATA.aau.setClockDivider(MOTOR_HIGH_RES_DATA.CLKDiv_1, false);                       // clock is divided by 15. Turbo is off/false
  MOTOR_HIGH_RES_DATA.aau.timer(MOTOR_HIGH_RES_DATA.TIMR, MOTOR_HIGH_RES_DATA.CLKDiv_2, 1000, true);  // Timer 0 is set to Generic Clock divided by 15, resolution is 1000, single-slope PWM
  setAAU(5,500);
  delay(6000);
  
  TurnOnMotor();
}


void SlowDownMotor() {
  while (abs(getWheelSpeed()) > 10) {
    if (getWheelSpeed() < 0) {
      setAAU(5, 380);
    } else {
      setAAU(5, 620);
    }
  }
  lastState = currentState;
  SetServorAngle(BRAKE_ANG);
  TurnOffMotor();


  delay(500);
}


void TurnOffMotor() {
  digitalWrite(MOTOR_HIGH_RES_DATA.enbPin, LOW);
}


void TurnOnMotor() {
  digitalWrite(MOTOR_HIGH_RES_DATA.enbPin, HIGH);
}


void InitServo() {
  servo.attach(SERVO_PIN);
  servo.write(RESET_ANG);
}






void SetServorAngle(float angle)
{
  servo.write(angle);
}


void setAAU(const int pin, int cycle) {

  TurnOnMotor();
  if (cycle < MOTOR_HIGH_RES_DATA.aauMinCyc) {
    cycle = MOTOR_HIGH_RES_DATA.aauMinCyc;
  }
  if (cycle > MOTOR_HIGH_RES_DATA.aauMaxCyc) {
    cycle = MOTOR_HIGH_RES_DATA.aauMaxCyc;
  }


  //Serial.println(cycle);
  MOTOR_HIGH_RES_DATA.aau.analogWrite(pin, cycle);

  //Serial.println(MOTOR_HIGH_RES_DATA.aau.analogWrite(pin, cycle));
}
