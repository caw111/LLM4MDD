#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

// Buttons connect their assigned pins to GND when pressed.
const uint8_t ITEM_PINS[3] = {2, 3, 4};
const uint8_t CONFIRM_PIN = 7;
const uint8_t TM_CLK_PIN = 8;
const uint8_t TM_DIO_PIN = 5;
const uint8_t BUZZER_PIN = 6;
const uint8_t LED_PIN = 13;

const uint32_t DEBOUNCE_MS = 30;
const uint32_t LONG_PRESS_MS = 1500;
const uint32_t SERIAL_IDLE_MS = 20;
const uint32_t ERROR_DISPLAY_MS = 2000;

// Prices are in cents. Display format: item number followed by X.XX.
// Example: item 2 priced at 150 cents displays "21.50".
const uint16_t prices[3] = {100, 150, 200};
uint16_t inventory[3] = {5, 5, 5};

int8_t selectedItem = -1;
bool adminMode = false;

struct Button {
  uint8_t pin;
  bool raw;
  bool stable;
  bool pressed;
  bool released;
  uint32_t changedAt;
};

Button itemButtons[3];
Button confirmButton;

bool confirmHeld = false;
bool confirmLongHandled = false;
uint32_t confirmStartedAt = 0;

bool errorVisible = false;
uint8_t currentError = 0;
uint32_t errorStartedAt = 0;

char serialBuffer[80];
uint8_t serialLength = 0;
bool serialOverflow = false;
uint32_t lastSerialByteAt = 0;

bool signalActive = false;
bool signalOn = false;
uint8_t pulsesRemaining = 0;
uint16_t signalFrequency = 0;
uint16_t signalOnMs = 0;
uint16_t signalOffMs = 0;
uint32_t signalChangedAt = 0;

const uint8_t DIGITS[10] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66,
  0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

// Standalone TM1637 driver; no external display library is required.
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void tmDataLow();
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void tmDataRelease();
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void tmClock(bool high);
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void tmStart();
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void tmStop();
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void tmWriteByte(uint8_t value);
#line 117 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void tmShow(const uint8_t segments[4]);
#line 134 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void renderDisplay();
#line 163 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void updateLed();
#line 167 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void startSignal(uint8_t pulses, uint16_t frequency, uint16_t onMs, uint16_t offMs);
#line 184 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void updateSignal(uint32_t now);
#line 209 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void showError(uint8_t code);
#line 217 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void beginButton(Button &button, uint8_t pin);
#line 227 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void updateButton(Button &button, uint32_t now);
#line 245 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void printStatus();
#line 257 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void printHelp();
#line 263 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void setAdminMode(bool enabled);
#line 280 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void dispenseSelected();
#line 303 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
bool parseUnsigned(const char *text, uint32_t maximum, uint32_t &value);
#line 324 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void badCommand();
#line 329 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void processSerialCommand(char *line);
#line 396 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void finishSerialFrame();
#line 408 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void pollSerial();
#line 430 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 453 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_010\\direct\\attempt_0\\sketch\\sketch.ino"
void tmDataLow() {
  digitalWrite(TM_DIO_PIN, LOW);
  pinMode(TM_DIO_PIN, OUTPUT);
}

void tmDataRelease() {
  pinMode(TM_DIO_PIN, INPUT_PULLUP);
}

void tmClock(bool high) {
  digitalWrite(TM_CLK_PIN, high ? HIGH : LOW);
  delayMicroseconds(5);
}

void tmStart() {
  tmDataRelease();
  tmClock(true);
  tmDataLow();
  delayMicroseconds(5);
  tmClock(false);
}

void tmStop() {
  tmClock(false);
  tmDataLow();
  delayMicroseconds(5);
  tmClock(true);
  tmDataRelease();
  delayMicroseconds(5);
}

void tmWriteByte(uint8_t value) {
  for (uint8_t bit = 0; bit < 8; ++bit) {
    tmClock(false);
    if (value & 0x01) {
      tmDataRelease();
    } else {
      tmDataLow();
    }
    delayMicroseconds(5);
    tmClock(true);
    value >>= 1;
  }

  tmClock(false);
  tmDataRelease();
  delayMicroseconds(5);
  tmClock(true);  // ACK clock; module pulls DIO low if present.
  (void)digitalRead(TM_DIO_PIN);
  tmClock(false);
}

void tmShow(const uint8_t segments[4]) {
  tmStart();
  tmWriteByte(0x40);  // Automatic address increment.
  tmStop();

  tmStart();
  tmWriteByte(0xC0);
  for (uint8_t i = 0; i < 4; ++i) {
    tmWriteByte(segments[i]);
  }
  tmStop();

  tmStart();
  tmWriteByte(0x8B);  // Display on, medium brightness.
  tmStop();
}

