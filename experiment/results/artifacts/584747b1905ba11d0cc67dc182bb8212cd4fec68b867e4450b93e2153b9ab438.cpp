#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

#define TRIG_IN   D8
#define ECHO_IN   D9
#define TRIG_OUT  D10
#define ECHO_OUT  D11

#define DHTPIN    D7
#define DHTTYPE   DHT11

#define BTN_PIN   D2

#define FAN_PWM   D5  // PWM output

// TM1637: CLK=D4, DIO=D3
#define TM1637_CLK D4
#define TM1637_DIO D3

// ---------- Configurable behavior constants (preserved/implemented) ----------
static const uint16_t TEMP_MAX_VALID_C = 60;  // sanity clamp for display/logic
static const uint32_t ULTRASONIC_TIMEOUT_US = 25000UL; // ~4m max

// Fan control: depends on people count and temperature
// (No explicit thresholds provided; implement reasonable, deterministic combined control)
static const int16_t FAN_OFF_PEOPLE = 0;       // if people == 0 => eco mode, fan off
static const uint16_t TEMP_TARGET_C = 26;     // baseline comfort temp
static const uint16_t TEMP_FULL_C   = 34;     // max temp for ramp to high speed

// People influence scales up additional fan speed
static const uint16_t PEOPLE_SCALE_PER_PERSON = 12; // PWM increment per person (capped)
static const uint16_t FAN_PWM_MIN = 0;
static const uint16_t FAN_PWM_MAX = 255;

// Display refresh
static const uint32_t DISPLAY_INTERVAL_MS = 200;

// Button debounce
static const uint32_t DEBOUNCE_MS = 40;

// Time gating for entry/exit detection to avoid double-counting
static const uint32_t EVENT_LOCKOUT_MS = 800;
static const uint32_t PAIR_TRIGGER_WINDOW_MS = 1200;

// Ultrasonic measurement helper
static uint32_t lastEventAtMs = 0;

// ---------- Global state ----------
DHT dht(DHTPIN, DHTTYPE);
TM1637Display tm1637(Tm1637_CLK, TM1637_DIO);

volatile int16_t peopleCount = 0;
bool showTemp = false;

// Button state
bool lastBtnStable = HIGH;
bool btnStable = HIGH;
uint32_t lastBtnChangeMs = 0;

// Sensor state for directional paired detection
enum PairState : uint8_t { IDLE = 0, IN_WAIT_OUT, OUT_WAIT_IN };
PairState pairState = IDLE;

bool inDetected = false;
bool outDetected = false;
uint32_t pairStateStartMs = 0;

uint32_t lastDisplayMs = 0;

float lastTempC = NAN;
uint32_t lastTempReadMs = 0;
static const uint32_t TEMP_READ_INTERVAL_MS = 2000;

// ---------- Utility ----------
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static long readUltrasonicCm(uint8_t trigPin, uint8_t echoPin);
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool detectPresenceIn();
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool detectPresenceOut();
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setFanPWM(uint16_t pwm);
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t computeFanPWM(int16_t people, float tempC);
#line 145 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplay();
#line 165 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 190 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static long readUltrasonicCm(uint8_t trigPin, uint8_t echoPin) {
  // Ensure clean trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure echo pulse width
  unsigned long start = micros();
  unsigned long duration = pulseIn(echoPin, HIGH, ULTRASONIC_TIMEOUT_US);
  if (duration == 0) return -1; // timeout or no echo

  // Convert to cm: duration(us) / 58.0
  float cm = duration / 58.0f;
  if (cm < 0) return -1;
  return (long)cm;
}

static bool detectPresenceIn() {
  // Door enter direction: if object within threshold
  // Threshold not specified; use typical 2..5cm range; choose 20cm to be robust.
  const long THRESH_CM = 30;
  long cm = readUltrasonicCm(TRIG_IN, ECHO_IN);
  return (cm > 0 && cm <= THRESH_CM);
}

static bool detectPresenceOut() {
  // Door exit direction: if object within threshold
  const long THRESH_CM = 30;
  long cm = readUltrasonicCm(TRIG_OUT, ECHO_OUT);
  return (cm > 0 && cm <= THRESH_CM);
}

static void setFanPWM(uint16_t pwm) {
  if (pwm > FAN_PWM_MAX) pwm = FAN_PWM_MAX;
  analogWrite(FAN_PWM, pwm);
}

static uint16_t computeFanPWM(int16_t people, float tempC) {
  if (people <= 0) return 0; // eco mode closes fan

  // Clamp temperature
  if (isnan(tempC)) tempC = TEMP_TARGET_C;
  if (tempC < 0) tempC = 0;
  if (tempC > TEMP_MAX_VALID_C) tempC = TEMP_MAX_VALID_C;

  // Temperature factor 0..1
  float t = (float)tempC;
  float denom = (float)(TEMP_FULL_C - TEMP_TARGET_C);
  float tf = 0.0f;
  if (t <= TEMP_TARGET_C) tf = 0.0f;
  else if (t >= TEMP_FULL_C) tf = 1.0f;
  else tf = (t - (float)TEMP_TARGET_C) / denom;

  // Base PWM from temperature, plus people scaling
  uint16_t tempBase = (uint16_t)( (float)FAN_PWM_MAX * tf );

  // People adds additional speed (capped)
  uint16_t peopleAdd = (uint16_t)min<uint32_t>(FAN_PWM_MAX, (uint32_t)people * PEOPLE_SCALE_PER_PERSON / 1);

  uint16_t pwm = (uint16_t)min<uint32_t>(FAN_PWM_MAX, (uint32_t)tempBase + (uint32_t)peopleAdd);

  // Keep some minimum if people exist but temp is low (still may ventilate lightly)
  if (pwm < 60) pwm = 60;
  return pwm;
}

