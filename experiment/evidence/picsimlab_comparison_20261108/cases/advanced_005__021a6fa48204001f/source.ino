#include <Arduino.h>
#include <Wire.h>
#include <TM1637Display.h>
#include <DHT.h>

// ================= Frozen interface contract bindings =================
static const uint8_t PIN_LIGHT_LIVING = A0; // io_0
static const uint8_t PIN_LIGHT_BEDROOM = A1; // io_1

static const uint8_t PIN_DHT_LIVING = 7;  // D7  (io_2, io_3, io_4)
static const uint8_t PIN_DHT_BEDROOM = 8; // D8  (io_5, io_6, io_7)

static const uint8_t PIN_BTN_ROOM = 2; // io_8  active_high=false, pullup=true => press => LOW
static const uint8_t PIN_BTN_MODE = 3; // io_9  press => LOW

static const uint8_t PIN_LED_LIVING_ALARM = 12; // io_10
static const uint8_t PIN_LED_BEDROOM_ALARM = 13; // io_11

static const uint8_t PIN_TM1637_CLK = 4; // D4
static const uint8_t PIN_TM1637_DIO = 5; // D5

// Serial contract
static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t SERIAL_REPORT_PERIOD_MS = 5000;

// ================= DHT configuration =================
#ifndef DHTTYPE_DHT11
#define DHTTYPE_DHT11 DHT11
#endif

DHT dhtLiving(PIN_DHT_LIVING, DHTTYPE_DHT11);
DHT dhtBedroom(PIN_DHT_BEDROOM, DHTTYPE_DHT11);

// ================= TM1637 setup =================
TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ================= Model state (exact) =================
static int32_t roomIndex = 0;             // i32 initial 0
static int32_t displayMode = 0;          // i32 initial 0
static bool prevBtnRoom = false;         // bool initial false
static bool prevBtnMode = false;         // bool initial false
static int32_t serialAccumMs = 0;        // i32 initial 0
static bool nextSerialReady = false;    // bool initial false
static int32_t tm1637NumberLatched = 0; // i32 initial 0

// ================= Inputs (exact port set) =================
static float io_0 = 0.0f; // living light adc_count
static float io_1 = 0.0f; // bedroom light adc_count

static float io_2 = 0.0f; // living temp degC
static float io_3 = 0.0f; // living rh %RH
static bool  io_4 = false; // living dht_valid

static float io_5 = 0.0f; // bedroom temp
static float io_6 = 0.0f; // bedroom rh
static bool  io_7 = false; // bedroom dht_valid

static bool io_8 = false; // button1
static bool io_9 = false; // button2

static int32_t io_13 = 0; // delta elapsed ms (control period increment delta_since_previous_step)

// ================= Preserving "hold_last_valid" invalid policy =================
static float lastLivingTemp = 0.0f;
static float lastLivingHum  = 0.0f;
static bool  lastLivingValid = false;

static float lastBedroomTemp = 0.0f;
static float lastBedroomHum  = 0.0f;
static bool  lastBedroomValid = false;

// DHT sampling periods from contract: 2000ms
static uint32_t lastDhtLivingMs = 0;
static uint32_t lastDhtBedroomMs = 0;

// Light sampling period from contract: 10ms
static uint32_t lastLightSampleMs = 0;

// Buttons sampling period from contract: 10ms
static uint32_t lastButtonSampleMs = 0;

// ================= Timer for control period =================
static uint32_t lastTickMs = 0;

// ================= Serial outputs on trigger =================
static bool serial_0_value_6 = false; // living alarm active
static bool serial_0_value_7 = false; // bedroom alarm active

static float serial_0_value_0 = 0.0f; // living temp
static float serial_0_value_1 = 0.0f; // living rh
static float serial_0_value_2 = 0.0f; // living light adc

static float serial_0_value_3 = 0.0f; // bedroom temp (if io_7 true else io_5)
static float serial_0_value_4 = 0.0f; // bedroom rh   (if io_7 true else io_6)
static float serial_0_value_5 = 0.0f; // bedroom light adc

static bool io_10 = false; // living alarm LED output
static bool io_11 = false; // bedroom alarm LED output

static float serialTempLiving = 0.0f;
static float serialHumLiving  = 0.0f;
static float serialTempBedroom = 0.0f;
static float serialHumBedroom  = 0.0f;
static float serialLightLiving = 0.0f;
static float serialLightBedroom = 0.0f;

// ================= Model tick implementing exactly SysML/IR/IL semantics =================
static inline float realFloor(float x) {
  return floorf(x);
}