void renderDisplay() {
  uint8_t segments[4];

  if (errorVisible) {
    // E001 = sold out; E002 = no selection; E003 = bad serial command.
    segments[0] = 0x79;  // E
    segments[1] = DIGITS[0];
    segments[2] = DIGITS[currentError / 10];
    segments[3] = DIGITS[currentError % 10];
  } else if (adminMode) {
    segments[0] = 0x77;  // A
    segments[1] = 0x5E;  // d
    segments[2] = 0x40;
    segments[3] = 0x40;
  } else if (selectedItem >= 0) {
    uint16_t price = prices[selectedItem];
    segments[0] = DIGITS[selectedItem + 1];
    segments[1] = DIGITS[(price / 100) % 10] | 0x80;
    segments[2] = DIGITS[(price / 10) % 10];
    segments[3] = DIGITS[price % 10];
  } else {
    for (uint8_t i = 0; i < 4; ++i) {
      segments[i] = 0x40;
    }
  }

  tmShow(segments);
}

void updateLed() {
  digitalWrite(LED_PIN, (adminMode || signalOn) ? HIGH : LOW);
}

void startSignal(uint8_t pulses, uint16_t frequency,
                 uint16_t onMs, uint16_t offMs) {
  noTone(BUZZER_PIN);
  pulsesRemaining = pulses;
  signalFrequency = frequency;
  signalOnMs = onMs;
  signalOffMs = offMs;
  signalActive = pulses > 0;
  signalOn = signalActive;
  signalChangedAt = millis();

  if (signalOn) {
    tone(BUZZER_PIN, signalFrequency);
  }
  updateLed();
}

void updateSignal(uint32_t now) {
  if (!signalActive) {
    return;
  }

  uint16_t interval = signalOn ? signalOnMs : signalOffMs;
  if ((uint32_t)(now - signalChangedAt) < interval) {
    return;
  }

  signalChangedAt = now;
  if (signalOn) {
    noTone(BUZZER_PIN);
    signalOn = false;
    --pulsesRemaining;
    if (pulsesRemaining == 0) {
      signalActive = false;
    }
  } else {
    signalOn = true;
    tone(BUZZER_PIN, signalFrequency);
  }
  updateLed();
}

void showError(uint8_t code) {
  currentError = code;
  errorVisible = true;
  errorStartedAt = millis();
  renderDisplay();
  startSignal(3, 500, 180, 120);
}

void beginButton(Button &button, uint8_t pin) {
  button.pin = pin;
  pinMode(pin, INPUT_PULLUP);
  button.raw = HIGH;
  button.stable = HIGH;
  button.pressed = false;
  button.released = false;
  button.changedAt = millis();
}

void updateButton(Button &button, uint32_t now) {
  button.pressed = false;
  button.released = false;
  bool reading = digitalRead(button.pin);

  if (reading != button.raw) {
    button.raw = reading;
    button.changedAt = now;
  }

  if (button.raw != button.stable &&
      (uint32_t)(now - button.changedAt) >= DEBOUNCE_MS) {
    button.stable = button.raw;
    button.pressed = (button.stable == LOW);
    button.released = (button.stable == HIGH);
  }
}

void printStatus() {
  Serial.println(adminMode ? F("mode:admin") : F("mode:user"));
  for (uint8_t i = 0; i < 3; ++i) {
    Serial.print(F("item"));
    Serial.print(i + 1);
    Serial.print(F(" price_cents="));
    Serial.print(prices[i]);
    Serial.print(F(" stock="));
    Serial.println(inventory[i]);
  }
}

void printHelp() {
  Serial.println(F("Hold D7 for 1500ms: enter/leave admin."));
  Serial.println(F("Commands: help, status"));
  Serial.println(F("Admin: restock item1 5 (or restock 1 5), exit"));
}

void setAdminMode(bool enabled) {
  adminMode = enabled;
  errorVisible = false;

  // Do not allow a partial command to cross a mode boundary.
  serialLength = 0;
  serialOverflow = false;

  renderDisplay();
  startSignal(1, enabled ? 1500 : 1000, 150, 80);
  Serial.println(enabled ? F("admin:entered") : F("admin:exited"));
  if (enabled) {
    printHelp();
    printStatus();
  }
}

void dispenseSelected() {
  if (selectedItem < 0) {
    Serial.println(F("error:no_selection"));
    showError(2);
    return;
  }

  if (inventory[selectedItem] == 0) {
    Serial.print(F("error:sold_out:item"));
    Serial.println(selectedItem + 1);
    showError(1);
    return;
  }

  --inventory[selectedItem];
  Serial.print(F("dispense:item"));
  Serial.println(selectedItem + 1);

  errorVisible = false;
  renderDisplay();
  startSignal(2, 2200, 100, 80);
}

