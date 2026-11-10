#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_020\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Buttons connect their respective input pins to GND when pressed.
const uint8_t FLOOR_BUTTON_PINS[3] = {2, 3, 4};
const uint8_t LOAD_SENSOR_PIN = A0;
const uint8_t BUZZER_PIN = 6;
const uint8_t DISPLAY_CLK_PIN = 7;
const uint8_t DISPLAY_DIO_PIN = 5;
const uint8_t LED_PIN = 13;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

// Adjust these calibration values for the actual analog weight sensor.
// ADC 0 represents no load; ADC 682 represents 100% rated capacity.
// Values above rated capacity remain measurable and trigger overload.
const int EMPTY_LOAD_ADC = 0;
const int RATED_LOAD_ADC = 682;

const unsigned long DEBOUNCE_MS = 30;
const unsigned long LOAD_SAMPLE_MS = 20;
const unsigned long DISPLAY_UPDATE_MS = 100;
const unsigned long TRAVEL_MS_PER_FLOOR = 2000;
const unsigned int ALARM_FREQUENCY_HZ = 2000;

struct ButtonState {
  uint8_t rawState;
  uint8_t stableState;
  unsigned long lastChangeMs;
};

ButtonState buttons[3];

uint8_t currentFloor = 1;
uint8_t targetFloor = 1;
uint16_t loadPercent = 0;
bool overloaded = false;
bool moving = false;
bool alarmActive = false;

unsigned long lastLoadSampleMs = 0;
unsigned long lastDisplayUpdateMs = 0;
unsigned long lastMotionTickMs = 0;
unsigned long remainingTravelMs = 0;

#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_020\\direct\\attempt_0\\sketch\\sketch.ino"
void sampleLoad();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_020\\direct\\attempt_0\\sketch\\sketch.ino"
void updateAlarm();
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_020\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay();
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_020\\direct\\attempt_0\\sketch\\sketch.ino"
void requestFloor(uint8_t requestedFloor, unsigned long now);
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_020\\direct\\attempt_0\\sketch\\sketch.ino"
void updateButtons(unsigned long now);
#line 123 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_020\\direct\\attempt_0\\sketch\\sketch.ino"
void updateMovement(unsigned long now, bool previouslyOverloaded);
#line 148 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_020\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 176 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_020\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_020\\direct\\attempt_0\\sketch\\sketch.ino"
void sampleLoad() {
  const int raw = analogRead(LOAD_SENSOR_PIN);
  const int calibratedLoad = max(0, raw - EMPTY_LOAD_ADC);
  const int ratedSpan = RATED_LOAD_ADC - EMPTY_LOAD_ADC;

  overloaded = calibratedLoad > ratedSpan;
  loadPercent = (calibratedLoad * 100UL + ratedSpan / 2) / ratedSpan;

  // Do not display 100% while a slightly-over-capacity load is alarming.
  if (overloaded && loadPercent <= 100) {
    loadPercent = 101;
  }

  if (loadPercent > 999) {
    loadPercent = 999;
  }
}

void updateAlarm() {
  digitalWrite(LED_PIN, overloaded ? HIGH : LOW);

  if (overloaded && !alarmActive) {
    tone(BUZZER_PIN, ALARM_FREQUENCY_HZ);
    alarmActive = true;
  } else if (!overloaded && alarmActive) {
    noTone(BUZZER_PIN);
    alarmActive = false;
  }
}

void updateDisplay() {
  // Four digits: [current floor][load hundreds][load tens][load units].
  // For example, "1075" means floor 1, load 75%; "3120" means 3, 120%.
  uint8_t segments[4];
  segments[0] = display.encodeDigit(currentFloor);
  segments[1] = display.encodeDigit((loadPercent / 100) % 10);
  segments[2] = display.encodeDigit((loadPercent / 10) % 10);
  segments[3] = display.encodeDigit(loadPercent % 10);
  display.setSegments(segments);
}

void requestFloor(uint8_t requestedFloor, unsigned long now) {
  // Requests made during overload or an existing trip are not queued.
  if (overloaded || moving || requestedFloor == currentFloor) {
    return;
  }

  targetFloor = requestedFloor;
  const uint8_t distance = (targetFloor > currentFloor)
                               ? targetFloor - currentFloor
                               : currentFloor - targetFloor;

  remainingTravelMs = TRAVEL_MS_PER_FLOOR * distance;
  lastMotionTickMs = now;
  moving = true;
}

void updateButtons(unsigned long now) {
  for (uint8_t i = 0; i < 3; ++i) {
    const uint8_t reading = digitalRead(FLOOR_BUTTON_PINS[i]);

    if (reading != buttons[i].rawState) {
      buttons[i].rawState = reading;
      buttons[i].lastChangeMs = now;
    }

    if (reading != buttons[i].stableState &&
        now - buttons[i].lastChangeMs >= DEBOUNCE_MS) {
      buttons[i].stableState = reading;

      if (reading == LOW) {
        requestFloor(i + 1, now);
      }
    }
  }
}

void updateMovement(unsigned long now, bool previouslyOverloaded) {
  if (!moving) {
    return;
  }

  const unsigned long elapsed = now - lastMotionTickMs;
  lastMotionTickMs = now;

  // Pause the trip during overload. The current floor does not change.
  // Also discard the first interval after overload clears.
  if (overloaded || previouslyOverloaded) {
    return;
  }

  if (elapsed >= remainingTravelMs) {
    remainingTravelMs = 0;
    currentFloor = targetFloor;
    moving = false;
    updateDisplay();
    Serial.println(F("movement complete"));
  } else {
    remainingTravelMs -= elapsed;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(LOAD_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  const unsigned long now = millis();
  for (uint8_t i = 0; i < 3; ++i) {
    pinMode(FLOOR_BUTTON_PINS[i], INPUT_PULLUP);
    buttons[i].rawState = digitalRead(FLOOR_BUTTON_PINS[i]);
    buttons[i].stableState = buttons[i].rawState;
    buttons[i].lastChangeMs = now;
  }

  display.setBrightness(5, true);
  display.clear();

  sampleLoad();
  updateAlarm();
  updateDisplay();

  lastLoadSampleMs = millis();
  lastDisplayUpdateMs = lastLoadSampleMs;
}

void loop() {
  const unsigned long now = millis();
  const bool previouslyOverloaded = overloaded;

  if (now - lastLoadSampleMs >= LOAD_SAMPLE_MS) {
    lastLoadSampleMs = now;
    sampleLoad();
  }

  updateAlarm();
  updateButtons(now);
  updateMovement(now, previouslyOverloaded);

  if (now - lastDisplayUpdateMs >= DISPLAY_UPDATE_MS) {
    lastDisplayUpdateMs = now;
    updateDisplay();
  }
}
