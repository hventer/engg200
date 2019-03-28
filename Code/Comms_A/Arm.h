#ifndef Arm_h
#define Arm_h

#include "Arduino.h"

class Arm {
private:
  int _cansCollected;//cans already collected;

  int _armMotorSpeed;

  int _hitBackWall; //boolean
  int _encoderValue;
  virtual int _backWall; //value dependent on arm type

  int _sensorGrabState;
  int _sensorCountState;
  int _sensorCountPrev;

public:
  Arm();//constructor
  int collectCans(int numCans);
  void armExtend();
  void armRetract();
  void encoder();
  virtual void grab();
  virtual void grabStop();
};

#endif
