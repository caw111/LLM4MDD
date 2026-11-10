#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// -------------------- Pin Assignments (authoritative) --------------------
static const uint8_t TRIG_IN  = 8;   // 门口进入方向 Trig
static const uint8_t ECHO_IN  = 9;   // 门口进入方向 Echo
static const uint8_t TRIG_OUT = 10;  // 门口离开方向 Trig
static const uint8_t ECHO_OUT = 11;  // 门口离开方向 Echo

static const uint8_t DHT_PIN = 7;     // DHT11 数据 D7
static const uint8_t FAN_PWM = 5;    // 风扇 PWM D5

static const uint8_t TM_CLK = 4;     // TM1637 CLK D4
static const uint8_t TM_DIO = 3;     // TM1637 DIO D3

static const uint8_t BTN_PIN = 2;     // 按钮 D2：切换显示温度/人数

// DHT
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

// TM1637
TM1637Display display(TM_CLK, TM_DIO);

// -------------------- System State --------------------
volatile long people = 0;

// Display toggle
bool showTemp = false;

// Button handling (debounce)
static bool lastBtnStable = HIGH;
static unsigned long lastBtnChangeMs = 0;
static const unsigned long BTN_DEBOUNCE_MS = 30;

// Fan enabled state (energy-saving)
static bool fanEnabled = true;

// -------------------- Ultrasonic / People Counting --------------------
static const unsigned long SENSOR_POLL_MS = 60;
static unsigned long lastPollMs = 0;

// Presence distance threshold (cm)
static const float DETECT_CM_IN  = 60.0f;
static const float DETECT_CM_OUT = 60.0f;

// Keep last detection times (ms)
static unsigned long tInSeen = 0;
static unsigned long tOutSeen = 0;

// Sequence timeout window (ms)
static const unsigned long SEQ_WINDOW_MS = 1200;

// Minimum interval between count updates
static const unsigned long COUNT_COOLDOWN_MS = 1500;
static unsigned long lastCountMs = 0;

static float measureDistanceCm(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

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
  float distanceCm = (echoTimeUs * 0.0343f) / 2.0f;
  return distanceCm;
}

static bool inPresence() {
  float d = measureDistanceCm(TRIG_IN, ECHO_IN);
  return (d > 0.0f && d < DETECT_CM_IN);
}

static bool outPresence() {
  float d = measureDistanceCm(TRIG_OUT, ECHO_OUT);
  return (d > 0.0f && d < DETECT_CM_OUT);
}

// 根据两个超声波先后触发顺序估计人数增减
static void updatePeopleCount() {
  unsigned long now = millis();

  if (now - lastPollMs < SENSOR_POLL_MS) return;
  lastPollMs = now;

  bool inDet = inPresence();
  bool outDet = outPresence();

  if (inDet)  tInSeen = now;
  if (outDet) tOutSeen = now;

  if (now - lastCountMs < COUNT_COOLDOWN_MS) return;

  if (tInSeen != 0 && tOutSeen != 0) {
    unsigned long dt = (tOutSeen >= tInSeen) ? (tOutSeen - tInSeen) : (tInSeen - tOutSeen);
    if (dt <= SEQ_WINDOW_MS) {
      // Enter: IN then OUT => +1
      if (tInSeen < tOutSeen) {
        people++;
        lastCountMs = now;
        tInSeen = 0;
        tOutSeen = 0;
      } else {
        // Exit: OUT then IN => -1 (not below 0)
        if (people > 0) people--;
        lastCountMs = now;
        tInSeen = 0;
        tOutSeen = 0;
      }
    }
  }
}

// -------------------- Temperature & Fan Control --------------------
static float readTemperatureC() {
  float t = dht.readTemperature(); // Celsius
  if (isnan(t)) return -999.0f;
  return t;
}

static void setFanPWM(uint8_t pwmVal) {
  analogWrite(FAN_PWM, pwmVal);
}

