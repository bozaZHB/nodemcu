const int ledPin = 13; // the pin that the LED is attached to
int incomingByte;      // a variable to read incoming serial data into

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // see if there's incoming serial data:
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 'd') {
      digitalWrite(ledPin, HIGH);
      Serial.write('d');
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == 'f') {
      digitalWrite(ledPin, LOW);
      Serial.write('f');
    }
  }
}
