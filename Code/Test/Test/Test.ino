#include <Servo.h>  //This comes with the Arduino
#define motorPin 11

int state;

Servo MotorArm;


void setup() {
  MotorArm.attach(motorPin, 1000, 2000);
  MotorArm.writeMicroseconds(1500);
  Serial.begin(9600);
  state = 1;
  run();
}

void loop() {
  state = 0;
}

void run() {
      for (int i = 1500; i <= 2000; i += 10) {
        MotorArm.writeMicroseconds(i);
        Serial.println(i);
      }
  }

  /*
    MotorArm.writeMicroseconds(1600);
    MotorArm.writeMicroseconds(1700);
    MotorArm.writeMicroseconds(1800);
    MotorArm.writeMicroseconds(1900);
    MotorArm.writeMicroseconds(2000);
  */