static void modelTickComputeOutputsAndTm1637() {
  // Outputs per model contract:
  // io_10 = (if io_4 ? (io_0>=700 && io_2>=30) : false)
  io_10 = (io_4 ? ( (io_0 >= 700.0f) && (io_2 >= 30.0f) ) : false);

  // io_11 = (if io_7 ? (io_1>=700 && io_5>=30) : false)
  io_11 = (io_7 ? ( (io_1 >= 700.0f) && (io_5 >= 30.0f) ) : false);

  // io_12 handled by tm1637NumberLatched
  // serial values:
  serial_0_value_0 = (io_4 ? io_2 : io_2);
  serial_0_value_1 = (io_4 ? io_3 : io_3);
  serial_0_value_2 = io_0;

  serial_0_value_3 = (io_7 ? io_5 : io_5);
  serial_0_value_4 = (io_7 ? io_6 : io_6);
  serial_0_value_5 = io_1;

  serial_0_value_6 = (io_4 && ( (io_2 >= 30.0f) && (io_0 >= 700.0f) ));
  serial_0_value_7 = (io_7 && ( (io_5 >= 30.0f) && (io_1 >= 700.0f) ));

  // Bind to hardware:
  digitalWrite(PIN_LED_LIVING_ALARM, io_10 ? HIGH : LOW);
  digitalWrite(PIN_LED_BEDROOM_ALARM, io_11 ? HIGH : LOW);

  tm1637.displayInt(tm1637NumberLatched, false, 0); // leading_zero=false; invalid_policy=blank
}

static void modelTickUpdateState() {
  // delta elapsed: io_13 already prepared

  // Update ordering from model:
  // if (io_8 and not prevBtnRoom) roomIndex := (roomIndex==0?1:0)
  if (io_8 && !prevBtnRoom) {
    roomIndex = (roomIndex == 0) ? 1 : 0;
  }

  // if (io_9 and not prevBtnMode) displayMode := (displayMode==0?1:(displayMode==1?2:0))
  if (io_9 && !prevBtnMode) {
    if (displayMode == 0) displayMode = 1;
    else if (displayMode == 1) displayMode = 2;
    else displayMode = 0;
  }

  prevBtnRoom = io_8;
  prevBtnMode = io_9;

  serialAccumMs = serialAccumMs + io_13;

  if (serialAccumMs >= 5000) {
    serialAccumMs = serialAccumMs - 5000;
    nextSerialReady = true;
  } else {
    nextSerialReady = false;
  }

  // tm1637NumberLatched updates based on roomIndex and displayMode:
  // if roomIndex == 0:
  //   if displayMode==0 floor(io_2)
  //   else if displayMode==1 floor(io_3)
  //   else floor(io_0)
  // else:
  //   if displayMode==0 floor(io_5)
  //   else if displayMode==1 floor(io_6)
  //   else floor(io_1)
  if (roomIndex == 0) {
    if (displayMode == 0) tm1637NumberLatched = (int32_t)realFloor(io_2);
    else if (displayMode == 1) tm1637NumberLatched = (int32_t)realFloor(io_3);
    else tm1637NumberLatched = (int32_t)realFloor(io_0);
  } else {
    if (displayMode == 0) tm1637NumberLatched = (int32_t)realFloor(io_5);
    else if (displayMode == 1) tm1637NumberLatched = (int32_t)realFloor(io_6);
    else tm1637NumberLatched = (int32_t)realFloor(io_1);
  }
}

// ================= Serial report serialization (fielded numeric/booleans) =================
static void outputSerialReport() {
  // Trigger happens when nextSerialReady is true (port: serial_0_trigger).
  // Contract says "这里按类型选择一组字段化数值/布尔状态进行串口输出；具体文本排版未给出".
  Serial.print("ENV_REPORT,");
  Serial.print("living_temp_c="); Serial.print(serial_0_value_0, 2); Serial.print(",");
  Serial.print("living_humidity_rh="); Serial.print(serial_0_value_1, 2); Serial.print(",");
  Serial.print("living_light_adc="); Serial.print(serial_0_value_2, 0); Serial.print(",");
  Serial.print("living_alarm_active="); Serial.print(serial_0_value_6 ? "true" : "false"); Serial.print(",");

  Serial.print("bedroom_temp_c="); Serial.print(serial_0_value_3, 2); Serial.print(",");
  Serial.print("bedroom_humidity_rh="); Serial.print(serial_0_value_4, 2); Serial.print(",");
  Serial.print("bedroom_light_adc="); Serial.print(serial_0_value_5, 0); Serial.print(",");
  Serial.print("bedroom_alarm_active="); Serial.println(serial_0_value_7 ? "true" : "false");
}

