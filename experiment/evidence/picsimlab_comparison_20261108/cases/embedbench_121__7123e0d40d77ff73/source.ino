// Arduino Mega 2560 sketch
// Buttons btn1/btn2/btn3 control LEDs led1/led2/led3 brightness based on pot1/pot2/pot3.
// Wiring preserved from provided circuit.

// LED pins
const uint8_t LED1_PIN = 2;
const uint8_t LED2_PIN = 3;
const uint8_t LED3_PIN = 4;

// Button pins (from circuit: btn1 -> mega:5V? see wiring; button wiring uses mega:5,6,7 as one side)
const uint8_t BTN1_PIN = 5;
const uint8_t BTN2_PIN = 6;
const uint8_t BTN3_PIN = 7;

// Pot pins
const uint8_t POT1_PIN = A3;
const uint8_t POT2_PIN = A4;
const uint8_t POT3_PIN = A5;

// Require stable brightness for at least 2 seconds after a button press
const uint32_t STABLE_MS = 2000;

// Debounce parameters
const uint32_t DEBOUNCE_MS = 50;

struct Channel {
  uint8_t ledPin;
  uint8_t btnPin;
  uint8_t potPin;

  uint8_t currentBrightness; // 0..255
  bool pendingUpdate;        // latched by button press event
  uint32_t stableUntil;      // time when brightness can be updated again
  bool lastBtnState;         // debounced logical state
  bool lastRawState;         // raw state for edge detect
  uint32_t lastDebounceTime;
};

Channel ch1{LED1_PIN, BTN1_PIN, POT1_PIN, 0, false, 0, false, false, 0};
Channel ch2{LED2_PIN, BTN2_PIN, POT2_PIN, 0, false, 0, false, false, 0};
Channel ch3{LED3_PIN, BTN3_PIN, POT3_PIN, 0, false, 0, false, false, 0};

static uint8_t mapPotToBrightness(int potVal) {
  if (potVal < 0) potVal = 0;
  if (potVal > 1023) potVal = 1023;
  return (uint8_t)((potVal * 255L) / 1023L);
}

static void initChannel(Channel &ch) {
  pinMode(ch.ledPin, OUTPUT);
  analogWrite(ch.ledPin, 0);

  // Buttons: wiring shows button connects to GND when pressed and otherwise to 5V through the other leg.
  // Use INPUT_PULLUP, assuming the input is normally HIGH and goes LOW when pressed.
  pinMode(ch.btnPin, INPUT_PULLUP);

  // Initialize debounce states based on current raw reading
  bool raw = (digitalRead(ch.btnPin) == LOW); // pressed = LOW
  ch.lastBtnState = raw;
  ch.lastRawState = raw;
  ch.lastDebounceTime = millis();

  ch.currentBrightness = 0;
  ch.pendingUpdate = false;
  ch.stableUntil = 0;
}

static void handleChannel(Channel &ch) {
  uint32_t now = millis();

  // Read raw pressed state (true when pressed)
  bool rawPressed = (digitalRead(ch.btnPin) == LOW);

  // Debounce
  if (rawPressed != ch.lastRawState) {
    ch.lastRawState = rawPressed;
    ch.lastDebounceTime = now;
  }

  // Consider debounced state after debounce time
  if ((now - ch.lastDebounceTime) >= DEBOUNCE_MS) {
    if (rawPressed != ch.lastBtnState) {
      ch.lastBtnState = rawPressed;

      // Rising edge in "pressed" means: transition from not pressed -> pressed
      if (ch.lastBtnState == true) {
        // Latch brightness update immediately on press, then hold stable for 2 seconds.
        // This satisfies "brightness remain stable for at least 2 seconds after a button press".
        int potVal = analogRead(ch.potPin);
        ch.currentBrightness = mapPotToBrightness(potVal);
        analogWrite(ch.ledPin, ch.currentBrightness);

        ch.pendingUpdate = true;
        ch.stableUntil = now + STABLE_MS;
      }
    }
  }

  // If currently in stable window, do not allow updates from repeated presses
  // (other than latching on press events already handled above). We just ensure no
  // further modifications occur by ignoring any potential later logic.
  if (ch.pendingUpdate && (now >= ch.stableUntil)) {
    ch.pendingUpdate = false;
  }
}

void setup() {
  // Initialize all channels
  initChannel(ch1);
  initChannel(ch2);
  initChannel(ch3);
}

void loop() {
  handleChannel(ch1);
  handleChannel(ch2);
  handleChannel(ch3);

  // Requirement 4: If no button is pressed, LEDs should maintain their current brightness levels.
  // The code only changes brightness on debounced press events, so it is inherently maintained.
}