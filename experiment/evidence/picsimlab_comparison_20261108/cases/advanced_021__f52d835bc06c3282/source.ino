#include <Arduino.h>
#include <TM1637Display.h>
#include <math.h>

static const uint8_t PIN_LM35 = A1;     // io_0
static const uint8_t PIN_LIGHT = A0;   // io_1
static const uint8_t PIN_LEVEL = A2;   // io_2

static const uint8_t PIN_HEATER = 10;  // io_3 (PWM)
static const uint8_t PIN_LED = 9;      // io_4 (PWM)
static const uint8_t PIN_PUMP = 5;     // io_5 (digital)
static const uint8_t PIN_BUZZER = 6;   // io_6 (tone output)

static const uint8_t PIN_BTN = 2;      // io_7
static const uint8_t PIN_TM_CLK = 4;    // tm1637 CLK
static const uint8_t PIN_TM_DIO = 3;    // tm1637 DIO (resolved remap)

TM1637Display tm1637(PIN_TM_CLK, PIN_TM_DIO);

// ===== Controller state (frozen model) =====
static int32_t displayMode = 0;         // initial 0
static bool previousPressed = false;    // initial false
static bool alarmState = false;         // initial false

// ==== Helper: RealFunctions::floor(x) for non-negative ADC counts ====
static inline int32_t rFloor(double x) {
  return (int32_t)floor(x);
}

static inline double readADCf(uint8_t pin) {
  // adc_count semantics preserved: raw ADC reading (0..1023) as f64
  return (double)analogRead(pin);
}

// ===== Outputs (frozen model semantics) =====
static void applyOutputs(double io_0, double io_1, double io_2) {
  // io_3: heater PWM command
  // io_3 = (if alarmState ? 0.0 else (if ((io_0) < (250.0)) ? 255.0 else 0.0))
  double io_3 = 0.0;
  if (alarmState) {
    io_3 = 0.0;
  } else {
    io_3 = (io_0 < 250.0) ? 255.0 : 0.0;
  }
  // Map 0..255 to PWM (UNO analogWrite uses 0..255)
  analogWrite(PIN_HEATER, (uint8_t)io_3);

  // io_4: illumination PWM command
  // io_4 = (if alarmState ? 0.0 else (if ((io_1) < (300.0)) ? 200.0 else 0.0))
  double io_4 = 0.0;
  if (alarmState) {
    io_4 = 0.0;
  } else {
    io_4 = (io_1 < 300.0) ? 200.0 : 0.0;
  }
  analogWrite(PIN_LED, (uint8_t)io_4);

  // io_5: water pump / aeration pump on/off
  // io_5 = (if alarmState ? true else false)
  bool io_5 = alarmState;
  digitalWrite(PIN_PUMP, io_5 ? HIGH : LOW);

  // io_6: low water alarm tone control (i32 Hz)
  // io_6 = (if alarmState ? 1000 else 0)
  int32_t io_6 = alarmState ? 1000 : 0;

  // tone output with invalid_policy="silence"
  if (io_6 <= 0) {
    noTone(PIN_BUZZER);
  } else {
    tone(PIN_BUZZER, (unsigned int)io_6);
  }

  // io_8: TM1637 numeric display for selected sensor value
  // io_8 = (if displayMode==0 ? floor(io_0) : (if displayMode==1 ? floor(io_1) : floor(io_2)))
  int32_t io_8 = 0;
  if (displayMode == 0) {
    io_8 = rFloor(io_0);
  } else if (displayMode == 1) {
    io_8 = rFloor(io_1);
  } else {
    io_8 = rFloor(io_2);
  }

  // invalid_policy="blank", leading_zero=false, brightness=7
  // Model gives integer; always valid in normal ADC range. Still, set directly.
  tm1637.setBrightness(7);
  tm1637.showNumberDec(io_8, false);
}

// ===== Controller tick =====
static void controllerTick(double io_0, double io_1, double io_2, bool io_7) {
  // Updates, preserving exact ordering logic:
  // tick:
  //  - start
  //  - then if io_7 and not previousPressed: displayMode = displayMode + 1
  //  - then if displayMode == 3: displayMode = 0
  //  - then previousPressed = io_7
  //  - then if io_2 < 300: alarmState = true else alarmState = false

  if (io_7 && !previousPressed) {
    displayMode = displayMode + 1;
  }
  if (displayMode == 3) {
    displayMode = 0;
  }
  previousPressed = io_7;

  if (io_2 < 300.0) {
    alarmState = true;
  } else {
    alarmState = false;
  }

  // Outputs computed from final state
  applyOutputs(io_0, io_1, io_2);
}

void setup() {
  pinMode(PIN_HEATER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_PUMP, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // digital_input uses catalog default by omitting pullup/active_high configuration keys
  // but the frozen interface contract specifies active_high=true, pullup=false.
  pinMode(PIN_BTN, INPUT);

  // Initialize PWM pins to initial model output values (all initial 0, alarmState false)
  analogWrite(PIN_HEATER, 0);
  analogWrite(PIN_LED, 0);
  digitalWrite(PIN_PUMP, LOW);
  noTone(PIN_BUZZER);

  tm1637.setBrightness(7);
  tm1637.showNumberDec(0, false);

  // displayMode/previousPressed/alarmState already statically initialized
}

void loop() {
  static const uint32_t control_period_ms = 10;
  static uint32_t lastTick = 0;

  uint32_t now = millis();
  if (now - lastTick < control_period_ms) return;
  lastTick = now;

  // Sample inputs at 10ms (frozen interface contract sample_period_ms=10)
  double io_0 = readADCf(PIN_LM35);  // adc_count
  double io_1 = readADCf(PIN_LIGHT); // adc_count
  double io_2 = readADCf(PIN_LEVEL); // adc_count
  bool io_7 = (digitalRead(PIN_BTN) == HIGH);

  controllerTick(io_0, io_1, io_2, io_7);
}