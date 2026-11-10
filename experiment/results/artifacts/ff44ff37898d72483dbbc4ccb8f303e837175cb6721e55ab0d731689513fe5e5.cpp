#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT11

// TM1637 pins
#define TM1637_CLK 4
#define TM1637_DIO 5

// Buttons
#define BTN_MENU 2   // D2: switch menu item
#define BTN_OK   3   // D3: confirm

// DHT
DHT dht(DHTPIN, DHTTYPE);

// TM1637
TM1637Display display(TM1637_CLK, TM1637_DIO);

// Simple menu states
enum MenuState : uint8_t {
  SHOW_TEMP = 0,
  SHOW_HUM  = 1,
  SHOW_POT  = 2
};

volatile bool btnMenuEvent = false;
volatile bool btnOkEvent   = false;

static const uint8_t NUM_ITEMS = 3;

static MenuState currentMenu = SHOW_TEMP;
static MenuState activeShow  = SHOW_TEMP;

// Debounce parameters
static const unsigned long DEBOUNCE_MS = 180;
unsigned long lastMenuPressMs = 0;
unsigned long lastOkPressMs   = 0;

// For non-blocking display updates
unsigned long lastSensorMs = 0;
unsigned long sensorIntervalMs = 1000; // reasonable refresh for DHT11

float lastTempC = NAN;
float lastHum = NAN;
int lastPot = 0;

#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void updatePot();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void displayPotScaled();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void displayTemperatureC();
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void displayHumidity();
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void displayMenuCode(MenuState s);
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void readDHTIfDue();
#line 124 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 144 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void updatePot() {
  lastPot = analogRead(A0);
}

// Helper: display pot as integer 0-1023 scaled to 3-4 digits with decimals not needed.
// TM1637 has limited digits; show as 3-digit value by scaling to 0-999.
void displayPotScaled() {
  int scaled = (lastPot * 999 + 511) / 1023; // rounded scaling to 0..999
  display.showNumberDecEx(scaled, 0b01000000, true, 3, 0); // leading zeros off, 3 digits
  // Note: no decimal point usage; D7 reserved for DHT
}

// Helper: show temperature with one decimal place (e.g., 23.4)
// DHT11 provides integer-ish values; we still show one decimal by multiplying.
void displayTemperatureC() {
  if (isnan(lastTempC)) {
    // show placeholder
    display.showNumberDecEx(0, 0, true, 3, 0);
    return;
  }
  int scaled = (int)round(lastTempC * 10.0); // 1 decimal
  // show -99.9..999.? by using 3 digits and decimal point (4th digit not used due to 3-digit command)
  // Use showNumberDecEx with decimal point at right place:
  // For 3 digits with dp: use digitSegments accordingly.
  // We'll display with 3 digits and decimal point in middle:
  // Example: 23.4 => digits "234" with dp after first digit => 0b01000000 for dp at position 2.
  // But TM1637Display library's flag: 0b01000000 sets decimal point for digit 2 (rightmost? depends on lib)
  // We'll follow typical usage for TM1637Display: 0b01000000 sets decimal point on second digit.
  int absScaled = abs(scaled);
  int hundreds = absScaled / 100;
  int tensOnes = absScaled % 100;
  int tens = tensOnes / 10;
  int ones = tensOnes % 10;

  int number3 = hundreds * 100 + tens * 10 + ones;

  uint8_t flags = 0b01000000; // enable decimal point at digit 2
  if (scaled < 0) flags |= 0x40; // library uses this bit for leading minus in some configs
  // Use showNumberDecEx: value, dots, leading zeros, length, zeroblanks
  display.showNumberDecEx(scaled, flags, true, 3, 0);
}

// Helper: show humidity with integer percent (0..100)
void displayHumidity() {
  if (isnan(lastHum)) {
    display.showNumberDecEx(0, 0, true, 3, 0);
    return;
  }
  int humInt = (int)round(lastHum);
  display.showNumberDecEx(humInt, 0, true, 3, 0);
}

// Map menu index to display indicators during selection.
// Use three digits "tH " etc is not possible reliably; instead show numeric codes:
// 0: temp, 1: hum, 2: pot
void displayMenuCode(MenuState s) {
  int code = (s == SHOW_TEMP) ? 0 : (s == SHOW_HUM) ? 1 : 2;
  // Show code with no decimal point
  display.showNumberDecEx(code, 0, true, 1, 0);
}

void readDHTIfDue() {
  unsigned long now = millis();
  if (now - lastSensorMs < sensorIntervalMs) return;
  lastSensorMs = now;

  // DHT11 read (blocking internally, but interval is 1s)
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  if (!isnan(h)) lastHum = h;
  if (!isnan(t)) lastTempC = t;
}

void setup() {
  pinMode(BTN_MENU, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);

  dht.begin();

  display.setBrightness(0x0f);
  display.clear();

  currentMenu = SHOW_TEMP;
  activeShow = SHOW_TEMP;

  // Initial sensor values
  updatePot();
  readDHTIfDue();

  // Show initial menu code until first confirm
  displayMenuCode(currentMenu);
}

void loop() {
  // Polling with debounce (interrupts avoided for simplicity and preserving behavior).
  unsigned long now = millis();

  // D2: switch menu item
  if (digitalRead(BTN_MENU) == LOW) {
    if (now - lastMenuPressMs >= DEBOUNCE_MS) {
      lastMenuPressMs = now;
      currentMenu = (MenuState)((currentMenu + 1) % NUM_ITEMS);
      displayMenuCode(currentMenu);
    }
    // Wait for release to avoid repeated presses
    while (digitalRead(BTN_MENU) == LOW) { delay(1); }
  }

  // D3: confirm
  if (digitalRead(BTN_OK) == LOW) {
    if (now - lastOkPressMs >= DEBOUNCE_MS) {
      lastOkPressMs = now;
      activeShow = currentMenu;
      // After confirmation, continue showing the selected value
      // No extra action; we'll update value below.
    }
    while (digitalRead(BTN_OK) == LOW) { delay(1); }
  }

  // Update and display according to active selection
  updatePot();
  readDHTIfDue();

  switch (activeShow) {
    case SHOW_TEMP:
      displayTemperatureC();
      break;
    case SHOW_HUM:
      displayHumidity();
      break;
    case SHOW_POT:
      displayPotScaled();
      break;
  }

  // small delay to limit TM1637 updates
  delay(50);
}
