#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_007\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Wiring: door switch connects D2 to GND when the door is closed.
constexpr uint8_t LM35_PIN = A1;
constexpr uint8_t DOOR_PIN = 2;
constexpr uint8_t RELAY_PIN = 10;
constexpr uint8_t BUZZER_PIN = 6;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;
constexpr uint8_t LED_PIN = 13;

constexpr uint8_t DOOR_OPEN_LEVEL = HIGH;
constexpr uint8_t RELAY_ON_LEVEL = HIGH;
constexpr uint8_t RELAY_OFF_LEVEL = LOW;

// Adjustable refrigerator limits; hysteresis prevents relay chatter.
constexpr float UPPER_LIMIT_C = 8.0f;
constexpr float LOWER_LIMIT_C = 4.0f;

// LM35 produces 10 mV/degree C. Match this value to the actual Uno AVcc.
constexpr float ADC_REFERENCE_MV = 5000.0f;
constexpr float TEMPERATURE_GAIN = 1.0f;
constexpr float TEMPERATURE_OFFSET_C = 0.0f;

constexpr unsigned long TEMPERATURE_SAMPLE_MS = 250UL;
constexpr unsigned long DOOR_ALARM_DELAY_MS = 10000UL;
constexpr unsigned long LED_HALF_PERIOD_MS = 500UL;
constexpr unsigned long DISPLAY_PAGE_MS = 2000UL;
constexpr unsigned long DISPLAY_UPDATE_MS = 200UL;
constexpr unsigned long CRITICAL_REPORT_MS = 1000UL;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

float temperatureC = 0.0f;
bool coolingOn = false;
bool doorOpen = false;
bool alarmOn = false;
bool criticalPreviously = false;

unsigned long doorOpenedAt = 0;
unsigned long lastTemperatureSample = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastCriticalReport = 0;

#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_007\\direct\\attempt_0\\sketch\\sketch.ino"
float readTemperatureC();
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_007\\direct\\attempt_0\\sketch\\sketch.ino"
void updateCooling();
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_007\\direct\\attempt_0\\sketch\\sketch.ino"
void showTemperature();
#line 82 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_007\\direct\\attempt_0\\sketch\\sketch.ino"
void showDoorDuration(unsigned long elapsedMs);
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_007\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_007\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_007\\direct\\attempt_0\\sketch\\sketch.ino"
float readTemperatureC() {
  unsigned long total = 0;
  for (uint8_t i = 0; i < 8; ++i) {
    total += analogRead(LM35_PIN);
  }

  const float adc = total / 8.0f;
  const float millivolts = adc * ADC_REFERENCE_MV / 1023.0f;
  return (millivolts / 10.0f) * TEMPERATURE_GAIN
         + TEMPERATURE_OFFSET_C;
}

void updateCooling() {
  if (temperatureC >= UPPER_LIMIT_C) {
    coolingOn = true;
  } else if (temperatureC <= LOWER_LIMIT_C) {
    coolingOn = false;
  }

  // Cooling remains available even while the door is open.
  digitalWrite(RELAY_PIN, coolingOn ? RELAY_ON_LEVEL : RELAY_OFF_LEVEL);
}

void showTemperature() {
  int roundedTemperature =
      static_cast<int>(temperatureC >= 0.0f
                           ? temperatureC + 0.5f
                           : temperatureC - 0.5f);

  roundedTemperature = constrain(roundedTemperature, -99, 999);
  display.showNumberDec(roundedTemperature, false, 3, 0);

  const uint8_t cSegment = SEG_A | SEG_D | SEG_E | SEG_F;
  display.setSegments(&cSegment, 1, 3);
}

