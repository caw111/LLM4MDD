#include <Arduino.h>
#include <TM1637Display.h>

// --- Pin assignments (authoritative wiring; Arduino UNO) ---
static const uint8_t LM35_PIN   = A1; // LM35 analog input (A1)
static const uint8_t BTN_PIN    = 2;  // D2 (remapped for UNO headers)
static const uint8_t TM1637_CLK = 4; // D4 (remapped for UNO headers)
static const uint8_t TM1637_DIO = 5; // D5 (remapped for UNO headers)

// --- Serial configuration ---
static const uint32_t BAUD = 9600;

// --- Temperature conversion (LM35: 10mV/°C) ---
// Arduino UNO ADC: 10-bit, 5V reference.
// tempC = (adc * 5.0 / 1023.0) / 0.01 = adc * (500.0 / 1023.0)
static const float ADC_TO_C = 500.0f / 1023.0f;

// --- Display / timing ---
static const uint32_t SHOW_MAX_MS = 3000; // show historical max for 3 seconds

// --- Debounce ---
static const uint16_t DEBOUNCE_MS = 30;

// --- Global state ---
TM1637Display display(TM1637_CLK, TM1637_DIO);

float currentTempC = 0.0f;
float maxTempC = -1000.0f;

bool buttonLatched = false;
uint32_t buttonPressTime = 0; // kept to preserve original behavior/structure

// Debounce tracking
bool lastButtonRaw = false;
bool buttonStableState = false;
uint32_t lastDebounceChangeMs = 0;

// Display mode control
bool showMaxActive = false;
uint32_t showMaxStartMs = 0;

static float readTemperatureC() {
  int adc = analogRead(LM35_PIN);
  return (float)adc * ADC_TO_C;
}

static void printTempsToSerial(float a, float b, bool showingMax) {
  // Required: "串口持续输出两者" (continually outputs both)
  // Format: CUR:xx.xC MAX:yy.yC MODE:CUR|MAX
  Serial.print(F("CUR:"));
  Serial.print(a, 1);
  Serial.print(F("C MAX:"));
  Serial.print(b, 1);
  Serial.print(F("C MODE:"));
  Serial.println(showingMax ? F("MAX") : F("CUR"));
}

static void displayCurrent(float tC) {
  // Display temp with 1 decimal place using dot.
  int16_t scaled = (int16_t)round(tC * 10.0f); // e.g., 23.4 -> 234
  bool negative = (scaled < 0);
  if (negative) scaled = -scaled;

  if (scaled > 9999) scaled = 9999;

  // Use dots mask: bit1 corresponds to the decimal point position for "XX.X"
  uint8_t dotMask = 1 << 1;

  if (negative) {
    // LM35 typically non-negative; preserve behavior by showing absolute value.
    display.showNumberDecEx(scaled, dotMask, true);
  } else {
    display.showNumberDecEx(scaled, dotMask, true);
  }
}

static void displayMax(float tC) {
  displayCurrent(tC);
}

static bool isButtonPressedEdge() {
  // Button D2: use INPUT_PULLUP; pressed is LOW.
  bool rawPressed = (digitalRead(BTN_PIN) == LOW);

  uint32_t now = millis();

  // Debounce update
  if (rawPressed != buttonStableState) {
    if (now - lastDebounceChangeMs >= DEBOUNCE_MS) {
      buttonStableState = rawPressed;
    }
  }

  // Track raw changes time (kept as in original structure)
  if (rawPressed != lastButtonRaw) {
    lastButtonRaw = rawPressed;
    lastDebounceChangeMs = now;
  }

  // Edge detection: rising to pressed event
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

  display.setBrightness(0x0f, true);
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
      buttonPressTime = now; // preserved variable
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

  // Pacing (still continuous)
  delay(100);
}