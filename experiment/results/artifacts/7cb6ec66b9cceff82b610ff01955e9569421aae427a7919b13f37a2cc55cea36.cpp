#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_7_segment_display_serial\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t SEG_A  = 2;
static const uint8_t SEG_B  = 3;
static const uint8_t SEG_C  = 4;
static const uint8_t SEG_D  = 5;
static const uint8_t SEG_E  = 6;
static const uint8_t SEG_F  = 7;
static const uint8_t SEG_G  = 8;
static const uint8_t SEG_DP = 9;

// Common anode display:
// - HIGH turns segment OFF
// - LOW turns segment ON
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_7_segment_display_serial\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setSegmentsAllOff();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_7_segment_display_serial\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showDigitChar(char ch);
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_7_segment_display_serial\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_7_segment_display_serial\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_7_segment_display_serial\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setSegmentsAllOff() {
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
  digitalWrite(SEG_DP, HIGH);
}

// Segment bit order: A,B,C,D,E,F,G
static void showDigitChar(char ch) {
  // Default: all segments off
  uint8_t a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0;

  // Digits for a typical 7-seg (no DP)
  // Order used in assigns below:  A B C D E F G
  switch (ch) {
    case '0': a=1; b=1; c=1; d=1; e=1; f=1; g=0; break;
    case '1': a=0; b=1; c=1; d=0; e=0; f=0; g=0; break;
    case '2': a=1; b=1; c=0; d=1; e=1; f=0; g=1; break;
    case '3': a=1; b=1; c=1; d=1; e=0; f=0; g=1; break;
    case '4': a=0; b=1; c=1; d=0; e=0; f=1; g=1; break;
    case '5': a=1; b=0; c=1; d=1; e=0; f=1; g=1; break;
    case '6': a=1; b=0; c=1; d=1; e=1; f=1; g=1; break;
    case '7': a=1; b=1; c=1; d=0; e=0; f=0; g=0; break;
    case '8': a=1; b=1; c=1; d=1; e=1; f=1; g=1; break;
    case '9': a=1; b=1; c=1; d=1; e=0; f=1; g=1; break;
    default:
      // For anything else, turn all segments off (best effort)
      setSegmentsAllOff();
      return;
  }

  // Since common anode: segment ON = LOW
  digitalWrite(SEG_A,  a ? LOW : HIGH);
  digitalWrite(SEG_B,  b ? LOW : HIGH);
  digitalWrite(SEG_C,  c ? LOW : HIGH);
  digitalWrite(SEG_D,  d ? LOW : HIGH);
  digitalWrite(SEG_E,  e ? LOW : HIGH);
  digitalWrite(SEG_F,  f ? LOW : HIGH);
  digitalWrite(SEG_G,  g ? LOW : HIGH);

  // DP not used; keep OFF
  digitalWrite(SEG_DP, HIGH);
}

void setup() {
  // Per requirement: set 2~9 as output
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  pinMode(SEG_DP, OUTPUT);

  // Start with all segments off (DP off as well)
  setSegmentsAllOff();

  // Serial input: 9600 baud, line_or_idle; idle_timeout_ms=20
  Serial.begin(9600);
}

void loop() {
  static uint32_t lastRx = 0;
  static char lastChar = '\0';

  // Continuously check if the serial is available.
  while (Serial.available() > 0) {
    int r = Serial.read();
    if (r < 0) break;

    lastRx = millis();
    lastChar = (char)r;

    // Show serial input on 7-seg
    showDigitChar(lastChar);

    // If framing by line_or_idle, newline/CR can be treated as end.
    // Behavior requirement: show corresponding value as serial input.
    // We keep displaying as soon as a character arrives.
  }

  // idle_timeout_ms=20 (line_or_idle)
  // If no new serial data arrives for 20ms, do nothing else (display already set).
  // This block exists only to preserve the intended "line_or_idle" behavior context.
  (void)lastRx;
}
