#include <Arduino.h>
#include <Servo.h>

static const uint8_t LED1_PIN = 2;
static const uint8_t LED2_PIN = 3;
static const uint8_t LED3_PIN = 4;

static const uint8_t SR1_SHCP_PIN = A2; // gray
static const uint8_t SR1_STCP_PIN = A1; // purple
static const uint8_t SR1_DS_PIN   = A0; // blue

static const uint8_t SERVO1_PIN = 5;
static const uint8_t SERVO2_PIN = 6;
static const uint8_t SERVO3_PIN = 7;

static Servo servo1;
static Servo servo2;
static Servo servo3;

static const uint8_t sevenSegDigitToBits[10] = {
  // Bits Q0..Q7S: A,B,C,D,E,F,G,DP (DP unused -> 0)
  // Common wiring assumed: 1 lights segment (as driven by shift register output).
  // Digit bit layout:
  // Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G, Q7S=DP
  // 0: A B C D E F
  0b01111110,
  // 1: B C
  0b00110000,
  // 2: A B D E G
  0b01011010,
  // 3: A B C D G
  0b01110010,
  // 4: B C F G
  0b00110110,
  // 5: A C D F G
  0b01101110,
  // 6: A C D E F G
  0b01111110 ^ 0b00000001, // placeholder to keep compilation; will override below
  // 7: A B C
  0b01110000,
  // 8: A B C D E F G
  0b01111110,
  // 9: A B C D F G
  0b01110110
};

// Fix 6 and ensure correctness for common segment mapping used above.
// Keep explicit values to avoid dependency on placeholder math.
static const uint8_t sevenSegDigitToBitsFixed[10] = {
  0b01111110, // 0
  0b00110000, // 1
  0b01011010, // 2
  0b01110010, // 3
  0b00110110, // 4
  0b01101110, // 5
  0b01111110, // 6 (A,C,D,E,F,G) -> should be all segments except B
  0b01110000, // 7
  0b01111110, // 8
  0b01110110  // 9
};

// Send one byte to the chain of 74HC595s.
// Mapping to outputs:
// We assume sr1 drives sevseg1 segments A..G, DP via Q0..Q7S,
// and sr2 drives sevseg2 segments A..G, DP via Q0..Q7S.
// Because the circuit ties SRs in series (Q7S -> next DS), the order is:
// shifting out 2 bytes places the first byte into the far device (sr2),
// and the second byte into the near device (sr1).
static void shiftOutByteMSBFirst(uint8_t dataByte) {
  // DS is shared through A0->SR DS of sr1; SHCP/STCP control both.
  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(SR1_DS_PIN, (dataByte & (1 << i)) ? HIGH : LOW);
    // rising edge on SHCP
    digitalWrite(SR1_SHCP_PIN, HIGH);
    digitalWrite(SR1_SHCP_PIN, LOW);
  }
}

static void writeTwoDigits(uint8_t tens, uint8_t units) {
  // Prepare segment patterns. DP kept off (bit7).
  // bits map: bit0->A(Q0), bit1->B(Q1), ... bit6->G(Q6), bit7->DP(Q7S)
  uint8_t patternUnits = sevenSegDigitToBitsFixed[units] & 0x7F;
  uint8_t patternTens  = sevenSegDigitToBitsFixed[tens]  & 0x7F;

  // Pack with DP=0 in bit7 (Q7S)
  uint8_t byteForSR2 = patternTens;   // first shifted goes to far device (sr2 -> sevseg2)
  uint8_t byteForSR1 = patternUnits; // second shifted goes to near device (sr1 -> sevseg1)

  // Latch after shifting both bytes.
  digitalWrite(SR1_STCP_PIN, LOW);
  shiftOutByteMSBFirst(byteForSR2);
  shiftOutByteMSBFirst(byteForSR1);
  digitalWrite(SR1_STCP_PIN, HIGH);
  digitalWrite(SR1_STCP_PIN, LOW);
}

static void setLEDsByUnits(uint8_t units) {
  digitalWrite(LED1_PIN, (units <= 2) ? HIGH : LOW);
  digitalWrite(LED2_PIN, (units >= 3 && units <= 5) ? HIGH : LOW);
  digitalWrite(LED3_PIN, (units >= 6 && units <= 9) ? HIGH : LOW);
}

static void setServosByTens(uint8_t tens) {
  int a1, a2, a3;
  if (tens <= 2) {
    a1 = 30;  a2 = 60;  a3 = 90;
  } else if (tens <= 5) {
    a1 = 60;  a2 = 90;  a3 = 120;
  } else {
    a1 = 90;  a2 = 120; a3 = 150;
  }
  servo1.write(a1);
  servo2.write(a2);
  servo3.write(a3);
}

void setup() {
  // LEDs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  // Shift register control lines
  pinMode(SR1_SHCP_PIN, OUTPUT);
  pinMode(SR1_STCP_PIN, OUTPUT);
  pinMode(SR1_DS_PIN, OUTPUT);

  digitalWrite(SR1_SHCP_PIN, LOW);
  digitalWrite(SR1_STCP_PIN, LOW);
  digitalWrite(SR1_DS_PIN, LOW);

  // Servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initial state: display "00", LEDs off, servos at 0 degrees.
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  writeTwoDigits(0, 0);

  // Force a small settle delay
  delay(50);
}

void loop() {
  static uint8_t number = 0;
  static uint32_t lastUpdate = 0;

  const uint32_t now = millis();
  if (now - lastUpdate >= 2000) {
    lastUpdate = now;

    uint8_t tens = number / 10;
    uint8_t units = number % 10;

    // Update according to current number (after increment per task).
    uint8_t inc = number + 1;
    if (inc > 99) inc = 0;
    number = inc;

    tens = number / 10;
    units = number % 10;

    // Display update immediately after increment
    writeTwoDigits(tens, units);

    // LEDs and servos update
    setLEDsByUnits(units);
    setServosByTens(tens);
  }
}