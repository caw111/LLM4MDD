#include <Arduino.h>
#include <TM1637Display.h>

// ===== Pin assignments (authoritative) =====
static const uint8_t PIN_LM35_A1  = A1;
static const uint8_t PIN_BTN_D2   = 2;   // Stage 1 target adjust (up)
static const uint8_t PIN_BTN_D3   = 3;   // Stage 2 target adjust (up)
static const uint8_t PIN_BTN_D4   = 4;   // TM1637 CLK
static const uint8_t PIN_LED_D13   = 13;
static const uint8_t PIN_BUZZ_D6   = 6;   // Buzzer

static const uint8_t PIN_TM1637_CLK = 4;  // D4 (remapped conflict resolved)
static const uint8_t PIN_TM1637_DIO = 5;  // D5

// ===== TM1637 =====
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ===== LM35 settings =====
// LM35 output: 10mV/°C => Vout = T(°C) * 0.01 V
static const float LM35_MV_PER_DEG_C = 10.0f;
static const float ADC_REF_V = 5.0f;
static const int ADC_MAX = 1023;

// ===== Stage model =====
enum Stage : uint8_t { PREHEAT = 0, COOK = 1, KEEP_WARM = 2, DONE = 3 };

struct StageConfig {
  float targetTempC;   // target temperature
  uint32_t durationSec; // stage duration in seconds
};

static StageConfig cfg[3] = {
  { 60.0f,  120 },  // PREHEAT: target 60C, 2 minutes
  { 180.0f, 300 },  // COOK:    target 180C, 5 minutes
  {  80.0f, 180 }   // KEEP:    target 80C, 3 minutes
};

// ===== UI / Buttons =====
// Requirements mention D2, D3, D4 as buttons, but D4 is used as TM1637 CLK.
// Hardware configuration remaps D4 to CLK; therefore D4 cannot be a button.
// We preserve functionality for D2 and D3 as stage target adjust buttons.
// - D2: increase PREHEAT target by 1°C
// - D3: increase COOK target by 1°C
// - KEEP_WARM target is fixed (no available third button in remapped pins).
//
// If you intended a different behavior for D4 as button, the pin remap makes it impossible
// without changing wiring; this sketch preserves the resolved hardware pin assignments.
static const uint32_t DEBOUNCE_MS = 50;
static bool lastBtn2 = HIGH;
static bool lastBtn3 = HIGH;
static uint32_t lastChange2 = 0;
static uint32_t lastChange3 = 0;

// ===== Cooking state =====
static Stage stage = PREHEAT;

static uint32_t stageStartMs = 0;     // when current stage began
static bool targetReachedBeeped = false;

// ===== Timing / display =====
static uint32_t lastSecondTickMs = 0;

// ===== Buzzer =====
static uint32_t lastBeepToggleMs = 0;
static bool buzzerOn = false;

// ===== Helper: read temperature =====
float readTempC() {
  int raw = analogRead(PIN_LM35_A1);
  float v = (raw * ADC_REF_V) / ADC_MAX; // volts
  float tempC = (v * 1000.0f) / LM35_MV_PER_DEG_C; // mV / (10 mV/°C)
  return tempC;
}

// ===== Helper: beeper alert when reaching target =====
// Emits periodic short beeps while target is reached and stage is active,
// but only begins once per stage (targetReachedBeeped gate).
void updateBuzzer(bool enabled) {
  if (!enabled) {
    buzzerOn = false;
    digitalWrite(PIN_BUZZ_D6, LOW);
    return;
  }

  // Use toggling to create a simple tone-like beep without tone() (pin requested as D6).
  // Still acceptable on UNO for buzzer.
  const uint32_t intervalMs = 180; // beep cadence
  const uint32_t onMs = 60;

  uint32_t now = millis();
  // Determine on/off based on a modulo window
  uint32_t t = now % intervalMs;
  if (t < onMs) {
    if (!buzzerOn) {
      // Quick toggle for audible beep; keep it HIGH
      digitalWrite(PIN_BUZZ_D6, HIGH);
      buzzerOn = true;
    }
  } else {
    if (buzzerOn) {
      digitalWrite(PIN_BUZZ_D6, LOW);
      buzzerOn = false;
    }
  }
}

