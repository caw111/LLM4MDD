// One Digit 7-segment Display (Common Anode)
// Segments A..G and DP wired through 330 ohm resistors to Arduino D2..D9
// Common anode COM.1 connected to Arduino 5V.

const int SEG_A  = 2;  // A
const int SEG_B  = 3;  // B
const int SEG_C  = 4;  // C
const int SEG_D  = 5;  // D
const int SEG_E  = 6;  // E
const int SEG_F  = 7;  // F
const int SEG_G  = 8;  // G
const int SEG_DP = 9;  // DP (not specified to light)

void setup() {
  // 1. Set 2~9 pin as output.
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  pinMode(SEG_DP, OUTPUT);

  // 2. For common anode display:
  //    Segment ON requires sinking current through the segment resistor -> LOW.
  //    Segment OFF -> HIGH.
  //
  // Number 5 uses segments: A, F, G, C, D (and not B, E, DP).
  digitalWrite(SEG_A,  LOW); // A ON
  digitalWrite(SEG_B,  HIGH); // B OFF
  digitalWrite(SEG_C,  LOW); // C ON
  digitalWrite(SEG_D,  LOW); // D ON
  digitalWrite(SEG_E,  HIGH); // E OFF
  digitalWrite(SEG_F,  LOW); // F ON
  digitalWrite(SEG_G,  LOW); // G ON
  digitalWrite(SEG_DP, HIGH); // DP OFF
}

void loop() {
  // Keep showing the number 5 (no further behavior specified).
}