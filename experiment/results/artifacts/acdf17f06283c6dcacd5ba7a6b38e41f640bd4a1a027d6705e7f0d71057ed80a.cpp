#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGB(int r, int g, int b);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setBarGraph(int count);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t segMaskForDigit(int d);
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setDigit(int digit);
#line 123 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setColorForDigit(int digit);
#line 134 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void initPins();
#line 158 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void renderNumber(int n);
#line 164 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 180 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGB(int r, int g, int b) {
  // rgb1: cathode common (COM to GND). R/G/B are active-HIGH.
  digitalWrite(2, r ? HIGH : LOW); // R
  digitalWrite(3, g ? HIGH : LOW); // G
  digitalWrite(4, b ? HIGH : LOW); // B
}

void shiftWrite74HC595(uint8_t data, bool outputEnableActiveLow = true) {
  // sr1 wiring:
  // A2 -> SHCP (shift clock)
  // A1 -> STCP (latch clock)
  // A0 -> DS (data)
  // OE tied to GND via sr1:OE so output enabled by default; keep function for completeness.
  // MR tied to VCC (through red) so no reset is needed in normal operation.

  // Data on DS (A0)
  // We'll shift MSB->LSB so that Q0..Q7 map in order (typical expectation):
  // bit0 -> Q0 (least significant)
  // Since we need Q0..Q6 for segments A..G, we will use a mapping such that:
  // Q0 is segment A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G.
  const uint8_t msbFirstStart = 7;

  for (int i = msbFirstStart; i >= 0; --i) {
    digitalWrite(A0, (data >> i) & 0x01 ? HIGH : LOW);
    // rising edge on SHCP
    digitalWrite(A2, LOW);
    digitalWrite(A2, HIGH);
  }

  // Latch STCP (A1) rising edge
  digitalWrite(A1, LOW);
  digitalWrite(A1, HIGH);
}

void setBarGraph(int count) {
  // bargraph1: "from pin:A1 to pin:A10" requirement corresponds to A1..A10 driving A1..A10 pins.
  // However, the provided circuit uses:
  // - mega:5..13 -> bargraph A1..A9
  // - mega:44 -> bargraph A10
  // and all Cx pins are tied to GND (common cathode setup).
  // We light exactly `count` segments (1..10) for numbers 0..9:
  // Requirement: "number of segments equal to current number (e.g., 5 segments for number 5)."
  // For number 0 => all off.
  // Thus we will enable segments 1..count (count may be 0..9).
  // Pin mapping from circuit:
  // A1  -> mega pin 5
  // A2  -> mega pin 6
  // A3  -> mega pin 7
  // A4  -> mega pin 8
  // A5  -> mega pin 9
  // A6  -> mega pin 10
  // A7  -> mega pin 11
  // A8  -> mega pin 12
  // A9  -> mega pin 13
  // A10 -> mega pin 44
  const uint8_t pins[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};

  for (int i = 0; i < 10; i++) {
    digitalWrite(pins[i], (i + 1) <= count ? HIGH : LOW);
  }
}

uint8_t segMaskForDigit(int d) {
  // sevseg1 driven by sr1 Q0..Q6:
  // Q0 -> A
  // Q1 -> B
  // Q2 -> C
  // Q3 -> D
  // Q4 -> E
  // Q5 -> F
  // Q6 -> G
  //
  // We'll assume segment inputs are active-HIGH (typical for Wokwi sevseg).
  // Patterns for common-cathode 7-seg (A B C D E F G):
  // 0: A B C D E F
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: B C F G
  // 5: A C D F G
  // 6: A C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  //
  // Convert to Q bits (Q0=A ... Q6=G).
  bool A=false,B=false,C=false,D=false,E=false,F=false,G=false;

  switch (d) {
    case 0: A=true;B=true;C=true;D=true;E=true;F=true;G=false; break;
    case 1: A=false;B=true;C=true;D=false;E=false;F=false;G=false; break;
    case 2: A=true;B=true;C=false;D=true;E=true;F=false;G=true; break;
    case 3: A=true;B=true;C=true;D=true;E=false;F=false;G=true; break;
    case 4: A=false;B=true;C=true;D=false;E=false;F=true;G=true; break;
    case 5: A=true;B=false;C=true;D=true;E=false;F=true;G=true; break;
    case 6: A=true;B=false;C=true;D=true;E=true;F=true;G=true; break;
    case 7: A=true;B=true;C=true;D=false;E=false;F=false;G=false; break;
    case 8: A=true;B=true;C=true;D=true;E=true;F=true;G=true; break;
    case 9: A=true;B=true;C=true;D=true;E=false;F=true;G=true; break;
  }

  uint8_t mask = 0;
  if (A) mask |= (1u << 0); // Q0 -> A
  if (B) mask |= (1u << 1); // Q1 -> B
  if (C) mask |= (1u << 2); // Q2 -> C
  if (D) mask |= (1u << 3); // Q3 -> D
  if (E) mask |= (1u << 4); // Q4 -> E
  if (F) mask |= (1u << 5); // Q5 -> F
  if (G) mask |= (1u << 6); // Q6 -> G

  return mask; // Q7 left unused (bit7 = 0)
}

void setDigit(int digit) {
  uint8_t mask7 = segMaskForDigit(digit);
  // We output full 8 bits to 74HC595. Q0..Q7 corresponds to bits 0..7.
  uint8_t out = mask7;
  shiftWrite74HC595(out);
}

void setColorForDigit(int digit) {
  // Red for 0-3, Green for 4-6, Blue for 7-9
  if (digit <= 3) {
    setRGB(1, 0, 0);
  } else if (digit <= 6) {
    setRGB(0, 1, 0);
  } else {
    setRGB(0, 0, 1);
  }
}

void initPins() {
  // RGB pins
  pinMode(2, OUTPUT); // R
  pinMode(3, OUTPUT); // G
  pinMode(4, OUTPUT); // B

  // Shift register pins
  pinMode(A0, OUTPUT); // DS
  pinMode(A1, OUTPUT); // STCP
  pinMode(A2, OUTPUT); // SHCP

  // Ensure clocks low
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);

  // Bar graph pins (from circuit)
  const uint8_t barPins[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};
  for (int i = 0; i < 10; i++) {
    pinMode(barPins[i], OUTPUT);
    digitalWrite(barPins[i], LOW);
  }
}

void renderNumber(int n) {
  setDigit(n);
  setBarGraph(n);        // "equal to current number" for 0..9
  setColorForDigit(n);   // RGB scheme
}

void setup() {
  initPins();

  // Initialization requirements:
  // - sevseg shows "0"
  // - bargraph all off
  // - rgb set to red
  setRGB(1, 0, 0); // red

  // bargraph all off
  setBarGraph(0);

  // show 0 on sevseg
  setDigit(0);
}

void loop() {
  static int current = 0;
  static unsigned long lastUpdate = 0;

  unsigned long now = millis();
  if (now - lastUpdate >= 2000UL) {
    lastUpdate = now;

    // increment each 2 seconds; after 9 reset to 0
    current++;
    if (current > 9) current = 0;

    renderNumber(current);
  }
}
