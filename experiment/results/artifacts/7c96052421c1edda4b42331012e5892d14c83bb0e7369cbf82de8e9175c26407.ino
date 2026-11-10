#include <Arduino.h>
#include <Servo.h>

static const uint8_t SR_PIN_SHCP = A2; // SHCP -> gray
static const uint8_t SR_PIN_STCP = A1; // STCP -> purple
static const uint8_t SR_PIN_DS   = A0;  // DS   -> blue
static const uint8_t SR_PIN_OE   = 255; // tied to GND in circuit (sr1:OE -> black)

static const uint8_t SERVO_PIN = 5; // mega:5 -> servo1:PWM

Servo servo1;

// 74HC595 (sr1) outputs are connected to sevseg1 segments:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// sevseg1 type in Wokwi defaults to common cathode: segment ON = HIGH.
// But the circuit uses sr1 with MR tied to VCC? (MR connected to sr1:VCC)
// We'll explicitly shift desired segment bits.

static const uint8_t COLOR_STEP_DELAY_MS = 2000;

// Colors sequence: Red, Green, Blue, Yellow, Cyan, Magenta, White
// rgb1 is common cathode per circuit: COM -> GND, pins R/G/B are active HIGH.
// rgb1:COM to GND means turning on by setting R/G/B HIGH.
struct RGB {
  bool r, g, b;
};

static const RGB colors[] = {
  {true,  false, false}, // Red
  {false, true,  false}, // Green
  {false, false, true }, // Blue
  {true,  true,  false}, // Yellow
  {false, true,  true }, // Cyan
  {true,  false, true }, // Magenta
  {true,  true,  true }  // White
};

static const uint8_t COLOR_COUNT = sizeof(colors) / sizeof(colors[0]);

// Segment bit mapping to 74HC595 outputs Q0..Q6:
// bit0 -> Q0 -> A
// bit1 -> Q1 -> B
// bit2 -> Q2 -> C
// bit3 -> Q3 -> D
// bit4 -> Q4 -> E
// bit5 -> Q5 -> F
// bit6 -> Q6 -> G
// bit7 unused (Q7 not connected to anything)
static uint8_t seg7DigitsCommonCathode[10] = {
  // A B C D E F G
  // 0: A,B,C,D,E,F on, G off
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),
  // 1: B,C
  (1<<1)|(1<<2),
  // 2: A,B,D,E,G
  (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),
  // 3: A,B,C,D,G
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),
  // 4: B,C,F,G
  (1<<1)|(1<<2)|(1<<5)|(1<<6),
  // 5: A,C,D,F,G
  (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),
  // 6: A,C,D,E,F,G
  (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
  // 7: A,B,C
  (1<<0)|(1<<1)|(1<<2),
  // 8: A,B,C,D,E,F,G
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
  // 9: A,B,C,D,F,G
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)
};

// RGB pins: mega 2->R, 3->G, 4->B
static const uint8_t RGB_PIN_R = 2;
static const uint8_t RGB_PIN_G = 3;
static const uint8_t RGB_PIN_B = 4;

static inline void srWriteByte(uint8_t value)
{
  // Shift out MSB->LSB is typical; but since we want Q0..Q7 order,
  // we must match the bit order used when shifting.
  // We'll shift MSB first so that the first bit shifted ends up in Q7.
  // Therefore, to place bit0 into Q0, we must shift LSB first.
  // We'll shift LSB first for direct bit mapping: DS sends value bit0 first -> Q0.
  digitalWrite(SR_PIN_SHCP, LOW);
  digitalWrite(SR_PIN_STCP, LOW);

  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR_PIN_DS, (value >> i) & 0x01);
    digitalWrite(SR_PIN_SHCP, HIGH);
    digitalWrite(SR_PIN_SHCP, LOW);
  }

  // Latch
  digitalWrite(SR_PIN_STCP, HIGH);
  digitalWrite(SR_PIN_STCP, LOW);
}

static inline void setSevenSegDigit(uint8_t digit)
{
  if (digit > 9) digit = 0;
  // Output bits Q0..Q6 according to seg7DigitsCommonCathode[digit]
  uint8_t pattern = seg7DigitsCommonCathode[digit] & 0x7F;
  srWriteByte(pattern);
}

static inline void setRGB(const RGB& c)
{
  digitalWrite(RGB_PIN_R, c.r ? HIGH : LOW);
  digitalWrite(RGB_PIN_G, c.g ? HIGH : LOW);
  digitalWrite(RGB_PIN_B, c.b ? HIGH : LOW);
}

void setup()
{
  pinMode(SR_PIN_SHCP, OUTPUT);
  pinMode(SR_PIN_STCP, OUTPUT);
  pinMode(SR_PIN_DS, OUTPUT);

  pinMode(RGB_PIN_R, OUTPUT);
  pinMode(RGB_PIN_G, OUTPUT);
  pinMode(RGB_PIN_B, OUTPUT);

  digitalWrite(SR_PIN_SHCP, LOW);
  digitalWrite(SR_PIN_STCP, LOW);
  digitalWrite(SR_PIN_DS, LOW);

  // Initialization states required:
  // RGB LED off
  setRGB({false, false, false});
  // 7-seg display shows "9"
  setSevenSegDigit(9);
  // servo at 0 degrees
  servo1.attach(SERVO_PIN);
  servo1.write(0);

  // small settle time (doesn't violate synchronous behavior; only for startup)
  delay(20);
}

void loop()
{
  // Colors should cycle through 7 colors, each displayed for 2 seconds.
  // Simultaneously, countdown from 9 to 0, each displayed for 2 seconds.
  // That means 10 steps; colors wrap or continue as sequence requires color order.
  // We'll keep RGB color index advancing each step; if more than 7 steps, continue cyclically.
  // "After reaching 0, the system should reset and repeat the sequence."
  // Therefore each full countdown run corresponds to 10 synchronous updates.
  for (int countdown = 9; countdown >= 0; --countdown) {
    // Synchronous update: set RGB, 7-seg, and servo for the current countdown.
    uint8_t colorIndex = (uint8_t)(9 - countdown) % COLOR_COUNT; // step 0 corresponds to countdown=9
    setRGB(colors[colorIndex]);
    setSevenSegDigit((uint8_t)countdown);

    // Servo angle corresponding to countdown value:
    // 0 degrees for 9, 20 degrees for 8, ..., 180 degrees for 0
    // Linear mapping: angle = (9 - countdown) * 20
    int angle = (9 - countdown) * 20;
    servo1.write(angle);

    delay(COLOR_STEP_DELAY_MS);
  }

  // After reaching 0, loop repeats (system reset and repeat)
}