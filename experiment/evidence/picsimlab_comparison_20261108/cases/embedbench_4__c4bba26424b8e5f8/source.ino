// RGB LED control sketch for Arduino Mega 2560
// Circuit wiring preserved:
// Mega GND -> RGB COM (common cathode)
// Mega D2 -> R
// Mega D3 -> G
// Mega D4 -> B

const uint8_t RGB_R_PIN = 2;
const uint8_t RGB_G_PIN = 3;
const uint8_t RGB_B_PIN = 4;

const unsigned long STEP_MS = 3000UL;

void setColor(bool rOn, bool gOn, bool bOn) {
  // Common cathode: HIGH = OFF, LOW = ON
  digitalWrite(RGB_R_PIN, rOn ? LOW : HIGH);
  digitalWrite(RGB_G_PIN, gOn ? LOW : HIGH);
  digitalWrite(RGB_B_PIN, bOn ? LOW : HIGH);
}

void setup() {
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Initialization: RGB LED off
  setColor(false, false, false);
}

void loop() {
  // Repeat indefinitely: off after each full sequence cycle
  setColor(true, false, false);   // Red
  delay(STEP_MS);

  setColor(true, true, false);    // Yellow (Red + Green)
  delay(STEP_MS);

  setColor(true, true, true);     // White (Red + Green + Blue)
  delay(STEP_MS);

  setColor(false, false, false);  // Off
  delay(STEP_MS);
}