void showDoorDuration(unsigned long elapsedMs) {
  const unsigned long seconds = elapsedMs / 1000UL;

  if (seconds <= 9999UL) {
    // Duration page: elapsed seconds, including leading zeros.
    display.showNumberDec(static_cast<int>(seconds), true);
  } else {
    // Very long openings: show three digits plus a unit.
    const unsigned long minutes = seconds / 60UL;
    if (minutes <= 999UL) {
      display.showNumberDec(static_cast<int>(minutes), false, 3, 0);
      const uint8_t mSegment = SEG_C | SEG_E | SEG_G; // Lowercase "n"/minutes.
      display.setSegments(&mSegment, 1, 3);
    } else {
      const unsigned long hours = seconds / 3600UL;
      display.showNumberDec(
          static_cast<int>(hours > 999UL ? 999UL : hours), false, 3, 0);
      const uint8_t hSegment = SEG_B | SEG_C | SEG_E | SEG_F | SEG_G;
      display.setSegments(&hSegment, 1, 3);
    }
  }
}

void setup() {
  // Establish the inactive relay level before enabling the output.
  digitalWrite(RELAY_PIN, RELAY_OFF_LEVEL);
  pinMode(RELAY_PIN, OUTPUT);

  pinMode(DOOR_PIN, INPUT_PULLUP);
  pinMode(LM35_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);

  Serial.begin(9600, SERIAL_8N1);
  analogReference(DEFAULT);

  display.setBrightness(5, true);
  display.clear();

  // Discard the first ADC conversion after reference/channel selection.
  analogRead(LM35_PIN);
  temperatureC = readTemperatureC();
  updateCooling();

  const unsigned long now = millis();
  doorOpen = digitalRead(DOOR_PIN) == DOOR_OPEN_LEVEL;
  doorOpenedAt = now;
  lastTemperatureSample = now;
  lastDisplayUpdate = now - DISPLAY_UPDATE_MS;
}

void loop() {
  const unsigned long now = millis();

  const bool currentDoorOpen =
      digitalRead(DOOR_PIN) == DOOR_OPEN_LEVEL;

  if (currentDoorOpen != doorOpen) {
    doorOpen = currentDoorOpen;
    if (doorOpen) {
      doorOpenedAt = now;
    }
    lastDisplayUpdate = now - DISPLAY_UPDATE_MS;
  }

  const unsigned long doorOpenMs =
      doorOpen ? now - doorOpenedAt : 0UL;

  if (now - lastTemperatureSample >= TEMPERATURE_SAMPLE_MS) {
    lastTemperatureSample = now;
    temperatureC = readTemperatureC();
    updateCooling();
  }

  const bool alarmRequired =
      doorOpen && doorOpenMs > DOOR_ALARM_DELAY_MS;

  if (alarmRequired != alarmOn) {
    alarmOn = alarmRequired;
    if (alarmOn) {
      tone(BUZZER_PIN, 2000);
    } else {
      noTone(BUZZER_PIN);
      digitalWrite(LED_PIN, LOW);
    }
  }

  if (alarmOn) {
    const unsigned long alarmElapsed =
        doorOpenMs - DOOR_ALARM_DELAY_MS;
    digitalWrite(
        LED_PIN,
        ((alarmElapsed / LED_HALF_PERIOD_MS) & 1UL) ? LOW : HIGH);
  }

  const bool critical = doorOpen && temperatureC > UPPER_LIMIT_C;
  if (critical &&
      (!criticalPreviously ||
       now - lastCriticalReport >= CRITICAL_REPORT_MS)) {
    // Repeated newline-delimited reports; never block the control loop.
    Serial.println(F("critical cooling loss"));
    lastCriticalReport = now;
  }
  criticalPreviously = critical;

  if (now - lastDisplayUpdate >= DISPLAY_UPDATE_MS) {
    lastDisplayUpdate = now;

    // A four-digit display alternates temperature and door-open duration.
    if (doorOpen && ((doorOpenMs / DISPLAY_PAGE_MS) & 1UL)) {
      showDoorDuration(doorOpenMs);
    } else {
      showTemperature();
    }
  }
}
