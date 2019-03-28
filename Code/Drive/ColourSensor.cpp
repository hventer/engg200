#include "Arduino.h"
#include "ColourSensor.h"


ColourSensor::ColourSensor() {//default constructor

}

ColourSensor::ColourSensor(int controlPin0, int controlPin1, int controlPin2, int controlPin3, int outputPin) {//paramaterized constructor
	pinMode(controlPin0, OUTPUT);
	pinMode(controlPin1, OUTPUT);
	pinMode(controlPin2, OUTPUT);
	pinMode(controlPin3, OUTPUT);
	pinMode(outputPin, INPUT);

	//Setting Fequency Scaling to 100% better for detection (Can change or remove):
	digitalWrite(controlPin0, HIGH);
	digitalWrite(controlPin1, HIGH);
}


bool ColourSensor::checkRed() {
	int redValue = this->readRed();
	if(redValue == 0) {
		int greenValue = this->readGreen();
		if(greenValue == 0) {
			int blueValue = this->readBlue();
			if(blueValue == 0) {
				return true;
			}
		}
	}
	return false;
}

bool ColourSensor::checkGreen() {
	int redValue = this->readRed();
	if(redValue == 0) {
		int greenValue = this->readGreen();
		if(greenValue == 0) {
			int blueValue = this->readBlue();
			if(blueValue == 0) {
				return true;
			}
		}
	}
	return false;
}

bool ColourSensor::checkBlue() {
	int redValue = this->readRed();
	if(redValue == 0) {
		int greenValue = this->readGreen();
		if(greenValue == 0) {
			int blueValue = this->readBlue();
			if(blueValue == 0) {
				return true;
			}
		}
	}
	return false;
}


int ColourSensor::readRed() {
	//reading the RedPhotides (both pins set to LOW)
    digitalWrite(this->_controlPin2, LOW);
    digitalWrite(this->_controlPin3, LOW);
    return pulseIn(this->_outputPin, LOW);
}

int ColourSensor::readGreen() {
	//reading the GreenPhotides (both pins set to HIGH)
    digitalWrite(this->_controlPin2, HIGH);
    digitalWrite(this->_controlPin3, HIGH);
    return pulseIn(this->_outputPin, LOW);
}


int ColourSensor::readBlue() {
	//reading the BluePhotides (C2 LOW, C3 HIGH);
    digitalWrite(this->_controlPin2, LOW);
    digitalWrite(this->_controlPin3, HIGH);
    return pulseIn(this->_outputPin, LOW);
}
