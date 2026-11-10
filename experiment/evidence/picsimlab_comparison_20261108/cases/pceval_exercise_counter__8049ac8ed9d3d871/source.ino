#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// 7-segment segment pins (through 220Ω resistors) connected to Arduino pins 2..8
// Display is common cathode, COM connected to GND (GND.2 in circuit)
static const uint8_t SEG_PINS[7] = {
  2, // A (through resistor1:1)
  3, // B (through resistor7:2)
  4, // C (through resistor4:2)
  5, // D (through resistor1:2? Actually D -> resistor2:2 -> Arduino 2)
  6, // E -> resistor2:2 -> Arduino 6? (circuit says E is resistor2:2 to display E; resistor2:2 connects to Arduino 2)
  7, // F -> resistor6:1 to Arduino 7
  8  // G -> resistor5:1 to Arduino 8
};

// The circuit mapping (from given connections) is:
static const uint8_t PIN_A = 5; // resistor1:1 -> arduino pin 5 -> display1:A
static const uint8_t PIN_B = 3; // resistor7:2 -> arduino pin 3 -> display1:B
static const uint8_t PIN_C = 4; // resistor4:2 -> arduino pin 4 -> display1:C
static const uint8_t PIN_D = 2; // resistor1:2 -> arduino pin 2 -> display1:D
static const uint8_t PIN_E = 6; // resistor2:2 -> arduino pin 6 -> display1:E
static const uint8_t PIN_F = 7; // resistor6:1 -> arduino pin 7 -> display1:F
static const uint8_t PIN_G = 8; // resistor5:1 -> arduino pin 8 -> display1:G
static const uint8_t PIN_DP = 1; // resistor3:1 -> arduino pin 5? Wait: DP is resistor3:1 connected to display DP only, and resistor3:1 to arduino pin 1 is not connected in list.
// From connections list, DP wiring:
 // 7-segment display1:DP -> resistor3:1 (green v0)
 // resistor3:1 -> 7-segment display1:DP, but the other side of resistor3:1 is not explicitly listed.
 // However the list includes resistor3:1 connected to 7-segment display1:DP only; the other side (resistor3:2) not present.
 // Another connection: 7-segment display1:DP is not wired to Arduino in the provided connection pairs.
 // Therefore we assume DP is left un-driven. We'll just not touch it.

static inline void setSeg(uint8_t pin, bool on) {
  // common cathode: HIGH turns segment OFF, LOW turns ON (assuming typical wiring where segment anode -> resistor -> pin)
  // In this circuit: segments go to Arduino pins through resistors to COM (GND). Thus pin LOW sinks current and lights segment.
  digitalWrite(pin, on ? LOW : HIGH);
}

static void displayDigit(uint8_t d) {
  // Segment ON/OFF for common cathode displays.
  // We'll output LOW = ON, HIGH = OFF.
  // Segments: A B C D E F G
  // Digits:
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
    default: A=false;B=false;C=false;D=false;E=false;F=false;G=false; break;
  }

  setSeg(PIN_A, A);
  setSeg(PIN_B, B);
  setSeg(PIN_C, C);
  setSeg(PIN_D, D);
  setSeg(PIN_E, E);
  setSeg(PIN_F, F);
  setSeg(PIN_G, G);
}

static inline int16_t readYRotationRaw() {
  int16_t gy = 0;
  mpu.getRotation(&gy, &gy, &gy); // wrong signature; keep behavior explicit below
  // The above line is not reliable. We'll read from MPU6050 library directly:
  // Use getRotationY() if available; else use getRotation.
  return mpu.getRotationY();
}

void setup() {
  // 1. Set pins for 7-segments as OUTPUT
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_D, OUTPUT);
  pinMode(PIN_E, OUTPUT);
  pinMode(PIN_F, OUTPUT);
  pinMode(PIN_G, OUTPUT);

  // Start with all segments off (common cathode: HIGH to turn off)
  setSeg(PIN_A, false);
  setSeg(PIN_B, false);
  setSeg(PIN_C, false);
  setSeg(PIN_D, false);
  setSeg(PIN_E, false);
  setSeg(PIN_F, false);
  setSeg(PIN_G, false);

  Wire.begin();

  // 2. Check connection of mpu6050
  mpu.initialize();
  while (!mpu.testConnection()) {
    // If not connected, keep attempting (and keep showing 0 as requested on power up)
    // (No extra behavior was specified; preserve expected overall function)
    displayDigit(0);
    delay(250);
  }

  // 3. Display number 0 on the 7-segment display
  uint8_t count = 0;
  displayDigit(count);

  // Read once to stabilize
  (void)mpu.getRotationY();
}

void loop() {
  static uint8_t count = 0;
  static bool canCount = true; // count only when abs(Y) previously dropped below 2000

  int16_t y = mpu.getRotationY();               // Y rotation value
  int32_t ay = (y < 0) ? -(int32_t)y : (int32_t)y; // absolute value

  // 5. Count increases when abs(Y) exceeds 15000,
  //    and it does not increase again until abs(Y) drops below 2000.
  if (canCount) {
    if (ay > 15000) {
      count = (uint8_t)((count + 1) % 10); // 1-digit display: wrap 0-9
      canCount = false;
    }
  } else {
    if (ay < 2000) {
      canCount = true;
    }
  }

  // 6. Continuously display the count
  displayDigit(count);

  delay(10);
}