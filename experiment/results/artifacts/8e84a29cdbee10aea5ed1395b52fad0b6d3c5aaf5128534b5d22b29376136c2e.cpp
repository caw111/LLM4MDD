#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_distance_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

const uint8_t TRIG_PIN = 3;
const uint8_t ECHO_PIN = 2;

// Sensor VCC connects to 5V; sensor GND connects to GND.3.
const unsigned long MEASUREMENT_INTERVAL_MS = 60;
const unsigned long ECHO_TIMEOUT_US = 40000;
const unsigned long INPUT_IDLE_TIMEOUT_MS = 20;

char inputBuffer[32];
size_t inputLength = 0;
bool inputOverflow = false;
unsigned long lastInputTime = 0;

unsigned long lastMeasurementTime = 0;
unsigned long measuredDistanceCm = 0;
bool measurementValid = false;
bool firstMeasurement = true;

#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_distance_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
void printPrompt();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_distance_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
void measureDistance();
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_distance_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
void processInput();
#line 75 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_distance_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_distance_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_distance_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
void printPrompt() {
  Serial.print(F("Enter a distance in cm to compare: "));
}

void measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  measurementValid = (duration != 0);

  if (measurementValid) {
    // Integer division converts the measurement to whole centimeters.
    measuredDistanceCm = duration / 58UL;
  }
}

void processInput() {
  inputBuffer[inputLength] = '\0';
  Serial.println();

  char *end;
  errno = 0;
  long targetDistanceCm = strtol(inputBuffer, &end, 10);
  bool hasNumber = (end != inputBuffer);

  while (*end != '\0' && isspace(static_cast<unsigned char>(*end))) {
    ++end;
  }

  if (inputOverflow || !hasNumber || *end != '\0' ||
      errno == ERANGE || targetDistanceCm < 0) {
    Serial.println(F("Invalid distance. Enter a non-negative integer."));
  } else if (!measurementValid) {
    Serial.println(F("No echo received; distance measurement unavailable."));
  } else if (measuredDistanceCm > static_cast<unsigned long>(targetDistanceCm)) {
    Serial.println(F("Measured distance is greater"));
  } else if (measuredDistanceCm == static_cast<unsigned long>(targetDistanceCm)) {
    Serial.println(F("Measured distance is equal to the target"));
  } else {
    Serial.println(F("Measured distance is less"));
  }

  inputLength = 0;
  inputOverflow = false;
  printPrompt();
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  Serial.begin(9600);
  printPrompt();
}

void loop() {
  unsigned long now = millis();

  if (firstMeasurement ||
      now - lastMeasurementTime >= MEASUREMENT_INTERVAL_MS) {
    firstMeasurement = false;
    lastMeasurementTime = now;
    measureDistance();
  }

  while (Serial.available() > 0) {
    char received = static_cast<char>(Serial.read());
    lastInputTime = millis();

    if (received == '\r' || received == '\n') {
      // Ignore empty lines, including the LF following a CR.
      if (inputLength > 0 || inputOverflow) {
        processInput();
      }
    } else if (inputLength < sizeof(inputBuffer) - 1) {
      inputBuffer[inputLength++] = received;
    } else {
      inputOverflow = true;
    }
  }

  // Accept input without a line ending after 20 ms of serial inactivity.
  if ((inputLength > 0 || inputOverflow) &&
      millis() - lastInputTime >= INPUT_IDLE_TIMEOUT_MS) {
    processInput();
  }
}
