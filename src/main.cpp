#include <AccelStepper.h>

#define NUM_STEPPERS 2

#define STEPPER0_DIR_PIN 5
#define STEPPER0_STEP_PIN 4

#define STEPPER1_DIR_PIN 8
#define STEPPER1_STEP_PIN 7

const long STEPS_PER_MM = 45;

const char  eol = '\n';
unsigned long timer;

boolean stepperRunning[NUM_STEPPERS] = {false, false};

// wrap the motors in an AccelStepper object array
AccelStepper steppers[NUM_STEPPERS] = {
  AccelStepper(AccelStepper::DRIVER, STEPPER0_STEP_PIN, STEPPER0_DIR_PIN),
  AccelStepper(AccelStepper::DRIVER, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN)
};

void setup() {  
    Serial.begin(9600);
    Serial.println("Started");
}

void loop() {

  if (millis() > timer + 300) {
    timer = millis();
    String result = String(-(steppers[0].currentPosition() / STEPS_PER_MM)) + " " + String(-(steppers[1].currentPosition() / STEPS_PER_MM));
    Serial.println(result);
  }

  for( int i=0; i <  NUM_STEPPERS; i++) {
    if (steppers[i].distanceToGo() == 0 && stepperRunning[i]) {
      stepperRunning[i] = false;
    } else {
      steppers[i].run();
    }
  }
  
  if (Serial.available() > 0) {
    int stepperID = Serial.parseInt();
    String stepperIDstring = String(stepperID);
    int distance = Serial.parseInt();
    float speed = Serial.parseFloat();
    float acceleration = Serial.parseFloat();
    if (Serial.read() == eol) {
      acceleration = max(acceleration,1);
      if (stepperID >= 0 && stepperID < NUM_STEPPERS) {
        if (speed > 0) {
          steppers[stepperID].setMaxSpeed(speed * STEPS_PER_MM);
          steppers[stepperID].setAcceleration(acceleration * STEPS_PER_MM);
          steppers[stepperID].moveTo(-(distance * STEPS_PER_MM));
          stepperRunning[stepperID] = true;
        } else {
          steppers[stepperID].setCurrentPosition(steppers[stepperID].currentPosition());
          steppers[stepperID].moveTo(steppers[stepperID].currentPosition());
          stepperRunning[stepperID] = false;
        }
      } else if (stepperID == 10) {
        for( int i=0; i <  NUM_STEPPERS; i++) {
          if (speed > 0) {
          steppers[i].setMaxSpeed(speed * STEPS_PER_MM);
          steppers[i].setAcceleration(acceleration * STEPS_PER_MM);
          steppers[i].moveTo(-(distance * STEPS_PER_MM));
          stepperRunning[i] = true;
        } else {
          steppers[i].setCurrentPosition(steppers[i].currentPosition());
          steppers[i].moveTo(steppers[i].currentPosition());
        }
        }
      }
    }
  }
}