#include <ArmRollers.h>
//#include <ArmMagnet.h>

ArmRollers myArmRollers;
//ArmMagnet myArmMagnet;

//int _encoderValue = 0;
//int _encoderPinA = 1;
//int _encoderPinB = 1;
//
//void encoder(){
//  if(_encoderPinB){
//    _encoderValue++;
//  }
//  else{
//    _encoderValue--;
//  }
//  return;
//}

void getEncoder() {
  myArmRollers.encoder();
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(myArmRollers.getEncoderPinA), getEncoder, RISING);

}

void loop() {
  myArmRollers.collectCans(3);
  //myArmMagnet.collectCans(3);
}
