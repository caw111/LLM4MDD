#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_7_segment_display_counter\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t segPins[8] = {
  2, // A
  3, // B
  4, // C
  5, // D
  6, // E
  7, // F
  8, // G
  9  // DP
};

#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_7_segment_display_counter\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setSegmentsCommonAnode(bool onA, bool onB, bool onC, bool onD, bool onE, bool onF, bool onG, bool onDP);
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_7_segment_display_counter\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void showDigit1To3ForElapsed(uint32_t secondsElapsed);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_7_segment_display_counter\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_7_segment_display_counter\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_7_segment_display_counter\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setSegmentsCommonAnode(bool onA, bool onB, bool onC, bool onD, bool onE, bool onF, bool onG, bool onDP) {
  // Common anode display:
  // ON segment -> LOW (current sinks to segments)
  // OFF segment -> HIGH
  digitalWrite(segPins[0], onA  ? LOW : HIGH);
  digitalWrite(segPins[1], onB  ? LOW : HIGH);
  digitalWrite(segPins[2], onC  ? LOW : HIGH);
  digitalWrite(segPins[3], onD  ? LOW : HIGH);
  digitalWrite(segPins[4], onE  ? LOW : HIGH);
  digitalWrite(segPins[5], onF  ? LOW : HIGH);
  digitalWrite(segPins[6], onG  ? LOW : HIGH);
  digitalWrite(segPins[7], onDP ? LOW : HIGH);
}

void showDigit1To3ForElapsed(uint32_t secondsElapsed) {
  // Expected behavior:
  // - Display 1 between 1–2 seconds
  // - Display 2 between 2–3 seconds
  // - Display 3 between 3–4 seconds
  // For seconds outside those ranges, keep segments off.
  if (secondsElapsed >= 1 && secondsElapsed < 2) {
    // Digit "1": segments B and C
    setSegmentsCommonAnode(
      false, true, true, false, false, false, false, false
    );
  } else if (secondsElapsed >= 2 && secondsElapsed < 3) {
    // Digit "2": segments A, B, D, E, G
    setSegmentsCommonAnode(
      true, true, false, true, true, false, true, false
    );
  } else if (secondsElapsed >= 3 && secondsElapsed < 4) {
    // Digit "3": segments A, B, C, D, G
    setSegmentsCommonAnode(
      true, true, true, true, false, false, true, false
    );
  } else {
    // Blank display (all segments OFF)
    setSegmentsCommonAnode(false, false, false, false, false, false, false, false);
  }
}

void setup() {
  // 1. Set 2~9 pin as output.
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(segPins[i], OUTPUT);
  }

  // Start with display off for deterministic behavior.
  setSegmentsCommonAnode(false, false, false, false, false, false, false, false);
}

void loop() {
  static uint32_t startMs = 0;
  if (startMs == 0) startMs = millis();

  uint32_t secondsElapsed = (millis() - startMs) / 1000UL;

  // 2. Show elapsed time in seconds on the 7-segment display
  showDigit1To3ForElapsed(secondsElapsed);

  // Small delay to reduce flicker/updates to once per loop iteration.
  delay(10);
}
