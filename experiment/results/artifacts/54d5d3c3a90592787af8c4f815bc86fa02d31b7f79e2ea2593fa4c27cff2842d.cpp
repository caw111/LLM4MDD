#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_011\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t LED_PIN = 9;
const unsigned long IDLE_TIMEOUT_MS = 20;

char inputBuffer[24];
size_t inputLength = 0;
bool inputOverflow = false;
unsigned long lastByteTime = 0;

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_011\\direct\\attempt_0\\sketch\\sketch.ino"
void processInput();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_011\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_011\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_011\\direct\\attempt_0\\sketch\\sketch.ino"
void processInput() {
  const size_t length = inputLength;
  const bool overflow = inputOverflow;
  inputLength = 0;
  inputOverflow = false;

  if (overflow || length == 0) {
    return;
  }

  size_t start = 0;
  size_t end = length;

  while (start < end &&
         (inputBuffer[start] == ' ' || inputBuffer[start] == '\t')) {
    ++start;
  }
  while (end > start &&
         (inputBuffer[end - 1] == ' ' || inputBuffer[end - 1] == '\t')) {
    --end;
  }

  if (start == end) {
    return;
  }

  uint16_t brightness = 0;
  for (size_t i = start; i < end; ++i) {
    const char c = inputBuffer[i];
    if (c < '0' || c > '9') {
      return;
    }

    brightness = brightness * 10 + (c - '0');
    if (brightness > 255) {
      return;
    }
  }

  analogWrite(LED_PIN, static_cast<uint8_t>(brightness));
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
  Serial.begin(9600);
}

void loop() {
  // Complete an idle-terminated command before accepting newly arrived bytes.
  if ((inputLength > 0 || inputOverflow) &&
      millis() - lastByteTime >= IDLE_TIMEOUT_MS) {
    processInput();
  }

  while (Serial.available() > 0) {
    const char c = static_cast<char>(Serial.read());
    lastByteTime = millis();

    if (c == '\r' || c == '\n') {
      processInput();
    } else if (!inputOverflow) {
      if (inputLength < sizeof(inputBuffer)) {
        inputBuffer[inputLength++] = c;
      } else {
        inputOverflow = true;
      }
    }
  }

  if ((inputLength > 0 || inputOverflow) &&
      millis() - lastByteTime >= IDLE_TIMEOUT_MS) {
    processInput();
  }
}
