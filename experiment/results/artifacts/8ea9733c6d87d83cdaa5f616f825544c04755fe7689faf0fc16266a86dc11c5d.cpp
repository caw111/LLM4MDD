#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_011\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <string.h>

const uint8_t LED_PIN = 9;
const unsigned long SERIAL_IDLE_TIMEOUT_MS = 20;
const unsigned long FADE_INTERVAL_MS = 10;
const int FADE_STEP = 5;

enum Mode : uint8_t {
  MODE_FADE = 1,
  MODE_ON = 2
};

Mode currentMode = MODE_FADE;
int brightness = 0;
int fadeDirection = 1;
unsigned long lastFadeMs = 0;

char commandBuffer[32];
size_t commandLength = 0;
bool commandOverflow = false;
unsigned long lastSerialByteMs = 0;

#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_011\\direct\\attempt_0\\sketch\\sketch.ino"
void selectMode(Mode mode);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_011\\direct\\attempt_0\\sketch\\sketch.ino"
void processCommand();
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_011\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_011\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_011\\direct\\attempt_0\\sketch\\sketch.ino"
void selectMode(Mode mode) {
  currentMode = mode;

  if (mode == MODE_FADE) {
    brightness = 0;
    fadeDirection = 1;
    lastFadeMs = millis();
    analogWrite(LED_PIN, brightness);
  } else {
    analogWrite(LED_PIN, 255);
  }
}

void processCommand() {
  if (!commandOverflow && commandLength > 0) {
    commandBuffer[commandLength] = '\0';

    char *start = commandBuffer;
    while (*start == ' ' || *start == '\t') {
      ++start;
    }

    char *end = commandBuffer + commandLength;
    while (end > start && (end[-1] == ' ' || end[-1] == '\t')) {
      --end;
    }
    *end = '\0';

    if (strcmp(start, "MODE1") == 0) {
      selectMode(MODE_FADE);
    } else if (strcmp(start, "MODE2") == 0) {
      selectMode(MODE_ON);
    }
  }

  commandLength = 0;
  commandOverflow = false;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  selectMode(MODE_FADE);
}

void loop() {
  while (Serial.available() > 0) {
    const char received = static_cast<char>(Serial.read());
    const unsigned long now = millis();

    // Finish an idle-framed command before accepting a later command.
    if ((commandLength > 0 || commandOverflow) &&
        now - lastSerialByteMs >= SERIAL_IDLE_TIMEOUT_MS) {
      processCommand();
    }

    lastSerialByteMs = now;

    if (received == '\r' || received == '\n') {
      processCommand();
    } else if (!commandOverflow) {
      // Reject embedded NULs and oversized commands as a whole.
      if (received == '\0' ||
          commandLength >= sizeof(commandBuffer) - 1) {
        commandOverflow = true;
      } else {
        commandBuffer[commandLength++] = received;
      }
    }
  }

  const unsigned long now = millis();

  if ((commandLength > 0 || commandOverflow) &&
      now - lastSerialByteMs >= SERIAL_IDLE_TIMEOUT_MS) {
    processCommand();
  }

  if (currentMode == MODE_FADE &&
      now - lastFadeMs >= FADE_INTERVAL_MS) {
    lastFadeMs = now;
    brightness += fadeDirection * FADE_STEP;

    if (brightness >= 255) {
      brightness = 255;
      fadeDirection = -1;
    } else if (brightness <= 0) {
      brightness = 0;
      fadeDirection = 1;
    }

    analogWrite(LED_PIN, brightness);
  }
}
