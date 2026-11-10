#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// --- Pin assignments (per authoritative wiring) ---
static const uint8_t LM35_PIN = A1;      // LM35 analog input (A1)
static const uint8_t BTN_PIN  = D2;      // Button (D2)
static const uint8_t TM1637_CLK = D4;    // TM1637 CLK (D4)
static const uint8_t TM1637_DIO = D5;    // TM1637 DIO (D5)

// --- Serial configuration ---
static const uint32_t BAUD = 9600;

// --- Temperature conversion (LM35: 10mV/°C) ---
// Arduino UNO ADC: 10-bit, 5V reference.
// tempC = (adc * 5.0 / 1023.0) / 0.01 = adc * (500.0 / 1023.0)
static const float ADC_TO_C = 500.0f / 1023.0f;

// --- Display / timing ---
static const uint32_t SHOW_MAX_MS = 3000;  // show historical max for 3 seconds

// --- Debounce ---
static const uint16_t DEBOUNCE_MS = 30;

// --- Global state ---
TM1637Display display(TM1637_CLK, TM1637_DIO);

float currentTempC = 0.0f;
float maxTempC = -1000.0f;

bool buttonLatched = false;
uint32_t buttonPressTime = 0;

// Debounce tracking
bool lastButtonRaw = false;
bool buttonStableState = false;
uint32_t lastDebounceChangeMs = 0;

// Display mode control
bool showMaxActive = false;
uint32_t showMaxStartMs = 0;

#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void printTempsToSerial(float a, float b, bool showingMax);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayCurrent(float tC);
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayMax(float tC);
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonRaw();
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isButtonPressedEdge();
#line 129 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 145 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC() {
  int adc = analogRead(LM35_PIN);
  return (float)adc * ADC_TO_C;
}

static void printTempsToSerial(float a, float b, bool showingMax) {
  // Required: "串口持续输出两者" (continually outputs both)
  // Also include which one is currently on display.
  // Format: CUR:xx.xC MAX:yy.yC MODE:CUR|MAX
  Serial.print(F("CUR:"));
  Serial.print(a, 1);
  Serial.print(F("C MAX:"));
  Serial.print(b, 1);
  Serial.print(F("C MODE:"));
  Serial.println(showingMax ? F("MAX") : F("CUR"));
}

static void displayCurrent(float tC) {
  // Display temperature with 1 decimal place if possible.
  // TM1637Display::showNumberDecEx supports decimals via the dot parameter.
  // We'll use "xx.x" by scaling by 10 and setting one decimal point.
  int16_t scaled = (int16_t)round(tC * 10.0f); // e.g., 23.4 -> 234
  bool negative = (scaled < 0);
  if (negative) scaled = -scaled;

  // Limit to displayable range (TM1637 typical 4 digits). We'll clamp.
  if (scaled > 9999) scaled = 9999;

  // showNumberDecEx expects integer value in units of what to show.
  // For one decimal place with dot on the right place:
  // Value: scaled (e.g., 234 => "23.4") and dot position: 2 (from right) -> using TM1637 seg dots.
  // dots mapping in library: 0..3 corresponding to digit positions (rightmost=0).
  // We'll set the dot after 1st least significant decimal digit => digit position 1? 
  // For "23.4" on 4 digits, digits are [2][3][.][4] => dot at position 1 (from right) is commonly used.
  // Use TM1637Display::setSegments not available; rely on showNumberDecEx with mask for dots.
  uint8_t dotMask = 0x10; // typical: 0b00010000 = dot at digit 2 (from left) in many examples
  // To avoid library-dot mismatch across versions, we instead use showNumberDecEx with the "dots" argument.
  // Most TM1637Display implementations: bit0=DP of digit0 (rightmost), bit3=leftmost.
  // So for "23.4" dot between 3 and 4 => digit position 1 from right -> bit1.
  dotMask = 1 << 1;

  if (negative) {
    // Negative: showNumberDecEx can't easily do '-'. We'll just display absolute value;
    // LM35 assumed non-negative in typical usage. Keep behavior consistent.
    display.showNumberDecEx(scaled, dotMask, true);
  } else {
    display.showNumberDecEx(scaled, dotMask, true);
  }
}

static void displayMax(float tC) {
  displayCurrent(tC);
}

static bool readButtonRaw() {
  // Button D2: assume pull-down? Not stated. We'll use INPUT_PULLUP for safer default.
  // If wiring is pull-up, this will work; if pull-down, invert logic may be needed.
  // Since "preserve all requirements and wiring" but no explicit resistor wiring is given,
  // use INPUT_PULLUP and treat LOW as pressed (typical).
  return digitalRead(BTN_PIN) == HIGH;
}

static bool isButtonPressedEdge() {
  // Detect falling edge (from HIGH to LOW) with debounce.
  bool rawPressed = (digitalRead(BTN_PIN) == LOW);

  uint32_t now = millis();
  // Update debounce
  if (rawPressed != buttonStableState) {
    // If changed, start debounce timer
    if (now - lastDebounceChangeMs >= DEBOUNCE_MS) {
      buttonStableState = rawPressed;
    }
  }
  // Track raw changes time
  if (rawPressed != lastButtonRaw) {
    lastButtonRaw = rawPressed;
    lastDebounceChangeMs = now;
  }

  // Edge detection: pressed -> true event
  static bool prevStablePressed = false;
  bool event = (!prevStablePressed && buttonStableState);
  prevStablePressed = buttonStableState;
  return event;
}

void setup() {
  pinMode(LM35_PIN, INPUT);

  // Button with pull-up assumption
  pinMode(BTN_PIN, INPUT_PULLUP);

  Serial.begin(BAUD);

  display.setBrightness(0x0f, true); // max brightness, typical
  display.showNumberDecEx(0, 0, true);

  // Initialize temps
  currentTempC = readTemperatureC();
  maxTempC = currentTempC;
}

void loop() {
  // Read current temperature continuously
  currentTempC = readTemperatureC();
  if (currentTempC > maxTempC) {
    maxTempC = currentTempC;
  }

  uint32_t now = millis();

  // Handle button press: show historical max for 3 seconds, then restore current
  if (!showMaxActive) {
    if (isButtonPressedEdge()) {
      showMaxActive = true;
      showMaxStartMs = now;
      buttonLatched = true;
    }
  } else {
    if (now - showMaxStartMs >= SHOW_MAX_MS) {
      showMaxActive = false;
      buttonLatched = false;
    }
  }

  // Update display
  if (showMaxActive) {
    displayMax(maxTempC);
  } else {
    displayCurrent(currentTempC);
  }

  // Serial continuous output of both current and max
  printTempsToSerial(currentTempC, maxTempC, showMaxActive);

  // Avoid flooding serial too aggressively; still "continuous"
  // Respect idle_timeout_ms with some pacing.
  delay(100);
}
