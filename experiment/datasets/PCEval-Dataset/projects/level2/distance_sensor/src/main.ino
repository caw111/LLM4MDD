/**
  Measure distance with Ultrasonic distance sensor.
  3 APR 2025 by Blind Author
*/

#define PIN_TRIG 3
#define PIN_ECHO 2

void setup() {
  Serial.begin(9600);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
}

void loop() {
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  int duration = pulseIn(PIN_ECHO, HIGH);
  int centimeter = duration / 58;

  Serial.println("Enter a distance in cm to compare: ");
  delay(200);
  if (Serial.available() > 0) {
    int inputNumber = Serial.parseInt();
    if (inputNumber > centimeter) {
      Serial.println("Measured distance is less");
    } else if (inputNumber < centimeter) {
      Serial.println("Measured distance is greater");
    } else {
      Serial.println("Measured distance is equal to the target");
    }
  }
}