#include "Arduino.h"
#include "TileRunner.h"
#include "Servo.h"

TileRunner::TileRunner() {
	this->_countLeft = 0;
	this->_countRight = 0;

	this->_motorLeft.attach(4);
	this->_motorRight.attach(5);

	this->_lineSensor = ColourSensor();

	this->_gainPLeft = 0;
	this->_gainILeft = 0;
	this->_gainDLeft = 0;

	this->_gainPRight = 0;
	this->_gainIRight = 0;
	this->_gainDRight = 0;

	this->_gainPColour = 0;
	this->_gainIColour = 0;
	this->_gainDColour = 0;

	this->_integralLeft = 0;
	this->_integralRight = 0;
	this->_integralColour = 0;

	this->_lastErrorLeft = 0;
	this->_lastErrorRight = 0;
	this->_lastErrorColour = 0;

	this->_throttleLeft = 90;
	this->_throttleRight = 90;
	this->_throttleMax = 150;//can be any integer between 91 and 180 inclusive
	this->_throttleMin = 30;//can be any integer between 0 and 89 inclusive
	this->_throttleMid = 90;

	this->_redBaseline = 0;
	this->_greenBaseline = 0;
	this->_blueBaseline = 0;
	this->_redMin = 0;
	this->_greenMin = 0;
	this->_blueMin = 0;
	this->_redMax = 0;
	this->_greenMax = 0;
	this->_blueMax = 0;
}

TileRunner::TileRunner(float gainPLeft, float gainILeft, float gainDLeft,
					float gainPRight, float gainIRight, float gainDRight,
					float gainPColour, float gainIColour, float gainDColour,
					int lineSensorPin0, int lineSensorPin1, int lineSensorPin2, int lineSensorPin3, int lineSensorPinOut) {
	this->_countLeft = 0;
	this->_countRight = 0;

	this->_motorLeft.attach(4);
	this->_motorRight.attach(5);

	this->_lineSensor = ColourSensor(lineSensorPin0, lineSensorPin1, lineSensorPin2, lineSensorPin3, lineSensorPinOut);

	this->_gainPLeft = gainPLeft;
	this->_gainILeft = gainILeft;
	this->_gainDLeft = gainDLeft;

	this->_gainPRight = gainPRight;
	this->_gainIRight = gainIRight;
	this->_gainDRight = gainDRight;

	this->_gainPColour = gainPColour;
	this->_gainIColour = gainIColour;
	this->_gainDColour = gainDColour;

	this->_integralLeft = 0;
	this->_integralRight = 0;
	this->_integralColour = 0;

	this->_lastErrorLeft = 0;
	this->_lastErrorRight = 0;
	this->_lastErrorColour = 0;

	this->_throttleLeft = 90;
	this->_throttleRight = 90;
	this->_throttleMax = 150;//can be any integer between 91 and 180 inclusive
	this->_throttleMin = 30;//can be any integer between 0 and 89 inclusive
	this->_throttleMid = 90;

	this->_redBaseline = 30;
	this->_greenBaseline = 30;
	this->_blueBaseline = 35;
	this->_redMin = 14;
	this->_greenMin = 15;
	this->_blueMin = 17;
	this->_redMax = 105;
	this->_greenMax = 130;
	this->_blueMax = 147;
}


void TileRunner::kill() {
	this->_gainPLeft = 0.0;
	this->_gainILeft = 0.0;
	this->_gainDLeft = 0.0;
	this->_gainPRight = 0.0;
	this->_gainIRight = 0.0;
	this->_gainDRight = 0.0;
	this->_gainPColour = 0.0;
	this->_gainIColour = 0.0;
	this->_gainDColour = 0.0;
	this->_throttleMin = 90;
	this->_throttleMax = 90;
}

void TileRunner::idle() {
	this->_motorLeft.write(_throttleMid);
	this->_motorRight.write(_throttleMid);
}



/*
 * If the red/green/blue Error values are positive (i.e. baseline is greater than reading)
 * then we are drifting towards the white side of the tape and need to correct to the right
 * by slowng the right side down.
 * Opposite is true for negative Error values
 */
