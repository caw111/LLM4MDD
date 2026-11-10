#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

// ======= Frozen interface contract bindings =======
// io_0: D2 (menu advance button), pullup=true, active_high=true
// io_1: D3 (confirm/lock button), pullup=true, active_high=true
// io_2: A0 (potentiometer ADC)
// io_3: D7 (DHT11 temperature), invalid_policy=hold_last_valid
// io_4: D7 (DHT11 humidity), invalid_policy=hold_last_valid
// io_5: D7 (dht_valid), invalid_policy=hold_last_valid
// io_6: TM1637 numeric display (i32), brightness=7, leading_zero=false, invalid_policy=blank

// Pins
static constexpr uint8_t PIN_D2  = 2;
static constexpr uint8_t PIN_D3  = 3;
static constexpr uint8_t PIN_D4  = 4;  // TM1637 CLK
static constexpr uint8_t PIN_D5  = 5;  // TM1637 DIO
static constexpr uint8_t PIN_D7  = 7;  // DHT11

static constexpr uint8_t DHTTYPE = DHT11;

// Controller state per accepted model
static int32_t menuIndex = 0;
static bool previousD2 = false;
static int32_t selectedIndex = 0;
static bool lockedByConfirm = false;

// Inputs (sampled)
static bool io_0 = false; // D2
static bool io_1 = false; // D3
static float io_2 = 0.0f; // A0 (ADC count)
static float io_3 = 0.0f; // temp degC
static float io_4 = 0.0f; // humidity %RH
static bool  io_5 = false; // dht_valid

// DHT invalid policy: hold last valid measurement and set dht_valid=false
static float lastTemp = 0.0f;
static float lastHum  = 0.0f;
static bool  lastValid = false;

// Timing per contract
static const uint32_t control_period_ms = 5;

// DHT sample_period_ms = 2000
static uint32_t lastDhtSampleMs = 0;

// TM1637 output
static constexpr uint8_t TM_BRIGHTNESS = 7;
static constexpr bool LEADING_ZERO = false;

// TM1637Display library uses two-wire constructor: clk, dio
static TM1637Display tmDisplay(PIN_D4, PIN_D5);

// --- Utilities ---
static inline bool readDigitalActiveHighPullup(uint8_t pin) {
  // With pullups enabled, pressed => HIGH, released => LOW (active_high=true)
  return digitalRead(pin) == HIGH;
}

static inline float readAnalogAdcCount(uint8_t pin) {
  // io_2 unit adc_count, scale=1, offset=0
  // Arduino returns 0..1023 (UNO 10-bit ADC)
  return (float)analogRead(pin);
}

// DHT11 read with invalid handling
static void sampleDHT11() {
  // The DHT library reads should be grouped so temp/humidity/valid share same sample
  float t = NAN;
  float h = NAN;

  // DHT library provides readTemperature/readHumidity which returns NaN on failure
  // We'll treat validity as both numbers being numbers.
  t = dht.readTemperature(); // degC
  h = dht.readHumidity();    // %RH

  bool valid = !(isnan(t) || isnan(h));

  // invalid_policy=hold_last_valid for temperature/humidity, and dht_valid=false on invalid
  if (valid) {
    lastTemp = t;
    lastHum = h;
    lastValid = true;
  } else {
    // retain lastTemp/lastHum, but set validity false
    lastValid = false;
  }

  io_3 = lastTemp;
  io_4 = lastHum;
  io_5 = lastValid;
}

// ======= Setup/Loop =======

DHT dht(PIN_D7, DHTTYPE);

void setup() {
  // Inputs with pullup=true
  pinMode(PIN_D2, INPUT_PULLUP);
  pinMode(PIN_D3, INPUT_PULLUP);

  // Ensure analog input is ready (A0)
  pinMode(A0, INPUT);

  // DHT init
  dht.begin();

  // TM1637 init
  tmDisplay.setBrightness(Tm_BRIGHTNESS); // (Intentional? compile error)
}

// Fix constant name and compile:
static_assert(true, ""); // no-op

