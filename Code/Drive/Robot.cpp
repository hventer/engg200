#include "Arduino.h"
#include "Robot.h"

Robot::Robot() {
	this->_myTR = TileRunner();
	this->_canSensor = ColourSensor();
	//this->_activated = false;
	this->_operatingState = 1;//operating state 1 is the sleep state
	this->_target = 0;
	this->_redNeed = 0;
	this->_greenNeed = 0;
	this->_blueNeed = 0;
	this->_redHeld = 0;
	this->_greenHeld = 0;
	this->_blueHeld = 0;
}

Robot::Robot(float gainPLeft, float gainILeft, float gainDLeft,
				float gainPRight, float gainIRight, float gainDRight,
				float gainPColour, float gainIColour, float gainDColour,
				int lineSensorPin0, int lineSensorPin1, int lineSensorPin2, int lineSensorPin3, int lineSensorPinOut,
				int canSensorPin0, int canSensorPin1, int canSensorPin2, int canSensorPin3, int canSensorPinOut) {
	this->_myTR = TileRunner(gainPLeft, gainILeft, gainDLeft,  gainPRight, gainIRight, gainDRight, gainPColour, gainIColour, gainDColour, lineSensorPin0, lineSensorPin1, lineSensorPin2, lineSensorPin3, lineSensorPinOut);
	this->_canSensor = ColourSensor(canSensorPin0, canSensorPin1, canSensorPin2, canSensorPin3, canSensorPinOut);
	//this->_activated = false;
	this->_operatingState = 1;//operating state 1 is the sleep state
	this->_target = 0;
	this->_redNeed = 0;
	this->_greenNeed = 0;
	this->_blueNeed = 0;
	this->_redHeld = 0;
	this->_greenHeld = 0;
	this->_blueHeld = 0;
}

/*
void Robot::activate() {
	this->_activated = true;
}*/

void Robot::receiveOrder() {
	//receives the order and stores the corresponding amount of r,g,b cans in "need" fields
	char colour = Serial.read();
	if(colour == 'r') {
    	this->_redNeed = (int) Serial.read();
	}
	else if(colour == 'g') {
    	this->_greenNeed = (int) Serial.read();
	}
	else if(colour == 'b') {
    	this->_blueNeed = (int) Serial.read();
	}
	else {//something is wrong
    	Serial.print("ya done fucked up") ;
	}
}

bool Robot::validateOrder() {
	//returns true if there are less than 10 cans in the order
	int numCans = this->_redNeed + this->_greenNeed + this->_blueNeed;
	if(numCans > 10 || numCans < 0 || this->_redNeed < 0 || this->_greenNeed < 0 || this->_blueNeed < 0) {
		return false;
	}
	return true;
}

void Robot::changeState(int state) {//in this function we will implement the sending of updates
	switch(state) {
		case 0 ://killed
			this->_operatingState = 0;
			this->_myTR.kill();
			break;
		case 1 ://sleep
			this->_operatingState = 1;
			break;
		case 2 ://idle (waiting for an order)
			this->_operatingState = 2;
			break;
		case 3 ://processing an order
			this->_operatingState = 3;
			break;
		case 11 ://driving until the red cans are seen
			this->_operatingState = 11;
			break;
		case 12 ://aligning with the red cans
			this->_operatingState = 12;
			break;
		case 13 ://collecting red cans
			this->_operatingState = 13;
			break;
		case 14 ://small buffer forwards towards 2nd set of red cans
			this->_operatingState = 14;
			break;
		case 21 ://driving until the green cans are seen
			this->_operatingState = 21;
			break;
		case 22 ://aligning with the green cans
			this->_operatingState = 22;
			break;
		case 23 ://collecting green cans
			this->_operatingState = 23;
			break;
		case 24 ://small buffer forwards towards 2nd set of red cans
			this->_operatingState = 24;
			break;
		case 31 ://driving until the blue cans are seen
			this->_operatingState = 31;
			break;
		case 32 ://aligning with the blue cans
			this->_operatingState = 32;
			break;
		case 33 ://collecting blue cans
			this->_operatingState = 33;
			break;
		case 34 ://small buffer forwards towards 2nd set of red cans
			this->_operatingState = 34;
			break;
		case 41 ://driving to end of line
			this->_operatingState = 41;
			break;
		case 42 ://driving forward to basket
			this->_operatingState = 42;
			break;
		case 43 ://unloading cans
			this->_operatingState = 43;
			break;
		default ://
			this->_operatingState = -1;
	}
}


