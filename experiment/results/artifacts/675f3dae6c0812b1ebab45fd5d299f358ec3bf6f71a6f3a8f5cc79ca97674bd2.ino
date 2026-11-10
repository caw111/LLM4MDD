#include <Servo.h>

const byte BTN1_PIN = 2;  // per wiring: mega:2 -> btn1:1.l
const byte SERVO1_PWM_PIN = 3; // mega:3 -> servo1:PWM
const byte SERVO2_PWM_PIN = 4; // mega:4 -> servo2:PWM

// Debounce settings
const unsigned long DEBOUNCE_MS = 50;

Servo servo1;
Servo servo2;

volatile bool lastButtonStableHigh = false;
int lastButtonRead = LOW;
unsigned long lastDebounceTime = 0;

int targetPos1 = 0;
int targetPos2 = 0;

int currentPos1 = 0;
int currentPos2 = 0;

// Smooth movement step timing
const unsigned long MOVE_INTERVAL_MS = 12;
unsigned long lastMoveTime = 0;

bool toggleState = false; 
// false: servo1=0 servo2=180
// true : servo1=180 servo2=0

void setToggleTargets(bool state) {
  toggleState = state;
  if (!toggleState) {
    // servo1 at 0, servo2 at 180
    targetPos1 = 0;
    targetPos2 = 180;
  } else {
    // servo1 at 180, servo2 at 0
    targetPos1 = 180;
    targetPos2 = 0;
  }
}

void setup() {
  servo1.attach(SERVO1_PWM_PIN);
  servo2.attach(SERVO2_PWM_PIN);

  // Initialization: both servos set to 0°
  currentPos1 = 0;
  currentPos2 = 0;
  targetPos1 = 0;
  targetPos2 = 0;
  servo1.write(currentPos1);
  servo2.write(currentPos2);

  // Button with pullup (wiring: button between mega:2 and GND; one terminal to 5V in circuit)
  // Circuit indicates:
  //  - mega:GND -> btn1:2.r
  //  - mega:2   -> btn1:1.l
  //  - btn1: color only
  // In Wokwi, this is typically wired so pressing connects the signal to GND.
  // We'll enable internal pull-up so unpressed is HIGH, pressed is LOW.
  pinMode(BTN1_PIN, INPUT_PULLUP);

  int initialRead = digitalRead(BTN1_PIN);
  lastButtonRead = initialRead;
  lastDebounceTime = millis();
  lastButtonStableHigh = (initialRead == HIGH);
}

void loop() {
  // --- Debounced button press detection (detect transition to pressed state) ---
  int reading = digitalRead(BTN1_PIN);

  if (reading != lastButtonRead) {
    lastDebounceTime = millis();
    lastButtonRead = reading;
  }

  if ((millis() - lastDebounceTime) >= DEBOUNCE_MS) {
    bool stableHigh = (reading == HIGH);

    // Button pressed when stableHigh == false (because INPUT_PULLUP)
    // Trigger only on a press event: stableHigh transitions from true to false.
    if (lastButtonStableHigh && !stableHigh) {
      // Toggle positions on each button press
      if (servo1.read() == 0) {
        // If at 0/180, toggle to 180/0
        setToggleTargets(true);
      } else {
        // Otherwise toggle to 0/180
        setToggleTargets(false);
      }
    }

    lastButtonStableHigh = stableHigh;
  }

  // --- Smooth servo movement toward targets ---
  unsigned long now = millis();
  if (now - lastMoveTime >= MOVE_INTERVAL_MS) {
    lastMoveTime = now;

    if (currentPos1 < targetPos1) currentPos1++;
    else if (currentPos1 > targetPos1) currentPos1--;

    if (currentPos2 < targetPos2) currentPos2++;
    else if (currentPos2 > targetPos2) currentPos2--;

    servo1.write(currentPos1);
    servo2.write(currentPos2);
  }
}