#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_016\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

#define CLK_PIN D4
#define DIO_PIN D5

#define BTN_MENU_PIN D2
#define BTN_OK_PIN   D3

#define DHT_PIN D7
#define DHT_TYPE DHT11

// Fix for platforms where D2/D3/D4/D5/D7 are not defined as macros.
// If they are already defined, this has no effect.
#ifndef D2
  #define D2 2
#endif
#ifndef D3
  #define D3 3
#endif
#ifndef D4
  #define D4 4
#endif
#ifndef D5
  #define D5 5
#endif
#ifndef D7
  #define D7 7
#endif

TM1637Display display(CLK_PIN, DIO_PIN);
DHT dht(DHT_PIN, DHT_TYPE);

enum MenuItem : uint8_t {
  MENU_TEMP = 0,
  MENU_HUM  = 1,
  MENU_POT  = 2
};

static const uint8_t NUM_MENU_ITEMS = 3;

// Debounce helper (active-low buttons using INPUT_PULLUP)
struct Button {
  uint8_t pin;
  bool lastStable;
  bool lastReading;
  unsigned long lastChangeMs;
  unsigned long debounceMs;

  void begin(uint8_t p, unsigned long dbMs = 35) {
    pin = p;
    debounceMs = dbMs;
    pinMode(pin, INPUT_PULLUP);
    lastStable = digitalRead(pin);
    lastReading = lastStable;
    lastChangeMs = 0;
  }

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

#line 94 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_016\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void showTemp(float tC);
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_016\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void showHum(float h);
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_016\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void showPotRaw(int raw);
#line 126 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_016\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void showMenuIndex(uint8_t idx);
#line 132 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_016\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 145 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_016\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_016\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void showNumber4(int value, bool leadingBlank, uint8_t decimalPos = 255) {
  // decimalPos: 0..3 for where dot should be placed, 255 for none
  if (decimalPos == 255) {
    display.showNumberDec(value, 4, leadingBlank);
  } else {
    // bitmask for decimal point selection (digit index 0..3 from left)
    uint8_t dots = (1u << decimalPos);
    display.showNumberDecEx(value, 4, dots, leadingBlank);
  }
}

static void showTemp(float tC) {
  int scaled = (int)lround(tC * 10.0f); // e.g., 23.4 -> 234
  bool neg = (scaled < 0);
  if (neg) scaled = -scaled;
  if (scaled > 999) scaled = 999;

  uint8_t decimalPos = 1; // digits "23.4" => dot after digit index 1
  uint8_t dots = (1u << decimalPos);
  int signedVal = neg ? -scaled : scaled;

  display.showNumberDecEx(signedVal, 4, dots, true);
}

static void showHum(float h) {
  int scaled = (int)lround(h * 10.0f); // 56.7 -> 567
  bool neg = (scaled < 0);
  if (neg) scaled = -scaled;
  if (scaled > 999) scaled = 999;

  uint8_t decimalPos = 1; // "h.h"
  uint8_t dots = (1u << decimalPos);
  int signedVal = neg ? -scaled : scaled;

  display.showNumberDecEx(signedVal, 4, dots, true);
}

static void showPotRaw(int raw) {
  if (raw < 0) raw = 0;
  if (raw > 1023) raw = 1023;
  display.showNumberDec(raw, 4, true);
}

static void showMenuIndex(uint8_t idx) {
  // TEMP -> "1", HUM -> "2", POT -> "3"
  uint8_t val = (idx % NUM_MENU_ITEMS) + 1;
  display.showNumberDec(val, 4, true);
}

void setup() {
  btnMenu.begin(BTN_MENU_PIN);
  btnOk.begin(BTN_OK_PIN);

  dht.begin();

  display.setBrightness(4);
  display.clear();

  currentItem = 0;
  showMenuIndex(currentItem);
}

void loop() {
  static bool inMenuSelection = true;

  bool menuPressed = btnMenu.update();
  bool okPressed   = btnOk.update();

  if (menuPressed) {
    if (inMenuSelection) {
      currentItem = (currentItem + 1) % NUM_MENU_ITEMS;
      showMenuIndex(currentItem);
    } else {
      // When locked on a value, D2 switches menu again
      inMenuSelection = true;
      currentItem = (currentItem + 1) % NUM_MENU_ITEMS;
      showMenuIndex(currentItem);
    }
  }

  if (okPressed) {
    inMenuSelection = false;
  }

  if (inMenuSelection) {
    showMenuIndex(currentItem);
    delay(60);
    return;
  }

  // Locked display mode: continuously show corresponding value until menu switched again
  if (currentItem == MENU_TEMP) {
    float t = dht.readTemperature(); // Celsius
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