static uint8_t computeFanPWM(float tempC, long p) {
  // 若人数为0 -> 节能模式关闭风扇
  if (p <= 0) return 0;

  if (tempC < -50.0f) tempC = 25.0f; // fallback

  // 基于温度的基础速度
  float base;
  if (tempC <= 18.0f) base = 40.0f;
  else if (tempC <= 24.0f) base = 40.0f + (tempC - 18.0f) * (110.0f / 6.0f); // up to ~150
  else if (tempC <= 30.0f) base = 150.0f + (tempC - 24.0f) * (105.0f / 6.0f); // up to ~255
  else base = 255.0f;

  // 根据人数放大
  float mult = 1.0f + (float)min((long)10, p) * (0.5f / 10.0f); // 1..10 => 1.0..1.5
  float pwm = base * mult;
  if (pwm > 255.0f) pwm = 255.0f;

  // 确保人数>0时有最小转速
  if (pwm < 30.0f) pwm = 30.0f;

  return (uint8_t)(pwm + 0.5f);
}

// -------------------- Display --------------------
static void showPeopleOnDisplay() {
  long p = people;
  if (p < 0) p = 0;
  int v;
  if (p > 9999) v = 9999;
  else v = (int)p;
  display.showNumberDec(v, false);
}

static void showTempOnDisplay() {
  float t = readTemperatureC();
  if (t < -50.0f) t = 0.0f;

  int ti = (int)round(t);
  if (ti > 999) ti = 999;
  if (ti < -99) ti = -99;

  display.showNumberDec(ti, true);
}

static void updateDisplay() {
  if (showTemp) showTempOnDisplay();
  else showPeopleOnDisplay();
}

// -------------------- Setup / Loop --------------------
void setup() {
  Serial.begin(9600);

  pinMode(TRIG_IN, OUTPUT);
  pinMode(ECHO_IN, INPUT);
  pinMode(TRIG_OUT, OUTPUT);
  pinMode(ECHO_OUT, INPUT);

  digitalWrite(TRIG_IN, LOW);
  digitalWrite(TRIG_OUT, LOW);

  pinMode(BTN_PIN, INPUT_PULLUP);

  pinMode(FAN_PWM, OUTPUT);

  dht.begin();

  display.setBrightness(7, true);

  showTemp = false;

  setFanPWM(0);
  fanEnabled = false;

  updateDisplay();
}

void loop() {
  // 1) 按钮去抖并切换显示模式
  bool btnRaw = digitalRead(BTN_PIN); // HIGH=未按下（上拉）
  if (btnRaw != lastBtnStable) {
    lastBtnChangeMs = millis();
    lastBtnStable = btnRaw;
  }

  static bool lastToggleStatePressed = false;
  unsigned long now = millis();

  if (now - lastBtnChangeMs > BTN_DEBOUNCE_MS) {
    bool pressed = (digitalRead(BTN_PIN) == LOW);
    if (pressed && !lastToggleStatePressed) {
      showTemp = !showTemp;
      lastToggleStatePressed = true;
      updateDisplay();
    } else if (!pressed) {
      lastToggleStatePressed = false;
    }
  }

  // 2) 人数统计：通过两个超声波先后触发顺序估计增减
  long prevPeople = people;
  updatePeopleCount();

  // 3) 温度采样并结合人数控制风扇PWM
  static unsigned long lastTempMs = 0;
  static float lastTempC = 25.0f;
  const unsigned long TEMP_INTERVAL_MS = 1000;

  if (millis() - lastTempMs >= TEMP_INTERVAL_MS) {
    lastTempMs = millis();
    float t = readTemperatureC();
    if (t > -50.0f) lastTempC = t;

    uint8_t pwm = computeFanPWM(lastTempC, people);

    if (pwm == 0) {
      if (fanEnabled) {
        setFanPWM(0);
        fanEnabled = false;
      }
    } else {
      setFanPWM(pwm);
      fanEnabled = true;
    }

    Serial.print("people=");
    Serial.print(people);
    Serial.print(", tempC=");
    Serial.print(lastTempC, 1);
    Serial.print(", fanPWM=");
    Serial.println((int)pwm);
  } else {
    // 快速人数变化时确保节能条件一致
    if (people <= 0 && fanEnabled) {
      setFanPWM(0);
      fanEnabled = false;
    }
  }

  // 4) 刷新数码管显示：默认人数，按钮可切换温度
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
      float t = readTemperatureC();
      if (t > -50.0f) lastTempC = t;
      if (fabs(lastTempC - lastShownTemp) >= 0.5f) {
        showTempOnDisplay();
        lastShownTemp = lastTempC;
      }
    }
  }

  // 5) 人数变化时立即更新显示（在人数模式下）
  if (!showTemp && people != prevPeople) {
    showPeopleOnDisplay();
    lastShownPeople = people;
  }
}