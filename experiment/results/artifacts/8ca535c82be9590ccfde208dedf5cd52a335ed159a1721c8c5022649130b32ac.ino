// Bar LED - turns on bar graph LEDs up to threshold 6 and stays on.

const uint8_t anodePins[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const uint8_t threshold = 6;

void setup() {
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(anodePins[i], OUTPUT);
  }

  // Turn on LEDs up to the threshold (pins 1..6), others off (pins 7..10).
  for (uint8_t i = 0; i < 10; i++) {
    // i corresponds to LED index (0->C1/A1, ..., 9->C10/A10)
    digitalWrite(anodePins[i], (i < threshold) ? HIGH : LOW);
  }
}

void loop() {
  // No actions required; LEDs remain in their state.
}