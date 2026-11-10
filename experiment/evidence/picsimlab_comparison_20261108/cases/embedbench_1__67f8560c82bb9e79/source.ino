const byte LED1_PIN = 2;
const byte LED2_PIN = 3;

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Both LEDs off on power-up/reset
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
}

void loop() {
  static unsigned long startMs = 0;
  if (startMs == 0) startMs = millis();

  unsigned long elapsed = millis() - startMs;

  // Sequence repeats after a 20-second cycle:
  // 0-5s: both off
  // 5-10s: LED1 on
  // 10-15s: LED2 on (LED1 remains on)
  // 15-20s: both off simultaneously
  unsigned long phase = elapsed % 20000UL;

  if (phase < 5000UL) {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
  } else if (phase < 10000UL) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);
  } else if (phase < 15000UL) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
  } else {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
  }
}