bool parseUnsigned(const char *text, uint32_t maximum, uint32_t &value) {
  if (text == NULL || *text == '\0') {
    return false;
  }

  uint32_t result = 0;
  while (*text) {
    if (*text < '0' || *text > '9') {
      return false;
    }
    uint8_t digit = *text++ - '0';
    if (result > maximum / 10 ||
        (result == maximum / 10 && digit > maximum % 10)) {
      return false;
    }
    result = result * 10 + digit;
  }
  value = result;
  return true;
}

void badCommand() {
  Serial.println(F("error:command; use help"));
  showError(3);
}

void processSerialCommand(char *line) {
  char *command = strtok(line, " \t");
  if (command == NULL) {
    return;
  }

  char *argument1 = strtok(NULL, " \t");
  char *argument2 = strtok(NULL, " \t");
  char *extra = strtok(NULL, " \t");

  if (strcmp(command, "help") == 0 && argument1 == NULL) {
    printHelp();
    return;
  }

  if (strcmp(command, "status") == 0 && argument1 == NULL) {
    printStatus();
    return;
  }

  if (!adminMode) {
    Serial.println(F("error:admin_required; hold D7"));
    return;
  }

  if (strcmp(command, "exit") == 0 && argument1 == NULL) {
    setAdminMode(false);
    return;
  }

  if (strcmp(command, "restock") != 0 ||
      argument1 == NULL || argument2 == NULL || extra != NULL) {
    badCommand();
    return;
  }

  const char *itemText = argument1;
  if (strncmp(itemText, "item", 4) == 0) {
    itemText += 4;
  }

  uint32_t itemNumber;
  uint32_t quantity;
  if (!parseUnsigned(itemText, 3, itemNumber) || itemNumber < 1 ||
      !parseUnsigned(argument2, 65535UL, quantity) || quantity == 0) {
    badCommand();
    return;
  }

  uint8_t index = itemNumber - 1;
  if (quantity > 65535UL - inventory[index]) {
    Serial.println(F("error:stock_overflow"));
    showError(3);
    return;
  }

  inventory[index] += quantity;
  Serial.print(F("restocked:item"));
  Serial.print(itemNumber);
  Serial.print(F(" stock="));
  Serial.println(inventory[index]);

  errorVisible = false;
  renderDisplay();
  startSignal(1, 2000, 100, 80);
}

void finishSerialFrame() {
  if (serialOverflow) {
    Serial.println(F("error:line_too_long"));
    showError(3);
  } else if (serialLength > 0) {
    serialBuffer[serialLength] = '\0';
    processSerialCommand(serialBuffer);
  }
  serialLength = 0;
  serialOverflow = false;
}

void pollSerial() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastSerialByteAt = millis();

    if (c == '\r' || c == '\n') {
      finishSerialFrame();
    } else if (!serialOverflow) {
      if (serialLength < sizeof(serialBuffer) - 1) {
        serialBuffer[serialLength++] = c;
      } else {
        serialOverflow = true;
      }
    }
  }

  if ((serialLength > 0 || serialOverflow) &&
      (uint32_t)(millis() - lastSerialByteAt) >= SERIAL_IDLE_MS) {
    finishSerialFrame();
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  pinMode(TM_CLK_PIN, OUTPUT);
  digitalWrite(TM_CLK_PIN, HIGH);
  tmDataRelease();

  for (uint8_t i = 0; i < 3; ++i) {
    beginButton(itemButtons[i], ITEM_PINS[i]);
  }
  beginButton(confirmButton, CONFIRM_PIN);

  renderDisplay();
  Serial.println(F("vending:ready"));
  printHelp();
  printStatus();
}

void loop() {
  uint32_t now = millis();

  for (uint8_t i = 0; i < 3; ++i) {
    updateButton(itemButtons[i], now);
    if (itemButtons[i].pressed && !adminMode) {
      selectedItem = i;
      errorVisible = false;
      renderDisplay();

      Serial.print(F("selected:item"));
      Serial.print(i + 1);
      Serial.print(F(" price_cents="));
      Serial.println(prices[i]);
    }
  }

  updateButton(confirmButton, now);

  if (confirmButton.pressed) {
    confirmHeld = true;
    confirmLongHandled = false;
    confirmStartedAt = now;
  }

  if (confirmHeld && confirmButton.stable == LOW &&
      !confirmLongHandled &&
      (uint32_t)(now - confirmStartedAt) >= LONG_PRESS_MS) {
    confirmLongHandled = true;
    setAdminMode(!adminMode);
  }

  if (confirmButton.released && confirmHeld) {
    confirmHeld = false;
    // Dispense only on short-press release, never on a long press.
    if (!confirmLongHandled && !adminMode) {
      dispenseSelected();
    }
  }

  pollSerial();

  now = millis();
  updateSignal(now);

  if (errorVisible &&
      (uint32_t)(now - errorStartedAt) >= ERROR_DISPLAY_MS) {
    errorVisible = false;
    renderDisplay();
  }
}
