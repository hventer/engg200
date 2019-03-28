#include <Servo.h>  //This comes with the Arduino
#define enA 5 //white
#define in1 3 //blue
#define in2 4

int motorMax = 255;

Servo MotorArm;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  Serial.begin(9600);
  // Reads the initial state of the outputA
}

void loop() {
  analogWrite(enA, 255);
  delay(1500);
  analogWrite(enA, 0);
  delay(1000);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(1000);
  analogWrite(enA, 255);
  delay(1000);
  analogWrite(enA, 0);
  delay(1000);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(1000);

  //run()

}
