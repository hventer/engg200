#define SENSORPIN 4

int count;
// variables will change:
int sensorState = 0;
int lastState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:      
  // initialize the sensor pin as an input:
  pinMode(SENSORPIN, INPUT_PULLUP);     
  
  Serial.begin(9600);
}
void loop(){
  // read the state of the pushbutton value:
  sensorState = digitalRead(SENSORPIN);
  
  if (sensorState) {
    Serial.println("Unbroken");
  } 
  
  if (!sensorState) {
    count++;
    Serial.print("Broken   __   ");
    Serial.println(count);
  }
  
  //lastState = sensorState;
  //Serial.println(sensorState);
}
