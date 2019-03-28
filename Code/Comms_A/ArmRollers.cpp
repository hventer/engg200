#include "Arduino.h"
#include "ArmRollers.h"
#include "Servo.h"

ArmRollers::ArmRollers() {//default constructor
  //arm motor
  this-> _armMotorInput1 = 1;
  this-> _armMotorInput2 = 1;
  this-> _armMotorEnA = 1;
  //pin
  this-> _motorPinArm = 1;
  pinMode(_armMotorEnA, OUTPUT);
  pinMode(_armMotorInput1, OUTPUT);
  pinMode(_armMotorInput2, OUTPUT);

  //roller motor 1
  this-> _motorRoller1EnA = 8;
  this-> _motorRoller1In1 = 33;
  this-> _motorRoller1In2 = 35;
  //pin
  this-> _motorPinRoller1 = 1;
  pinMode(_motorRoller1EnA, OUTPUT);
  pinMode(_motorRoller1In1, OUTPUT);
  pinMode(_motorRoller1In2, OUTPUT);

  //roller motor 2
  this-> _motorRoller2EnA = 1;
  this-> _motorRoller2In1 = 1;
  this-> _motorRoller2In2 = 1;
  //pin
  this-> _motorPinRoller2 = 1;
  pinMode(_motorRoller2EnA, OUTPUT);
  pinMode(_motorRoller2In1, OUTPUT);
  pinMode(_motorRoller2In2, OUTPUT);

/*
  this->_encoderValue = 0;
  this->_encoderPinA = 1;
  this->_encoderPinB = 1;
  */

  //sensors
  this-> _sensorPinGrab = 1;
  this-> _sensorPinCount = 1;
  this->_sensorGrabState = 1; //normally high
  this->_sensorCountState = 1; //normally high
  this->_sensorCountPrev = 1; //normally high
  pinMode(_sensorPinGrab, INPUT_PULLUP);
  pinMode(_sensorPinCount, INPUT_PULLUP);

  //attachInterrupt(_encoderPinA, encoder, RISING);

  this->_backWall = 900;
  this->_hitBackWall = 0;//not hit back wall

  this->_cansCollected = 0; //starts empty
}

int ArmRollers::collectCans(int numCans){
  for (int i=0; i<numCans; i++){
    this->grab();
    this->armExtend();
    if (this->_hitBackWall){
      break;
    }
    this->grabStop();
    this->armRetract();
  }
  this->armRetract();
  return this->_cansCollected;
}

void ArmRollers::grab() {
  digitalWrite(_motorRoller1In1, LOW);
  digitalWrite(_motorRoller1In2, HIGH);
  analogWrite(_motorRoller1EnA, 150);

  digitalWrite(_motorRoller2In1, LOW);
  digitalWrite(_motorRoller2In2, HIGH);
  analogWrite(_motorRoller2EnA, 150);
}

void ArmRollers::grabStop() {
  digitalWrite(_motorRoller1In1, LOW);
  digitalWrite(_motorRoller1In2, LOW);
  analogWrite(_motorRoller1EnA, 0);

  digitalWrite(_motorRoller2In1, LOW);
  digitalWrite(_motorRoller2In2, LOW);
  analogWrite(_motorRoller2EnA, 0);
}

void ArmRollers::armExtend() {
  this->_sensorGrabState = 1;
  digitalWrite(_armMotorInput1, LOW);
  digitalWrite(_armMotorInput2, HIGH);
  analogWrite(_armMotorEnA, 150);

  while(_encoderValue < this->_backWall){
    this->_sensorGrabState = digitalRead(_sensorPinGrab);
    if(!this->_sensorGrabState){
      break;
    }
  }
  digitalWrite(_armMotorInput1, LOW);
  digitalWrite(_armMotorInput2, LOW);
  analogWrite(_armMotorEnA, 0);

  if (_encoderValue>=this->_backWall){
    this->_hitBackWall=1;
  }
  return;
}

void ArmRollers::armRetract(){
  this->_sensorCountPrev=1;
  digitalWrite(_armMotorInput1, HIGH);
  digitalWrite(_armMotorInput2, LOW);
  analogWrite(_armMotorEnA, 150);

  while(_encoderValue > 0){
    this->_sensorCountState = digitalRead(_sensorPinCount);
    if(!this->_sensorCountPrev && this->_sensorCountState){
      this->_cansCollected++;
      break;
    }
    this->_sensorCountPrev = this->_sensorCountState;
  }

  digitalWrite(_armMotorInput1, LOW);
  digitalWrite(_armMotorInput2, LOW);
  analogWrite(_armMotorEnA, 0);

  return;
}
/*
void encoder(){
  if(this->_encoderPinB){
    this->_encoderValue++;
  }
  else{
    this->_encoderValue--;
  }
  return;
}*/
