#include <Arduino.h>
#include <TM1637Display.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

TM1637Display display(8, 5);
const uint8_t buttonPins[4] = {2, 3, 4, 7};
const int prices[3] = {1, 2, 3};
int stock[3] = {2, 2, 2};
int selected = -1;
bool adminMode = false;
bool rawPressed[4] = {false, false, false, false};
bool stablePressed[4] = {false, false, false, false};
unsigned long rawChanged[4] = {0, 0, 0, 0};
unsigned long confirmStart = 0;
bool holdHandled = false;
unsigned long selectionStart = 0;
bool errorVisible = false;
unsigned long errorStart = 0;
int lastDisplayed = -32768;
char frame[64];
uint8_t frameLength = 0;
bool frameOverflow = false;
unsigned long lastByteAt = 0;

void renderDisplay(unsigned long now) {
  if (errorVisible && now - errorStart >= 200UL) errorVisible = false;
  int value;
  if (errorVisible) value = -1;
  else if (selected < 0) value = 0;
  else if (((now - selectionStart) / 1000UL) % 2UL == 0) value = selected + 1;
  else value = prices[selected];
  if (value != lastDisplayed) {
    display.showNumberDec(value, false);
    lastDisplayed = value;
  }
}

void toggleAdmin() {
  adminMode = !adminMode;
  digitalWrite(13, adminMode ? HIGH : LOW);
}

void attemptSale(unsigned long now) {
  if (adminMode || selected < 0) return;
  if (stock[selected] > 0) {
    --stock[selected];
    Serial.print("dispense:item");
    Serial.println(selected + 1);
    tone(6, 2000, 100);
    errorVisible = false;
  } else {
    errorVisible = true;
    errorStart = now;
    tone(6, 400, 200);
  }
  renderDisplay(now);
}

void processFrame() {
  frame[frameLength] = '\0';
  if (!frameOverflow && adminMode) {
    char *p = frame;
    while (*p && isspace((unsigned char)*p)) ++p;
    char *command = p;
    while (*p && !isspace((unsigned char)*p)) ++p;
    if (*p) {
      *p++ = '\0';
      while (*p && isspace((unsigned char)*p)) ++p;
      bool digits = false;
      bool valid = true;
      unsigned long amount = 0;
      while (*p && isdigit((unsigned char)*p)) {
        digits = true;
        amount = amount * 10UL + (unsigned long)(*p - '0');
        if (amount > 9999UL) { valid = false; break; }
        ++p;
      }
      while (*p && isspace((unsigned char)*p)) ++p;
      if (*p) valid = false;
      int item = -1;
      if (strcmp(command, "RESTOCK1") == 0) item = 0;
      else if (strcmp(command, "RESTOCK2") == 0) item = 1;
      else if (strcmp(command, "RESTOCK3") == 0) item = 2;
      if (valid && digits && item >= 0) {
        unsigned long total = (unsigned long)stock[item] + amount;
        stock[item] = (int)(total > 9999UL ? 9999UL : total);
      }
    }
  }
  frameLength = 0;
  frameOverflow = false;
}

void readSerial(unsigned long now) {
  if ((frameLength || frameOverflow) && now - lastByteAt >= 20UL) processFrame();
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastByteAt = millis();
    if (c == '\n' || c == '\r') {
      if (frameLength || frameOverflow) processFrame();
    } else if (frameLength < sizeof(frame) - 1) {
      frame[frameLength++] = c;
    } else {
      frameOverflow = true;
    }
  }
}

void scanButtons(unsigned long now) {
  for (uint8_t i = 0; i < 4; ++i) {
    bool pressed = digitalRead(buttonPins[i]) == LOW;
    if (pressed != rawPressed[i]) {
      rawPressed[i] = pressed;
      rawChanged[i] = now;
    }
    if (stablePressed[i] != rawPressed[i] && now - rawChanged[i] >= 20UL) {
      stablePressed[i] = rawPressed[i];
      if (i < 3) {
        if (stablePressed[i]) {
          selected = i;
          selectionStart = now;
          errorVisible = false;
          renderDisplay(now);
        }
      } else if (stablePressed[i]) {
        confirmStart = rawChanged[i];
        holdHandled = false;
      } else {
        unsigned long physicalHeld = rawChanged[i] - confirmStart;
        if (!holdHandled && physicalHeld >= 2000UL) {
          toggleAdmin();
          holdHandled = true;
        }
        if (!holdHandled) attemptSale(now);
      }
    }
  }
  if (stablePressed[3] && rawPressed[3] && !holdHandled && now - confirmStart >= 2000UL) {
    holdHandled = true;
    toggleAdmin();
  }
}

void setup() {
  for (uint8_t i = 0; i < 4; ++i) pinMode(buttonPins[i], INPUT_PULLUP);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  Serial.begin(9600);
  display.setBrightness(7);
  display.showNumberDec(0, false);
  lastDisplayed = 0;
}

void loop() {
  unsigned long now = millis();
  scanButtons(now);
  readSerial(now);
  renderDisplay(now);
}
