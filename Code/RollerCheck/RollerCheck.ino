
#include <Servo.h>

//STATES OF GRABBING
//state 1: magnet off SG1
//state 2: magnet on SG2

//STATES OF ARM EXTENSION
//state 1: retracted SAE1
//state 2: extending SAE2
//state 3: extended SAE3
//state 4: retracting SAE4

//STATES OF THE ARM GRABBING
//state 1: magnet off but arm is retracted 
//state 2: magnets off and arm is extending -> magnet is off always when extending
//state 3: magnet off but arm is extended
//state 4: magnet on and arm is extended
//state 5: magnet on but arm is retracting -> magnet is always on when extending
//state 6: magnet on but arm is retracted

/*
 * States combined.
 * state 1: SG1 SAE1
 * state 2: SG1 SAE2
 * state 3: SG1 SAE3
 * state 4: SG2 SAE3
 * state 5: SG2 SAE4
 * state 6: SG2 SAE1
 */

//pin locations
int MotorPin = 11;
int MotorOnPin = 5;
int led = 13;
Servo Motor;

int MotorMaxForward = 2000;  
int MotorMaxReverse = 1000;    
int MotorStop = 1500;         
unsigned long nextStep;
unsigned long lastStep;
int state = 1;



int extendToggleState = 0;
int retractToggleState = 0;

bool extend = false;
bool retract = false;
bool magnet = false;

void setup() {
  pinMode(led, OUTPUT);     // setup the LED to blink as we run the loop
  pinMode(MotorOnPin, OUTPUT);
  Motor.attach(MotorPin, 1000, 2000);  // and another Motor is out here  
  Motor.writeMicroseconds(MotorStop);
  lastStep = millis();  // where we are
  nextStep = lastStep + 1000;

  pinMode(extendLedPin, OUTPUT);
  pinMode(retractLedPin, OUTPUT);
  
  pinMode(extendToggler, INPUT);
  pinMode(retractToggler, INPUT);
}

//lets assume that this program is needed to get 3 cans and we get one can at a time.
int cans = 0;

//lets assume that it takes 5 seconds to do each action (extend and retract

//need to set "setCans" to set global variable cans.
int collectCans(int numberCans) { //extend. pick up can. retract is one execute
  cans=numberCans;
  if(numberCans != 0) {

    state = 1;
    
    //Start by extending hand
    extend = true;
    armExtend();
    delay(5000);
    extend = false;
    armExtend();
    

    //magnet on
    magnet = true;
    armGrab();

    //retract
    retract = true;
    armRetract();
    delay(5000);
    retract = false;
    armRetract();

    magnet = false;
    armGrab();
    
    cans--;
  }
  return cans;
}

void loop() {
   unsigned long inStep;  // how long we are in the current step
  
  inStep = (millis() - lastStep)/2;
  if (inStep > 1000) {
    inStep = 1000;  // don't go beyond the step time
  }
  switch(state) {
    case 0:
      digitalWrite(led, HIGH);
      digitalWrite(MotorOnPin, LOW);
      Motor.writeMicroseconds(MotorStop + inStep);
      break;
    case 1:
      digitalWrite(MotorOnPin, LOW);
      Motor.writeMicroseconds(MotorMaxForward - inStep);
      break;
    case 2:
      digitalWrite(led, LOW);
      digitalWrite(MotorOnPin, HIGH);
      Motor.writeMicroseconds(MotorStop - inStep);
      break;
    case 3:
      digitalWrite(MotorOnPin, HIGH);
      Motor.writeMicroseconds(MotorMaxReverse + inStep);
      break;
  }
  
  // and move to the next state if necessary
  if (millis() > next_step) {
    state++;
    if (state > 3) {
      state= 0;
    }
    last_step = next_step;
    next_step += 1000;
    
  }
  
}



  
  //initialising state of buttons
  //HIGH state means button being pressed
  //LOW state means not pressed
  magnetToggleState = digitalRead(magnetToggler);
  extendToggleState = digitalRead(extendToggler);
  retractToggleState = digitalRead(retractToggler);
  
  if(magnetToggleState == HIGH){
    //magnet = !magnet;
    //delay(500);
    cans = 3;
  }

  if(extendToggleState == HIGH) {
    //extend = !extend;
    //delay(500);
    cans = 1;
  }

  if(retractToggleState == HIGH) {
    //retract =! retract;
    //delay(500);
    cans = 2;
  }
  
  
  //----- Functions below are just active listening functions, listening to global vars------
  //if magnet is toggled on, no need for armGrab dropCan functions

  /*
  if(magnet == true) {
    armGrab();
  } else {
    dropCan();
  }

  armExtend();
  armRetract();
  */

  counter(cans);
  collectCans(cans);
  counter(cans);
  
}

void counter(int x) {
  lcd.setCursor(0,1);
  lcd.print("  Cans left: ");
  lcd.setCursor(12,1);
  lcd.print(x);
  delay(250);
}

void armGrab() {
    if(magnet == true) {
    digitalWrite(magnetLedPin, HIGH);
  } else {
    digitalWrite(magnetLedPin, LOW);
  }//digitalWrite(magnetLedPin, HIGH);
}

void dropCan() {
   digitalWrite(magnetLedPin, LOW);
}

void armExtend() {
  if(extend == true) {
    digitalWrite(extendLedPin, HIGH);
  } else {
    digitalWrite(extendLedPin, LOW);
  }
}

void armRetract() {
  if(retract == true) {
    digitalWrite(retractLedPin, HIGH);
  } else {
    digitalWrite(retractLedPin, LOW);
  }
}






 
