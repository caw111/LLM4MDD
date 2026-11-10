#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_110\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

static const unsigned long INTERVAL_MS = 2000;

// Wiring (from circuit)
static const int LED1_PIN = 2;          // led1:A -> Mega 2
static const int RGB1_R_PIN = 3;        // rgb1:R -> Mega 3
static const int RGB1_G_PIN = 4;        // rgb1:G -> Mega 4
static const int RGB1_B_PIN = 5;        // rgb1:B -> Mega 5

static const int RGB2_R_PIN = 6;        // rgb2:R -> Mega 6
static const int RGB2_G_PIN = 7;        // rgb2:G -> Mega 7
static const int RGB2_B_PIN = 8;        // rgb2:B -> Mega 8

static const int SERVO1_PWM_PIN = 9;    // servo1:PWM -> Mega 9
static const int SERVO2_PWM_PIN = 10;   // servo2:PWM -> Mega 10
static const int SERVO3_PWM_PIN = 11;   // servo3:PWM -> Mega 11

Servo servo1;
Servo servo2;
Servo servo3;

enum ColorIndex : uint8_t {
  RED = 0,
  GREEN,
  BLUE,
  YELLOW,
  CYAN,
  MAGENTA,
  WHITE,
  COLOR_COUNT
};

static const ColorIndex kStartColor = RED;
static ColorIndex currentColor = kStartColor;

#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_110\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBPinsCommonCathode(bool r, bool g, bool b);
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_110\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyColor(ColorIndex idx);
#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_110\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int mapToTarget(int angle);
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_110\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateServosAtInterval();
#line 112 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_110\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 144 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_110\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_110\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBPinsCommonCathode(bool r, bool g, bool b) {
  // Common cathode: HIGH on pin turns LED off if using "active-low"? In Wokwi RGB LED
  // with common="cathode", COM is tied to GND, so pins source current (active-high).
  // Therefore: HIGH => ON.
  digitalWrite(RGB1_R_PIN, r ? HIGH : LOW);
  digitalWrite(RGB1_G_PIN, g ? HIGH : LOW);
  digitalWrite(RGB1_B_PIN, b ? HIGH : LOW);

  digitalWrite(RGB2_R_PIN, r ? HIGH : LOW);
  digitalWrite(RGB2_G_PIN, g ? HIGH : LOW);
  digitalWrite(RGB2_B_PIN, b ? HIGH : LOW);
}

static void applyColor(ColorIndex idx) {
  // Sequence requested: red, green, blue, yellow, cyan, magenta, white
  switch (idx) {
    case RED:     setRGBPinsCommonCathode(true,  false, false); break; // R
    case GREEN:   setRGBPinsCommonCathode(false, true,  false); break; // G
    case BLUE:    setRGBPinsCommonCathode(false, false, true ); break; // B
    case YELLOW:  setRGBPinsCommonCathode(true,  true,  false); break; // R+G
    case CYAN:    setRGBPinsCommonCathode(false, true,  true ); break; // G+B
    case MAGENTA: setRGBPinsCommonCathode(true,  false, true ); break; // R+B
    case WHITE:   setRGBPinsCommonCathode(true,  true,  true ); break; // R+G+B
    default:      setRGBPinsCommonCathode(true,  false, false); break; // fallback to red
  }
}

static unsigned long lastTickLED = 0;
static unsigned long lastTickRGB = 0;
static unsigned long lastTickServo = 0;

static bool led1StateOff = true; // init OFF

static int servo1Angle = 0;     // init 0 degrees
static int servo2Angle = 180;   // init 180 degrees
static int servo3Angle = 90;    // init 90 degrees

static bool servo1AtZero = true;   // if at 0, scan to 180
static bool servo2At180 = true;   // if at 180, scan to 0
static bool servo3At90 = true;    // if at 90, scan to 135

static int mapToTarget(int angle) { return angle; }

static void updateServosAtInterval() {
  // Each servo performs a synchronous "scan" between two target positions,
  // toggling each time the 2-second interval elapses.

  // Servo 1: 0 <-> 180
  if (servo1AtZero) {
    servo1Angle = 180;
  } else {
    servo1Angle = 0;
  }
  servo1.write(mapToTarget(servo1Angle));
  servo1AtZero = !servo1AtZero;

  // Servo 2: 180 <-> 0
  if (servo2At180) {
    servo2Angle = 0;
  } else {
    servo2Angle = 180;
  }
  servo2.write(mapToTarget(servo2Angle));
  servo2At180 = !servo2At180;

  // Servo 3: 90 <-> 135
  if (servo3At90) {
    servo3Angle = 135;
  } else {
    servo3Angle = 90;
  }
  servo3.write(mapToTarget(servo3Angle));
  servo3At90 = !servo3At90;
}

void setup() {
  // LED init: off
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  led1StateOff = true;

  // RGB init: display red light
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);
  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  applyColor(currentColor);

  // Servos init
  servo1.attach(SERVO1_PWM_PIN);
  servo2.attach(SERVO2_PWM_PIN);
  servo3.attach(SERVO3_PWM_PIN);

  servo1.write(0);
  servo2.write(180);
  servo3.write(90);

  // Initialize tick times so updates occur after 2s synchronously thereafter.
  unsigned long now = millis();
  lastTickLED = now;
  lastTickRGB = now;
  lastTickServo = now;
}

void loop() {
  unsigned long now = millis();

  // 1) LED blinking with 2-second interval (on/off)
  if (now - lastTickLED >= INTERVAL_MS) {
    lastTickLED += INTERVAL_MS;

    led1StateOff = !led1StateOff;
    digitalWrite(LED1_PIN, led1StateOff ? LOW : HIGH);
  }

  // 2) Rainbow color cycling every 2 seconds
  if (now - lastTickRGB >= INTERVAL_MS) {
    lastTickRGB += INTERVAL_MS;

    currentColor = static_cast<ColorIndex>((static_cast<uint8_t>(currentColor) + 1) % static_cast<uint8_t>(COLOR_COUNT));
    applyColor(currentColor);
  }

  // 3) Servo synchronous scanning every 2 seconds
  if (now - lastTickServo >= INTERVAL_MS) {
    lastTickServo += INTERVAL_MS;
    updateServosAtInterval();
  }
}
