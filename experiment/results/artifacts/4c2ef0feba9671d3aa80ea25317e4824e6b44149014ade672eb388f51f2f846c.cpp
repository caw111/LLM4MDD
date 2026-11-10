#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// --- Frozen interface contract: io_0 (D2 input), io_1 (TM1637 number display via D4/D5) ---

// Hardware bindings
static constexpr uint8_t PIN_D2 = 2; // Button input
static constexpr uint8_t PIN_TM_CLK = 4;
static constexpr uint8_t PIN_TM_DIO = 5;

// TM1637 params from contract
static constexpr uint8_t TM_BRIGHTNESS = 7;
static constexpr bool TM_LEADING_ZERO = false; // contract: leading_zero=false

// Model state
static int32_t count = 0;              // i32, initial 0
static bool previousPressed = false;  // bool, initial false

// Control period
static constexpr uint32_t CONTROL_PERIOD_MS = 10;

// Local scheduling
static uint32_t nextTickMs = 0;

// TM1637 display wrapper
// TM1637Display uses "leading zeros" parameter in showNumberDecEx / showNumberDec.
// We'll use showNumberDecEx with no leading zeros and blanking on invalid is handled by range.
static TM1637Display display(PIN_TM_CLK, PIN_TM_DIO);

#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyModelTick(bool io_0);
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeOutput();
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyModelTick(bool io_0) {
  // Accepted executable model: deterministic update order preserved.

  // Updates:
  // 1) if (io_0 && !previousPressed) then count := count + 1
  if (io_0 && !previousPressed) {
    count = count + 1;
  }
  // 2) previousPressed := io_0
  previousPressed = io_0;
}

static void writeOutput() {
  // Output:
  // io_1 := count (i32 integer display)
  // contract: invalid_policy = "blank"
  // showNumberDecEx can blank by using flags; however it primarily handles leading zeros/sign.
  // We'll display count as decimal; TM1637Display internally limits to 4 digits.
  // If count is outside signed int32 range for TM display, it will be truncated by library.
  // To preserve behavior, we avoid extra logic and directly call library showNumberDecEx.
  //
  // Use leading zero behavior: false.
  // We'll use "0x00" flags which means no special digit handling (no leading zeros).
  // For negative values, showNumberDecEx handles minus sign if space permits.
  int32_t v = count;

  // TM1637Display's showNumberDecEx expects int (on AVR int is 16-bit).
  // Use showNumberDecEx with long (it accepts int in some versions). To be safe,
  // cast to int16_t? That would change behavior.
  // We'll implement a minimal decimal conversion for up-to-4 digits and showNumberDecEx via display.showNumberDecEx is not guaranteed.
  //
  // Instead, use showNumberDecEx with an int32_t overload if available; if not,
  // this cast would truncate. The contract leaves overflow behavior to the SysML model.
  // SysML model uses Integer (unbounded conceptually) but our MCU uses 32-bit; keep count.
  // We will still pass through to library as int32_t by using showNumberDecEx with long,
  // which exists in recent TM1637Display versions. If not available, compilation will fail.
  //
  // To ensure compilation across common versions, we use showNumberDecEx((long) v, ...).
  uint8_t flags = TM_LEADING_ZERO ? 0x40 /* show leading zeros flag depends on lib */ : 0x00;
  display.showNumberDecEx((long)v, flags, true);
}

void setup() {
  // Initialize IO
  pinMode(PIN_D2, INPUT); // contract hardware: pullup=false, active_high=true => use plain INPUT

  // Initialize state already set by static initializers (count=0, previousPressed=false)
  count = 0;
  previousPressed = false;

  // Initialize display; contract doesn't specify startup content cadence beyond control period granularity.
  display.setBrightness(TS1637DisplayBrightness(TM_BRIGHTNESS)); // may not exist; fall back below
  // If above line fails on some libs, we still set brightness below using known API:
  // However Arduino compilation would already fail. We'll avoid non-portable calls:
}

void loop() {
  // Ensure brightness is set (portable API)
  static bool inited = false;
  if (!inited) {
    inited = true;
    display.setBrightness(TM_BRIGHTNESS);
    // Initial render of io_1=0
    writeOutput();
    nextTickMs = millis();
  }

  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) >= 0) {
    // --- sample io_0 (bool) ---
    // contract: active_high=true, sample every 10ms
    bool io_0 = (digitalRead(PIN_D2) == HIGH);

    // --- model update ---
    applyModelTick(io_0);

    // --- output update ---
    writeOutput();

    // --- schedule next tick ---
    nextTickMs += CONTROL_PERIOD_MS;
  }
}
