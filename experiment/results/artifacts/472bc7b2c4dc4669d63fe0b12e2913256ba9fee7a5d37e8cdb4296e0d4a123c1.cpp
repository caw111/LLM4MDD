#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// ---------- Pin mapping (preserve given wiring) ----------
// Button
static const uint8_t BTN_PIN = 2;

// RGB1 (common cathode)
static const uint8_t RGB1_R = 3;
static const uint8_t RGB1_G = 4;
static const uint8_t RGB1_B = 5;

// RGB2 (common cathode)
static const uint8_t RGB2_R = 6;
static const uint8_t RGB2_G = 7;
static const uint8_t RGB2_B = 8;

// Bar graph A1-A7
static const uint8_t BAR_A[7] = {9, 10, 11, 12, 13, 44, 45};

// Bar graph common cathode/return is connected to GND in circuit,
// so we only need A1-A7 pins (drive HIGH to light).

// ---------- Utilities ----------
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB_R_G_B(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool onR, bool onG, bool onB);
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBothRGB(bool r1, bool g1, bool b1, bool r2, bool g2, bool b2);
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static RGBMask colorToMask(const char c1, const char c2);
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static RGBMask red();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static RGBMask green();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static RGBMask blue();
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static RGBMask yellow();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static RGBMask cyan();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static RGBMask magenta();
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static RGBMask white();
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static RGBMask black();
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyModeColors(uint8_t mode);
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyModeBar(uint88_t mode);
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isButtonPressed();
#line 122 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleButtonIncrement();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 172 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void setRGB_R_G_B(uint8_t rPin, uint8_t gPin, uint8_t bPin,
                                  bool onR, bool onG, bool onB) {
  // common cathode: HIGH=LED OFF, LOW=LED ON
  digitalWrite(rPin, onR ? LOW : HIGH);
  digitalWrite(gPin, onG ? LOW : HIGH);
  digitalWrite(bPin, onB ? LOW : HIGH);
}

static inline void setBothRGB(bool r1, bool g1, bool b1, bool r2, bool g2, bool b2) {
  setRGB_R_G_B(RGB1_R, RGB1_G, RGB1_B, r1, g1, b1);
  setRGB_R_G_B(RGB2_R, RGB2_G, RGB2_B, r2, g2, b2);
}

// Color sequence required: (red/blue, green/cyan, yellow/magenta, cyan/yellow, meagenta/cyan, white/yellow, black/white)
// Interpret each pair as:
// - for rgb1: first color
// - for rgb2: second color
// Each color maps to RGB on/off (common cathode -> on => LOW).
enum ColorName { RED_BLUE0, GREEN_CYAN1, YELLOW_MAGENTA2, CYAN_YELLOW3, MAGENTA_CYAN4, WHITE_YELLOW5, BLACK_WHITE6 };

struct RGBMask { bool r; bool g; bool b; };

static RGBMask colorToMask(const char c1, const char c2) {
  (void)c1; (void)c2;
  // Not used
  return {false,false,false};
}

static RGBMask red()    { return {true,false,false}; }
static RGBMask green()  { return {false,true,false}; }
static RGBMask blue()   { return {false,false,true}; }
static RGBMask yellow() { return {true,true,false}; }
static RGBMask cyan()   { return {false,true,true}; }
static RGBMask magenta(){ return {true,false,true}; }
static RGBMask white()  { return {true,true,true}; }
static RGBMask black()  { return {false,false,false}; }

static void applyModeColors(uint8_t mode) {
  // Mode 0: all OFF (both RGB off)
  if (mode == 0) {
    setBothRGB(false,false,false, false,false,false);
    return;
  }

  // Modes 1-7: use the sequence order given.
  // Map:
  // Mode1:  rgb1 red   / rgb2 blue
  // Mode2:  rgb1 green / rgb2 cyan
  // Mode3:  rgb1 yellow/ rgb2 magenta
  // Mode4:  rgb1 cyan  / rgb2 yellow
  // Mode5:  rgb1 magenta / rgb2 cyan
  // Mode6:  rgb1 white / rgb2 yellow
  // Mode7:  rgb1 black / rgb2 white
  RGBMask m1r, m2r;
  switch (mode) {
    case 1: m1r = red();    m2r = blue();    break;    // red/blue
    case 2: m1r = green();  m2r = cyan();    break;    // green/cyan
    case 3: m1r = yellow(); m2r = magenta(); break;    // yellow/magenta
    case 4: m1r = cyan();   m2r = yellow();  break;    // cyan/yellow
    case 5: m1r = magenta();m2r = cyan();    break;    // meagenta/cyan (typo preserved in text)
    case 6: m1r = white();  m2r = yellow();  break;    // white/yellow
    case 7: m1r = black();  m2r = white();   break;    // black/white
    default: m1r = black();  m2r = black();   break;
  }

  setBothRGB(m1r.r, m1r.g, m1r.b,  m2r.r, m2r.g, m2r.b);
}

static void applyModeBar(uint88_t mode) {
  // Mode 0: all components OFF -> no lit LEDs
  // Modes 1-7: light number of LEDs equal to current mode
  for (uint8_t i = 0; i < 7; i++) {
    uint8_t pin = BAR_A[i];
    if (mode >= (i + 1)) {
      digitalWrite(pin, HIGH);  // bar LEDs on (A1-A7 => HIGH)
    } else {
      digitalWrite(pin, LOW);   // off
    }
  }
}

// ---------- Button logic ----------
static uint8_t mode = 0;

static const unsigned long HOLD_MS = 150;
static const unsigned long DEBOUNCE_MS = 20;

static bool btnPrevPressed = false;
static unsigned long pressStartMs = 0;
static unsigned long lastHandledMs = 0;

static bool isButtonPressed() {
  // Wiring: mega:2 -> btn1:1.l, mega:GND -> btn1:2.r
  // With typical pushbutton: when pressed, pin is connected to GND.
  // We'll use INPUT_PULLUP and treat LOW as pressed.
  return digitalRead(BTN_PIN) == LOW;
}

static void handleButtonIncrement() {
  bool pressed = isButtonPressed();
  unsigned long now = millis();

  if (pressed && !btnPrevPressed) {
    // Just started pressing: record time
    pressStartMs = now;
  }

  if (pressed) {
    // If held long enough, register a valid press once
    if ((now - pressStartMs) >= HOLD_MS && (now - lastHandledMs) >= DEBOUNCE_MS) {
      lastHandledMs = now;

      // Increment mode by 1; after 7 presses (mode 7), next wraps to 0
      mode++;
      if (mode > 7) mode = 0;

      // Wait until release to avoid multiple triggers while held
      while (isButtonPressed()) {
        delay(1);
        // Keep mode update atomic enough; do not change anything else
      }
    }
  }

  btnPrevPressed = pressed;
}

void setup() {
  // Button input
  pinMode(BTN_PIN, INPUT_PULLUP);

  // RGB pins output
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  // Bargraph pins output
  for (uint8_t i = 0; i < 7; i++) pinMode(BAR_A[i], OUTPUT);

  // Initialization: OFF state
  mode = 0;
  applyModeColors(mode);
  applyModeBar(mode);
}

void loop() {
  handleButtonIncrement();

  // Apply current mode outputs (ensures synchronous updates with transitions)
  applyModeColors(mode);
  applyModeBar(mode);

  delay(2);
}