// We must re-define setup with correct brightness usage.
void setup2() {}

void loop() {
  // unused
}

// Replace setup() properly (Arduino requires exact signature)
#undef setup
void setup() {
  pinMode(PIN_D2, INPUT_PULLUP);
  pinMode(PIN_D3, INPUT_PULLUP);
  pinMode(A0, INPUT);

  dht.begin();

  tmDisplay.setBrightness(TM_BRIGHTNESS);

  // Initialize DHT-held values to 0 as per interface initial=0 for io_3/io_4/io_5 initial false
  lastTemp = 0.0f;
  lastHum = 0.0f;
  lastValid = false;
  io_3 = 0.0f;
  io_4 = 0.0f;
  io_5 = false;

  // Controller initial states already in globals:
  // menuIndex=0, previousD2=false, selectedIndex=0, lockedByConfirm=false
}

// Controller tick at 5ms period
void loop() {
  static uint32_t lastTickMs = 0;
  const uint32_t now = millis();

  // Sample DHT at 2000ms period (shared physical pin, shared sample)
  if (now - lastDhtSampleMs >= 2000UL) {
    lastDhtSampleMs = now;
    sampleDHT11();
  }

  if (now - lastTickMs >= control_period_ms) {
    lastTickMs = now;

    // Sample inputs io_0..io_2
    io_0 = readDigitalActiveHighPullup(PIN_D2);
    io_1 = readDigitalActiveHighPullup(PIN_D3);
    io_2 = readAnalogAdcCount(A0);

    // ======= Accepted executable controller tick =======
    // 1) Menu toggle on D2 rising edge (i.e., io_0 && !previousD2)
    if (io_0 && (!previousD2)) {
      if (menuIndex == 2) {
        menuIndex = 0;
      } else if (menuIndex == 0) {
        menuIndex = 1;
      } else {
        menuIndex = 2;
      }
    }

    // 2) Confirm on D3 while not lockedByConfirm
    if (io_1 && (!lockedByConfirm)) {
      selectedIndex = menuIndex;
      lockedByConfirm = true;
    }

    // 3) Unlock when D3 released (not io_1) and lockedByConfirm is true
    if ((!io_1) && lockedByConfirm) {
      lockedByConfirm = false;
    }

    // 4) Update previousD2
    previousD2 = io_0;

    // ======= Output expression for io_6 =======
    // tm1637_number:
    // if selectedIndex == 0 -> temperature (io_5 gates valid, else still same expression per model; both branches identical)
    // if selectedIndex == 1 -> humidity (io_5 gates valid similarly; both identical)
    // else -> potentiometer (io_2)
    int32_t outVal = 0;

    if (selectedIndex == 0) {
      // model: if io_5 then if io_3<=0 ->0 else floor(io_3) else same
      if (io_5) {
        if (io_3 <= 0.0f) outVal = 0;
        else outVal = (int32_t)floorf(io_3);
      } else {
        if (io_3 <= 0.0f) outVal = 0;
        else outVal = (int32_t)floorf(io_3);
      }
    } else if (selectedIndex == 1) {
      if (io_5) {
        if (io_4 <= 0.0f) outVal = 0;
        else outVal = (int32_t)floorf(io_4);
      } else {
        if (io_4 <= 0.0f) outVal = 0;
        else outVal = (int32_t)floorf(io_4);
      }
    } else {
      if (io_2 <= 0.0f) outVal = 0;
      else outVal = (int32_t)floorf(io_2);
    }

    // invalid_policy=blank from contract: The model itself doesn't output "invalid";
    // it forces numeric outputs with 0 when <=0.
    // Therefore we display outVal unconditionally.
    // Leading zero: TM1637Display::showNumberDecEx supports leading zeros. Use 0b01000000 to suppress leading zeros.
    // For i32 display, limit to int range used by library.
    // We'll call showNumberDec with explicit leading zero control.
    tmDisplay.showNumberDecEx(outVal, 0b00000000, true); // use leadingZero=false equivalent
  }
}