#ifndef ArmMagnet_h
#define ArmMagnet_h

#include "Arduino.h"
#include "Servo.h"

class ArmRollers {
private:
  //arm
  Servo _motorArm;
  int _armMotorInput1;
  int _armMotorInput2;
  int _armMotorEnA;
  int _motorPinArm;

  int _magnetPin;

  //sensors
  int _sensorPinGrab;
  int _sensorPinCount;
  int _sensorGrabState;
  int _sensorCountState;
  int _sensorCountPrev;

  int _backWall; //value dependent on arm type
  int _hitBackWall; //boolean

  int _cansCollected;//cans already collected;

public:
  ArmMagnet();
  int collectCans(int numCans);
  void armExtend();
  void armRetract();
  void encoder();
  void grab();
  void grabStop();
};

#endif