static void updateDisplay() {
  if (showTemp) {
    // Display temperature; keep simple: integer degrees on 4 digits.
    int16_t tInt = 0;
    if (!isnan(lastTempC)) {
      tInt = (int16_t)roundf(lastTempC);
    }
    if (tInt < 0) tInt = 0;
    if (tInt > 99) tInt = 99;
    tm1637.showNumberDec(tInt, false, 4, 0);
    // no colon
  } else {
    int16_t p = peopleCount;
    if (p < 0) p = 0;
    if (p > 9999) p = 9999;
    tm1637.showNumberDec(p, false, 4, 0);
  }
}

// ---------- Setup/Loop ----------
void setup() {
  pinMode(TRIG_IN, OUTPUT);
  pinMode(ECHO_IN, INPUT);

  pinMode(TRIG_OUT, OUTPUT);
  pinMode(ECHO_OUT, INPUT);

  pinMode(BTN_PIN, INPUT_PULLUP);

  pinMode(FAN_PWM, OUTPUT);

  // DHT
  dht.begin();

  // TM1637
  tm1637.setBrightness(7, true);

  // Ensure initial fan state (eco)
  setFanPWM(0);

  // Initial display
  showTemp = false;
  updateDisplay();
}

void loop() {
  uint32_t now = millis();

  // -------- Button handling (D2 switch display temp/people) --------
  bool btnRead = digitalRead(BTN_PIN); // LOW when pressed (INPUT_PULLUP)
  if (btnRead != lastBtnStable) {
    lastBtnChangeMs = now;
    lastBtnStable = btnRead;
  }
  if ((now - lastBtnChangeMs) >= DEBOUNCE_MS && btnStable != lastBtnStable) {
    btnStable = lastBtnStable;
    // Trigger on falling edge (pressed)
    if (btnStable == LOW) {
      showTemp = !showTemp;
      // Update immediately for responsiveness
      updateDisplay();
    }
  }

  // -------- Periodic temperature reading --------
  if (now - lastTempReadMs >= TEMP_READ_INTERVAL_MS) {
    lastTempC = dht.readTemperature(); // Celsius
    if (isnan(lastTempC)) {
      // keep previous value if read failed
    }
    lastTempReadMs = now;
  }

  // -------- People counting via ordered dual ultrasonic triggering --------
  // Uses sequence: first IN then OUT => enter (+1)
  // first OUT then IN => exit (-1)
  // Applies lockout to avoid bouncing/double counting.
  bool inNow = detectPresenceIn();
  bool outNow = detectPresenceOut();

  // Event lockout to avoid rapid re-trigger
  if (now - lastEventAtMs >= EVENT_LOCKOUT_MS) {
    // Detect starting edge for sequence
    if (pairState == IDLE) {
      if (inNow && !outNow) {
        pairState = IN_WAIT_OUT;
        inDetected = true;
        outDetected = false;
        pairStateStartMs = now;
      } else if (outNow && !inNow) {
        pairState = OUT_WAIT_IN;
        outDetected = true;
        inDetected = false;
        pairStateStartMs = now;
      } else if (inNow && outNow) {
        // Both triggered simultaneously; treat as no valid direction to prevent wrong count
      }
    } else if (pairState == IN_WAIT_OUT) {
      // Wait for OUT confirmation
      if (outNow) {
        peopleCount++;
        lastEventAtMs = now;
        pairState = IDLE;
        inDetected = false;
        outDetected = false;
      } else if (now - pairStateStartMs > PAIR_TRIGGER_WINDOW_MS) {
        // Timeout, reset
        pairState = IDLE;
        inDetected = false;
        outDetected = false;
      }
    } else if (pairState == OUT_WAIT_IN) {
      // Wait for IN confirmation
      if (inNow) {
        peopleCount--;
        if (peopleCount < 0) peopleCount = 0;
        lastEventAtMs = now;
        pairState = IDLE;
        inDetected = false;
        outDetected = false;
      } else if (now - pairStateStartMs > PAIR_TRIGGER_WINDOW_MS) {
        pairState = IDLE;
        inDetected = false;
        outDetected = false;
        pairState = IDLE;
      }
    }
  } else {
    // During lockout, reset any pending pair to ensure clean counting
    if (pairState != IDLE) {
      pairState = IDLE;
      inDetected = false;
      outDetected = false;
    }
  }

  // -------- Fan control based on people and temperature; eco mode when 0 --------
  // Keep deterministic update rate
  static uint32_t lastFanUpdateMs = 0;
  if (now - lastFanUpdateMs >= 250) {
    uint16_t pwm = computeFanPWM(peopleCount, lastTempC);
    setFanPWM(pwm);
    lastFanUpdateMs = now;
  }

  // -------- Display update interval --------
  if (now - lastDisplayMs >= DISPLAY_INTERVAL_MS) {
    updateDisplay();
    lastDisplayMs = now;
  }

  // Small delay to reduce CPU load from repeated ultrasonic reads
  delay(20);
}
