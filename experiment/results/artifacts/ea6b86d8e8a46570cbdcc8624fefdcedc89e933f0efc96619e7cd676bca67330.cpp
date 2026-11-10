#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// -------------------- Pin Assignments (authoritative) --------------------
static const uint8_t TRIG_IN  = 8;   // Door enter direction
static const uint8_t ECHO_IN  = 9;   // Door enter direction
static const uint8_t TRIG_OUT = 10;  // Door exit direction
static const uint8_t ECHO_OUT = 11;  // Door exit direction

static const uint8_t DHT_PIN   = 7;   // DHT11
static const uint8_t FAN_PWM   = 5;   // Fan PWM output (PWM capable on UNO)

static const uint8_t TM_CLK    = 4;   // TM1637 CLK
static const uint8_t TM_DIO    = 3;   // TM1637 DIO

static const uint8_t BTN_PIN   = 2;   // Button: toggle display (temp/number)

// DHT type
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

// TM1637
TM1637Display display(TM_CLK, TM_DIO);

// -------------------- System State --------------------
volatile long people = 0;

// Display toggle
bool showTemp = false;

// Button handling
static bool lastBtnStable = HIGH;
static unsigned long lastBtnChangeMs = 0;
static const unsigned long BTN_DEBOUNCE_MS = 30;

// Energy saving / fan control
static bool fanEnabled = true;

// -------------------- Ultrasonic / People Counting --------------------
// We use sequential triggering of two HC-SR04 modules to estimate direction.
// The requirement says: "通过两个超声波先后触发顺序估计人数增减；"
// Implementation: we detect a "presence event" from IN sensor then look for a
// follow-up from OUT within a time window (enter sequence), and vice versa.
static const unsigned long SENSOR_POLL_MS = 60;
static unsigned long lastPollMs = 0;

// Distance threshold for "someone detected" (cm)
static const float DETECT_CM_IN  = 60.0f; // presence if closer than this
static const float DETECT_CM_OUT = 60.0f;

// Keep last detection times (ms)
static unsigned long tInSeen  = 0;
static unsigned long tOutSeen = 0;

// Sequence timeout window (ms)
static const unsigned long SEQ_WINDOW_MS = 1200;

// Minimum interval between count updates (ms) to prevent double counting
static const unsigned long COUNT_COOLDOWN_MS = 1500;
static unsigned long lastCountMs = 0;

// Helper: measure distance using HC-SR04
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float measureDistanceCm(uint8_t trigPin, uint8_t echoPin);
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool inPresence();
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool outPresence();
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updatePeopleCount();
#line 143 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setFanPWM(uint8_t pwmVal);
#line 156 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t computeFanPWM(float tempC, long p);
#line 189 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showPeopleOnDisplay();
#line 196 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showTempOnDisplay();
#line 212 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplay();
#line 221 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 257 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float measureDistanceCm(uint8_t trigPin, uint8_t echoPin) {
  // Trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo with timeout
  const unsigned long timeoutUs = 30000UL; // ~5m
  unsigned long start = micros();
  while (digitalRead(echoPin) == LOW) {
    if ((micros() - start) > timeoutUs) return 9999.0f;
  }

  start = micros();
  while (digitalRead(echoPin) == HIGH) {
    if ((micros() - start) > timeoutUs) return 9999.0f;
  }

  unsigned long echoTimeUs = micros() - start;
  // Speed of sound: 343 m/s -> 0.0343 cm/us
  // Distance = echoTime * 0.0343 / 2
  float distanceCm = (echoTimeUs * 0.0343f) / 2.0f;
  return distanceCm;
}

// Detect presence from sensors
static bool inPresence() {
  float d = measureDistanceCm(TRIG_IN, ECHO_IN);
  return (d > 0.0f && d < DETECT_CM_IN);
}

static bool outPresence() {
  float d = measureDistanceCm(TRIG_OUT, ECHO_OUT);
  return (d > 0.0f && d < DETECT_CM_OUT);
}

