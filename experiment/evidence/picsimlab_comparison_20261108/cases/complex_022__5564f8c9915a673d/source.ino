#include <Arduino.h>

// 接线：D2、D3 按钮分别接 GND，使用内部上拉。
// PWM LED：D9；板载状态 LED：D13。
// TM1637：CLK=D4，DIO=D5，电源接 5V/GND。
constexpr uint8_t BUTTON_MODE_PIN = 2;
constexpr uint8_t BUTTON_PARAM_PIN = 3;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;
constexpr uint8_t PWM_LED_PIN = 9;
constexpr uint8_t STATUS_LED_PIN = 13;

constexpr uint32_t DEBOUNCE_MS = 30;
constexpr uint32_t MODE_DISPLAY_MS = 1000;

enum Mode : uint8_t {
  MODE_OFF = 0,       // 显示模式编号 1
  MODE_ON = 1,        // 显示模式编号 2
  MODE_BREATH = 2     // 显示模式编号 3
};

const uint8_t BRIGHTNESS_LEVELS[] = {
  32, 64, 96, 128, 160, 192, 224, 255
};

// 呼吸参数为一个完整“渐亮 + 渐灭”周期的毫秒数。
// D3 依次加快，达到最快后返回最慢。
const uint16_t BREATH_PERIODS_MS[] = {
  4000, 3000, 2000, 1200, 800
};

constexpr uint8_t BRIGHTNESS_COUNT =
    sizeof(BRIGHTNESS_LEVELS) / sizeof(BRIGHTNESS_LEVELS[0]);
constexpr uint8_t BREATH_PERIOD_COUNT =
    sizeof(BREATH_PERIODS_MS) / sizeof(BREATH_PERIODS_MS[0]);

struct Button {
  uint8_t pin;
  bool lastRaw;
  bool stable;
  uint32_t changedAt;

  void begin(uint8_t inputPin) {
    pin = inputPin;
    pinMode(pin, INPUT_PULLUP);
    lastRaw = digitalRead(pin);
    stable = HIGH;
    changedAt = millis();
  }

  // 每次稳定按下仅返回一次 true；长按不会连续触发。
  bool pressed(uint32_t now) {
    const bool raw = digitalRead(pin);

    if (raw != lastRaw) {
      lastRaw = raw;
      changedAt = now;
    }

    if (raw != stable && uint32_t(now - changedAt) >= DEBOUNCE_MS) {
      stable = raw;
      return stable == LOW;
    }
    return false;
  }
};

// 无需外部库的 TM1637 驱动，信号线采用开漏式驱动。
class TM1637 {
 public:
  void begin() {
    release(DISPLAY_CLK_PIN);
    release(DISPLAY_DIO_PIN);
  }

  void showNumber(uint16_t number) {
    static const uint8_t digits[] = {
      0x3F, 0x06, 0x5B, 0x4F, 0x66,
      0x6D, 0x7D, 0x07, 0x7F, 0x6F
    };

    uint8_t segments[4] = {0, 0, 0, 0};
    for (int8_t i = 3; i >= 0; --i) {
      segments[i] = digits[number % 10];
      number /= 10;
      if (number == 0) {
        break;
      }
    }

    start();
    writeByte(0x40);  // 自动递增地址
    stop();

    start();
    writeByte(0xC0);
    for (uint8_t i = 0; i < 4; ++i) {
      writeByte(segments[i]);
    }
    stop();

    start();
    writeByte(0x8B);  // 开启显示，亮度等级 3/7
    stop();
  }

 private:
  static void waitBus() {
    delayMicroseconds(5);
  }

  static void release(uint8_t pin) {
    pinMode(pin, INPUT_PULLUP);
  }

  static void pullLow(uint8_t pin) {
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
  }

  static void start() {
    release(DISPLAY_DIO_PIN);
    release(DISPLAY_CLK_PIN);
    waitBus();
    pullLow(DISPLAY_DIO_PIN);
    waitBus();
    pullLow(DISPLAY_CLK_PIN);
  }

  static void stop() {
    pullLow(DISPLAY_CLK_PIN);
    pullLow(DISPLAY_DIO_PIN);
    waitBus();
    release(DISPLAY_CLK_PIN);
    waitBus();
    release(DISPLAY_DIO_PIN);
    waitBus();
  }

  static void writeByte(uint8_t value) {
    for (uint8_t i = 0; i < 8; ++i) {
      pullLow(DISPLAY_CLK_PIN);
      if (value & 0x01) {
        release(DISPLAY_DIO_PIN);
      } else {
        pullLow(DISPLAY_DIO_PIN);
      }
      waitBus();
      release(DISPLAY_CLK_PIN);
      waitBus();
      pullLow(DISPLAY_CLK_PIN);
      value >>= 1;
    }

    // 第九个时钟释放 DIO，供 TM1637 应答。
    release(DISPLAY_DIO_PIN);
    waitBus();
    release(DISPLAY_CLK_PIN);
    waitBus();
    pullLow(DISPLAY_CLK_PIN);
    waitBus();
  }
};

