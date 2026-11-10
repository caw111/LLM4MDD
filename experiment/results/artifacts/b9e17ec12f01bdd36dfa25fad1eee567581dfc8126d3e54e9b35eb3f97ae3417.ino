#include <Arduino.h>
#include <TM1637Display.h>

// ===== Frozen interface contract / bindings =====
// control_period_ms = 10
static constexpr uint32_t CONTROL_PERIOD_MS = 10;

// Pins (authoritative remap)
static constexpr uint8_t PIN_FAN_PWM_D5 = 5;   // D5
static constexpr uint8_t PIN_BUZZER_D6 = 6;    // D6
static constexpr uint8_t PIN_LED_D13 = 13;     // D13
static constexpr uint8_t TM1637_CLK_D4 = 4;    // D4
static constexpr uint8_t TM1637_DIO_D3 = 3;    // D3
static constexpr uint8_t PIN_VIBRATION_D2 = 2; // D2
static constexpr uint8_t ANALOG_IO_0_A0 = A0;   // current adc_count
static constexpr uint8_t ANALOG_IO_1_A1 = A1;   // lm35 adc_count

// Serial
static constexpr uint32_t SERIAL_BAUD = 9600;

// ===== Model state (must preserve exactly) =====
static bool previousVibration = false;
static int32_t stateCode = 0;
static int32_t faultCode = 0;

// ===== IO values (sampled each model tick) =====
static float io_0_adc_count = 0.0f; // A0
static float io_1_adc_count = 0.0f; // A1
static bool  io_2_vibration = false;

// Outputs (computed each model tick)
static float  io_3_fan_pwm_count = 0.0f; // D5 PWM
static int32_t io_4_buzzer_hz = 0;      // D6 tone
static bool   io_5_led_on = false;       // D13
static int32_t io_6_tm1637_value = 0;    // stateCode/fault display

// TM1637
TM1637Display tm1637(TM1637_CLK_D4, TM1637_DIO_D3);

// Model requires tm1637 to "轮流显示", but contract only exposes a single integer output.
// Therefore we implement the model output exactly: continuously display the latest io_6 value.
// (No other model variables specify cadence/rotation.)
static void tm1637DisplayValue(int32_t v) {
  // invalid_policy=blank, leading_zero=false.
  // Contract does not specify blank conditions beyond invalid policy; no invalid data occurs.
  // Display only non-negative integer range supported by TM1637Display:
  // clamp to 0..9999 to avoid undefined behavior.
  if (v < 0) v = 0;
  if (v > 9999) v = 9999;
  tm1637.showNumberDec(v, false);
}

// ===== Serial reporting gate =====
static void serialReport() {
  // serial_0_trigger is always true in the model, so report each tick.
  // Values are the raw model interface mappings:
  // current_A  := io_0
  // temp_C     := io_1
  // vibration_fault := io_2
  // state_code := stateCode
  // fault_code := faultCode

  // Protocol details are not specified; use a single line numeric CSV-like record.
  // Replace this with fixed exact text if required by a stronger contract.
  // Format: current_A,temp_C,vibration_fault,state_code,fault_code
  Serial.print(io_0_adc_count, 6);
  Serial.print(',');
  Serial.print(io_1_adc_count, 6);
  Serial.print(',');
  Serial.print(io_2_vibration ? 1 : 0);
  Serial.print(',');
  Serial.print(stateCode);
  Serial.print(',');
  Serial.println(faultCode);
}

// ===== Hardware I/O helpers =====
static void applyOutputs() {
  // io_3 (fan PWM): if stateCode==3 then 255 else 0
  // PWM on D5
  uint8_t pwm = (io_3_fan_pwm_count >= 255.0f) ? 255 : (uint8_t)io_3_fan_pwm_count;
  analogWrite(PIN_FAN_PWM_D5, pwm);

  // io_4 (buzzer): tone on D6
  if (io_4_buzzer_hz != 0) {
    tone(PIN_BUZZER_D6, (unsigned int)io_4_buzzer_hz);
  } else {
    noTone(PIN_BUZZER_D6);
  }

  // io_5 (LED)
  digitalWrite(PIN_LED_D13, io_5_led_on ? HIGH : LOW);

  // io_6 (TM1637 number): "轮流显示" not definable from provided model interface,
  // so display state/fault encoding as per frozen model.
  tm1637DisplayValue(io_6_tm1637_value);
}

