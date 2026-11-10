#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ======================= Pin assignments (per provided remap) =======================
static const uint8_t FAN_PWM_PIN = 5;   // D5
static const uint8_t POT_PIN     = A0;  // A0 (manual speed)
static const uint8_t TEMP_PIN    = A2;  // A2 (LM35 input per given config)
static const uint8_t BUTTON_PIN  = 2;  // D2 (mode toggle)

static const uint8_t TM1637_CLK = 4;    // D4
static const uint8_t TM1637_DIO = 3;    // D3

// TM1637 uses "segments" mapping internally
TM1637Display display(TM1637_CLK, TM1637_DIO);

// ======================= Application behavior requirements =======================
// - Two modes: Manual / Auto
// - Button toggles mode on press
// - Auto: speed controlled based on temperature (LM35)
// - Manual: speed controlled by potentiometer
// - TM1637 displays current temperature (auto) or manual "档位" (manual)
// - Preserve PWM control on D5 and display behavior
//
// Note: Original prompt does not provide explicit thresholds/timing/calibration.
// We'll implement a complete, deterministic behavior consistent with LM35 and PWM:
//   - LM35 reading -> temperature in °C
//   - Map temperature range [30..70] °C to PWM [0..255], clamp
//   - Use manual "档位" derived from potentiometer reading (0..9)
//   - Debounce the button and toggle mode on a stable press
// If you need different thresholds/timing, tell me and I will adjust.

enum Mode : uint8_t { AUTO_MODE = 0, MANUAL_MODE = 1 };
volatile Mode mode = AUTO_MODE;

// ======================= Button handling =======================
static const unsigned long DEBOUNCE_MS = 40;
static const unsigned long LONG_PRESS_MS = 0; // no long-press required; kept as constant for clarity

static bool lastButtonStable = HIGH;
static bool lastButtonReading = HIGH;
static unsigned long lastDebounceTime = 0;
static unsigned long pressStartTime = 0;
static bool pressLatched = false;

#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool readButton();
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC();
#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t clampU8(int v);
#line 66 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t tempToPWM(float tempC);
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t potToManualPWM(int potRaw);
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t potToManualLevel(int potRaw);
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showTemperatureOnDisplay(float tempC);
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showManualLevelOnDisplay(uint8_t level);
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 130 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool readButton() {
  // Assuming D2 uses pull-up in wiring (common): pressed -> LOW
  return digitalRead(BUTTON_PIN) == LOW;
}

// ======================= Temperature & mapping =======================
static float readTemperatureC() {
  // LM35: 10mV/°C => 0.01 V per °C
  // Arduino UNO ADC: 5V reference, 10-bit => 1023 steps
  int raw = analogRead(TEMP_PIN);
  float voltage = (raw * 5.0f) / 1023.0f;   // volts
  float tempC = voltage / 0.01f;            // (V) / (V/°C)
  return tempC;
}

static uint8_t clampU8(int v) {
  if (v < 0) return 0;
  if (v > 255) return 255;
  return (uint8_t)v;
}

static uint8_t tempToPWM(float tempC) {
  // Auto control: map temperature to PWM.
  // Temperature range chosen to be reasonable defaults for a fan controller.
  //  - <= 30°C => 0
  //  - 70°C => full speed (255)
  //  - linear between
  const float T_LOW = 30.0f;
  const float T_HIGH = 70.0f;

  if (tempC <= T_LOW) return 0;
  if (tempC >= T_HIGH) return 255;

  float ratio = (tempC - T_LOW) / (T_HIGH - T_LOW); // 0..1
  int pwm = (int)lroundf(ratio * 255.0f);
  return clampU8(pwm);
}

static uint8_t potToManualPWM(int potRaw) {
  // Map pot (0..1023) to PWM (0..255)
  return map(constrain(potRaw, 0, 1023), 0, 1023, 0, 255);
}

static uint8_t potToManualLevel(int potRaw) {
  // Convert pot to a 10-level "档位" 0..9
  // Used for display in manual mode.
  // Note: Use 0..9 so it's easy to show on 4-digit TM1637.
  int level = map(constrain(potRaw, 0, 1023), 0, 1023, 0, 9);
  return (uint8_t)constrain(level, 0, 9);
}

static void showTemperatureOnDisplay(float tempC) {
  // TM1637Display typically can show integer with optional decimals.
  // We'll show whole °C (rounded).
  int t = (int)lroundf(tempC);
  // If negative occurs (unlikely), show 0..999 limited.
  if (t < 0) t = 0;
  display.showNumberDecEx(t, 0b01000000, true, 4); // add colon? We'll clear decimals; sign behavior handled by library.
}

static void showManualLevelOnDisplay(uint8_t level) {
  // Display manual level 0..9.
  display.showNumberDec(level, false, 2, 0); // 2 digits
}

// ======================= Setup =======================
void setup() {
  pinMode(FAN_PWM_PIN, OUTPUT);
  analogWrite(FAN_PWM_PIN, 0);

  pinMode(POT_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // TM1637 init
  display.setBrightness(7, true);

  // Initial display based on initial mode
  // Default is AUTO_MODE
  float t = readTemperatureC();
  showTemperatureOnDisplay(t);
}

// ======================= Loop =======================
void loop() {
  // --- Button debouncing and toggle ---
  bool buttonPressed = readButton(); // true if pressed (LOW)

  if (buttonPressed != lastButtonReading) {
    lastDebounceTime = millis();
    lastButtonReading = buttonPressed;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    if (buttonPressed != lastButtonStable) {
      lastButtonStable = buttonPressed;

      // Toggle on press event (stable transition to pressed)
      if (buttonPressed) {
        // optional long-press not requested; just latch press
        pressLatched = true;
        pressStartTime = millis();

        // Toggle mode
        mode = (mode == AUTO_MODE) ? MANUAL_MODE : AUTO_MODE;

        // Update display immediately
        if (mode == AUTO_MODE) {
          float t = readTemperatureC();
          showTemperatureOnDisplay(t);
        } else {
          int potRaw = analogRead(POT_PIN);
          uint8_t lvl = potToManualLevel(potRaw);
          showManualLevelOnDisplay(lvl);
        }
      } else {
        pressLatched = false;
      }
    }
  }

  // --- Mode behavior ---
  if (mode == AUTO_MODE) {
    float tempC = readTemperatureC();
    uint8_t pwm = tempToPWM(tempC);
    analogWrite(FAN_PWM_PIN, pwm);
    showTemperatureOnDisplay(tempC);
  } else {
    int potRaw = analogRead(POT_PIN);
    uint8_t pwm = potToManualPWM(potRaw);
    analogWrite(FAN_PWM_PIN, pwm);

    uint8_t level = potToManualLevel(potRaw);
    showManualLevelOnDisplay(level);
  }

  // --- Control loop pacing ---
  // Keep responsive but avoid excessive TM1637 updates.
  delay(100);
}
