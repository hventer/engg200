#include <Servo.h>  //This comes with the Arduino
#define armOutputA 7 //motorControler outputA
#define armOutputB 8 //motorControler outputB
#define motorPinArm 11 //pin for arm motor
#define motorPinRoller 12 //pin for roller motor
#define ARMPOSMAX 200 //max distance that arm can extend
#define sensorPin 4 //IR sensor to detect can on shelf
#define sensorPin1 5 //IR sensor to detect can dropping into robot

#define buttonPinExtend 2 //testing button

Servo MotorArm;
Servo MotorRoller;

int motorArmPos = 0; //starting position of the motorArm
int armOutputACurState; //current state of outputA
int armOutputAPrevState; //previous state of outputA

int sensorState = 0; //current sensor state
int prevSensorState = 0; //previous sensor state

int buttonStateExtend = 0; // variable for reading the pushbutton status

int MotorMaxForward = 2000; //this might not actually be forward
int MotorMaxReverse = 1000; //this might not actually be reverse
int MotorStop = 1500;       //this is definitely stop (if the Motor is calibrated right)

int state = 0; //which case of the switch statement we are in
int cans = 0; //number of cans to collect

void setup() {
  MotorArm.attach(motorPinArm, 1000, 2000);
  MotorArm.writeMicroseconds(MotorStop); //make sure the motor is stopped initially

  MotorRoller.attach(motorPinRoller, 1000, 2000);
  MotorRoller.writeMicroseconds(MotorStop); //make sure the motor is stopped initially

  pinMode(buttonPinExtend, INPUT_PULLUP);

  pinMode (armOutputA, INPUT);
  pinMode (armOutputB, INPUT);

  pinMode(sensorPin, INPUT_PULLUP);

  Serial.begin(9600);

  // Reads the initial state of the motorArmOutputA
  armOutputAPrevState = digitalRead(armOutputA);
}

void loop() {
  buttonStateExtend = digitalRead(buttonPinExtend);
  sensorState = digitalRead(sensorPin);

  collectCans(3);

  if (buttonStateExtend == LOW && state == 0) { //HIGH BUTTON STATE MEANS PRESSED
    state = 1;
  }

  if (!sensorState && state == 1) {
    state = 2;
  }

  if (buttonStateExtend == LOW && state == 2) { //HIGH BUTTON STATE MEANS PRESSED
    state = 3;
    delay(1000);
  }

  if (buttonStateExtend == LOW && state == 3) { //HIGH BUTTON STATE MEANS PRESSED
    //state = 0;
    //delay(1000);
  }

}


void collectCans(int cans) {
  switch (state) {


    case 0: //both motors off
      MotorArm.writeMicroseconds(MotorStop);
      MotorRoller.writeMicroseconds(MotorStop);
      break;


    case 1: //arm extend && rollers on
      armOutputACurState = digitalRead(armOutputA); // Reads the "current" state of the armOutputA

      // If the previous and the current state of the armOutputA are different, that means a Pulse has occured
      if (armOutputACurState && !armOutputAPrevState) {

        // If the armOutputB state is different to the armOutputA state, that means the encoder is rotating clockwise
        if (!digitalRead(armOutputB)) {
          motorArmPos ++;
        } else { //anticlockwise
          motorArmPos --;
        }
        Serial.print("Position: ");
        Serial.println(motorArmPos);
      }
      armOutputAPrevState = armOutputACurState; //Updates the previous state of the armOutputA with the current state

      //If motors have reached the back wall, go to state 3
      if (motorArmPos >= ARMPOSMAX)
        state = 3;

      MotorArm.writeMicroseconds(MotorMaxForward);
      MotorRoller.writeMicroseconds(MotorMaxForward);

      prevSensorState = sensorState;
      /*Serial.print("Sensor State: ");
        Serial.println(sensorState);*/
      break;


    case 2: //arm off && rollers on
      MotorArm.writeMicroseconds(MotorStop);
      MotorRoller.writeMicroseconds(MotorMaxForward);
      break;


    case 3: //arm retract && roller off
      armOutputACurState = digitalRead(armOutputA); // Reads the "current" state of the armOutputA

      // If the previous and the current state of the armOutputA are different, that means a Pulse has occured
      if (armOutputACurState && !armOutputAPrevState) {

        // If the armOutputB state is different to the armOutputA state, that means the encoder is rotating clockwise
        if (!digitalRead(armOutputB)) {
          motorArmPos ++;
        } else {
          motorArmPos --;
        }
        Serial.print("Position: ");
        Serial.println(motorArmPos);
      }
      armOutputAPrevState = armOutputACurState; // Updates the previous state of the outputA with the current state

      if (motorArmPos <= 0)
        state = 0;

      MotorArm.writeMicroseconds(MotorMaxReverse);
      MotorRoller.writeMicroseconds(MotorStop);
      break;
  }
}

void checkArmEncoder() {
  armOutputACurState = digitalRead(armOutputA); // Reads the "current" state of the armOutputA

  // If the previous and the current state of the armOutputA are different, that means a Pulse has occured
  if (armOutputACurState && !armOutputAPrevState) {

    // If the armOutputB state is different to the armOutputA state, that means the encoder is rotating clockwise
    if (!digitalRead(armOutputB)) {
      motorArmPos ++;
    } else { //anticlockwise
      motorArmPos --;
    }
    Serial.print("Position: ");
    Serial.println(motorArmPos);
  }
  armOutputAPrevState = armOutputACurState; //Updates the previous state of the armOutputA with the current state
}
