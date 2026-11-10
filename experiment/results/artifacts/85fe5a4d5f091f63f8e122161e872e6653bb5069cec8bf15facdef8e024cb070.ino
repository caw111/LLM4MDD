#include <Arduino.h>

static const uint8_t PIN_A  = 2;
static const uint8_t PIN_B  = 3;
static const uint8_t PIN_C  = 4;
static const uint8_t PIN_D  = 5;
static const uint8_t PIN_E  = 6;
static const uint8_t PIN_F  = 7;
static const uint8_t PIN_G  = 8;
static const uint8_t PIN_DP = 9;

static inline void setSegmentsForDigit(char ch) {
  // Common anode:
  // Segment ON  -> LOW
  // Segment OFF -> HIGH
  //
  // Order: A B C D E F G DP
  // Values indicate segments that should be ON for the digit.
  bool onA=false, onB=false, onC=false, onD=false, onE=false, onF=false, onG=false, onDP=false;

  switch (ch) {
    case '0': onA=1; onB=1; onC=1; onD=1; onE=1; onF=1; onG=0; onDP=0; break;
    case '1': onA=0; onB=1; onC=1; onD=0; onE=0; onF=0; onG=0; onDP=0; break;
    case '2': onA=1; onB=1; onC=0; onD=1; onE=1; onF=0; onG=1; onDP=0; break;
    case '3': onA=1; onB=1; onC=1; onD=1; onE=0; onF=0; onG=1; onDP=0; break;
    case '4': onA=0; onB=1; onC=1; onD=0; onE=0; onF=1; onG=1; onDP=0; break;
    case '5': onA=1; onB=0; onC=1; onD=1; onE=0; onF=1; onG=1; onDP=0; break;
    case '6': onA=1; onB=0; onC=1; onD=1; onE=1; onF=1; onG=1; onDP=0; break;
    case '7': onA=1; onB=1; onC=1; onD=0; onE=0; onF=0; onG=0; onDP=0; break;
    case '8': onA=1; onB=1; onC=1; onD=1; onE=1; onF=1; onG=1; onDP=0; break;
    case '9': onA=1; onB=1; onC=1; onD=1; onE=0; onF=1; onG=1; onDP=0; break;
    default:
      // If input isn't a digit, clear all segments.
      onA=onB=onC=onD=onE=onF=onG=onDP=false;
      break;
  }

  // Apply to pins.
  digitalWrite(PIN_A,  onA  ? LOW : HIGH);
  digitalWrite(PIN_B,  onB  ? LOW : HIGH);
  digitalWrite(PIN_C,  onC  ? LOW : HIGH);
  digitalWrite(PIN_D,  onD  ? LOW : HIGH);
  digitalWrite(PIN_E,  onE  ? LOW : HIGH);
  digitalWrite(PIN_F,  onF  ? LOW : HIGH);
  digitalWrite(PIN_G,  onG  ? LOW : HIGH);
  digitalWrite(PIN_DP, onDP ? LOW : HIGH);
}

void setup() {
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_D, OUTPUT);
  pinMode(PIN_E, OUTPUT);
  pinMode(PIN_F, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_DP, OUTPUT);

  // Default: all segments OFF.
  // Common anode: OFF => HIGH.
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_D, HIGH);
  digitalWrite(PIN_E, HIGH);
  digitalWrite(PIN_F, HIGH);
  digitalWrite(PIN_G, HIGH);
  digitalWrite(PIN_DP, HIGH);

  Serial.begin(9600);
}

void loop() {
  static unsigned long lastCharTime = 0;
  static bool gotAny = false;
  static char lastDigit = '0';

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastCharTime = millis();
    gotAny = true;

    // "Taking into account that the display is common anode, set the corresponding pins
    // ... so that the serial input is shown"
    // For one-digit display, we show the last received digit character (0-9).
    if (c >= '0' && c <= '9') {
      lastDigit = c;
      setSegmentsForDigit(lastDigit);
    } else if (c == '\n' || c == '\r') {
      // ignore line endings
    } else {
      // if non-digit received, keep lastDigit displayed unless it's not desired;
      // requirement implies showing serial input value, so clear on unknown.
      setSegmentsForDigit(c);
    }
  }

  // Line_or_idle framing, idle_timeout_ms=20:
  // After 20ms of no new characters, consider the "frame" ended.
  // If a non-digit was last received, we already updated it. If only non-digits
  // and no lastDigit, they are handled by setSegmentsForDigit in-loop.
  if (gotAny) {
    unsigned long now = millis();
    if (now - lastCharTime > 20UL) {
      gotAny = false;
      // No additional action required; last received character already shown.
    }
  }
}