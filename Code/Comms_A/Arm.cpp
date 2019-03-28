#include "Arduino.h"
#include "Arm.h"
#include "Servo.h"

#define encoderPinA 19
#define encoderPinA 99
#define sensorPinGrab 4
#define sensorPinCount 5
#define armMotorEnA 99
#define armMotorInput1 99
#define armMotorInput2 99

Arm::Arm() {//arm constructor

}

ArmRollers::ArmRollers(){ //constructor
  attachInterrupt(digitalPinToInterrupt(encoderPinA), encoder, RISING);
  pinMode(sensorPinGrab, INPUT_PULLUP);
  this->_backWall = 900;
  this->_hitBackWall = 0;//not hit back wall
  this->_sensorGrabState = 1; //normally high
  this->_sensorCountState = 1; //normally high
  this->_sensorCountPrev = 1; //normally high
  this->_armMotorSpeed = 255; //full speed
  this->_cansCollected = 0; //starts empty

}

ArmMagnet::ArmMagnet(){ //constructor
  attachInterrupt(digitalPinToInterrupt(encoderPinA), encoder, RISING);
  pinMode(sensorPinGrab, INPUT_PULLUP);
  this->_backWall = 3000;
  this->_hitBackWall = 0;//not hit back wall
  this->_sensorGrabState = 1; //normally high
  this->_sensorCountState = 1; //normally high
  this->_sensorCountPrev = 1; //normally high
  this->_armMotorSpeed = 255; //full speed
  this->_cansCollected = 0; //starts empty

}

ArmRollers::collectCans(int numCans){
  for (int i=0; i<numCans; i++){
    this->grabStart();
    this->armExtend();
    if (this->_hitBackWall){
      break;
    }
    this->grabStop();
    this->armRetract();
  }
  return this->_cansCollected;
}

ArmMagnet::collectCans(int numCans){
  for (int i=0; i<numCans; i++){
    this->armExtend();
    if (this->_hitBackWall){
      break;
    }
    this->grabStart();
    this->armRetract();
    this->grabStop();
  }
  return this->_cansCollected;
}

Arm::armExtend(){
  this->_sensorGrabState = 1;
  digitalWrite(armMotorInput1, LOW);
  digitalWrite(armMotorInput2, HIGH);
  analogWrite(armMotorEnA,this->_armMotorSpeed)

  while(this->_encoderValue<this->_backWall){
    this->_sensorGrabState = digitalRead(sensorPinGrab);
    if(!this->_sensorGrabState){
      break;
    }
  }
  digitalWrite(armMotorInput1, LOW);
  digitalWrite(armMotorInput2, LOW);

  if (this->_encoderValue>=this->_backWall){
    this->_hitBackWall=1;
  }

  return;
}

Arm::armRetract(){
  this->_sensorCountPrev=1;
  digitalWrite(armMotorInput1, HIGH);
  digitalWrite(armMotorInput2, LOW);
  analogWrite(armMotorEnA,this->_armMotorSpeed)

  while(this->_encoderValue>0){
    this->_sensorCountState = digitalRead(sensorPinCount);
    if(!this->_sensorCountPrev && this->_sensorCountState){
      this->_cansCollected++;
      break;
    }
    this->_sensorCountPrev=this->_sensorCountState;
  }
  digitalWrite(armMotorInput1, LOW);
  digitalWrite(armMotorInput2, LOW);

  return;
}

Arm::encoder(){
  if(encoderPinB){
    this->_encoderValue++;
  }
  else{
    this->_encoderValue--;
  }
  return;
}
