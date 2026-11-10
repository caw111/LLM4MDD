#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

//
// Hardware mapping (frozen contract)
//
static constexpr uint8_t PIN_DHT11 = 7;   // D7
static constexpr uint8_t PIN_LIGHT_A0 = A0;
static constexpr uint8_t PIN_SOIL_A1  = A1;
static constexpr uint8_t PIN_MANUAL_A2 = A2;

static constexpr uint8_t PIN_FAN_PWM = 5;    // D5
static constexpr uint8_t PIN_LED_D9   = 9;    // D9 (digital output)
static constexpr uint8_t PIN_PUMP_D10 = 10;   // D10 (digital output)
static constexpr uint8_t PIN_BUZZER_D6 = 6;   // D6 (tone output)
static constexpr uint8_t PIN_BTN_D2 = 2;      // D2 (active high, pullup=true)
static constexpr uint8_t PIN_BTN_D3 = 3;      // D3 (active high, pullup=true)

static constexpr uint8_t PIN_TM_CLK = 4;     // D4
static constexpr uint8_t PIN_TM_DIO = 8;     // D8 (remapped from conflict)

static constexpr uint32_t CONTROL_PERIOD_MS = 10;
static constexpr uint32_t DHT_SAMPLE_MS = 2000;

//
// Model state (must preserve exact controller state)
//
struct ControllerState {
  bool modeIsAuto = true;
  bool manualTargetIsFan = true;
  bool previousD2Pressed = false;
  bool previousD3Pressed = false;

  float fanPwmManual = 0.0f;
  bool ledOnManual = false;
  bool pumpOnManual = false;

  int32_t tmItemIndex = 0;
  int32_t tmElapsedMs = 0;

  float lastTempValid = 0.0f;
  float lastHumValid = 0.0f;

  float lastIo3 = 0.0f;        // A0 light ADC
  float prevSoilHumValid = 0.0f; // A1 soil ADC

  // time bookkeeping
  uint32_t lastDhtReadMs = 0;
};

// Inputs from hardware each control tick
struct Inputs {
  float io_0 = 0.0f;  // temperature
  float io_1 = 0.0f;  // humidity
  bool  io_2 = true;  // dht_valid

  float io_3 = 0.0f;  // light ADC (A0)
  float io_4 = 0.0f;  // soil ADC (A1)
  float io_5 = 0.0f;  // manual knob ADC (A2)

  bool io_11 = false; // D2
  bool io_12 = false; // D3
};

static ControllerState st;

static DHT dht(PIN_DHT11, DHT11);
static TM1637Display tm1637(PIN_TM_CLK, PIN_TM_DIO);

static uint32_t lastTickMs = 0;

