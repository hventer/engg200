#include "Arduino.h"
#include "ArmMagnet.h"
#include "Servo.h"


ArmMagnet::ArmMagnet() {//default constructor
  //arm motor
  this-> _armMotorInput1 = 1;
  this-> _armMotorInput2 = 1;
  this-> _armMotorEnA = 1;
  //pin
  this-> _motorPinArm = 1;
  pinMode(_armMotorEnA, OUTPUT);
  pinMode(_armMotorInput1, OUTPUT);
  pinMode(_armMotorInput2, OUTPUT);

/*
  this->_encoderValue = 0;
  this->_encoderPinA = 1;
  this->_encoderPinB = 1;
  */

  //magnet
  this-> _magnetPin = 5;
  pinMode(_magnetPin, OUTPUT);

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

int ArmMagnet::collectCans(int numCans){
  for (int i=0; i<numCans; i++){
    this->armExtend();
    this->grab();
    if (this->_hitBackWall){
      break;
    }
    this->armRetract();
    this->grabStop();
  }
  this->armRetract();
  return this->_cansCollected;
}

void ArmMagnet::grab() {
  digitalWrite(_magnetPin, HIGH); // turn the magnet on (HIGH is the voltagelevel)
}

void ArmMagnet::grabStop() {
  digitalWrite(_magnetPin, LOW); // turn the magnet off by making the voltageLOW
}

void ArmMagnet::armExtend() {
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

void ArmMagnet::armRetract(){
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
