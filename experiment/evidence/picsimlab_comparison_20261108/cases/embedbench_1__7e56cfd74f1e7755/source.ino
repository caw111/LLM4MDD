// Arduino Mega 2560
// LED wiring:
// led1: anode -> Mega pin 2, cathode -> GND
// led2: anode -> Mega pin 3, cathode -> GND

const uint8_t led1Pin = 2;
const uint8_t led2Pin = 3;

unsigned long startMillis;

void setBothOff() {
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
}

void setup() {
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  setBothOff();
  startMillis = millis();
}

void loop() {
  unsigned long elapsed = millis() - startMillis;

  // Sequence repeats every 20 seconds:
  // 0-5s: both off
  // 5-10s: led1 on, led2 off
  // 10-15s: led1 on, led2 on
  // 15-20s: both off
  unsigned long phase = elapsed % 20000UL;

  if (phase < 5000UL) {
    setBothOff();
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
  } else if (phase < 10000UL) {
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, LOW);
  } else if (phase < 15000UL) {
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, HIGH);
  } else {
    setBothOff();
  }
}