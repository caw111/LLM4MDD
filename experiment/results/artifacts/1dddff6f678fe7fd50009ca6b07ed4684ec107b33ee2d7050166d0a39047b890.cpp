#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_65\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

enum ColorState : uint8_t {
  OFF = 0,
  RED,
  GREEN,
  BLUE,
  YELLOW,
  CYAN,
  MAGENTA,
  WHITE
};

static const uint8_t STATE_COUNT = 8; // RED..WHITE (OFF is separate in display logic)

const uint8_t BTN_PIN = 2;

// RGB LED pins (common cathode => HIGH turns channel ON)
const uint8_t RGB_R_PIN = 3;
const uint8_t RGB_G_PIN = 4;
const uint8_t RGB_B_PIN = 5;

const unsigned long MIN_PRESS_MS = 150;
const unsigned long MIN_STATE_MS = 2000;

ColorState state = OFF;

unsigned long stateStartMs = 0;

// Button press qualification (hold >= MIN_PRESS_MS)
bool pressArmed = false;
bool pressQualified = false;
unsigned long pressStartMs = 0;

#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_65\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGB(ColorState s);
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_65\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
ColorState nextState(ColorState current);
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_65\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void advanceState();
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_65\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 121 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_65\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_65\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGB(ColorState s) {
  // Ensure "off state" means all channels disabled
  switch (s) {
    case OFF:
      digitalWrite(RGB_R_PIN, LOW);
      digitalWrite(RGB_G_PIN, LOW);
      digitalWrite(RGB_B_PIN, LOW);
      break;
    case RED:
      digitalWrite(RGB_R_PIN, HIGH);
      digitalWrite(RGB_G_PIN, LOW);
      digitalWrite(RGB_B_PIN, LOW);
      break;
    case GREEN:
      digitalWrite(RGB_R_PIN, LOW);
      digitalWrite(RGB_G_PIN, HIGH);
      digitalWrite(RGB_B_PIN, LOW);
      break;
    case BLUE:
      digitalWrite(RGB_R_PIN, LOW);
      digitalWrite(RGB_G_PIN, LOW);
      digitalWrite(RGB_B_PIN, HIGH);
      break;
    case YELLOW: // Red + Green
      digitalWrite(RGB_R_PIN, HIGH);
      digitalWrite(RGB_G_PIN, HIGH);
      digitalWrite(RGB_B_PIN, LOW);
      break;
    case CYAN: // Green + Blue
      digitalWrite(RGB_R_PIN, LOW);
      digitalWrite(RGB_G_PIN, HIGH);
      digitalWrite(RGB_B_PIN, HIGH);
      break;
    case MAGENTA: // Red + Blue
      digitalWrite(RGB_R_PIN, HIGH);
      digitalWrite(RGB_G_PIN, LOW);
      digitalWrite(RGB_B_PIN, HIGH);
      break;
    case WHITE: // Red + Green + Blue
      digitalWrite(RGB_R_PIN, HIGH);
      digitalWrite(RGB_G_PIN, HIGH);
      digitalWrite(RGB_B_PIN, HIGH);
      break;
  }
}

ColorState nextState(ColorState current) {
  // Sequence: Off -> Red -> Green -> Blue -> Yellow -> Cyan -> Magenta -> White -> Off -> Repeat
  switch (current) {
    case OFF:      return RED;
    case RED:      return GREEN;
    case GREEN:    return BLUE;
    case BLUE:     return YELLOW;
    case YELLOW:  return CYAN;
    case CYAN:     return MAGENTA;
    case MAGENTA: return WHITE;
    case WHITE:   return OFF;
    default:       return OFF;
  }
}

void advanceState() {
  state = nextState(state);
  stateStartMs = millis();
  setRGB(state);
}

void setup() {
  // Button wiring: btn1 right side to GND, left side to pin 2.
  // With this configuration, we use internal pull-up and interpret pressed as LOW.
  pinMode(BTN_PIN, INPUT_PULLUP);

  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Initialization: RGB LED must start in off state
  state = OFF;
  setRGB(state);
  stateStartMs = millis();

  pressArmed = false;
  pressQualified = false;
  pressStartMs = 0;
}

void loop() {
  const unsigned long now = millis();

  // Read button (pressed => LOW due to pull-up)
  const bool pressed = (digitalRead(BTN_PIN) == LOW);

  if (pressed) {
    if (!pressArmed) {
      pressArmed = true;
      pressQualified = false;
      pressStartMs = now;
    } else {
      // qualify once it has been held long enough
      if (!pressQualified && (now - pressStartMs) >= MIN_PRESS_MS) {
        pressQualified = true;
      }
    }
  } else {
    // Button released; if it was a qualified press, count it once.
    if (pressArmed && pressQualified) {
      // Each color state must last at least 2 seconds.
      if ((now - stateStartMs) >= MIN_STATE_MS) {
        advanceState();
      }
      // If state hasn't lasted long enough, ignore the press (no extra toggles).
    }

    // Reset press tracking for next press
    pressArmed = false;
    pressQualified = false;
  }
}
