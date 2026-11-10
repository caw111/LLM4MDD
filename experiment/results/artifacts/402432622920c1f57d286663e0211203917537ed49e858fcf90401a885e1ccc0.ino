#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

#define CLK_PIN D4
#define DIO_PIN D5

#define BTN_MENU_PIN D2
#define BTN_OK_PIN   D3

#define DHT_PIN D7
#define DHT_TYPE DHT11

TM1637Display display(CLK_PIN, DIO_PIN);
DHT dht(DHT_PIN, DHT_TYPE);

enum MenuItem : uint8_t {
  MENU_TEMP = 0,
  MENU_HUM  = 1,
  MENU_POT  = 2
};

static const uint8_t NUM_MENU_ITEMS = 3;

// Debounce helper
struct Button {
  uint8_t pin;
  bool lastStable;
  bool lastReading;
  unsigned long lastChangeMs;

  // Tune these if necessary
  unsigned long debounceMs;

  void begin(uint8_t p, unsigned long dbMs = 35) {
    pin = p;
    debounceMs = dbMs;
    pinMode(pin, INPUT_PULLUP);
    lastStable = digitalRead(pin);
    lastReading = lastStable;
    lastChangeMs = 0;
  }

  // Returns true on a new stable press event (active-low)
  bool update() {
    bool reading = digitalRead(pin);
    unsigned long now = millis();

    if (reading != lastReading) {
      lastChangeMs = now;
      lastReading = reading;
    }

    if ((now - lastChangeMs) >= debounceMs && reading != lastStable) {
      lastStable = reading;
      // active-low press
      return (lastStable == LOW);
    }

    return false;
  }
};

Button btnMenu;
Button btnOk;

static uint8_t currentItem = 0;

// Display formatting for TM1637 (4 digits)
static void showNumber4(int value, bool leadingBlank, uint8_t decimalPos = 255) {
  // decimalPos: 0..3 for where dot should be placed, 255 for none
  // tm1637 library's showNumberDecEx allows decimal point selection via bit mask.
  // But we avoid non-standard masks; we use showNumberDecEx with correct mask.
  if (decimalPos == 255) {
    display.showNumberDec(value, 4, leadingBlank);
  } else {
    // Dot mask: bit0 for digit0, bit1 for digit1, bit2 for digit2, bit3 for digit3
    // showNumberDecEx uses "dots" bitmask where dots is a bit mask.
    uint8_t dots = (1u << decimalPos);
    display.showNumberDecEx(value, 4, dots, leadingBlank);
  }
}

static void showTemp(float tC) {
  // Format: e.g., 23.4 as 234 with decimal at position 2 (x.xx depends on value range).
  // We'll show with one decimal if possible while fitting 4 digits:
  // Multiply by 10 and place decimal before last digit (e.g., 235 => 23.5)
  int scaled = (int)lround(tC * 10.0f); // e.g., 23.4 -> 234
  // Determine sign
  bool neg = (scaled < 0);
  if (neg) scaled = -scaled;

  // Fit: scaled up to 3 digits (0..999) plus possible sign consumes a digit.
  // We'll clamp to 999.
  if (scaled > 999) scaled = 999;

  // Put decimal after two integer digits (position 1 means dot after digit index 1 from left?):
  // In tm1637 library, decimalPos is "position from left" for dots bitmask:
  // digit indices: 0..3 left->right. If we want 23.4 => digits "23.4":
  // decimal after '3' (index 1). That corresponds to dots bit at index 1.
  uint8_t decimalPos = 1;

  // showNumberDecEx uses signed value, but leading blanks depends on sign and options.
  // We'll create a signed value to allow negative display.
  int signedVal = neg ? -scaled : scaled;

  // leadingBlank=true to clean unused digits
  uint8_t dots = (1u << decimalPos);
  display.showNumberDecEx(signedVal, 4, dots, true);
}

static void showHum(float h) {
  // Show with one decimal using same scaling and decimal placement "h.h"
  // Example: 56.7 -> 56.7 (4 digits with one decimal => 567, decimal after digit 2nd from left? => index 1)
  int scaled = (int)lround(h * 10.0f); // 56.7 -> 567
  bool neg = (scaled < 0);
  if (neg) scaled = -scaled;
  if (scaled > 999) scaled = 999;

  uint8_t decimalPos = 1;
  uint8_t dots = (1u << decimalPos);
  int signedVal = neg ? -scaled : scaled;
  display.showNumberDecEx(signedVal, 4, dots, true);
}

static void showPotRaw(int raw) {
  // Show 0..1023 on 4 digits, no decimal.
  if (raw < 0) raw = 0;
  if (raw > 1023) raw = 1023;
  display.showNumberDec(raw, 4, true);
}

static void showMenuIndex(uint8_t idx) {
  // Simple 1..3 menu indicator:
  // TEMP -> "1"
  // HUM  -> "2"
  // POT  -> "3"
  // Keep it robust even when idx outside
  uint8_t val = (idx % NUM_MENU_ITEMS) + 1;
  // Show as number with leading blanks
  display.showNumberDec(val, 4, true);
}

void setup() {
  // Buttons and sensors
  btnMenu.begin(BTN_MENU_PIN);
  btnOk.begin(BTN_OK_PIN);

  dht.begin();

  // TM1637 init
  // Set brightness (0..7). Choose a typical reasonable value.
  display.setBrightness(4);
  display.clear();

  // Initial state: show menu item selector until OK is pressed
  currentItem = 0;
  showMenuIndex(currentItem);
}

void loop() {
  static bool inMenuSelection = true;

  // Read buttons (with debounce)
  bool menuPressed = btnMenu.update();
  bool okPressed   = btnOk.update();

  if (menuPressed) {
    // Switch menu item
    if (inMenuSelection) {
      currentItem = (currentItem + 1) % NUM_MENU_ITEMS;
      showMenuIndex(currentItem);
    } else {
      // When locked on a value, D2 switches menu again (as requirement: until again switch)
      inMenuSelection = true;
      currentItem = (currentItem + 1) % NUM_MENU_ITEMS;
      showMenuIndex(currentItem);
    }
  }

  if (okPressed) {
    // Confirm selected item; then continuously display its value
    inMenuSelection = false;
  }

  if (inMenuSelection) {
    // Keep selector displayed
    showMenuIndex(currentItem);
    delay(60);
    return;
  }

  // Locked display mode: continuously show corresponding value until menu switched again
  if (currentItem == MENU_TEMP) {
    float t = dht.readTemperature(); // Celsius
    // If read fails, show last known? Requirement doesn't specify; keep behavior deterministic:
    // We'll attempt again; if fails, display 0.
    if (isnan(t)) {
      showNumber4(0, true);
    } else {
      showTemp(t);
    }
    delay(300);
  } else if (currentItem == MENU_HUM) {
    float h = dht.readHumidity();
    if (isnan(h)) {
      showNumber4(0, true);
    } else {
      showHum(h);
    }
    delay(300);
  } else { // MENU_POT
    int raw = analogRead(A0);
    showPotRaw(raw);
    delay(100);
  }
}