Button modeButton;
Button parameterButton;
TM1637 display;

Mode currentMode = MODE_OFF;
uint8_t brightnessIndex = 3;  // 默认常亮亮度 128
uint8_t breathPeriodIndex = 2;  // 默认呼吸周期 2000 ms

uint32_t modeChangedAt = 0;
uint32_t breathStartedAt = 0;
bool showParameterImmediately = false;
uint16_t lastDisplayedNumber = 0xFFFF;

void changeMode(uint32_t now) {
  currentMode = static_cast<Mode>((static_cast<uint8_t>(currentMode) + 1) % 3);
  modeChangedAt = now;
  showParameterImmediately = false;

  if (currentMode == MODE_BREATH) {
    breathStartedAt = now;
  }
}

void adjustParameter(uint32_t now) {
  switch (currentMode) {
    case MODE_OFF:
      // 常灭没有可调参数；D3 保持常灭并显示模式编号 1。
      break;

    case MODE_ON:
      brightnessIndex = (brightnessIndex + 1) % BRIGHTNESS_COUNT;
      showParameterImmediately = true;
      break;

    case MODE_BREATH: {
      // 保留当前呼吸相位，调整速度时避免亮度突变。
      const uint16_t oldPeriod = BREATH_PERIODS_MS[breathPeriodIndex];
      const uint32_t oldPosition = uint32_t(now - breathStartedAt) % oldPeriod;

      breathPeriodIndex = (breathPeriodIndex + 1) % BREATH_PERIOD_COUNT;
      const uint16_t newPeriod = BREATH_PERIODS_MS[breathPeriodIndex];
      const uint32_t newPosition = oldPosition * newPeriod / oldPeriod;
      breathStartedAt = now - newPosition;
      showParameterImmediately = true;
      break;
    }
  }
}

void updateLed(uint32_t now) {
  uint8_t pwm = 0;

  switch (currentMode) {
    case MODE_OFF:
      pwm = 0;
      break;

    case MODE_ON:
      pwm = BRIGHTNESS_LEVELS[brightnessIndex];
      break;

    case MODE_BREATH: {
      const uint16_t period = BREATH_PERIODS_MS[breathPeriodIndex];
      const uint32_t elapsed = uint32_t(now - breathStartedAt);

      // 周期归一化，保持 millis() 回绕前后呼吸连续。
      const uint32_t completedTime = (elapsed / period) * period;
      breathStartedAt += completedTime;
      const uint16_t position = elapsed % period;

      const uint16_t ramp = uint32_t(position) * 510UL / period;
      pwm = (ramp <= 255) ? ramp : (510 - ramp);
      break;
    }
  }

  analogWrite(PWM_LED_PIN, pwm);
  digitalWrite(STATUS_LED_PIN, currentMode == MODE_OFF ? LOW : HIGH);
}

void updateDisplay(uint32_t now) {
  uint16_t number = static_cast<uint8_t>(currentMode) + 1;

  // 切换模式后先显示模式编号 1 秒，随后显示该模式参数。
  // 按 D3 调参则立即显示参数；常灭始终显示 1。
  const bool showParameter =
      showParameterImmediately ||
      uint32_t(now - modeChangedAt) >= MODE_DISPLAY_MS;

  if (showParameter) {
    if (currentMode == MODE_ON) {
      number = BRIGHTNESS_LEVELS[brightnessIndex];
    } else if (currentMode == MODE_BREATH) {
      number = BREATH_PERIODS_MS[breathPeriodIndex];
    }
  }

  if (number != lastDisplayedNumber) {
    display.showNumber(number);
    lastDisplayedNumber = number;
  }
}

void setup() {
  pinMode(PWM_LED_PIN, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  analogWrite(PWM_LED_PIN, 0);
  digitalWrite(STATUS_LED_PIN, LOW);

  modeButton.begin(BUTTON_MODE_PIN);
  parameterButton.begin(BUTTON_PARAM_PIN);
  display.begin();

  modeChangedAt = millis();
  breathStartedAt = modeChangedAt;
  updateDisplay(modeChangedAt);
}

void loop() {
  const uint32_t now = millis();

  const bool modePressed = modeButton.pressed(now);
  const bool parameterPressed = parameterButton.pressed(now);

  // 同时按下时，先切换模式，再调整新模式的参数。
  if (modePressed) {
    changeMode(now);
  }
  if (parameterPressed) {
    adjustParameter(now);
  }

  updateLed(now);
  updateDisplay(now);
}