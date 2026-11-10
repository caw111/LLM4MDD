#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

// Wiring (resolved/authoritative)
static const uint8_t DHT_PIN = 7;     // D7
static const uint8_t TM_CLK  = 4;     // D4
static const uint8_t TM_DIO  = 5;     // D5

// DHT11
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

// TM1637
TM1637Display display(TM_CLK, TM_DIO);

// Refresh timing: every 2 seconds
static const uint32_t REFRESH_MS = 2000UL;
static uint32_t lastUpdate = 0;

// Convert float temperature to displayable int with scaling option
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int16_t formatTemperatureToIntTenths(float t);
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int16_t formatTemperatureToIntTenths(float t) {
  // For DHT11, temperature is typically integer values (e.g., 23°C),
  // but we still handle fractional safely.
  return (int16_t)lround(t * 10.0f);
}

void setup() {
  // Serial not required by requirements
  dht.begin();

  // Initialize display
  display.setBrightness(0x0f); // reasonable default brightness
  display.clear();
}

void loop() {
  uint32_t now = millis();
  if (now - lastUpdate >= REFRESH_MS) {
    lastUpdate = now;

    float h = dht.readHumidity();
    float t = dht.readTemperature(); // Celsius by default

    // If read fails, keep behavior deterministic: show 0 (or blank) rather than crashing.
    // Requirement only states to display temperature every 2 seconds; preserve that behavior.
    if (isnan(t)) {
      // Show "--" by clearing and leaving as zeros (simple deterministic output).
      // Alternatively could keep previous; clearing is deterministic and safe.
      display.clear();
      display.showNumberDec(0, false, 4, 0);
      return;
    }

    // Display temperature in °C.
    // We'll display tenths if possible: use 4 digits.
    // Examples:
    //  23.4 -> "23.4"
    //  -5.0 -> "-5.0"
    // If value doesn't fit, it will naturally overflow/clamp by library behavior;
    // DHT11 is expected to fit within 4 digits.
    int16_t t10 = formatTemperatureToIntTenths(t); // tenths of °C

    bool negative = (t10 < 0);
    if (negative) t10 = -t10;

    uint8_t digits[4] = {0, 0, 0, 0};

    // Create number for library: showNumberDecEx supports decimal point placement
    // We want one decimal place at the second digit from the right: _ _ . _ _
    // Approach: display as integer of tenths with decimal point on the tenths position.
    // For example 234 => 23.4 (decimal between digits 2 and 3 from right).
    // TM1637Display::showNumberDecEx(number, dots, length, leading_zero)
    // dots is bitmask: 0b0100 sets D3? Library uses: decimal points for each digit.
    //
    // We'll call showNumberDecEx with dots controlling the decimal point at the correct position.
    // For 4 digits: dots mask typically uses bit positions for each digit:
    // bit0->digit0, bit1->digit1, bit2->digit2, bit3->digit3.
    //
    // We want decimal point between the 2nd and 3rd digit from the left:
    // digits: [d0][d1][d2][d3], want d1.d2 with decimal at d1.
    // This corresponds to dot on digit index 1.
    const uint8_t dots = 0b0010;

    int16_t numberToShow;
    if (negative) {
      numberToShow = -(t10); // keep sign for library
    } else {
      numberToShow = t10;
    }

    // Show with one decimal place using dots mask and 4 digits length
    display.showNumberDecEx(numberToShow, dots, 4, true);

    // Optional: no explicit degree symbol required by requirements.
    // If needed, could map characters; requirements only ask to display temperature.
  }

  // No other behavior requested.
}