static inline int clampInt(int v, int lo, int hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

// "tone_output invalid_policy=silence" in model: only used when alarm triggered (io_9=1000 => start tone)
static void applyBuzzerTone(int32_t hz) {
  if (hz > 0) {
    tone(PIN_BUZZER_D6, hz);
  } else {
    noTone(PIN_BUZZER_D6);
  }
}

// Floor for possibly negative floats (Arduino has floorf in math, but to keep consistent use (int32_t)floorf)
static float floorFloatToFloat(float x) {
  // Use built-in floorf if available; otherwise simple conversion for non-negative.
  // Contract uses floor(lastTempValid/lastIo3/prevSoilHumValid). Those are ADC-like or DHT-like; may be negative if sensor abnormal.
  // Use floorf.
  return floorf(x);
}

static void readHardware(Inputs &in, uint32_t nowMs) {
  // Buttons: active_high=true, pullup=true
  // If wiring provides active-high when pressed, then digitalRead returns HIGH for pressed.
  in.io_11 = (digitalRead(PIN_BTN_D2) == HIGH);
  in.io_12 = (digitalRead(PIN_BTN_D3) == HIGH);

  // Analog inputs: scale=1 offset=0, keep raw ADC counts (0..1023)
  in.io_3 = (float)analogRead(PIN_LIGHT_A0);
  in.io_4 = (float)analogRead(PIN_SOIL_A1);
  in.io_5 = (float)analogRead(PIN_MANUAL_A2);

  // DHT11: invalid_policy=hold_last_valid (for lastTempValid/lastHumValid maintained by model update ordering)
  // Model input io_2 is dht_valid; if invalid, io_0/io_1 should be irrelevant because update holds last valid.
  // We still compute io_0/io_1 for convenience, but model uses io_2 gating.
  if (nowMs - st.lastDhtReadMs >= DHT_SAMPLE_MS) {
    st.lastDhtReadMs = nowMs;

    float t = dht.readTemperature(); // C
    float h = dht.readHumidity();

    bool valid = (!isnan(t) && !isnan(h));
    in.io_2 = valid;
    in.io_0 = valid ? t : in.io_0; // irrelevant when invalid (gated by io_2)
    in.io_1 = valid ? h : in.io_1;
  } else {
    // Hold latest DHT validity and values for until next sampling.
    // Since model's internal lastTempValid/lastHumValid is updated only when io_2=true,
    // and outputs use those stored values, we can simply compute io_2=false between reads.
    // But serial fault logic uses io_2 directly (serial_1_trigger=io_2) and io_9 uses io_2 only (alarm when not io_2).
    // Therefore, between DHT reads, treat io_2 as last read validity to preserve correct behavior.
    // We'll keep a simple approach: perform a DHT read at least every 2000ms; in between assume invalid? would alter alarm.
    // Better: cache last io_2 (stored as local static).
  }

  // Cache DHT io_2/io_0/io_1 between reads
  static bool cachedValid = false;
  static float cachedT = 0.0f;
  static float cachedH = 0.0f;

  static bool initialized = false;
  if (!initialized) {
    initialized = true;
    cachedValid = false;
    cachedT = 0.0f;
    cachedH = 0.0f;
    st.lastDhtReadMs = nowMs; // allow immediate read if desired
  }

  if (nowMs - st.lastDhtReadMs < DHT_SAMPLE_MS) {
    // If we didn't read now, keep cached
    in.io_2 = cachedValid;
    in.io_0 = cachedT;
    in.io_1 = cachedH;
  } else {
    // We just read at least once above; update cache from current in
    // (Only if we actually performed a read)
    // But we can't reliably know if nowMs - st.lastDhtReadMs >= DHT_SAMPLE_MS triggered the read.
    // Since we updated st.lastDhtReadMs in that block, it's considered read.
    cachedValid = in.io_2;
    cachedT = in.io_0;
    cachedH = in.io_1;
  }
}

static void tickController(const Inputs &io) {
  // Model perform action tick (ordered updates must match)
  // As in accepted bundle:
  // first start; then if io_2 {...}; then if true {...}; then button edge toggles, etc.
  // We'll implement exactly in order as specified.

  // if io_2 { assign lastTempValid := io_0; lastHumValid := io_1; }
  if (io.io_2) {
    st.lastTempValid = io.io_0;
    st.lastHumValid = io.io_1;
  }

  // then if true { lastIo3 := io_3; prevSoilHumValid := io_4; }
  st.lastIo3 = io.io_3;
  st.prevSoilHumValid = io.io_4;

  // if (io_11 and not previousD2Pressed) modeIsAuto := not modeIsAuto
  if (io.io_11 && !st.previousD2Pressed) {
    st.modeIsAuto = !st.modeIsAuto;
  }

  // if (io_12 and not previousD3Pressed) manualTargetIsFan := not manualTargetIsFan
  if (io.io_12 && !st.previousD3Pressed) {
    st.manualTargetIsFan = !st.manualTargetIsFan;
  }

  // previousD2Pressed := io_11 ; previousD3Pressed := io_12
  st.previousD2Pressed = io.io_11;
  st.previousD3Pressed = io.io_12;

  // fanPwmManual update:
  // fanPwmManual := (if manualTargetIsFan then (clamp io_5 to [0,1023]) else 0)
  if (st.manualTargetIsFan) {
    float v = io.io_5;
    if (v < 0.0f) v = 0.0f;
    else if (v > 1023.0f) v = 1023.0f;
    st.fanPwmManual = v;
  } else {
    st.fanPwmManual = 0.0f;
  }

  // ledOnManual := (if manualTargetIsFan then false else (io_5 >= 512 ? true : false))
  if (st.manualTargetIsFan) {
    st.ledOnManual = false;
  } else {
    st.ledOnManual = (io.io_5 >= 512.0f);
  }

  // pumpOnManual := (if manualTargetIsFan then false else (io_5 >= 256 ? true : false))
  if (st.manualTargetIsFan) {
    st.pumpOnManual = false;
  } else {
    st.pumpOnManual = (io.io_5 >= 256.0f);
  }

  // if ((not modeIsAuto) and (true)) { tmItemIndex := tmItemIndex; } else []
  // no-op

  // then if true { tmElapsedMs := tmElapsedMs + 10; tmItemIndex := (tmElapsedMs >=2000 ? ... ); tmElapsedMs := (tmElapsedMs >=2000 ? 0 : tmElapsedMs) }
  // Note: This is sequential with updated tmElapsedMs used in next assignment of tmItemIndex.
  // We'll mimic it using temp.
  st.tmElapsedMs = st.tmElapsedMs + 10;
  if (st.tmElapsedMs >= 2000) {
    if (st.tmItemIndex == 2) st.tmItemIndex = 0;
    else st.tmItemIndex = st.tmItemIndex + 1;
    st.tmElapsedMs = 0;
  }

  // then if true { tmItemIndex := tmItemIndex; } else []
  // no-op
}

static void applyOutputs(const Inputs &io) {
  // io_6: fan PWM output
  float io_6 = 0.0f;
  if (st.modeIsAuto && io.io_2) {
    if (st.lastTempValid >= 30.0f) {
      if (st.lastTempValid >= 40.0f) io_6 = 1023.0f;
      else io_6 = (st.lastTempValid - 30.0f) * 1023.0f / 10.0f;
    } else {
      io_6 = 0.0f;
    }
  } else {
    if (!st.modeIsAuto && st.manualTargetIsFan) {
      float v = st.fanPwmManual;
      if (v < 0.0f) v = 0.0f;
      else if (v > 1023.0f) v = 1023.0f;
      io_6 = v;
    } else {
      io_6 = 0.0f;
    }
  }

  int pwm = clampInt((int)lroundf(io_6), 0, 1023);
  analogWrite(PIN_FAN_PWM, pwm);

  // io_7: LED on
  bool io_7 = false;
  if (st.modeIsAuto && io.io_2) {
    io_7 = (st.lastIo3 >= 600.0f);
  } else {
    if (!st.modeIsAuto && !st.manualTargetIsFan) {
      io_7 = st.ledOnManual;
    } else {
      io_7 = false;
    }
  }
  digitalWrite(PIN_LED_D9, io_7 ? HIGH : LOW);

  // io_8: Pump on
  bool io_8 = false;
  if (st.modeIsAuto && io.io_2) {
    io_8 = (st.prevSoilHumValid <= 400.0f);
  } else {
    if (!st.modeIsAuto && !st.manualTargetIsFan) {
      io_8 = st.pumpOnManual;
    } else {
      io_8 = false;
    }
  }
  digitalWrite(PIN_PUMP_D10, io_8 ? HIGH : LOW);

  // io_9: buzzer tone Hz
  int32_t io_9 = 0;
  if ((!io.io_2) || (io.io_3 < 0.0f) || (io.io_4 < 0.0f)) io_9 = 1000;
  else io_9 = 0;
  applyBuzzerTone(io_9);

  // io_10: TM1637 number display (floor)
  int32_t io_10 = 0;
  if (st.tmItemIndex == 0) io_10 = (int32_t)floorFloatToFloat(st.lastTempValid);
  else if (st.tmItemIndex == 1) io_10 = (int32_t)floorFloatToFloat(st.lastIo3);
  else io_10 = (int32_t)floorFloatToFloat(st.prevSoilHumValid);

  // tm1637_number invalid_policy=blank, but model never blanks; it always assigns floor(...)
  tm1637.showNumberDec(io_10, false, 0);

  // serial contract:
  // serial_0_trigger and serial_0_value_0 are set when alarm condition is true:
  bool serial_0_trigger = ((!io.io_2) || (io.io_3 < 0.0f) || (io.io_4 < 0.0f));
  bool serial_0_value_0 = serial_0_trigger; // in bundle: or(...) same expression

  // serial_1_trigger := io_2
  bool serial_1_trigger = io.io_2;

  // "通过串口输出故障信息" choose typed numeric up/down and trigger gating.
  // We implement simple line messages at trigger time:
  // - if serial_0_trigger: print "FAULT dht_valid=0" (value_0 is bool)
  // - if serial_1_trigger: print "STATE temperature_c=..., light_adc=..., soil_adc=..."
  if (serial_0_trigger) {
    // Ensure line_or_idle: print only when triggered
    Serial.print(F("FAULT dht_valid="));
    Serial.println(serial_0_value_0 ? 1 : 0);
  }

  if (serial_1_trigger) {
    Serial.print(F("STATE temperature_c="));
    Serial.print(st.lastTempValid, 2);
    Serial.print(F(" light_adc="));
    Serial.print(st.lastIo3, 0);
    Serial.print(F(" soil_adc="));
    Serial.println(st.prevSoilHumValid, 0);
  }
}

void setup() {
  pinMode(PIN_BTN_D2, INPUT_PULLUP);
  pinMode(PIN_BTN_D3, INPUT_PULLUP);

  pinMode(PIN_LED_D9, OUTPUT);
  pinMode(PIN_PUMP_D10, OUTPUT);

  pinMode(PIN_FAN_PWM, OUTPUT);

  pinMode(PIN_BUZZER_D6, OUTPUT);

  // Initial outputs consistent with model initial state:
  digitalWrite(PIN_LED_D9, LOW);
  digitalWrite(PIN_PUMP_D10, LOW);
  analogWrite(PIN_FAN_PWM, 0);
  noTone(PIN_BUZZER_D6);

  Serial.begin(9600);

  dht.begin();

  tm1637.setBrightness(7, true);

  lastTickMs = millis();
  st.lastDhtReadMs = millis();
}

void loop() {
  uint32_t now = millis();
  // enforce control_period_ms=10 (best-effort)
  while ((uint32_t)(millis() - lastTickMs) < CONTROL_PERIOD_MS) {
    // allow background
    yield();
  }
  lastTickMs += CONTROL_PERIOD_MS;

  Inputs io;
  readHardware(io, now);

  tickController(io);
  applyOutputs(io);
}