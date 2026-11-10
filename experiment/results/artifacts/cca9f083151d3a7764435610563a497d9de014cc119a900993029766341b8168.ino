const int ledPins[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

void setup() {
  for (int i = 0; i < 10; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(A0, INPUT);
}

void loop() {
  int photoValue = analogRead(A0);   // 1023 (bright) -> 0 (dark) per given note (smaller means brighter)
  int mappedValue = map(photoValue, 0, 1023, 10, 0); // Convert to 0..10 based on "smaller analog = brighter"

  if (mappedValue < 0) mappedValue = 0;
  if (mappedValue > 10) mappedValue = 10;

  // Turn on only LEDs up to mapped value
  for (int i = 0; i < 10; i++) {
    // LED number smaller than mapped value => HIGH
    // i corresponds to LED "1..10" as i=0 -> pin2 (LED1), so (i+1) < mappedValue
    digitalWrite(ledPins[i], ((i + 1) < mappedValue) ? HIGH : LOW);
  }
}