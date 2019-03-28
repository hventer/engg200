#ifndef Robot_h
#define Robot_h

#include "Arduino.h"
#include "TileRunner.h"
#include "ColourSensor.h"
//#include "Arm.h" &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& the &'s represent where Comms_A have stuff

class Robot {
	private:
		TileRunner _myTR;
		ColourSensor _canSensor;

		//Arm _myArm; &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

		//bool _activated;

		int _operatingState;
		int _target;

		int _redNeed, _greenNeed, _blueNeed;
		int _redHeld, _greenHeld, _blueHeld;

	public:
		Robot();//default constructor
		Robot(float gainPLeft, float gainILeft, float gainDLeft,
				float gainPRight, float gainIRight, float gainDRight,
				float gainPColour, float gainIColour, float gainDColour,
				int lineSensorPin0, int lineSensorPin1, int lineSensorPin2, int lineSensorPin3, int lineSensorPinOut,
				int canSensorPin0, int canSensorPin1, int canSensorPin2, int canSensorPin3, int canSensorPinOut);//parameterized constructor

		//void activate();
		void receiveOrder();
		bool validateOrder();
		void changeState(int _state);
		void completeOrder();
		void idle();
		//read colour sensor
		int collectCans(int num);
		int unloadCans(int num);

		/*
		 * used to track encoder position; called by the ISRs (interrupt service routines)
		 */
		void countLeftIncrease();
		void countLeftDecrease();
		void countRightIncrease();
		void countRightDecrease();

		/*
		 * various getters for fields.
		 */
		int getCountLeft();
		int getCountRight();
		//above this line, the getters are for the TileRunner's fields
		int getRedNeed();
		int getGreenNeed();
		int getBlueNeed();
		int getRedHeld();
		int getGreenHeld();
		int getBlueHeld();
};

#endif