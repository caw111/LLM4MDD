#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiple_timezone\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Wire.h>
#include <RTClib.h>

static const uint8_t BUTTON1_PIN = 2; // UTC+9
static const uint8_t BUTTON2_PIN = 3; // UTC+4
static const uint8_t BUTTON3_PIN = 4; // UTC-10
static const uint8_t BUTTON4_PIN = 5; // UTC-5

RTC_DS1307 rtc;

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiple_timezone\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void printISO8601(const DateTime& dt);
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiple_timezone\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readSelectedOffsetHours();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiple_timezone\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiple_timezone\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiple_timezone\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void printISO8601(const DateTime& dt) {
  char buf[20]; // "YYYY-MM-DDTHH:MM:SS"
  snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d",
           dt.year(), dt.month(), dt.day(),
           dt.hour(), dt.minute(), dt.second());
  Serial.println(buf);
}

static int readSelectedOffsetHours() {
  // Buttons read HIGH when pressed.
  // If none pressed => UTC (0)
  bool b1 = (digitalRead(BUTTON1_PIN) == HIGH);
  bool b2 = (digitalRead(BUTTON2_PIN) == HIGH);
  bool b3 = (digitalRead(BUTTON3_PIN) == HIGH);
  bool b4 = (digitalRead(BUTTON4_PIN) == HIGH);

  // Match requirement mapping.
  // If multiple are pressed, give precedence in a deterministic order:
  // Button 1, then 2, then 3, then 4. (No explicit requirement; behavior must be deterministic.)
  if (b1) return +9;
  if (b2) return +4;
  if (b3) return -10;
  if (b4) return -5;
  return 0;
}

void setup() {
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  pinMode(BUTTON3_PIN, INPUT);
  pinMode(BUTTON4_PIN, INPUT);

  Serial.begin(9600);
  while (!Serial) {
    // wait for serial to be ready on boards that require it
  }

  // Check if the RTC module is available and initialize it
  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("RTC not found");
  } else {
    // If you want to ensure running, do not change time automatically
    if (!rtc.isrunning()) {
      // Keep behavior conservative; do not set time unless required.
      // The circuit includes an initTime for simulation.
      rtc.adjust(rtc.now());
    }
  }
}

void loop() {
  // Continuously retrieve current time from RTC
  DateTime nowUtc = rtc.now();

  // Check state of buttons and apply corresponding timezone
  int offsetHours = readSelectedOffsetHours();

  DateTime adjusted = nowUtc + TimeSpan((long)offsetHours * 3600L);

  // Print resulting timestamp
  printISO8601(adjusted);

  // Small delay to avoid flooding Serial
  delay(200);
}
