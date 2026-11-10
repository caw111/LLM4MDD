const uint8_t LED1_PIN = 12;
const uint8_t LED2_PIN = 13;
const unsigned long INTERVAL_MS = 1000UL;

unsigned long lastToggleTime = 0;
bool led1State = HIGH;
bool led2State = LOW;

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  digitalWrite(LED1_PIN, led1State);
  digitalWrite(LED2_PIN, led2State);

  lastToggleTime = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastToggleTime >= INTERVAL_MS) {
    lastToggleTime += INTERVAL_MS;

    led1State = !led1State;
    led2State = !led2State;

    digitalWrite(LED1_PIN, led1State);
    digitalWrite(LED2_PIN, led2State);
  }
}