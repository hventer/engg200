#ifndef ArmRollers_h
#define ArmRollers_h

#include "Arduino.h"
#include "Servo.h"

class ArmRollers {
private:
  //roller 1
  Servo _motorRoller1;
  int _motorRoller1EnA;
  int _motorRoller1In1;
  int _motorRoller1In2;
  int _motorPinRoller1;

  //roller 2
  Servo _motorRoller2;
  int _motorRoller2EnA;
  int _motorRoller2In1;
  int _motorRoller2In2;
  int _motorPinRoller2;

  //arm
  Servo _motorArm;
  int _armMotorInput1;
  int _armMotorInput2;
  int _armMotorEnA;
  int _motorPinArm;

  //encoders
  int _encoderPinA, _encoderPinB, _encoderValue;

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
  ArmRollers();
  int collectCans(int numCans);
  void armExtend();
  void armRetract();
  void encoder();
  void grab();
  void grabStop();
  int getEncoderPinA();
};

#endif
