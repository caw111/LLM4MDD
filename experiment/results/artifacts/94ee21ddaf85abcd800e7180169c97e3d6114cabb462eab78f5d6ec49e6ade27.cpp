#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_serial_RGB_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t R_pin = 4;
const uint8_t G_pin = 3;
const uint8_t B_pin = 2;

// Common anode RGB LED: fully ON requires LOW on the corresponding pin.
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_serial_RGB_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setColor(bool red, bool green, bool blue);
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_serial_RGB_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void clearAll();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_serial_RGB_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_serial_RGB_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_serial_RGB_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setColor(bool red, bool green, bool blue) {
  digitalWrite(R_pin, red   ? LOW : HIGH);
  digitalWrite(G_pin, green ? LOW : HIGH);
  digitalWrite(B_pin, blue  ? LOW : HIGH);
}

static void clearAll() {
  setColor(false, false, false);
}

void setup() {
  pinMode(R_pin, OUTPUT);
  pinMode(G_pin, OUTPUT);
  pinMode(B_pin, OUTPUT);

  clearAll();

  Serial.begin(9600);

  // Wait for Serial monitor to become available, but keep behavior robust.
  // Requirement: "Continuously check if the serial monitor is available"
}

void loop() {
  // Requirement: continuously check if serial is available, and read until '\n' appears.
  if (!Serial.available()) {
    return;
  }

  static String line;
  line = "";

  // Read until '\n' appears (line_or_idle behavior in prompt).
  unsigned long start = millis();
  while (true) {
    if (Serial.available()) {
      char c = (char)Serial.read();
      if (c == '\r') {
        // ignore carriage return
      } else if (c == '\n') {
        break;
      } else {
        line += c;
      }
    }

    // idle_timeout_ms=20: stop waiting if no data arrives within timeout
    if (millis() - start > 20) {
      // treat whatever we have so far as the line
      break;
    }
  }

  // Requirement: Check if the input contains "Red", "Green", or "Blue",
  // and fully turn on the corresponding color if it does.
  String s = line;
  s.toLowerCase();

  bool wantRed = (s.indexOf("red") != -1);
  bool wantGreen = (s.indexOf("green") != -1);
  bool wantBlue = (s.indexOf("blue") != -1);

  setColor(wantRed, wantGreen, wantBlue);
}
