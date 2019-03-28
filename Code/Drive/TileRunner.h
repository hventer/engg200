#ifndef TileRunner_h
#define TileRunner_h

#include "Arduino.h"
#include "Servo.h"
#include "ColourSensor.h"

class TileRunner {
	private:
		int _countLeft, _countRight;

		Servo _motorLeft;
		Servo _motorRight;
		ColourSensor _lineSensor;

		/*
		 * PID loop constants for Left motor (x-axis correction), Right motor (x-axis correction) and colour sensor (z-axis correction)
		 * We are saying that the x-axis lies in the direction of the tape, y-axis is vertical and z-axis is perpendicularto the tape along the ground.
		 */
		float _gainPLeft, _gainILeft, _gainDLeft;
		float _gainPRight, _gainIRight, _gainDRight;
		float _gainPColour, _gainIColour, _gainDColour;

		int _integralLeft, _integralRight, _integralColour;
		int _lastErrorLeft, _lastErrorRight, _lastErrorColour;

		int _throttleLeft, _throttleRight;
		int _throttleMax, _throttleMin, _throttleMid;

		/*
		 * Values obtained through experminetation with colour sensor and tape
		 */
		int _redBaseline, _greenBaseline, _blueBaseline;
		int _redMin, _greenMin, _blueMin;
		int _redMax, _greenMax, _blueMax;

	public:
		TileRunner();//default constructor
		TileRunner(float gainPLeft, float gainILeft, float gainDLeft,
					float gainPRight, float gainIRight, float gainDRight,
					float gainPColour, float gainIColour, float ginDColour,
					int lineSensorPin0, int lineSensorPin1, int lineSensorPin2, int lineSensorPin3, int lineSensorPinOut);//parameterized constructor
		void kill();//turns gain values to 0 (should stop the tile runner from moving) 
		void idle();
		void lineFollowRGB(int target);
		bool aligned(int target);//return true once the target is reached

		void countLeftIncrease();
		void countLeftDecrease();
		void countRightIncrease();
		void countRightDecrease();

		int getCountLeft();
		int getCountRight();
};

#endif