void Robot::completeOrder() {
	/*
	 * We will collect the cans in this order: red, green, blue
	 */
	switch(this->_operatingState) {
		case 0 ://killed
			this->_myTR.idle();
			break;
		case 1 ://sleep
			this->_myTR.idle();
			break;
		case 2 ://idle (waiting for an order)
			if(false/* an order comes through */) {
				this->changeState(3);
				break;
			}
			this->_myTR.idle();
			break;
		case 3 ://validating an order
			if(this->validateOrder()) {
				this->_target = 10000;
				if(this->_redNeed > 0) {
					this->changeState(11);
					break;
				}
				else if(this->_greenNeed > 0) {
					this->changeState(21);
					break;
				}
				else if(this->_blueNeed > 0) {
					this->changeState(31);
					break;
				}
				else {
					this->changeState(41);
				}
			}
			break;
		case 11 ://driving until the red cans are seen
			if(this->_canSensor.checkRed()) {
				this->_target = (this->_myTR.getCountLeft() + this->_myTR.getCountRight()) / 2;
				this->changeState(12);
				break;
			}
			else {
				this->_myTR.lineFollowRGB(this->_target);
			}
			break;
		case 12 ://aligning with the red cans
			if(this->_myTR.aligned(this->_target)) {
				this->changeState(13);
				break;
			}
			else {
				this->_myTR.lineFollowRGB(this->_target);
			}
			break;
		case 13 ://collecting red cans
			this->_myTR.idle();
			this->_redHeld += this->collectCans(this->_redNeed - this->_redHeld);//this funciton call should "pause" execution of the program
			if(this->_redHeld < this->_redNeed) {
				this->_target += 200;
				this->changeState(14);
				break;
			}
			else if(this->_redNeed == this->_redHeld) {
				if (this->_greenNeed > 0) {
					this->changeState(21);
					break;					
				}
				else if (this->_blueNeed > 0) {
					this->changeState(31);
					break;
				}
				else {
					changeState(41);
					break;
				}
			}
			else {// picked up too many red cans
				//send an error message
			}
			break;
		case 14 ://small buffer forwards on way to 2nd set of red cans
			if(this->_myTR.aligned(this->_target)) {
				this->changeState(11);
				break;
			}
			else {
				this->_myTR.lineFollowRGB(this->_target);
			}
			break;
		case 21 ://driving until the green cans are seen
			if(this->_canSensor.checkGreen()) {
				this->_target = (this->_myTR.getCountLeft() + this->_myTR.getCountRight()) / 2;
				this->changeState(22);
				break;
			}
			else {
				this->_myTR.lineFollowRGB(this->_target);			}
			break;
		case 22 ://aligning with the green cans
			if(this->_myTR.aligned(this->_target)) {
				this->changeState(23);
				break;
			}
			else {
				this->_myTR.lineFollowRGB(this->_target);
			}
			break;
		case 23 ://collecting green cans
			this->_myTR.idle();
			this->_greenHeld += this->collectCans(this->_greenNeed - this->_greenHeld);//this funciton call should "pause" execution of the program
			if(this->_greenHeld < this->_greenNeed) {
				this->_target += 200;
				this->changeState(24);
				break;
			}
			else if(this->_greenNeed == this->_greenHeld) {
				if(this->_blueNeed > 0) {
					this->changeState(31);
					break;
				}
				else {
					this->changeState(41);
					break;
				}
			}
			else {// picked up too many green cans
				//send an error message
			}
			break;
		case 24 ://small buffer forwards on way to 2nd set of green cans
			if(this->_myTR.aligned(this->_target)) {
				this->changeState(21);
				break;
			}
			else {
				this->_myTR.lineFollowRGB(this->_target);
			}
			break;	
		case 31 ://driving until the blue cans are seen
			if(this->_canSensor.checkBlue()) {
				this->_target = (this->_myTR.getCountLeft() + this->_myTR.getCountRight()) / 2;
				this->changeState(32);
				break;
			}
			else {
				this->_myTR.lineFollowRGB(this->_target);			}
			break;
		case 32 ://aligning with the blue cans
			if(this->_myTR.aligned(this->_target)) {
				this->changeState(33);
				break;
			}
			else {
				this->_myTR.lineFollowRGB(this->_target);
			}
			break;
		case 33 ://collecting blue cans
			this->_myTR.idle();
			this->_blueHeld += this->collectCans(this->_blueNeed - this->_blueHeld);//this funciton call should "pause" execution of the program
			if(this->_blueHeld < this->_blueNeed) {
				this->_target += 200;
				this->changeState(34);
				break;
			}
			else if(this->_blueNeed == this->_blueHeld) {
				this->changeState(41);
				break;
			}
			else {// picked up too many blue cans
				//send an error message
			}
			break;
		case 34 ://small buffer forwards on way to 2nd set of blue cans
			if(this->_myTR.aligned(this->_target)) {
				this->changeState(31);
				break;
			}
			else {
				this->_myTR.lineFollowRGB(this->_target);
			}
			break;
		case 41 ://driving to end of line
			if(false/* reached end of line */) {
				this->_target = (this->_myTR.getCountLeft() + this->_myTR.getCountRight()) / 2;
				this->changeState(42);
				break;
			}
			else {
				this->_myTR.lineFollowRGB(this->_target);			}
			break;
		case 42 ://driving forward to basket
			this->_myTR.lineFollowRGB(this->_target + 500);//this value represents how far the robot has to travel to reach the basket and will need to be adjusted
			break;
		case 43 ://unloading cans
			this->unloadCans(this->_redHeld + this->_greenHeld + this->_blueHeld);
			break;
		default ://
			break;
	}
}

void Robot::idle() {
	this->_myTR.idle();
}


int Robot::collectCans(int num) {
	//return _myArm.collectCans(num); &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
}

int Robot::unloadCans(int num) {
	//return _myArm.unloadCans(num); &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
}

void Robot::countLeftIncrease() {
	this->_myTR.countLeftIncrease();
}
void Robot::countLeftDecrease() {
	this->_myTR.countLeftDecrease();
}
void Robot::countRightIncrease() {
	this->_myTR.countRightIncrease();
}
void Robot::countRightDecrease() {
	this->_myTR.countRightDecrease();
}

int Robot::getCountLeft() {
	this->_myTR.getCountLeft();
}
int Robot::getCountRight() {
	this->_myTR.getCountRight();
}

int Robot::getRedNeed() {
	return this->_redNeed;
}
int Robot::getGreenNeed() {
	return this->_greenNeed;
}
int Robot::getBlueNeed() {
	return this->_blueNeed;
}
int Robot::getRedHeld() {
	return this->_redHeld;
}
int Robot::getGreenHeld() {
	return this->_greenHeld;
}
int Robot::getBlueHeld() {
	return this->_blueHeld;
}