void TileRunner::lineFollowRGB(int target) {
	float redError = this->_lineSensor.readRed() - this->_redBaseline;
	float greenError = this->_lineSensor.readGreen() - this->_greenBaseline;
	float blueError = this->_lineSensor.readBlue() - this->_blueBaseline;
	//negative values means over white; positive values means over black

	float redErrorUnit, greenErrorUnit, blueErrorUnit;//need to initialize

	if (redError < 0) {//on the white
		float redErrorUnit = redError / (this->_redBaseline - this->_redMin);
	}
	else if (redError > 0) {//on the black
		float redErrorUnit = redError / (this->_redMax - this->_redBaseline);
	}
	if (greenError < 0) {//on the white
		float greenErrorUnit = greenError / (this->_greenBaseline - this->_greenMin);
	}
	else if (greenError > 0) {//on the black
		float greenErrorUnit = greenError / (this->_greenMax - this->_greenBaseline);
	}
	if (blueError < 0) {//on the white
		float blueErrorUnit = blueError / (this->_blueBaseline - this->_blueMin);
	}
	else if (blueError > 0) {//on the black
		float blueErrorUnit = blueError / (this->_blueMax - this->_blueBaseline);
	}

	int errorLeft = target - this->_countLeft;
	int errorRight = target - this->_countRight;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	float errorColour = (redErrorUnit + greenErrorUnit + blueErrorUnit) / 3.0;//not sure if right
	
	this->_integralLeft += errorLeft;
	this->_integralRight += errorRight;
	this->_integralColour += errorColour;

	float lineError = constrain(this->_gainPColour * errorColour + this->_gainIColour * this->_integralColour + this->_gainDColour* (errorColour - this->_lastErrorColour), -1.0, 1.0);

	if (lineError == 0) {
		this->_throttleLeft = (int) 90.0 - (this->_gainPLeft * errorLeft + this->_gainILeft * this->_integralLeft + this->_gainDLeft * (errorLeft - this->_lastErrorLeft));
		this->_throttleRight = (int) 90.0 + (this->_gainPRight * errorRight + this->_gainIRight * this->_integralRight + this->_gainDRight * (errorRight - this->_lastErrorRight));
	}
	else if (lineError > 0) {//drifting on the white; relax right side
		this->_throttleLeft = (int) 90.0 - (this->_gainPLeft * errorLeft + this->_gainILeft * this->_integralLeft + this->_gainDLeft * (errorLeft - this->_lastErrorLeft));
		this->_throttleRight = (int) 90.0 + (this->_gainPRight * errorRight + this->_gainIRight * this->_integralRight + this->_gainDRight * (errorRight - this->_lastErrorRight)) - lineError * 50.0;
	}
	else {//drifting on the black; relax left side
		this->_throttleLeft = (int) 90.0 - (this->_gainPLeft * errorLeft + this->_gainILeft * this->_integralLeft + this->_gainDLeft * (errorLeft - this->_lastErrorLeft)) - lineError * 50.0;
		this->_throttleRight = (int) 90.0 + (this->_gainPRight * errorRight + this->_gainIRight * this->_integralRight + this->_gainDRight * (errorRight - this->_lastErrorRight));
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	this->_motorLeft.write(constrain(this->_throttleLeft, this->_throttleMin, this->_throttleMax));
	this->_motorRight.write(constrain(this->_throttleRight, this->_throttleMin, this->_throttleMax));
}



bool TileRunner::aligned(int target) {
	if(this->_countLeft == target && this->_countRight == target) {
		return true;
	}
	return false;
}


void TileRunner::countLeftIncrease() {
	this->_countLeft++;
}
void TileRunner::countLeftDecrease() {
	this->_countLeft--;
}
void TileRunner::countRightIncrease() {
	this->_countRight++;
}
void TileRunner::countRightDecrease() {
	this->_countRight--;
}

int TileRunner::getCountLeft() {
	return this->_countLeft;
}
int TileRunner::getCountRight() {
	return this->_countRight;
}