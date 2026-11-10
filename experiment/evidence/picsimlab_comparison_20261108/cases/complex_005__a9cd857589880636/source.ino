#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t LIGHT_SENSOR_PIN = A0;
constexpr uint8_t LED_PWM_PIN      = 9;
constexpr uint8_t BUTTON_PIN       = 2;
constexpr uint8_t MODE_LED_PIN     = 13;
constexpr uint8_t DISPLAY_CLK_PIN  = 4;
constexpr uint8_t DISPLAY_DIO_PIN  = 5;

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long SAMPLE_MS   = 20;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

bool displayFrozen = false;
bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;

unsigned long lastButtonChangeMs = 0;
unsigned long lastSampleMs = 0;

uint8_t currentLightValue = 0;
uint8_t displayedValue = 0;

uint8_t readMappedLight()
{
  const int raw = analogRead(LIGHT_SENSOR_PIN);
  return static_cast<uint8_t>(map(raw, 0, 1023, 0, 255));
}

void updateDisplay(uint8_t value)
{
  displayedValue = value;
  display.showNumberDec(value, false, 4, 0);
}

void setup()
{
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(LED_PWM_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // 按钮连接 D2 与 GND。
  pinMode(MODE_LED_PIN, OUTPUT);

  digitalWrite(MODE_LED_PIN, LOW);   // D13 亮起表示冻结显示模式。
  display.setBrightness(5, true);

  currentLightValue = readMappedLight();
  analogWrite(LED_PWM_PIN, currentLightValue);
  updateDisplay(currentLightValue);

  lastSampleMs = millis();
  lastButtonChangeMs = lastSampleMs;
}

void loop()
{
  const unsigned long now = millis();

  // LED 始终随光照变化；冻结模式仅保持数码管显示值。
  if (now - lastSampleMs >= SAMPLE_MS) {
    lastSampleMs = now;
    currentLightValue = readMappedLight();
    analogWrite(LED_PWM_PIN, currentLightValue);
  }

  const bool buttonReading = digitalRead(BUTTON_PIN);

  if (buttonReading != lastButtonReading) {
    lastButtonReading = buttonReading;
    lastButtonChangeMs = now;
  }

  if (now - lastButtonChangeMs >= DEBOUNCE_MS &&
      buttonReading != stableButtonState) {
    stableButtonState = buttonReading;

    // 只在消抖后的按下沿切换，长按不会重复切换。
    if (stableButtonState == LOW) {
      displayFrozen = !displayFrozen;
      digitalWrite(MODE_LED_PIN, displayFrozen ? HIGH : LOW);

      if (!displayFrozen) {
        updateDisplay(currentLightValue);
      }
    }
  }

  if (!displayFrozen && currentLightValue != displayedValue) {
    updateDisplay(currentLightValue);
  }
}