// ===== Helper: log =====
void logLine(const __FlashStringHelper* s) {
  Serial.println(s);
}

void logStageConfig() {
  Serial.println(F("=== Stage configs ==="));
  Serial.print(F("PREHEAT: target=")); Serial.print(cfg[0].targetTempC); Serial.print(F("C, duration=")); Serial.print(cfg[0].durationSec); Serial.println(F("s"));
  Serial.print(F("COOK:    target=")); Serial.print(cfg[1].targetTempC); Serial.print(F("C, duration=")); Serial.print(cfg[1].durationSec); Serial.println(F("s"));
  Serial.print(F("KEEP:    target=")); Serial.print(cfg[2].targetTempC); Serial.print(F("C, duration=")); Serial.print(cfg[2].durationSec); Serial.println(F("s"));
}

void enterStage(Stage newStage) {
  stage = newStage;
  stageStartMs = millis();
  lastSecondTickMs = stageStartMs;
  targetReachedBeeped = false;

  digitalWrite(PIN_LED_D13, (stage == PREHEAT ? HIGH : LOW)); // visual cue
  if (stage == PREHEAT) {
    Serial.println(F(">> Enter Stage 1: 预热 (PREHEAT)"));
  } else if (stage == COOK) {
    Serial.println(F(">> Enter Stage 2: 烹饪 (COOK)"));
  } else if (stage == KEEP_WARM) {
    Serial.println(F(">> Enter Stage 3: 保温 (KEEP_WARM)"));
  } else if (stage == DONE) {
    Serial.println(F(">> Process DONE"));
  }
}

// ===== Display =====
// Requirement: Display current stage number and remaining time.
// TM1637 4-digit typically. We'll show:
// - Digits [1..2] = stage number (1/2/3) as "01/02/03"
// - Digits [3..4] = remaining seconds (00..99) limited to last two digits
// This preserves "stage编号 + 剩余时间" within 4 digits.
// For larger remaining times, last two digits cycle.
void updateDisplay(uint8_t stageNumber, uint32_t remainingSec) {
  uint16_t mm = (uint16_t)(stageNumber * 100); // 100,200,300
  uint16_t lastTwo = (uint16_t)(remainingSec % 100);

  // Compose number: stageNumber (1..3) in hundreds and lastTwo in ones
  // e.g., stage=1 remaining=5 => 105
  // But we want fixed 2-digit stage prefix: "01"..."03".
  // We will encode as: stageNumber*100 + lastTwo => 100+lastTwo ... 300+lastTwo
  uint16_t toDisplay = (uint16_t)stageNumber * 100 + lastTwo;

  display.showNumberDecEx(toDisplay, 0b01000000, true, 4);
}

// ===== Debounced rising edge read (assuming buttons active-low or active-high?) =====
// We assume buttons are wired with INPUT_PULLUP commonly: pressed => LOW.
// Requirement didn't specify; use INPUT_PULLUP and interpret press as LOW.
bool readBtnPressEdge(uint8_t pin, bool &lastState, uint32_t &lastChangeMs, uint32_t nowMs) {
  bool state = digitalRead(pin);
  if (state != lastState && (nowMs - lastChangeMs) >= DEBOUNCE_MS) {
    lastChangeMs = nowMs;
    bool prev = lastState;
    lastState = state;
    // rising edge means release; we want press edge: falling edge in pullup config
    // active-low: press => HIGH->LOW (prev=HIGH, new=LOW)
    if (prev == HIGH && state == LOW) return true;
  }
  return false;
}

// ===== Arduino setup/loop =====
void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED_D13, OUTPUT);
  digitalWrite(PIN_LED_D13, LOW);

  pinMode(PIN_BUZZ_D6, OUTPUT);
  digitalWrite(PIN_BUZZ_D6, LOW);

  pinMode(PIN_BTN_D2, INPUT_PULLUP);
  pinMode(PIN_BTN_D3, INPUT_PULLUP);

  // TM1637 init
  display.setBrightness(0x0f);
  display.clear();

  logLine(F("=== 三阶段烹饪辅助系统启动 ==="));
  logLine(F("Sensors: LM35(A1), Display: TM1637(CLK=D4, DIO=D5), Buttons: D2/D3"));
  logStageConfig();

  // Start first stage immediately
  enterStage(PREHEAT);
}

