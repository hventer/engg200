#ifndef Arm_h
#define Arm_h

#include "Arduino.h"

class Arm {
  	private:
        int armEnable; //motorControler outputA green
        int armInput1; //motorControler outputB yellow
        int armInput2; //pin for arm motor

        virtual bool hitBackWall() = 0;


	public:
		Arm();

		virtual void armExtend() = 0;
    virtual void armRetract() = 0;
    virtual void grabStart() = 0;
    virtual void grabStop() = 0;
};

class ArmRollers: public Arm {
  private:
    int _armEnable; //motorControler outputA green
    int _armInput1; //motorControler outputB yellow
    int _armInput2; //pin for arm motor
    int _armRollerEncoder; //pin for encoder
    int _canSensor; //
    bool hitBackWall();

    //below is specific to arm rollers.
    int _rollerGrabber;

  public:
    ArmRollers();
    ArmRollers(int armEnable, int armInput1, int armInput2, int armRollerEncoder, int canSensor, int rollerGrabber);
    void armExtend();
    void armRetract();
    bool hitBackWall();
    void grabStart();
    void grabStop();
};


class ArmMagnet :: public Arm {
  private:
    int _armEnable; //motorControler outputA green
    int _armInput1; //motorControler outputB yellow
    int _armInput2; //pin for arm motor
    int _armRollerEncoder; //pin for encoder
    bool hitBackWall();

    //below is specific to arm magnet
    int _canSensor; //pin for sensor to sense can
    int _magnetOutput; //pin for the Magnet

  public:
    ArmMagnet();
    ArmMagnet(int armEnable, int armInput1, int armInput2, int armRollerEncoder, int canSensor, int magnetOutput);

    void armExtend();

    void armRetract();

    void grabStart();

    void grabStop();

};

#endif
