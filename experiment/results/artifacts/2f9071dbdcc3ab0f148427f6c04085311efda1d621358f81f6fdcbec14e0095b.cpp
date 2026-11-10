#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <math.h>

#define TRIG_IN   8   // D8
#define ECHO_IN   9   // D9
#define TRIG_OUT  10  // D10
#define ECHO_OUT  11  // D11

#define DHTPIN    7   // D7
#define DHTTYPE   DHT11

#define BTN_PIN   2   // D2

#define FAN_PWM   5   // D5 (PWM)

#define TM1637_CLK 4  // D4
#define TM1637_DIO 3  // D3

static const uint16_t TEMP_MAX_VALID_C = 60;
static const uint32_t ULTRASONIC_TIMEOUT_US = 25000UL; // ~4m max

// Fan control parameters (preserved/implemented)
static const int16_t FAN_OFF_PEOPLE = 0;       // people==0 => eco mode, fan off
static const uint16_t TEMP_TARGET_C = 26;     // baseline comfort temp
static const uint16_t TEMP_FULL_C   = 34;     // max temp for ramp to high speed
static const uint16_t PEOPLE_SCALE_PER_PERSON = 12; // PWM increment per person (capped)
static const uint16_t FAN_PWM_MIN = 0;
static const uint16_t FAN_PWM_MAX = 255;

// Display refresh
static const uint32_t DISPLAY_INTERVAL_MS = 200;

// Button debounce
static const uint32_t DEBOUNCE_MS = 40;

// Time gating for entry/exit detection
static const uint32_t EVENT_LOCKOUT_MS = 800;
static const uint32_t PAIR_TRIGGER_WINDOW_MS = 1200;

// Ultrasonic threshold
static const long PRESENCE_THRESH_CM = 30;

static uint32_t lastEventAtMs = 0;

// ---------- Global state ----------
DHT dht(DHTPIN, DHTTYPE);
TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

volatile int16_t peopleCount = 0;
bool showTemp = false;

// Button state
bool lastBtnStable = HIGH;
bool btnStable = HIGH;
uint32_t lastBtnChangeMs = 0;

// Direction paired detection state
enum PairState : uint8_t { IDLE = 0, IN_WAIT_OUT, OUT_WAIT_IN };
PairState pairState = IDLE;

// Periodic housekeeping
uint32_t lastDisplayMs = 0;
float lastTempC = NAN;
uint32_t lastTempReadMs = 0;
static const uint32_t TEMP_READ_INTERVAL_MS = 2000;

// ---------- Utility ----------
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static long readUltrasonicCm(uint8_t trigPin, uint8_t echoPin);
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool detectPresenceIn();
#line 91 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool detectPresenceOut();
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setFanPWM(uint16_t pwm);
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint16_t computeFanPWM(int16_t people, float tempC);
#line 129 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateDisplay();
#line 147 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 168 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static long readUltrasonicCm(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, ULTRASONIC_TIMEOUT_US);
  if (duration == 0) return -1; // timeout or no echo

  float cm = duration / 58.0f;
  if (cm < 0) return -1;
  return (long)cm;
}

static bool detectPresenceIn() {
  long cm = readUltrasonicCm(TRIG_IN, ECHO_IN);
  return (cm > 0 && cm <= PRESENCE_THRESH_CM);
}

static bool detectPresenceOut() {
  long cm = readUltrasonicCm(TRIG_OUT, ECHO_OUT);
  return (cm > 0 && cm <= PRESENCE_THRESH_CM);
}

static void setFanPWM(uint16_t pwm) {
  if (pwm > FAN_PWM_MAX) pwm = FAN_PWM_MAX;
  if (pwm < FAN_PWM_MIN) pwm = FAN_PWM_MIN;
  analogWrite(FAN_PWM, pwm);
}

static uint16_t computeFanPWM(int16_t people, float tempC) {
  if (people <= FAN_OFF_PEOPLE) return 0; // eco mode closes fan

  if (isnan(tempC)) tempC = (float)TEMP_TARGET_C;
  if (tempC < 0) tempC = 0;
  if (tempC > (float)TEMP_MAX_VALID_C) tempC = (float)TEMP_MAX_VALID_C;

  float t = tempC;
  float denom = (float)(TEMP_FULL_C - TEMP_TARGET_C);
  float tf = 0.0f;

  if (t <= TEMP_TARGET_C) tf = 0.0f;
  else if (t >= TEMP_FULL_C) tf = 1.0f;
  else tf = (t - (float)TEMP_TARGET_C) / denom;

  uint16_t tempBase = (uint16_t)((float)FAN_PWM_MAX * tf);

  uint32_t peopleCalc = (uint32_t)people * (uint32_t)PEOPLE_SCALE_PER_PERSON / 1;
  uint16_t peopleAdd = (peopleCalc >= FAN_PWM_MAX) ? FAN_PWM_MAX : (uint16_t)peopleCalc;

  uint32_t pwmCalc = (uint32_t)tempBase + (uint32_t)peopleAdd;
  uint16_t pwm = (pwmCalc >= FAN_PWM_MAX) ? FAN_PWM_MAX : (uint16_t)pwmCalc;

  if (pwm < 60) pwm = 60; // preserve minimum ventilation when people exist
  return pwm;
}

