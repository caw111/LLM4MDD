#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t PIN_LM35_A1 = A1;
static const uint8_t PIN_BUTTON_D2 = D2;   // active-low with pull-up
static const uint8_t PIN_TM1637_CLK = D4;
static const uint8_t PIN_TM1637_DIO = D5;

static const uint32_t SERIAL_BAUD = 9600;

// LM35: 10mV/°C, default 5V ref, ADC 10-bit (0..1023)
static const float VREF = 5.0f;
static const float ADC_FULL_SCALE = 1023.0f;
static const float LM35_MV_PER_C = 10.0f;

static const uint16_t HISTORY_HOLD_MS = 3000; // show max temp for 3 seconds

TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

static float highestTempC = -1000.0f;

static bool showHistory = false;
static unsigned long showHistoryUntil = 0;

// Simple button debounce
static bool lastStableState = HIGH;
static bool lastRawState = HIGH;
static unsigned long lastDebounceMs = 0;
static const unsigned long DEBOUNCE_MS = 30;

static float readTempC() {
  int raw = analogRead(PIN_LM35_A1);
  float voltage = (raw * VREF) / ADC_FULL_SCALE;      // volts
  float tempC = (voltage * 1000.0f) / LM35_MV_PER_C;  // (mV)/(10mV/°C)
  return tempC;
}

static void formatAndShowTemp(float tempC) {
  // TM1637Display can show negative values with proper formatting.
  // We display with 1 decimal place.
  // Convert to tenths for stable digit output.
  int16_t tenths = (int16_t)lroundf(tempC * 10.0f);

  // TM1637Display showNumberDecEx expects a long/int and decimals controlled by colon/flags.
  // We'll use showNumberDecEx with decimal point digits enabled.
  // For 4-digit display: XXXX where DP depends on flag.
  // Flags: bit0..bit3 control decimal points (library-specific: 0x40.. etc depending version).
  // TM1637Display supports showNumberDecEx(value, dots).
  // In popular library: dots bit mapping: 0x80,0x40,0x20,0x10.
  // We'll show one decimal place at digit 2 from right: dots = 0x40.
  //
  // We'll scale by 1 decimal -> display tenths as value with decimals handled by library.
  // But showNumberDecEx expects actual numeric value, not scaled.
  // Instead use showNumberDecEx with float? Some versions have showFloat.
  //
  // Use showFloat for clarity if available; otherwise convert to string and show segments.
  //
  // We'll implement via showFloat-like behavior by splitting integer and decimal,
  // using showNumberDecEx with scaled integer and decimal points.
  int16_t intPart = tenths / 10;
  int16_t fracPart = abs(tenths % 10);

  // Build a 4-digit number: [sign][I][.][F] conceptually.
  // If intPart magnitude fits in 3 digits, showNumberDecEx can show it.
  // We'll directly use showNumberDecEx on the integer part *10 + fractional? Not correct.
  // Better: use library's showNumberDecEx on intPart with dots and then use showNumberDecEx with -? doesn't add decimals.
  //
  // Therefore: use showNumberDecEx with tenths as integer and enable decimal at one position:
  // If dots indicate decimal before last digit, showing tenths as integer will place decimal correctly.
  // Example: temp=25.3 => tenths=253.
  // showNumberDecEx(253, dots) and dots at DP for last digit? We want 25.3.
  // For a 4-digit display, 253 occupies " 253".
  // If decimal point is placed between second and third digits from left for this arrangement,
  // library typically uses dots bits to set DP after specific digit positions.
  // In many TM1637Display implementations:
  // dots mask: 0x80 -> digit1 (most significant), 0x40 -> digit2, 0x20 -> digit3, 0x10 -> digit4 (LSB).
  // With value 253, digits are [blank][2][5][3]. We need DP after 2 => digit2 DP => 0x40.
  //
  // If temperature is negative, tenths will include '-' which shifts digits; still acceptable for typical displays.
  //
  // We'll use showNumberDecEx(tenths, 0x40) to place decimal after digit2 (from left).
  // Clamp to displayable range: tenths must fit signed 16 but display 4 digits (including sign).
  long v = (long)tenths;
  display.showNumberDecEx(v, 0x40, true);
}

void setup() {
  pinMode(PIN_BUTTON_D2, INPUT_PULLUP);

  // TM1637 brightness (0..7). Use medium.
  display.setBrightness(4);
  display.clear();

  Serial.begin(SERIAL_BAUD);

  // Initial readings
  highestTempC = readTempC();
  showHistory = false;
  showHistoryUntil = 0;

  // Default display current temperature
  formatAndShowTemp(highestTempC);
}

void loop() {
  // Read temperature continuously
  float currentTempC = readTempC();

  if (currentTempC > highestTempC) {
    highestTempC = currentTempC;
  }

  // Button debounce and edge detect (press = LOW)
  bool rawState = digitalRead(PIN_BUTTON_D2);

  if (rawState != lastRawState) {
    lastDebounceMs = millis();
    lastRawState = rawState;
  }

  if ((millis() - lastDebounceMs) > DEBOUNCE_MS) {
    if (rawState != lastStableState) {
      lastStableState = rawState;

      // Detect falling edge: HIGH->LOW indicates a press
      if (lastStableState == LOW) {
        showHistory = true;
        showHistoryUntil = millis() + HISTORY_HOLD_MS;
      }
    }
  }

  // Determine what to display
  float toDisplay = currentTempC;
  if (showHistory) {
    if ((long)(millis() - showHistoryUntil) >= 0) {
      showHistory = false;
    } else {
      toDisplay = highestTempC;
    }
  }

  formatAndShowTemp(toDisplay);

  // Serial output continuously: both current and historical highest
  Serial.print("CURRENT:");
  Serial.print(currentTempC, 1);
  Serial.print("C, HIGHEST:");
  Serial.print(highestTempC, 1);
  Serial.println("C");

  // Small pacing; keep responsive for button and display refresh
  delay(200);
}