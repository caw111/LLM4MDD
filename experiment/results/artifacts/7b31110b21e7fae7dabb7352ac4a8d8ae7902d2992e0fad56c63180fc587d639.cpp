#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

//
// Wiring (preserved):
//  Shift register sr1 (74HC595)
//    VCC  -> 5V
//    MR   -> sevseg1 COM.1 (tied together in circuit) and to VCC in provided diagram
//    OE   -> sr1:GND (per circuit, effectively permanently enabled/inhibited by wiring)
//    DS   -> Arduino A0
//    SHCP -> Arduino A2
//    STCP -> Arduino A1
//
//  7-seg display sevseg1 (common cathode assumed; segments A..G and COM.1 handled by circuit)
//    A -> Q0
//    B -> Q1
//    C -> Q2
//    D -> Q3
//    E -> Q4
//    F -> Q5
//    G -> Q6
//
//  LED bargraph1 controlled from shift register Q0..Q6 through additional wiring
//    A1..A10 are on Arduino pins 2..11 respectively
//    C1..C10 are all on Arduino GND.1
//  Note: With the given circuit, bargraph LEDs are driven directly by Arduino pins 2..11.
//

// Shift register control pins (Mega 2560)
static const uint8_t PIN_DS  = A0; // sr1:DS (blue)
static const uint8_t PIN_STCP = A1; // sr1:STCP (purple) - latch
static const uint8_t PIN_SHCP = A2; // sr1:SHCP (gray)   - clock

// Bar graph pins (as wired)
static const uint8_t BAR_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; // A1..A10
// All bargraph common/cathodes (C1..C10) are connected to GND.1 in circuit (no pin needed).

#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftWrite(uint8_t valueQ0toQ7);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBarGraph(uint8_t n);
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setSevenSegDigit(uint8_t digit);
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 138 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftWrite(uint8_t valueQ0toQ7)
{
  // DS provides Q0..Q7 data when clocked into shift register.
  // We will shift out 8 bits MSB first, where Q0 corresponds to bit0 in our value.
  // That requires reversing bit order during shifting.
  for (int i = 7; i >= 0; --i)
  {
    digitalWrite(PIN_DS, (valueQ0toQ7 >> i) & 0x01);

    // Toggle SHCP
    digitalWrite(PIN_SHCP, HIGH);
    delayMicroseconds(2);
    digitalWrite(PIN_SHCP, LOW);
    delayMicroseconds(2);
  }

  // Latch to outputs
  digitalWrite(PIN_STCP, HIGH);
  delayMicroseconds(2);
  digitalWrite(PIN_STCP, LOW);
}

static void setBarGraph(uint8_t n)
{
  // Light up LEDs A1..A10 corresponding to n (0..9).
  // Requirement: when n reaches 9, show all 9 LEDs; but "number of LEDs equal to current number"
  // => for n=0 -> none; for n=9 -> nine LEDs (A1..A9). A10 remains off.
  for (uint8_t i = 0; i < 10; ++i)
  {
    // For i=0 => LED1 (A1), should be on when n >= 1
    // i=8 => LED9 (A9), should be on when n >= 9
    // i=9 => LED10 (A10), should never be on for n<=9
    bool on = (i < n);
    digitalWrite(BAR_PINS[i], on ? HIGH : LOW);
  }
}

static void setSevenSegDigit(uint8_t digit)
{
  // Segment mapping from circuit (Q outputs):
  // Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
  //
  // Segment encoding for digits 0..9 (common cathode typically):
  // 0: A B C D E F (G off)
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
  // We place bits into valueQ0toQ7 where bit0=Q0(A), bit1=Q1(B), ..., bit6=Q6(G)
  uint8_t pattern = 0;

  switch (digit)
  {
    case 0: pattern = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5); break; // A B C D E F
    case 1: pattern = (1<<1)|(1<<2); break;                                 // B C
    case 2: pattern = (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6); break;           // A B D E G
    case 3: pattern = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6); break;           // A B C D G
    case 4: pattern = (1<<1)|(1<<2)|(1<<5)|(1<<6); break;                 // B C F G
    case 5: pattern = (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6); break;           // A C D F G
    case 6: pattern = (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6); break;   // A C D E F G
    case 7: pattern = (1<<0)|(1<<1)|(1<<2); break;                         // A B C
    case 8: pattern = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6); break; // A..G
    case 9: pattern = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6); break;   // A B C D F G
    default: pattern = 0; break;
  }

  // Shift out 8 bits; Q7 is unused -> keep 0.
  shiftWrite(pattern);
}

void setup()
{
  // Shift register pins
  pinMode(PIN_DS, OUTPUT);
  pinMode(PIN_STCP, OUTPUT);
  pinMode(PIN_SHCP, OUTPUT);

  digitalWrite(PIN_DS, LOW);
  digitalWrite(PIN_STCP, LOW);
  digitalWrite(PIN_SHCP, LOW);

  // Bar graph pins
  for (uint8_t i = 0; i < 10; ++i)
  {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  // Initialization requirement:
  //  - sevseg1 shows "0"
  //  - bargraph1 has no LEDs lit
  setSevenSegDigit(0);
  setBarGraph(0);
}

void loop()
{
  static uint8_t current = 0;
  static unsigned long lastUpdate = 0;

  unsigned long now = millis();
  if (now - lastUpdate >= 2000UL)
  {
    lastUpdate = now;

    // Increment every 2 seconds, reset after reaching 9 back to 0
    current++;
    if (current > 9) current = 0;

    // Update immediately to reflect the new number
    setSevenSegDigit(current);
    setBarGraph(current);
  }
}
