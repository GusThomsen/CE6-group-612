// Required libraries
//#include <Adafruit_MPU6050.h>   // https://github.com/adafruit/Adafruit_MPU6050/blob/master/Adafruit_MPU6050.h
#include <Adafruit_Sensor.h>  // IMU programmer library
#include <Wire.h>             // I2C communicator for IMUs


// PROGRAM DEFINITIONS
#define PI 3.141592
// Motor codes

// Filtering codes
#define COMPLEMENTARY 1
#define KALMAN 2
// Eror codes
#define IMUS_FAILED 1
#define WRONG_RESOLUTION_MODE 2
#define EXCEEDED_TIME 3

// TUNABLE VARIABLES-------------------------------------------------------------------------------------------------------------------------
#define DEBUG false                // Debug mode: prints information on screen
#define WORKING_ANG 15 * PI / 180  // º From which the controller can stabilize. If exceed, it jumps

#define STOP_IF_TIME_EXCEEDED true     // Stops the system if the time of the loop time exceeds the allowed one (system can't reach desired frequency)
#define FILTERING_MODE COMPLEMENTARY   // Chooses which mode to filter the angle emasurement with the IMU mode
#define LOOP_FREQ 50                   // Frequency of system in Hertzs
#define INIT_ANGLE_GUESS 0 / 180 * PI  // Initial center of mass estimation. Stored in untis
//------------------------------------------------------------------------------------------------------------------

// Timing variables
unsigned long LOOP_MICROS = (unsigned long)(1000000. / LOOP_FREQ);  // microseconds of a loop with the desired frequency
unsigned long last_time;                                            // Variable to keep track of elapsed time

// Kalman filter variables
struct {
  float Q_angle = 0.001 * PI / 180;
  float Q_gyroBias = 0.003 * PI / 180;
  float R_measure = 0.03;
  float dt = 1.0 / LOOP_FREQ;
  float angle = 0;
  float ang_speed = 0;
} kalman_filter;


// PINS DEFINITIONS
#define POT_PIN A1
#define EMG_LED LED_BUILTIN
#define READ_SWITCH_PIN 4
#define POWER_SWITCH_PIN 3






//Initialises the different sensors and actuators
void InitSystem() {


  InitServo();
  TurnOffMotor();
  SetServorAngle(BRAKE_ANG);


  // Switch ------------------------------------
  pinMode(READ_SWITCH_PIN, INPUT);
  pinMode(POWER_SWITCH_PIN, INPUT);
  // Emergency led------------------------------
  pinMode(EMG_LED, OUTPUT);
  digitalWrite(EMG_LED, HIGH);  //Led is low_active


  //Motor----------------------------------------
  InitMotor();

  InitIMUsNew();

  //InitIMU(clkSpeed, sda, scl, imuAddress1);

  //InitIMU(clkSpeed, sda, scl, imuAddress2);


  SetServorAngle(RESET_ANG);
  TurnOnMotor();
}





int Switch() {
  int temp11, temp22;
  temp11 = digitalRead(3);
  temp22 = digitalRead(4);
  if (temp11 == 1 and temp22 == 0) {
    return 1;
  } else if (temp11 == 1 and temp22 == 1) {
    return 2;
  } else if (temp11 == 0 and temp22 == 1) {
    return 3;
  } else {
    return 0;
  }
}




//Non used functions

/*
void ErrorManager(int code) {
  EmergencyStop();
  switch (code) {
    case IMUS_FAILED:
      Serial.println("ERROR: The IMUs could not be initialized corretly.");
      BlinkLed(4, 0, 125);
      break;
    case WRONG_RESOLUTION_MODE:
      Serial.println("ERROR: The selected resolution mode is not valid.");
      BlinkLed(3, 1, 125);
      break;
    case EXCEEDED_TIME:
      BlinkLed(2, 1, 125);
      break;
  }
}


void WaitNextRound() {
  static unsigned long elapsed_time;
  static unsigned long now_time;
  now_time = micros();

  elapsed_time = TimeDiffMicros(now_time, last_time);




  if (elapsed_time > LOOP_MICROS && !STOP_IF_TIME_EXCEEDED) {  // Exceeded time?
    Serial.println("Exceeded time");
    Serial.print("elapsed_time:");
    Serial.print(elapsed_time);
    Serial.print(" us    Max time: ");
    Serial.print(LOOP_MICROS);
    Serial.println(" us");
    ErrorManager(EXCEEDED_TIME);
  }


  //Not exceeded time, wait until its time

  while (elapsed_time < LOOP_MICROS) {

    now_time = micros();
    elapsed_time = TimeDiffMicros(now_time, last_time);
  };
  last_time = micros();
}

unsigned long TimeDiffMicros(unsigned long newest, unsigned long oldest) {  // Returns time diff between two times, corect for overflow
  if (newest > oldest) {                                                    // Time overflow?
    return newest - oldest;
  } else {
    return (unsigned long)4294967295 - (oldest - newest);  // max value - inverse difference
  }
}

*/



