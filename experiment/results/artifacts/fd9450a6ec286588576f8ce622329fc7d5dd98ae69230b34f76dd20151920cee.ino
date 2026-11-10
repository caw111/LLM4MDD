#include <Servo.h>
#include <TM1637Display.h>

// Arduino Uno wiring:
// LDR: A0; LM35 output: A1; servo signal: D9; indoor LED: D10.
// Buttons: D2 and D3 to GND (internal pull-ups enabled).
// TM1637: CLK = D4, DIO = D5. All devices share GND.
//
// Controls:
// D2 press: toggle automatic/manual mode.
// D3 short press (manual mode): increase opening by 10%, wrapping to 0%.
// D3 hold (either mode): toggle opening-percentage/light-value display.
// Manual mode overrides automatic curtain movement; lighting still follows
// ambient light. Temperature protection takes priority in automatic mode.

const uint8_t LIGHT_PIN = A0;
const uint8_t TEMP_PIN = A1;
const uint8_t SERVO_PIN = 9;
const uint8_t LED_PIN = 10;
const uint8_t MODE_BUTTON_PIN = 2;
const uint8_t ADJUST_BUTTON_PIN = 3;
const uint8_t DISPLAY_CLK_PIN = 4;
const uint8_t DISPLAY_DIO_PIN = 5;

// Set false if the LDR divider produces lower ADC readings in brighter light.
// The displayed light value is normalized: 0 = dark, 1023 = bright.
const bool LIGHT_ADC_RISES_WITH_BRIGHTNESS = true;

// Tune these values for the actual LDR divider and installation.
const int DARK_THRESHOLD = 300;
const int BRIGHT_THRESHOLD = 750;
const int LIGHT_HYSTERESIS = 40;

// LM35: 10 mV per degree Celsius.
// Set this to the measured Uno analog reference voltage for better accuracy.
const float ADC_REFERENCE_VOLTS = 5.0f;
const float HIGH_TEMP_C = 30.0f;
const float TEMP_RELEASE_C = 28.0f;

// Calibrate these endpoints to the curtain mechanism without stalling it.
const int SERVO_CLOSED_DEG = 0;
const int SERVO_OPEN_DEG = 180;

const unsigned long DEBOUNCE_MS = 30;
const unsigned long LONG_PRESS_MS = 800;
const unsigned long SENSOR_INTERVAL_MS = 100;
const unsigned long SERVO_INTERVAL_MS = 15;
const unsigned long DISPLAY_INTERVAL_MS = 150;

Servo curtainServo;
TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

struct Button {
  uint8_t pin;
  bool rawPressed;
  bool stablePressed;
  bool longHandled;
  bool shortPressEvent;
  bool longPressEvent;
  unsigned long rawChangedAt;
  unsigned long pressedAt;
};

Button modeButton = {
  MODE_BUTTON_PIN, false, false, false, false, false, 0, 0
};
Button adjustButton = {
  ADJUST_BUTTON_PIN, false, false, false, false, false, 0, 0
};

bool automaticMode = true;
bool showLightValue = false;
bool temperatureHigh = false;
bool darkCondition = false;
bool brightCondition = false;

float filteredLightADC = 0.0f;
float filteredTemperatureADC = 0.0f;
float temperatureC = 0.0f;
int lightValue = 0;

int targetOpeningPercent = 50;
int currentServoDegrees = 90;

unsigned long lastSensorAt = 0;
unsigned long lastServoAt = 0;
unsigned long lastDisplayAt = 0;

void updateButton(Button &button, unsigned long now) {
  button.shortPressEvent = false;
  button.longPressEvent = false;

  bool pressed = (digitalRead(button.pin) == LOW);

  if (pressed != button.rawPressed) {
    button.rawPressed = pressed;
    button.rawChangedAt = now;
  }

  if (button.stablePressed != button.rawPressed &&
      now - button.rawChangedAt >= DEBOUNCE_MS) {
    button.stablePressed = button.rawPressed;

    if (button.stablePressed) {
      button.pressedAt = now;
      button.longHandled = false;
    } else if (!button.longHandled) {
      button.shortPressEvent = true;
    }
  }

  if (button.stablePressed && !button.longHandled &&
      now - button.pressedAt >= LONG_PRESS_MS) {
    button.longHandled = true;
    button.longPressEvent = true;
  }
}

int openingToDegrees(int percent) {
  return (int)map(constrain(percent, 0, 100),
                  0, 100, SERVO_CLOSED_DEG, SERVO_OPEN_DEG);
}

int degreesToOpening(int degrees) {
  return constrain((int)map(degrees,
                            SERVO_CLOSED_DEG, SERVO_OPEN_DEG,
                            0, 100), 0, 100);
}

