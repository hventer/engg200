#include <Servo.h>  //This comes with the Arduino

#define armOutputA 7 //motorControler outputA green
#define armOutputB 8 //motorControler outputB yellow
#define motorPinArm 11 //pin for arm motor
#define motorPinRoller 12 //pin for roller motor
#define sensorPinGrab 4 //IR sensor to detect can on shelf
#define sensorPinCount 5 //IR sensor to detect can dropping into robot
#define magnetPin 13 //pin for electromagnet
#define servoUnloadPin 9 //pin for unload servo

#define buttonPinExtend 2 //testing button
#define buttonPin 3

#define ARMPOSMAX 400 //max distance that arm can extend

Servo MotorArm;
Servo MotorRoller;
Servo ServoUnload;

int motorArmPos = 0; //starting position of the motorArm
int armOutputACurState; //current state of outputA
int armOutputAPrevState; //previous state of outputA

int sensorGrabState = 0; //current arm sensor state
int sensorCountState = 0; //current count sensor state

int buttonStateExtend = 0; // variable for reading the pushbutton status
int buttonState = 0;

int MotorMaxForward = 2000; //this might not actually be forward
int MotorMaxReverse = 1000; //this might not actually be reverse
int MotorStop = 1500;       //this is definitely stop (if the Motor is calibrated right)

int armState = 0; //which case of the switch statement we are in
int cansToCollect = 2; //number of cans to collect

void setup() {
  MotorArm.attach(motorPinArm, 1000, 2000);
  MotorArm.writeMicroseconds(MotorStop); //make sure the motor is stopped initially

  MotorRoller.attach(motorPinRoller, 1000, 2000);
  MotorRoller.writeMicroseconds(MotorStop); //make sure the motor is stopped initially

  pinMode(buttonPinExtend, INPUT);
  pinMode(buttonPin, INPUT);

  pinMode (armOutputA, INPUT);
  pinMode (armOutputB, INPUT);

  pinMode(sensorPinGrab, INPUT_PULLUP);
  pinMode(sensorPinCount, INPUT_PULLUP);

  pinMode(magnetPin, OUTPUT);

  Serial.begin(9600);

  // Reads the initial state of the motorArmOutputA
  armOutputAPrevState = digitalRead(armOutputA);
}

void loop() {
  collectCans();
}


void collectCans() {
  buttonStateExtend = digitalRead(buttonPinExtend);
  buttonState = digitalRead(buttonPin);
  sensorGrabState = digitalRead(sensorPinGrab);
  sensorCountState = digitalRead(sensorPinCount);

  if (buttonStateExtend == HIGH) //replace this with if(cansToCollect > 0 && armState == 0)
    armState = 1;

  grab();
}

void grab() {
  switch (armState) {

    case 0: //not collecting alright to drive
      MotorArm.writeMicroseconds(MotorStop);
      MotorRoller.writeMicroseconds(MotorStop);
      magnetOff();
      break;

    case 1: //arm extend && rollers on
      checkArmEncoder();

      //If motors have reached the back wall, go to armState 4
      if (motorArmPos >= ARMPOSMAX)
        armState = 4;

      armExtend();
      rollerOn();

      if (canInHand())
        armState = 2;

      break;

    case 2:
      MotorArm.writeMicroseconds(MotorStop);
      rollerOn();
      magnetOn();

      delay(1000);
      armState = 3;
      break;

    case 3: //arm retract && roller on && magnet on
      checkArmEncoder();
      //If motors have reached the home (startPoint), go to armState 0
      if (motorArmPos <= 0)
        armState = 5;

      if (!canOverHole()) { //while its still retracting with can
        armRetract();
        rollerOn();
        magnetOn();
      }  else {
        MotorRoller.writeMicroseconds(MotorStop);
        magnetOff();
      }
      break;

    case 4: //reached back wall, no cans
      checkArmEncoder();
      if (motorArmPos <= 0) {
        armState = 0;
      }
      armRetract();
      MotorRoller.writeMicroseconds(MotorStop);
      magnetOff();

      break;

    case 5: //everything off, check if need to get more cans
      cansToCollect--;
      MotorArm.writeMicroseconds(MotorStop);
      MotorRoller.writeMicroseconds(MotorStop);
      magnetOff();

      if (cansToCollect > 0)
        armState = 1;
      else
        armState = 0;
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

boolean canInHand() {
  if (!sensorGrabState)
    return true;
  else
    return false;
}

boolean canOverHole() {
  if (buttonState == HIGH) //FIXME: insert sensor here when integrating code instead f button
    return true;
  else
    return false;
  /*
    if (!sensorCountState)
      return true;
    else
      return false;
  */
}

void armExtend() {
  MotorArm.writeMicroseconds(2000);

  // for (int i = 1500; i <= 2000; i += 10)
  //  MotorArm.writeMicroseconds(i);
  /*
     MotorArm.writeMicroseconds(1550);
     MotorArm.writeMicroseconds(1600);
     MotorArm.writeMicroseconds(1650);
     MotorArm.writeMicroseconds(1700);
     MotorArm.writeMicroseconds(1750);
     MotorArm.writeMicroseconds(1800);
     MotorArm.writeMicroseconds(1850);
     MotorArm.writeMicroseconds(1900);
     MotorArm.writeMicroseconds(1950);
     MotorArm.writeMicroseconds(2000);
  */
}

void armRetract() {
  MotorArm.writeMicroseconds(1000);

  //for (int i = 1500; i >= 1000; i -= 10)
  //  MotorArm.writeMicroseconds(i);
  /*
    MotorArm.writeMicroseconds(1450);
    MotorArm.writeMicroseconds(1400);
    MotorArm.writeMicroseconds(1350);
    MotorArm.writeMicroseconds(1300);
    MotorArm.writeMicroseconds(1250);
    MotorArm.writeMicroseconds(1200);
    MotorArm.writeMicroseconds(1150);
    MotorArm.writeMicroseconds(1100);
    MotorArm.writeMicroseconds(1050);
    MotorArm.writeMicroseconds(1000);
  */
}

void rollerOn() {
  for (int i = 1500; i <= 2000; i += 25)
    MotorRoller.writeMicroseconds(i);
  /*
    MotorRoller.writeMicroseconds(1550);
    MotorRoller.writeMicroseconds(1600);
    MotorRoller.writeMicroseconds(1650);
    MotorRoller.writeMicroseconds(1700);
    MotorRoller.writeMicroseconds(1750);
    MotorRoller.writeMicroseconds(1800);
    MotorRoller.writeMicroseconds(1850);
    MotorRoller.writeMicroseconds(1900);
    MotorRoller.writeMicroseconds(1950);
    MotorRoller.writeMicroseconds(2000);
  */
}

void magnetOn() {
  digitalWrite(magnetPin, HIGH); // turn the magnet on (HIGH is the voltagelevel)
}

void magnetOff() {
  digitalWrite(magnetPin, LOW); // turn the magnet off by making the voltageLOW
}

void unload() {
  /*
   * DO UNLOADING STUFF HERE
   */
}
