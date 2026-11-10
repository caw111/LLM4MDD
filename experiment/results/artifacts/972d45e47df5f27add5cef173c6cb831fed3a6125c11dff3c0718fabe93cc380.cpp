#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t PIN_LED_PWM = 9;     // PWM LED D9
const uint8_t PIN_BUTTON  = 2;     // Button D2 (mode toggle)
const uint8_t PIN_TRIG     = 8;     // HC-SR04 Trig D8
const uint8_t PIN_ECHO     = 3;     // HC-SR04 Echo D3
const uint8_t PIN_DEBUG    = 13;   // D13 (left as digital output per inventory)

const uint8_t PIN_LDR = A0;         // LDR analog input A0

// --- Behavior configuration (must preserve required logic) ---
static const uint8_t  LED_BRIGHTNESS_AUTO = 180;   // PWM value when auto-on
static const uint8_t  LED_BRIGHTNESS_FORCE = 255;  // PWM value when forced on

// Darkness threshold: only light up automatically if environment is "dark enough".
static const int LDR_DARK_THRESHOLD = 500; // A0: 0..1023 (tune as needed)

// HC-SR04 proximity threshold (in cm): consider "person close"
static const long PROXIMITY_CM_THRESHOLD = 60; // tune as needed

// Debounce parameters
static const unsigned long DEBOUNCE_MS = 40;

// Mode states
enum Mode : uint8_t {
  MODE_AUTO = 0,
  MODE_FORCE = 1
};

volatile Mode mode = MODE_AUTO;

// Debounce handling
bool lastButtonReading = HIGH;
bool buttonStableState = HIGH;
unsigned long lastDebounceTime = 0;

// Timing for sensor measurements / energy saving
static const unsigned long ULTRASONIC_INTERVAL_MS = 200;
static const unsigned long LDR_INTERVAL_MS = 300;

// Latest sensor values
long lastDistanceCm = 9999;
int  lastLdrValue = 1023;

unsigned long lastUltrasonicMs = 0;
unsigned long lastLdrMs = 0;

// Helper: set LED PWM according to mode/state
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void applyLed(bool on, uint8_t brightnessAuto, uint8_t brightnessForce, bool isForce);
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
long readDistanceCm();
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
int readLdr();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool isDarkEnough(int ldrValue);
#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void applyLed(bool on, uint8_t brightnessAuto, uint8_t brightnessForce, bool isForce) {
  if (!on) {
    analogWrite(PIN_LED_PWM, 0);
    return;
  }
  analogWrite(PIN_LED_PWM, isForce ? brightnessForce : brightnessAuto);
}

long readDistanceCm() {
  // Trigger pulse
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Read echo (timeout to avoid blocking forever)
  // Speed of sound: ~58 us/cm round trip, so 60cm => ~3480us.
  // We'll timeout at ~40ms.
  unsigned long duration = pulseIn(PIN_ECHO, HIGH, 40000UL);
  if (duration == 0) return 9999; // out of range / timeout

  // Convert to cm (duration is microseconds for round trip)
  long cm = (long)(duration / 58UL);
  return cm;
}

int readLdr() {
  return analogRead(PIN_LDR);
}

bool isDarkEnough(int ldrValue) {
  return ldrValue >= LDR_DARK_THRESHOLD;
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  pinMode(PIN_BUTTON, INPUT_PULLUP); // assumes button connects to GND when pressed

  pinMode(PIN_DEBUG, OUTPUT);
  digitalWrite(PIN_DEBUG, LOW);

  analogWrite(PIN_LED_PWM, 0);

  // Initialize stable button state
  lastButtonReading = digitalRead(PIN_BUTTON);
  buttonStableState = lastButtonReading;
  lastDebounceTime = millis();

  mode = MODE_AUTO;
}

void loop() {
  // ---- Button handling: toggle AUTO <-> FORCE ----
  bool reading = digitalRead(PIN_BUTTON);
  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
    lastButtonReading = reading;
  }

  // Consider a stable state change after debounce
  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    if (reading != buttonStableState) {
      buttonStableState = reading;

      // Detect press event (assuming active-low; pressed => LOW)
      if (buttonStableState == LOW) {
        mode = (mode == MODE_AUTO) ? MODE_FORCE : MODE_AUTO;
      }
    }
  }

  // ---- Read sensors at intervals to be energy efficient ----
  unsigned long now = millis();

  if (now - lastUltrasonicMs >= ULTRASONIC_INTERVAL_MS) {
    lastUltrasonicMs = now;
    lastDistanceCm = readDistanceCm();
  }

  if (now - lastLdrMs >= LDR_INTERVAL_MS) {
    lastLdrMs = now;
    lastLdrValue = readLdr();
  }

  // ---- Apply required behavior ----
  bool autoOn = false;
  if (mode == MODE_AUTO) {
    // Only light automatically if environment is dark AND person is close
    autoOn = isDarkEnough(lastLdrValue) && (lastDistanceCm <= PROXIMITY_CM_THRESHOLD);
    applyLed(autoOn, LED_BRIGHTNESS_AUTO, LED_BRIGHTNESS_FORCE, false);

    // Debug indicator: reflect auto-on state
    digitalWrite(PIN_DEBUG, autoOn ? HIGH : LOW);
  } else {
    // Forced mode: always on (regardless of LDR and distance)
    applyLed(true, LED_BRIGHTNESS_AUTO, LED_BRIGHTNESS_FORCE, true);
    digitalWrite(PIN_DEBUG, HIGH);
  }
}