void loop() {
  uint32_t now = millis();

  // ===== Button handling: adjust targets =====
  // D2: increase PREHEAT target by 1°C (cap 99C)
  // D3: increase COOK target by 1°C (cap 299C)
  bool btn2Pressed = readBtnPressEdge(PIN_BTN_D2, lastBtn2, lastChange2, now);
  bool btn3Pressed = readBtnPressEdge(PIN_BTN_D3, lastBtn3, lastChange3, now);

  if (btn2Pressed) {
    cfg[0].targetTempC = min(99.0f, cfg[0].targetTempC + 1.0f);
    Serial.print(F("[BTN D2] PREHEAT target updated => "));
    Serial.print(cfg[0].targetTempC);
    Serial.println(F(" C"));
  }
  if (btn3Pressed) {
    cfg[1].targetTempC = min(299.0f, cfg[1].targetTempC + 1.0f);
    Serial.print(F("[BTN D3] COOK target updated => "));
    Serial.print(cfg[1].targetTempC);
    Serial.println(F(" C"));
  }

  // ===== Read temperature =====
  float tempC = readTempC();

  // ===== Stage logic =====
  if (stage != DONE) {
    uint32_t elapsedMs = now - stageStartMs;
    uint32_t elapsedSec = elapsedMs / 1000UL;

    uint32_t durationSec = cfg[(uint8_t)stage].durationSec;
    uint32_t remainingSec = (elapsedSec >= durationSec) ? 0 : (durationSec - elapsedSec);

    // Determine if target reached for current stage
    float target = cfg[(uint8_t)stage].targetTempC;
    bool reached = (tempC >= target);

    // If reached and not beeped yet, log once; requirement: temperature reaches target => buzzer提示
    if (reached && !targetReachedBeeped) {
      targetReachedBeeped = true;
      Serial.print(F("[T=")); Serial.print(tempC, 1);
      Serial.print(F("C] Stage "));
      Serial.print((uint8_t)stage + 1);
      Serial.println(F(" target reached => beep"));
    }

    // Buzzer behavior: beep/alert while target has been reached during stage.
    // Turn off once stage ends.
    updateBuzzer(reached);

    // LED indicator by stage (simple mapping)
    if (stage == PREHEAT) digitalWrite(PIN_LED_D13, HIGH);
    else if (stage == COOK) digitalWrite(PIN_LED_D13, (millis() / 250) % 2 ? HIGH : LOW);
    else if (stage == KEEP_WARM) digitalWrite(PIN_LED_D13, HIGH);

    // Update display continuously
    uint8_t stageNum = (uint8_t)stage + 1;
    updateDisplay(stageNum, remainingSec);

    // Log complete flow with stage transitions and periodic temperature/remaining
    // Log every second with timestamp-like cadence (line log)
    if (now - lastSecondTickMs >= 1000UL) {
      lastSecondTickMs += 1000UL;

      Serial.print(F("[Stage ")); Serial.print(stageNum);
      Serial.print(F("] Temp=")); Serial.print(tempC, 1);
      Serial.print(F("C, Target=")); Serial.print(target, 1);
      Serial.print(F("C, Remaining=")); Serial.print(remainingSec);
      Serial.println(F("s"));
    }

    // If duration ended => auto switch to next stage
    if (elapsedSec >= durationSec) {
      Serial.print(F("<< Stage "));
      Serial.print(stageNum);
      Serial.println(F(" duration complete => switching"));
      // stop buzzer during transition
      updateBuzzer(false);
      if (stage == PREHEAT) enterStage(COOK);
      else if (stage == COOK) enterStage(KEEP_WARM);
      else if (stage == KEEP_WARM) {
        stage = DONE;
        updateBuzzer(false);
        digitalWrite(PIN_LED_D13, LOW);
        display.clear();
        Serial.println(F(">> 保温阶段结束：流程完成。"));
      }
      // Small delay to prevent rapid log spam right after transition
      delay(20);
    }
  } else {
    // DONE state: keep buzzer off, display blank
    updateBuzzer(false);
    digitalWrite(PIN_LED_D13, LOW);
    // Optional: show "0000" briefly
    display.showNumberDecEx(0, 0b01000000, true, 4);
    delay(200);
  }
}