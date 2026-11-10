#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t CURRENT_PIN = A0;
static const uint8_t TEMPERATURE_PIN = A1;
static const uint8_t VIBRATION_PIN = 2;
static const uint8_t FAN_PIN = 5;
static const uint8_t BUZZER_PIN = 6;
static const uint8_t LED_PIN = 13;

TM1637Display display(4, 3);
static unsigned long pageStarted = 0;
static uint8_t page = 0;
static bool previousMechanical = false;
static bool alarmActive = false;
static int lastDisplayed = -32768;

static uint8_t classifyMotor(int currentRaw, int temperatureRaw, bool mechanical) {
  if (mechanical) return 3;
  if ((long)temperatureRaw * 500L > 60L * 1023L) return 2;
  if ((long)currentRaw * 10L > 7L * 1023L) return 1;
  return 0;
}

void setup() {
  pinMode(CURRENT_PIN, INPUT);
  pinMode(TEMPERATURE_PIN, INPUT);
  pinMode(VIBRATION_PIN, INPUT_PULLUP);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(FAN_PIN, 0);
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
  Serial.begin(9600);
  display.setBrightness(7);
  display.clear();
  pageStarted = millis();
}

void loop() {
  const int currentRaw = analogRead(CURRENT_PIN);
  const int temperatureRaw = analogRead(TEMPERATURE_PIN);
  const bool mechanical = digitalRead(VIBRATION_PIN) == LOW;
  const bool hot = (long)temperatureRaw * 500L > 60L * 1023L;
  const uint8_t status = classifyMotor(currentRaw, temperatureRaw, mechanical);

  analogWrite(FAN_PIN, hot ? 255 : 0);
  if (mechanical != alarmActive) {
    alarmActive = mechanical;
    digitalWrite(LED_PIN, mechanical ? HIGH : LOW);
    if (mechanical) tone(BUZZER_PIN, 1000);
    else noTone(BUZZER_PIN);
  }
  if (mechanical && !previousMechanical) {
    Serial.println("fault code 3");
  }
  previousMechanical = mechanical;

  const unsigned long now = millis();
  while (now - pageStarted >= 2000UL) {
    pageStarted += 2000UL;
    page = (page + 1) % 3;
  }
  int value;
  if (page == 0) value = (int)((long)currentRaw * 10L / 1023L);
  else if (page == 1) value = (int)((long)temperatureRaw * 500L / 1023L);
  else value = status;

  if (value != lastDisplayed) {
    display.showNumberDec(value, false);
    lastDisplayed = value;
  }
  delay(5);
}