void updateSensorStates() {
  int rawLight = constrain((int)(filteredLightADC + 0.5f), 0, 1023);
  lightValue = LIGHT_ADC_RISES_WITH_BRIGHTNESS
                 ? rawLight : 1023 - rawLight;

  temperatureC = filteredTemperatureADC *
                 (ADC_REFERENCE_VOLTS / 1023.0f) * 100.0f;

  if (temperatureC >= HIGH_TEMP_C) {
    temperatureHigh = true;
  } else if (temperatureC <= TEMP_RELEASE_C) {
    temperatureHigh = false;
  }

  if (lightValue <= DARK_THRESHOLD) {
    darkCondition = true;
  } else if (lightValue >= DARK_THRESHOLD + LIGHT_HYSTERESIS) {
    darkCondition = false;
  }

  if (lightValue >= BRIGHT_THRESHOLD) {
    brightCondition = true;
  } else if (lightValue <= BRIGHT_THRESHOLD - LIGHT_HYSTERESIS) {
    brightCondition = false;
  }
}

void applyControl() {
  // Lamp control remains active even when temperature forces curtains closed.
  digitalWrite(LED_PIN, darkCondition ? HIGH : LOW);

  if (!automaticMode) {
    return;
  }

  if (temperatureHigh) {
    targetOpeningPercent = 0;
  } else if (brightCondition) {
    targetOpeningPercent = 0;
  } else if (darkCondition) {
    targetOpeningPercent = 100;
  }
  // Between thresholds, retain the previous curtain target.
}

void refreshDisplay() {
  if (showLightValue) {
    display.showNumberDec(lightValue, false, 4, 0);
  } else {
    // Percentage is derived from commanded servo position, not measured
    // mechanical feedback. The far-right decimal point identifies this view.
    int opening = degreesToOpening(currentServoDegrees);
    uint8_t segments[4] = {
      0,
      opening >= 100 ? display.encodeDigit(opening / 100) : (uint8_t)0,
      opening >= 10 ? display.encodeDigit((opening / 10) % 10) : (uint8_t)0,
      (uint8_t)(display.encodeDigit(opening % 10) | 0x80)
    };
    display.setSegments(segments);
  }
}

void setup() {
  pinMode(LIGHT_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ADJUST_BUTTON_PIN, INPUT_PULLUP);

  analogReference(DEFAULT);

  // Discard the first conversion after switching ADC channels.
  analogRead(LIGHT_PIN);
  filteredLightADC = analogRead(LIGHT_PIN);
  analogRead(TEMP_PIN);
  filteredTemperatureADC = analogRead(TEMP_PIN);

  updateSensorStates();
  applyControl();

  currentServoDegrees = openingToDegrees(targetOpeningPercent);
  curtainServo.write(currentServoDegrees);
  curtainServo.attach(SERVO_PIN);

  display.setBrightness(4, true);
  refreshDisplay();

  unsigned long now = millis();
  lastSensorAt = now;
  lastServoAt = now;
  lastDisplayAt = now;
}

void loop() {
  unsigned long now = millis();

  updateButton(modeButton, now);
  updateButton(adjustButton, now);

  // Both short and long D2 presses toggle mode once per physical press.
  if (modeButton.shortPressEvent || modeButton.longPressEvent) {
    automaticMode = !automaticMode;

    if (!automaticMode) {
      // Hold the current position when entering manual mode.
      targetOpeningPercent = degreesToOpening(currentServoDegrees);
    }
  }

  if (adjustButton.longPressEvent) {
    showLightValue = !showLightValue;
    refreshDisplay();
  }

  if (adjustButton.shortPressEvent && !automaticMode) {
    targetOpeningPercent += 10;
    if (targetOpeningPercent > 100) {
      targetOpeningPercent = 0;
    }
  }

  if (now - lastSensorAt >= SENSOR_INTERVAL_MS) {
    lastSensorAt = now;

    analogRead(LIGHT_PIN);
    int lightADC = analogRead(LIGHT_PIN);
    analogRead(TEMP_PIN);
    int temperatureADC = analogRead(TEMP_PIN);

    // Low-pass filtering reduces chatter from ADC and sensor noise.
    filteredLightADC += 0.25f * (lightADC - filteredLightADC);
    filteredTemperatureADC +=
      0.25f * (temperatureADC - filteredTemperatureADC);

    updateSensorStates();
  }

  applyControl();

  // Smooth, nonblocking movement keeps buttons and sensing responsive.
  if (now - lastServoAt >= SERVO_INTERVAL_MS) {
    lastServoAt = now;
    int targetDegrees = openingToDegrees(targetOpeningPercent);

    if (currentServoDegrees < targetDegrees) {
      ++currentServoDegrees;
      curtainServo.write(currentServoDegrees);
    } else if (currentServoDegrees > targetDegrees) {
      --currentServoDegrees;
      curtainServo.write(currentServoDegrees);
    }
  }

  if (now - lastDisplayAt >= DISPLAY_INTERVAL_MS) {
    lastDisplayAt = now;
    refreshDisplay();
  }
}