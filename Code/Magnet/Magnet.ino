int magnet = 8;
// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(magnet, OUTPUT);
}
// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(magnet, HIGH); // turn the LED on (HIGH is the voltagelevel)
  delay(5000); // wait for a second
  digitalWrite(magnet, LOW); // turn the LED off by making the voltageLOW
  delay(5000); // wait for a second
}
