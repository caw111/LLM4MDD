#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_002\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Wiring: LM35=A1, potentiometer=A0, fan PWM=D5,
// button=D2 (to GND), TM1637 CLK=D4, DIO=D3.
constexpr uint8_t TEMP_PIN = A1;
constexpr uint8_t POT_PIN = A0;
constexpr uint8_t FAN_PIN = 5;
constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 3;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

// LM35: 10 mV/degree C, using the Uno's nominal 5 V ADC reference.
// Adjust ADC_REFERENCE_V to the measured supply voltage for calibration.
constexpr float ADC_REFERENCE_V = 5.0f;
constexpr float LM35_VOLTS_PER_C = 0.010f;
constexpr float TEMP_OFFSET_C = 0.0f;

// Automatic control: stopped at/below 25 C, full speed at/above 40 C,
// with proportional PWM between these temperatures.
constexpr float AUTO_STOP_C = 25.0f;
constexpr float AUTO_FULL_C = 40.0f;

// Manual mode provides levels 0 through 10.
constexpr uint8_t MAX_MANUAL_LEVEL = 10;
constexpr unsigned long DEBOUNCE_MS = 35;
constexpr unsigned long CONTROL_INTERVAL_MS = 100;

bool automaticMode = true;
bool lastRawButtonState = HIGH;
bool stableButtonState = HIGH;
unsigned long buttonChangedAt = 0;
unsigned long lastControlAt = 0;
bool updateRequested = true;

#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_002\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t readAveragedAnalog(uint8_t pin);
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_002\\direct\\attempt_0\\sketch\\sketch.ino"
float readTemperatureC();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_002\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t automaticPwm(float temperatureC);
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_002\\direct\\attempt_0\\sketch\\sketch.ino"
void showTemperature(float temperatureC);
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_002\\direct\\attempt_0\\sketch\\sketch.ino"
void showManualLevel(uint8_t level);
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_002\\direct\\attempt_0\\sketch\\sketch.ino"
void handleButton(unsigned long now);
#line 131 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_002\\direct\\attempt_0\\sketch\\sketch.ino"
void updateControl();
#line 149 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_002\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 167 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_002\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_002\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t readAveragedAnalog(uint8_t pin) {
  // Discard the first reading after switching ADC channels.
  analogRead(pin);

  uint32_t sum = 0;
  constexpr uint8_t SAMPLE_COUNT = 16;
  for (uint8_t i = 0; i < SAMPLE_COUNT; ++i) {
    sum += analogRead(pin);
  }
  return static_cast<uint16_t>(
      (sum + SAMPLE_COUNT / 2) / SAMPLE_COUNT);
}

float readTemperatureC() {
  const uint16_t adc = readAveragedAnalog(TEMP_PIN);
  const float voltage = adc * ADC_REFERENCE_V / 1023.0f;
  return voltage / LM35_VOLTS_PER_C + TEMP_OFFSET_C;
}

uint8_t automaticPwm(float temperatureC) {
  if (temperatureC <= AUTO_STOP_C) {
    return 0;
  }
  if (temperatureC >= AUTO_FULL_C) {
    return 255;
  }

  const float fraction =
      (temperatureC - AUTO_STOP_C) / (AUTO_FULL_C - AUTO_STOP_C);
  return static_cast<uint8_t>(fraction * 255.0f + 0.5f);
}

void showTemperature(float temperatureC) {
  // Three integer temperature digits followed by C.
  int temperature = static_cast<int>(
      temperatureC >= 0.0f ? temperatureC + 0.5f
                          : temperatureC - 0.5f);
  temperature = constrain(temperature, -99, 999);

  uint8_t segments[4] = {0, 0, 0, 0x39};  // C

  if (temperature < 0) {
    const int magnitude = -temperature;
    segments[0] = 0x40;  // Minus sign
    if (magnitude >= 10) {
      segments[1] = display.encodeDigit(magnitude / 10);
    }
    segments[2] = display.encodeDigit(magnitude % 10);
  } else {
    if (temperature >= 100) {
      segments[0] = display.encodeDigit(temperature / 100);
    }
    if (temperature >= 10) {
      segments[1] = display.encodeDigit((temperature / 10) % 10);
    }
    segments[2] = display.encodeDigit(temperature % 10);
  }

  display.setSegments(segments);
}

void showManualLevel(uint8_t level) {
  // H identifies manual mode; the rightmost digits show level 0..10.
  uint8_t segments[4] = {
      0x76,  // H
      0x00,
      static_cast<uint8_t>(
          level >= 10 ? display.encodeDigit(level / 10) : 0),
      display.encodeDigit(level % 10)
  };
  display.setSegments(segments);
}

void handleButton(unsigned long now) {
  const bool rawState = digitalRead(BUTTON_PIN);

  if (rawState != lastRawButtonState) {
    lastRawButtonState = rawState;
    buttonChangedAt = now;
  }

  if (rawState != stableButtonState &&
      now - buttonChangedAt >= DEBOUNCE_MS) {
    stableButtonState = rawState;

    // Toggle once per debounced press, not continuously while held.
    if (stableButtonState == LOW) {
      automaticMode = !automaticMode;
      updateRequested = true;
    }
  }
}

void updateControl() {
  const float temperatureC = readTemperatureC();

  if (automaticMode) {
    analogWrite(FAN_PIN, automaticPwm(temperatureC));
    showTemperature(temperatureC);
  } else {
    const uint16_t pot = readAveragedAnalog(POT_PIN);
    const uint8_t level =
        static_cast<uint8_t>((pot * 10UL + 511UL) / 1023UL);
    const uint8_t pwm = static_cast<uint8_t>(
        (level * 255UL + MAX_MANUAL_LEVEL / 2) / MAX_MANUAL_LEVEL);

    analogWrite(FAN_PIN, pwm);
    showManualLevel(level);
  }
}

void setup() {
  pinMode(FAN_PIN, OUTPUT);
  analogWrite(FAN_PIN, 0);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(TEMP_PIN, INPUT);
  pinMode(POT_PIN, INPUT);
  analogReference(DEFAULT);

  display.setBrightness(5, true);
  display.clear();

  buttonChangedAt = millis();
  updateControl();
  lastControlAt = millis();
  updateRequested = false;
}

void loop() {
  const unsigned long now = millis();

  handleButton(now);

  if (updateRequested || now - lastControlAt >= CONTROL_INTERVAL_MS) {
    updateControl();
    lastControlAt = millis();
    updateRequested = false;
  }
}
