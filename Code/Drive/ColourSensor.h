#ifndef ColourSensor_h
#define ColourSensor_h

#include "Arduino.h"


class ColourSensor {
	private:
		int _controlPin0, _controlPin1, _controlPin2, _controlPin3;
		int _outputPin;
		int _frequency;


	public:
		ColourSensor();
		ColourSensor(int controlPin0, int controlPin1, int controlPin2, int controlPin3, int outputPin);

		bool checkRed();
		bool checkGreen();
		bool checkBlue();
		int readRed();
		int readGreen();
		int readBlue();

};


#endif
