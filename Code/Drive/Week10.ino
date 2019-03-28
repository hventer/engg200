/*
 * Date: 25/10/18
 * Author: Tim Love
 */

/*
 * Hey guys, so I have written this sketch along with the TileRunner and Robot libraries.
 * They should provide an almost there prototype.  Currently the sketch compiles fine however,
 * it does not have the functionality to:
 * -receive an order via bluetooth
 * -send updates via bluetooth
 * -use the colour sensor (i didn't know what specific sensor we are using so i don't know what input it gives us)
 *    (in the Robot.cpp file, the cases 11, 21 and 31 require a single line to be fixed to implement this.  Have
 *      a look and give it a go)
 * -receive a kill command
 * 
 * It is/should be able to:
 * -validate a can order if one were to be received
 * -follow the line forwards using the IR sensor and a P loop
 *    (we will most likely need to add the I term so we get a PI loop for the IR sensor)
 * -make fine (small) adjustments to reach a target position near it
 *    (it can actually make large adjustments but we will only use it for fine adjustments)
 * -execute a kill command if one were to be received
 * 
 * 
 * Depending on how you want to use this code and the libraries, you may have to go in and change some values
 * or add a few lines of code.  It does not matter if you alter these files on your own device but please
 * do not delete/overwrite the ones I have uploaded to the drive.  If you want to upload a version you worked
 * on then rename it something like "TileRuner.h(1)"
 * 
 * It may seem counter intuitive but in the TileRunner library, the driveFowards() function is a line following
 * function that only works opn the line while the align() function is more akin to the example from the master
 * class in the sketch "tile_runner_interrupts_stage_5"
 * 
 */


#include <TileRunner.h>
#include <ColourSensor.h>
#include <Robot.h>
#include <Metro.h>

Metro serialMetro = Metro(250);

////Robot paramaters////
float gainPLeft = 0.5 ;
float gainILeft = 0.1;
float gainDLeft = 0.0;
float gainPRight = 0.5;
float gainIRight = 0.1;
float gainDRight = 0.0;
float gainPColour = 0.1;
float gainIColour = 0.0;
float gainDColour = 0.0;
int lineSensorPin0 = 10;
int lineSensorPin1 = 11;
int lineSensorPin2 = 12;
int lineSensorPin3 = 13;
int lineSensorPinOut = 14;
int canSensorPin0 = 24;
int canSensorPin1 = 25;
int canSensorPin2 = 26;
int canSensorPin3 = 27;
int canSensorPinOut = 28;
////////////////////////

// declare and create instance of our robot class
Robot myRobot = Robot(gainPLeft, gainILeft, gainDLeft, gainPRight, gainIRight, gainDRight, gainPColour, gainIColour, gainDColour, lineSensorPin0, lineSensorPin1, lineSensorPin2, lineSensorPin3, lineSensorPinOut, canSensorPin0, canSensorPin1, canSensorPin2, canSensorPin3, canSensorPinOut);
//int targetPos = 0;
//int basketOffset = 0;// through testing we will determine the number that corresponds to the distance the basket is past the end of the line.


void setup() {
  Serial.begin(115200); // Start the Serial communication
  attachInterrupt(digitalPinToInterrupt(18), ISRleft, RISING);
  attachInterrupt(digitalPinToInterrupt(19), ISRright, RISING);
  attachInterrupt(digitalPinToInterrupt(20), ISRkill, RISING);
  pinMode(22,INPUT); // 18 and 22 come from left
  pinMode(23,INPUT); // 19 and 23 come from right
}

void loop() {
  if(serialMetro.check() == 1) {// check if the metro has passed it's interval
    //output encoder readings to serial monitor
    Serial.print(myRobot.getCountLeft());
    Serial.print('\t');
    Serial.print(myRobot.getCountRight());
  }
  myRobot.completeOrder();
}

/*
void changeRobotState(int _state) {
  myRobot.changeState(_state);
}*/





void ISRleft(){// interrupt service routine on the left encoder
  if(digitalRead(22)==LOW){
    //increment countLeft
    myRobot.countLeftIncrease();
  }
  else{
    //decrement countLeft
    myRobot.countLeftDecrease();
  }
}
void ISRright(){// interrupt service routine on the right encoder
  if(digitalRead(23)==HIGH){
    //increment countRight
    myRobot.countRightIncrease();
  }
  else{
    //decrement countRight
    myRobot.countRightDecrease();
  }
}

void ISRkill() {// interrupt service routine for kill command
  myRobot.changeState(0);
}



