#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_014\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <math.h>

const uint8_t LM35_PIN = A1;
const uint8_t BUTTON_PIN = 2;
const uint8_t DISPLAY_CLK_PIN = 4;
const uint8_t DISPLAY_DIO_PIN = 5;

const unsigned long SAMPLE_INTERVAL_MS = 100;
const unsigned long SERIAL_INTERVAL_MS = 500;
const unsigned long DEBOUNCE_MS = 30;
const unsigned long MAX_DISPLAY_DURATION_MS = 3000;

// LM35: 10 mV / degree Celsius; Uno DEFAULT ADC reference: nominal 5 V.
const float ADC_REFERENCE_VOLTS = 5.0f;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

float currentTemperatureC = 0.0f;
float highestTemperatureC = 0.0f;

bool showingMaximum = false;
bool displayNeedsUpdate = true;

uint8_t lastButtonReading = HIGH;
uint8_t stableButtonState = HIGH;

unsigned long lastButtonChangeMs = 0;
unsigned long maximumDisplayStartMs = 0;
unsigned long lastSampleMs = 0;
unsigned long lastSerialMs = 0;

#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_014\\direct\\attempt_0\\sketch\\sketch.ino"
float readTemperatureC();
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_014\\direct\\attempt_0\\sketch\\sketch.ino"
void updateTemperature();
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_014\\direct\\attempt_0\\sketch\\sketch.ino"
void updateButton(unsigned long now);
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_014\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay();
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_014\\direct\\attempt_0\\sketch\\sketch.ino"
void printTemperatures();
#line 95 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_014\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 121 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_014\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_014\\direct\\attempt_0\\sketch\\sketch.ino"
float readTemperatureC() {
  const int adcValue = analogRead(LM35_PIN);
  return adcValue * (ADC_REFERENCE_VOLTS / 1023.0f) * 100.0f;
}

void updateTemperature() {
  currentTemperatureC = readTemperatureC();

  if (currentTemperatureC > highestTemperatureC) {
    highestTemperatureC = currentTemperatureC;
  }

  displayNeedsUpdate = true;
}

void updateButton(unsigned long now) {
  const uint8_t reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastButtonReading = reading;
    lastButtonChangeMs = now;
  }

  if (reading != stableButtonState &&
      (unsigned long)(now - lastButtonChangeMs) >= DEBOUNCE_MS) {
    stableButtonState = reading;

    // Button connects D2 to GND; trigger once on each debounced press.
    if (stableButtonState == LOW) {
      showingMaximum = true;
      maximumDisplayStartMs = now;
      displayNeedsUpdate = true;
    }
  }
}

void updateDisplay() {
  const float temperature =
      showingMaximum ? highestTemperatureC : currentTemperatureC;

  // Display rounded Celsius in the first three positions, followed by C.
  // Serial output retains decimal precision.
  int roundedTemperature = (int)roundf(temperature);
  roundedTemperature = constrain(roundedTemperature, -99, 999);

  display.showNumberDec(roundedTemperature, false, 3, 0);

  const uint8_t cSegment = SEG_A | SEG_D | SEG_E | SEG_F;
  display.setSegments(&cSegment, 1, 3);

  displayNeedsUpdate = false;
}

void printTemperatures() {
  Serial.print(F("Current: "));
  Serial.print(currentTemperatureC, 2);
  Serial.print(F(" C, Maximum: "));
  Serial.print(highestTemperatureC, 2);
  Serial.println(F(" C"));
}

void setup() {
  pinMode(LM35_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  analogReference(DEFAULT);

  Serial.begin(9600);
  Serial.setTimeout(20);

  display.setBrightness(7, true);
  display.clear();

  // Discard the first conversion after configuring the ADC reference.
  analogRead(LM35_PIN);
  currentTemperatureC = readTemperatureC();
  highestTemperatureC = currentTemperatureC;

  const unsigned long now = millis();
  lastSampleMs = now;
  lastSerialMs = now;
  lastButtonChangeMs = now;
  lastButtonReading = digitalRead(BUTTON_PIN);

  updateDisplay();
  printTemperatures();
}

void loop() {
  const unsigned long now = millis();

  updateButton(now);

  if ((unsigned long)(now - lastSampleMs) >= SAMPLE_INTERVAL_MS) {
    lastSampleMs = now;
    updateTemperature();
  }

  if (showingMaximum &&
      (unsigned long)(now - maximumDisplayStartMs) >=
          MAX_DISPLAY_DURATION_MS) {
    showingMaximum = false;
    displayNeedsUpdate = true;
  }

  if (displayNeedsUpdate) {
    updateDisplay();
  }

  // Sampling, maximum tracking, and serial reporting continue in both modes.
  if ((unsigned long)(now - lastSerialMs) >= SERIAL_INTERVAL_MS) {
    lastSerialMs = now;
    printTemperatures();
  }
}