// ================= I/O acquisition matching sampling constraints =================
static void sampleButtonsIfDue() {
  uint32_t now = millis();
  if (now - lastButtonSampleMs >= 10) {
    lastButtonSampleMs = now;

    // model semantics: press corresponds to input true (true_level=LOW); thus io_8/io_9 = (digitalRead==LOW)
    io_8 = (digitalRead(PIN_BTN_ROOM) == LOW);
    io_9 = (digitalRead(PIN_BTN_MODE) == LOW);
  }
}

static void sampleLightsIfDue() {
  uint32_t now = millis();
  if (now - lastLightSampleMs >= 10) {
    lastLightSampleMs = now;

    io_0 = (float)analogRead(PIN_LIGHT_LIVING);
    io_1 = (float)analogRead(PIN_LIGHT_BEDROOM);
  }
}

static void sampleDhtIfDue() {
  uint32_t now = millis();

  // living every 2000ms
  if (now - lastDhtLivingMs >= 2000) {
    lastDhtLivingMs = now;

    float t = dhtLiving.readTemperature(); // degC
    float h = dhtLiving.readHumidity();    // %RH

    // Validity policy: invalid_policy hold_last_valid.
    // We treat NaN as invalid; otherwise valid.
    bool valid = !isnan(t) && !isnan(h);
    if (valid) {
      lastLivingTemp = t;
      lastLivingHum = h;
      lastLivingValid = true;
    }
    // hold_last_valid means: when invalid, keep last valid values; keep validity state as last valid.
    io_2 = lastLivingTemp;
    io_3 = lastLivingHum;
    io_4 = lastLivingValid;
  }

  // bedroom every 2000ms
  if (now - lastDhtBedroomMs >= 2000) {
    lastDhtBedroomMs = now;

    float t = dhtBedroom.readTemperature();
    float h = dhtBedroom.readHumidity();

    bool valid = !isnan(t) && !isnan(h);
    if (valid) {
      lastBedroomTemp = t;
      lastBedroomHum = h;
      lastBedroomValid = true;
    }
    io_5 = lastBedroomTemp;
    io_6 = lastBedroomHum;
    io_7 = lastBedroomValid;
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(PIN_BTN_ROOM, INPUT_PULLUP);
  pinMode(PIN_BTN_MODE, INPUT_PULLUP);

  pinMode(PIN_LED_LIVING_ALARM, OUTPUT);
  pinMode(PIN_LED_BEDROOM_ALARM, OUTPUT);

  // Initialize DHT sensors
  dhtLiving.begin();
  dhtBedroom.begin();

  // TM1637 brightness = 7, leading_zero=false, invalid_policy=blank (handled by displayInt)
  tm1637.setBrightness(7);

  // Initialize timing baselines
  lastTickMs = millis();
  lastLightSampleMs = lastTickMs;
  lastButtonSampleMs = lastTickMs;
  lastDhtLivingMs = lastTickMs;
  lastDhtBedroomMs = lastTickMs;

  // Initialize outputs
  digitalWrite(PIN_LED_LIVING_ALARM, LOW);
  digitalWrite(PIN_LED_BEDROOM_ALARM, LOW);
  tm1637.displayInt(0, false, 0);

  // Initialize model latched values (already zero by static init)
  // prevBtnRoom/prevBtnMode already false.
}

void loop() {
  // control_period_ms = 10; tick is driven by elapsed time
  uint32_t now = millis();
  uint32_t dt = now - lastTickMs;

  if (dt < 10) {
    // keep idle; but ensure serial idle_timeout_ms=20 framing is satisfied naturally by line outputs
    return;
  }

  // If dt is larger, we still compute io_13 as delta_since_previous_step (model contract).
  // For strictness, cap to int32.
  io_13 = (int32_t)dt;
  lastTickMs = now;

  // Sample inputs according to their sampling periods (10ms / 2000ms)
  sampleButtonsIfDue();
  sampleLightsIfDue();
  sampleDhtIfDue();

  // Update model state in frozen order
  modelTickUpdateState();

  // Compute outputs and drive hardware
  modelTickComputeOutputsAndTm1637();

  // Serial report on trigger (serial_0_trigger == nextSerialReady)
  if (nextSerialReady) {
    // Bind serial value ports (already computed in modelTickComputeOutputsAndTm1637).
    outputSerialReport();
  }
}