// Count logic based on sequential order
static void updatePeopleCount() {
  unsigned long now = millis();

  // Poll at a controlled rate
  if (now - lastPollMs < SENSOR_POLL_MS) return;
  lastPollMs = now;

  // Read both sensors (order not critical; "先后触发顺序" is captured by timestamps)
  bool inDet = inPresence();
  bool outDet = outPresence();

  if (inDet) tInSeen = now;
  if (outDet) tOutSeen = now;

  // Determine sequence only if enough time has passed since last count
  if (now - lastCountMs < COUNT_COOLDOWN_MS) return;

  // Enter: IN seen then OUT seen within window => +1
  if (tInSeen != 0 && tOutSeen != 0) {
    unsigned long dt = (tOutSeen >= tInSeen) ? (tOutSeen - tInSeen) : (tInSeen - tOutSeen);
    if (dt <= SEQ_WINDOW_MS) {
      if (tInSeen < tOutSeen) {
        people++;
        lastCountMs = now;
        // Prevent immediate repeated counts by clearing the timestamps slightly
        tInSeen = 0;
        tOutSeen = 0;
      } else {
        // Exit: OUT seen then IN seen within window => -1 (not below 0)
        if (people > 0) people--;
        lastCountMs = now;
        tInSeen = 0;
        tOutSeen = 0;
      }
    }
  }
}

// -------------------- Temperature & Fan Control --------------------
// Requirement: "根据人数和温度共同决定风扇转速；若人数为 0 则系统进入节能模式并关闭风扇。"
static float readTemperatureC() {
  // DHT11 single read; can return NAN if failed
  float t = dht.readTemperature(); // Celsius
  if (isnan(t)) return -999.0f;
  return t;
}

// PWM helper
static void setFanPWM(uint8_t pwmVal) {
  // pwmVal: 0..255 (UNO analogWrite)
  analogWrite(FAN_PWM, pwmVal);
}

static uint8_t computeFanPWM(float tempC, long p) {
  // If people == 0 => fan off (energy-saving)
  if (p <= 0) return 0;

  // Decide speed based on temperature AND people.
  // Since explicit thresholds were not provided, we choose reasonable behavior:
  // - Map temperature to a base PWM.
  // - Scale up with number of people.
  // Preserve requirement behavior: depend on both people count and temperature, and turn off at 0.
  if (tempC < -50.0f) tempC = 25.0f; // fallback if read failed

  // Base PWM from temperature (rough linear piecewise)
  // 18C => low, 24C => medium, 30C+ => high
  float base;
  if (tempC <= 18.0f) base = 40.0f;
  else if (tempC <= 24.0f) base = 40.0f + (tempC - 18.0f) * (110.0f / 6.0f); // up to ~150
  else if (tempC <= 30.0f) base = 150.0f + (tempC - 24.0f) * (105.0f / 6.0f); // up to ~255
  else base = 255.0f;

  // Scale with people: more people -> slightly higher speed
  // people 1..10 => multiplier 1.0..1.5 capped
  float mult = 1.0f + min(10L, p) * (0.5f / 10.0f);
  float pwm = base * mult;
  if (pwm > 255.0f) pwm = 255.0f;

  // Ensure non-zero when people>0
  if (pwm < 30.0f) pwm = 30.0f;
  return (uint8_t)(pwm + 0.5f);
}

// -------------------- Display --------------------
// Requirement:
// "TM1637 默认显示人数，按钮 D2 可切换显示当前温度。"
static void showPeopleOnDisplay() {
  long p = people;
  if (p < 0) p = 0; // safety
  // TM1637Display expects int; set to right-aligned
  display.showNumberDec((int)min<long>(p, 9999), false);
}

static void showTempOnDisplay() {
  float t = readTemperatureC();
  // If failed, show placeholder (e.g., 0)
  if (t < -50.0f) t = 0.0f;

  // Show temperature with sign? TM1637Display supports showNumberDecEx with decimals,
  // but simplest is integer temperature. We'll display rounded int with no decimal.
  int ti = (int)round(t);
  // Limit range for display
  if (ti > 999) ti = 999;
  if (ti < -99) ti = -99;

  display.showNumberDec(ti, true);
}

// Update display based on showTemp flag and people count
static void updateDisplay() {
  if (showTemp) {
    showTempOnDisplay();
  } else {
    showPeopleOnDisplay();
  }
}

