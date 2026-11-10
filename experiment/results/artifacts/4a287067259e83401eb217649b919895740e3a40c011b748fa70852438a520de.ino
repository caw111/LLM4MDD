#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

// ===== Frozen hardware contract (pins / roles) =====
static const uint8_t DHT_PIN = 7;   // D7
static const uint8_t TM_CLK   = 4;   // D4
static const uint8_t TM_DIO   = 5;   // D5

// DHT11
#define DHTTYPE DHT11
static DHT dht(DHT_PIN, DHTTYPE);

// TM1637
static TM1637Display tm1637(TM_CLK, TM_DIO);

// ===== Accepted executable model (Controller state & output mapping) =====
static double io_0 = 0.0;     // f64 degC
static double io_1 = 0.0;     // f64 %RH (unused by model output, but still acquired)
static bool   io_2 = false;   // bool validity (DHT read validity for display update)

static double lastValidTempC = 0.0; // initialized 0.0
static int32_t lastUpdatedMs = 0;   // initialized 0 (not used by provided updates, but preserved as state)

// floor semantics matching "RealFunctions::floor"
static inline int32_t floorClampRealToInt32(double v) {
  if (v < 0.0) return 0;
  // Clamp to 2147483647, then floor
  if (v > 2147483647.0) return 2147483647;
  // v is within [0, 2147483647]; compute floor
  // For non-negative numbers, casting to int32 after floor is safe.
  // But we must ensure exact floor for doubles: use floor().
  double f = floor(v);
  if (f < 0.0) return 0; // extra safety
  if (f > 2147483647.0) return 2147483647;
  return (int32_t)f;
}

// Controller tick equivalent
static inline int32_t controllerComputeOutputIo3() {
  if (io_2) {
    return floorClampRealToInt32(io_0);
  } else {
    return floorClampRealToInt32(lastValidTempC);
  }
}

// ===== DHT acquisition and model stepping =====
// Model requirement: temperature display refreshed every 2 seconds.
// Also: invalid policy "hold_last_valid" implemented by updating lastValidTempC only when io_2 is true
// and using it when io_2 is false.
//
// DHT sample_period_ms >= 2000ms; use 2000ms cadence.
static const uint32_t sample_period_ms = 2000;
static uint32_t lastSampleMs = 0;

// We also need to preserve ordered updates: tick has "if io_2 then assign lastValidTempC := io_0".
static void controllerTick() {
  if (io_2) {
    lastValidTempC = io_0;
  }
  lastUpdatedMs = (int32_t)millis();
}

// Determine DHT validity for io_2.
// DHT11 reading: library returns NaN on failure. Use that as invalid_policy driver.
static void acquireDHTInputs() {
  // Using readTemperature() and readHumidity(); must share same physical sensor pin.
  float t = dht.readTemperature(); // °C
  float h = dht.readHumidity();    // %RH

  bool tempValid = !isnan(t);
  bool humValid  = !isnan(h);

  if (tempValid) {
    io_0 = (double)t;
  }
  if (humValid) {
    io_1 = (double)h;
  }

  // io_2 is "dht_valid" for display update; tie validity to temperature validity (as contract indicates shared frame).
  io_2 = tempValid;
}

static void updateDisplayFromModel() {
  // Output io_3 is i32; model says invalid_policy blank.
  // However the accepted model defines io_3 value as held last valid on io_2 false (clamp/floor).
  // Therefore, we must follow the accepted executable model exactly: io_3 retains last valid value.
  int32_t io_3 = controllerComputeOutputIo3();

  // TM1637Display: showNumber accepts int/long. Keep as signed integer.
  // Contract specifies invalid_policy blank, but accepted model does not blank. The executable model is authoritative.
  tm1637.showNumber((long)io_3);
}

void setup() {
  // Initialize model state already set by static initialization, but ensure explicit resets
  lastValidTempC = 0.0;
  lastUpdatedMs = 0;

  // Initialize DHT and TM1637
  dht.begin();

  tm1637.setBrightness(3);

  // Initial acquisition to populate model quickly, but still honor sample period for subsequent updates.
  // If initial read is invalid, display will follow model (io_2 false => uses lastValidTempC=0).
  acquireDHTInputs();
  controllerTick();
  updateDisplayFromModel();

  lastSampleMs = millis();
}

void loop() {
  uint32_t now = millis();
  if (now - lastSampleMs >= sample_period_ms) {
    lastSampleMs = now;

    // acquisition
    acquireDHTInputs();

    // controller tick: ordered update lastValidTempC := io_0 only when io_2 true
    controllerTick();

    // update display according to accepted model output expression for io_3
    updateDisplayFromModel();
  }

  // No other behavior required
}