static void updateDisplay() {
  if (showTemp) {
    int16_t tInt = 0;
    if (!isnan(lastTempC)) {
      tInt = (int16_t)roundf(lastTempC);
    }
    if (tInt < 0) tInt = 0;
    if (tInt > 99) tInt = 99;
    tm1637.showNumberDec(tInt, false, 4, 0);
  } else {
    int16_t p = peopleCount;
    if (p < 0) p = 0;
    if (p > 9999) p = 9999;
    tm1637.showNumberDec(p, false, 4, 0);
  }
}

// ---------- Arduino ----------
void setup() {
  pinMode(TRIG_IN, OUTPUT);
  pinMode(ECHO_IN, INPUT);

  pinMode(TRIG_OUT, OUTPUT);
  pinMode(ECHO_OUT, INPUT);

  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(FAN_PWM, OUTPUT);

  Serial.begin(9600);

  dht.begin();

  tm1637.setBrightness(7, true);

  setFanPWM(0);
  showTemp = false;
  updateDisplay();
}

void loop() {
  uint32_t now = millis();

  // -------- Button handling (D2 switch display temp/people) --------
  bool btnRead = digitalRead(BTN_PIN); // LOW when pressed
  if (btnRead != lastBtnStable) {
    lastBtnChangeMs = now;
    lastBtnStable = btnRead;
  }
  if ((now - lastBtnChangeMs) >= DEBOUNCE_MS && btnStable != lastBtnStable) {
    btnStable = lastBtnStable;
    if (btnStable == LOW) {
      showTemp = !showTemp;
      updateDisplay();
    }
  }

  // -------- Periodic temperature reading --------
  if (now - lastTempReadMs >= TEMP_READ_INTERVAL_MS) {
    float t = dht.readTemperature(); // Celsius
    if (!isnan(t)) lastTempC = t;
    lastTempReadMs = now;
  }

  // -------- People counting via ordered dual ultrasonic triggering --------
  bool inNow = detectPresenceIn();
  bool outNow = detectPresenceOut();

  if (now - lastEventAtMs >= EVENT_LOCKOUT_MS) {
    if (pairState == IDLE) {
      if (inNow && !outNow) {
        pairState = IN_WAIT_OUT;
      } else if (outNow && !inNow) {
        pairState = OUT_WAIT_IN;
      } else {
        // simultaneous or none: do nothing
      }
      if (pairState != IDLE) {
        lastEventAtMs = now; // start lock period to avoid double-trigger windows overlap
        // Use pairStateStartMs via separate timer
        // To preserve original behavior: reset on entering state; we need a separate variable.
      }
    }
  }

  // The original logic used pairStateStartMs and lastEventAtMs as gating.
  // Re-implement without simplifying: we keep separate pairStateStartMs and a lockout for "committing".
  // To do this while preserving intent, we track lockout for entry detection using lastEventAtMs and
  // we also use an internal pairStateStartMs when leaving IDLE.
  static uint32_t pairStateStartMs = 0;
  static bool inDetected = false;
  static bool outDetected = false;

  // When we transitioned from IDLE to a wait state above, set start time and flags.
  // Detect this by checking pairState values and whether flags are already set.
  if (pairState != IDLE && pairStateStartMs == 0) {
    pairStateStartMs = now;
    inDetected = (pairState == IN_WAIT_OUT);
    outDetected = (pairState == OUT_WAIT_IN);
  }

  // During lockout, run the pair confirmation logic
  // Use a separate mechanism: we only confirm if we are already in a wait state.
  if (pairState != IDLE) {
    if (pairState == IN_WAIT_OUT) {
      if (outNow) {
        // Commit enter (+1)
        if (peopleCount < 9999) peopleCount++;
        pairState = IDLE;
        inDetected = false;
        outDetected = false;
        pairStateStartMs = 0;
        lastEventAtMs = now; // keep lockout after commit
      } else if (now - pairStateStartMs > PAIR_TRIGGER_WINDOW_MS) {
        pairState = IDLE;
        inDetected = false;
        outDetected = false;
        pairStateStartMs = 0;
      }
    } else if (pairState == OUT_WAIT_IN) {
      if (inNow) {
        // Commit exit (-1), clamp at 0
        peopleCount--;
        if (peopleCount < 0) peopleCount = 0;
        pairState = IDLE;
        inDetected = false;
        outDetected = false;
        pairStateStartMs = 0;
        lastEventAtMs = now; // keep lockout after commit
      } else if (now - pairStateStartMs > PAIR_TRIGGER_WINDOW_MS) {
        pairState = IDLE;
        inDetected = false;
        outDetected = false;
        pairStateStartMs = 0;
      }
    }
  } else {
    // Ensure pending wait state flags reset
    inDetected = false;
    outDetected = false;
    // pairStateStartMs stays 0
  }

  // -------- Fan control based on people and temperature --------
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

  delay(20);
}