// -------------------- Setup / Loop --------------------
void setup() {
  // Serial
  Serial.begin(9600);

  // GPIO
  pinMode(TRIG_IN, OUTPUT);
  pinMode(ECHO_IN, INPUT);
  pinMode(TRIG_OUT, OUTPUT);
  pinMode(ECHO_OUT, INPUT);

  digitalWrite(TRIG_IN, LOW);
  digitalWrite(TRIG_OUT, LOW);

  // Button input with pullup
  pinMode(BTN_PIN, INPUT_PULLUP);

  // Fan PWM output
  pinMode(FAN_PWM, OUTPUT);

  // DHT
  dht.begin();

  // TM1637 init
  display.setBrightness(7, true);

  // Default display: people
  showTemp = false;

  // Start with fan off if people=0
  setFanPWM(0);
  fanEnabled = false;

  // Initial show
  updateDisplay();
}

void loop() {
  // 1) Button debounce and toggle display mode
  bool btnRaw = digitalRead(BTN_PIN); // HIGH when not pressed due to pullup
  if (btnRaw != lastBtnStable) {
    lastBtnChangeMs = millis();
    lastBtnStable = btnRaw;
  }
  // Only toggle on stable LOW press
  static bool lastToggleStatePressed = false;
  unsigned long now = millis();

  if (now - lastBtnChangeMs > BTN_DEBOUNCE_MS) {
    bool pressed = (digitalRead(BTN_PIN) == LOW);
    if (pressed && !lastToggleStatePressed) {
      showTemp = !showTemp;
      lastToggleStatePressed = true;
      // Update display immediately on toggle
      updateDisplay();
    } else if (!pressed) {
      lastToggleStatePressed = false;
    }
  }

  // 2) People count update using ultrasonic sensors (sequential order estimation)
  long prevPeople = people;
  updatePeopleCount();

  // 3) Read temperature periodically (and compute fan speed based on people and temperature)
  static unsigned long lastTempMs = 0;
  static float lastTempC = 25.0f;
  const unsigned long TEMP_INTERVAL_MS = 1000;

  if (millis() - lastTempMs >= TEMP_INTERVAL_MS) {
    lastTempMs = millis();
    float t = readTemperatureC();
    if (t > -50.0f) lastTempC = t;

    // Energy saving: if people is 0 => fan off
    uint8_t pwm = computeFanPWM(lastTempC, people);
    bool nowFanEnabled = (pwm > 0);

    if (pwm == 0) {
      if (fanEnabled) {
        // Enter energy saving mode: close fan
        setFanPWM(0);
        fanEnabled = false;
      }
    } else {
      // Turn fan to computed speed
      setFanPWM(pwm);
      fanEnabled = nowFanEnabled;
    }

    // Serial output (optional but requested: "和串口")
    // Provide line-based status
    Serial.print("people=");
    Serial.print(people);
    Serial.print(", tempC=");
    Serial.print(lastTempC, 1);
    Serial.print(", fanPWM=");
    Serial.println((int)(pwm));
  } else {
    // Still keep energy-saving consistent if people changes quickly
    if (people <= 0 && fanEnabled) {
      setFanPWM(0);
      fanEnabled = false;
    }
  }

  // 4) Update display
  // - Default shows people; when showTemp active shows temp
  // - Ensure display changes when people count changes
  static long lastShownPeople = -1;
  static float lastShownTemp = 1e9;
  static unsigned long lastDisplayMs = 0;
  const unsigned long DISPLAY_INTERVAL_MS = 200;

  if (millis() - lastDisplayMs >= DISPLAY_INTERVAL_MS) {
    lastDisplayMs = millis();

    if (!showTemp) {
      if (people != lastShownPeople) {
        showPeopleOnDisplay();
        lastShownPeople = people;
      }
    } else {
      // Update temp display periodically
      float t = readTemperatureC();
      if (t > -50.0f) lastTempC = t;
      if (fabs(lastTempC - lastShownTemp) >= 0.5f) {
        showTempOnDisplay();
        lastShownTemp = lastTempC;
      }
    }
  }

  // 5) If people count changed and display is people mode, update immediately
  if (!showTemp && people != prevPeople) {
    showPeopleOnDisplay();
    lastShownPeople = people;
  }
}
