#include <Arduino.h>
#include <TM1637Display.h>
#include <string.h>

constexpr uint8_t BUTTON_ON_PIN  = 2;
constexpr uint8_t BUTTON_OFF_PIN = 3;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;
constexpr uint8_t BUZZER_PIN = 6;
constexpr uint8_t LED_PIN = 13;

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long SERIAL_IDLE_TIMEOUT_MS = 20;
constexpr unsigned int BEEP_FREQUENCY_HZ = 2000;
constexpr unsigned long BEEP_DURATION_MS = 150;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

struct DebouncedButton {
  uint8_t pin;
  uint8_t rawState;
  uint8_t stableState;
  unsigned long lastChangeMs;
};

DebouncedButton onButton  = {BUTTON_ON_PIN, HIGH, HIGH, 0};
DebouncedButton offButton = {BUTTON_OFF_PIN, HIGH, HIGH, 0};

// Buttons connect their input pins to GND when pressed.
uint64_t operationCount = 0;

constexpr size_t SERIAL_BUFFER_SIZE = 48;
char serialBuffer[SERIAL_BUFFER_SIZE];
size_t serialLength = 0;
bool serialOverflow = false;
unsigned long lastSerialByteMs = 0;

void updateDisplay() {
  // Keep the cumulative count internally; show its last four digits.
  display.showNumberDec(
      static_cast<int>(operationCount % 10000ULL), false, 4, 0);
}

void recordOperation() {
  ++operationCount;
  updateDisplay();
}

void setLight(bool enabled) {
  digitalWrite(LED_PIN, enabled ? HIGH : LOW);
  // Every accepted operation counts, even if the state is unchanged.
  recordOperation();
}

void beep() {
  // tone() stops automatically, without blocking button or serial handling.
  tone(BUZZER_PIN, BEEP_FREQUENCY_HZ, BEEP_DURATION_MS);
  recordOperation();
}

bool buttonPressed(DebouncedButton &button, unsigned long now) {
  const uint8_t reading = digitalRead(button.pin);

  if (reading != button.rawState) {
    button.rawState = reading;
    button.lastChangeMs = now;
  }

  if (button.rawState != button.stableState &&
      now - button.lastChangeMs >= DEBOUNCE_MS) {
    button.stableState = button.rawState;
    return button.stableState == LOW;
  }

  return false;
}

void executeCommand(char *command) {
  // Allow surrounding spaces and tabs while keeping command names exact.
  while (*command == ' ' || *command == '\t') {
    ++command;
  }

  size_t length = strlen(command);
  while (length > 0 &&
         (command[length - 1] == ' ' || command[length - 1] == '\t')) {
    command[--length] = '\0';
  }

  if (length == 0) {
    return;
  }

  if (strcmp(command, "LIGHT_ON") == 0) {
    setLight(true);
  } else if (strcmp(command, "LIGHT_OFF") == 0) {
    setLight(false);
  } else if (strcmp(command, "BEEP") == 0) {
    beep();
  } else if (strcmp(command, "COUNT_RESET") == 0) {
    // Reset leaves the displayed count at zero.
    operationCount = 0;
    updateDisplay();
  } else {
    Serial.println(F("ERR UNKNOWN_COMMAND"));
    return;
  }

  Serial.println(F("OK"));
}

void finishSerialFrame() {
  if (serialOverflow) {
    Serial.println(F("ERR COMMAND_TOO_LONG"));
  } else if (serialLength > 0) {
    serialBuffer[serialLength] = '\0';
    executeCommand(serialBuffer);
  }

  serialLength = 0;
  serialOverflow = false;
}

void serviceSerial() {
  unsigned long now = millis();

  // Finish a previously buffered command after the specified idle interval.
  if ((serialLength > 0 || serialOverflow) &&
      now - lastSerialByteMs >= SERIAL_IDLE_TIMEOUT_MS) {
    finishSerialFrame();
  }

  // Bound work per iteration so continuous serial input cannot starve buttons.
  uint8_t byteBudget = 64;
  while (byteBudget-- > 0 && Serial.available() > 0) {
    const char received = static_cast<char>(Serial.read());
    lastSerialByteMs = millis();

    if (received == '\r' || received == '\n') {
      finishSerialFrame();
    } else if (!serialOverflow) {
      // Reject embedded NULs as malformed rather than accepting a prefix.
      if (received == '\0' ||
          serialLength >= SERIAL_BUFFER_SIZE - 1) {
        serialOverflow = true;
      } else {
        serialBuffer[serialLength++] = received;
      }
    }
  }
}

void setup() {
  pinMode(BUTTON_ON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_OFF_PIN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  noTone(BUZZER_PIN);

  Serial.begin(9600);

  display.setBrightness(7, true);
  updateDisplay();

  const unsigned long now = millis();
  onButton.lastChangeMs = now;
  offButton.lastChangeMs = now;
}

void loop() {
  const unsigned long now = millis();

  if (buttonPressed(onButton, now)) {
    setLight(true);
  }

  if (buttonPressed(offButton, now)) {
    setLight(false);
  }

  serviceSerial();
}