static void sampleInputs() {
  // io_0, io_1 as adc_count; binding scale/offset are 1 and 0,
  // but conversion calibration parameters are not provided.
  // Contract interface explicitly says unit adc_count with scale/offset=1.0,0.0.
  io_0_adc_count = (float)analogRead(ANALOG_IO_0_A0);
  io_1_adc_count = (float)analogRead(ANALOG_IO_1_A1);

  // io_2 vibration switch: active_high=true pullup=false
  io_2_vibration = (digitalRead(PIN_VIBRATION_D2) == HIGH);
}

static void modelTick() {
  // Accepted executable model: update ordering and logic must match exactly.
  // The prompt's model:
  // perform action tick { first start;
  //   then if ((((io_2) and ((not previousVibration)))) or (io_2)) { ... }
  //   else { ... } then assign previousVibration := io_2; }
  //
  // We'll implement as the equivalent boolean expression:
  // if ( (io_2 && !previousVibration) || io_2 ) => always io_2 true => sets mechanical fault state.
  // But implement with same logic structure to preserve behavior.

  // Compute outputs exactly as per model.
  if ( ((io_2_vibration && (!previousVibration)) ) || io_2_vibration ) {
    stateCode = 4;
    faultCode = 1;
  } else {
    if (io_1_adc_count >= 0.0f) {
      faultCode = 0;
      if (io_1_adc_count >= 30.0f) {
        stateCode = 3;
      } else {
        if (io_0_adc_count >= 0.0f) {
          stateCode = 1;
        } else {
          stateCode = 0;
        }
      }
    } else {
      faultCode = 0;
      stateCode = 0;
    }
  }

  previousVibration = io_2_vibration;

  // outputs per model
  io_3_fan_pwm_count = (stateCode == 3) ? 255.0f : 0.0f;

  if (faultCode != 0) {
    io_4_buzzer_hz = 2000;
  } else {
    io_4_buzzer_hz = (stateCode == 3) ? 1000 : 0;
  }

  if (faultCode != 0) {
    io_5_led_on = true;
  } else {
    io_5_led_on = (stateCode == 3) ? true : false;
  }

  io_6_tm1637_value = (faultCode != 0) ? 40 : stateCode;

  // serial trigger/value ports are handled in serialReport() (always triggered)
}

void setup() {
  // Serial baud 9600
  Serial.begin(SERIAL_BAUD);

  // Inputs
  pinMode(PIN_VIBRATION_D2, INPUT); // pullup=false (conservative). external wiring required.

  // Outputs
  pinMode(PIN_LED_D13, OUTPUT);
  pinMode(PIN_BUZZER_D6, OUTPUT);
  pinMode(PIN_FAN_PWM_D5, OUTPUT);

  // Ensure initial outputs match initial model stateCode=0 faultCode=0 previousVibration=false
  // io_3=0, io_4=0, io_5=false, io_6=stateCode(0)
  analogWrite(PIN_FAN_PWM_D5, 0);
  noTone(PIN_BUZZER_D6);
  digitalWrite(PIN_LED_D13, LOW);
  tm1637.setBrightness(7);
  tm1637DisplayValue(0);
}

void loop() {
  static uint32_t nextTickMs = 0;
  if (nextTickMs == 0) nextTickMs = millis();

  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) >= 0) {
    nextTickMs += CONTROL_PERIOD_MS;

    // ordered updates: sample inputs -> model tick -> apply outputs -> serial report
    sampleInputs();
    modelTick();
    applyOutputs();
    serialReport();
  }
}