#include <Arduino.h>

const uint8_t LDR_PIN = A0;      // 光敏电阻
const uint8_t LED_PIN = 13;      // LED
const uint8_t BUZZER_PIN = 6;    // 蜂鸣器（D6）
const uint8_t TRIG_PIN = 8;      // HC-SR04 Trig
const uint8_t ECHO_PIN = 9;      // HC-SR04 Echo

// 需求：只有在“环境较暗”且“检测到20cm内物体”时触发报警
// 环境较暗阈值：使用LDR原始值进行判定（在未给定电路标定值时，使用常见经验阈值）
const int DARK_THRESHOLD = 400; // 可按需要微调（单位：A0原始ADC值0~1023）
const int ALARM_DISTANCE_CM = 20;

// 报警行为：LED闪烁、蜂鸣器间歇鸣叫，并通过串口输出 alarm
// 定义LED闪烁与蜂鸣器间歇的计时（避免使用delay影响测距与串口响应）
const unsigned long LED_TOGGLE_INTERVAL_MS = 250; // LED闪烁周期
const unsigned long BUZZ_ON_INTERVAL_MS = 200;     // 蜂鸣器鸣叫保持时间
const unsigned long BUZZ_OFF_INTERVAL_MS = 300;    // 蜂鸣器静默时间
const unsigned long ALARM_MIN_REPEAT_INTERVAL_MS = 5000; // 串口alarm输出最小重复间隔（避免刷屏）

// HC-SR04测距参数
const unsigned long ULTRASONIC_TIMEOUT_US = 25000UL; // 约4m距离超时
const float SOUND_SPEED_CM_PER_US = 0.0343f / 2.0f;   // 距离(cm)=time(us)*0.0343/2

// 运行状态
bool alarmActive = false;
unsigned long ledLastToggleMs = 0;
bool ledState = false;

unsigned long alarmLastOnMs = 0;   // 蜂鸣器“打开”开始时间
bool buzzerOn = false;

unsigned long lastAlarmSerialMs = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(9600);
}

static long measureDistanceCm() {
  // 触发脉冲
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // 读取回波脉冲宽度（us）
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, ULTRASONIC_TIMEOUT_US);
  if (duration == 0) {
    // 超时未收到回波：返回一个大于阈值的距离
    return 300;
  }

  float cm = duration * SOUND_SPEED_CM_PER_US;
  return (long)(cm + 0.5f);
}

void loop() {
  // 读取环境光强
  int ldrValue = analogRead(LDR_PIN);

  // 测距
  long distanceCm = measureDistanceCm();

  bool darkEnough = (ldrValue < DARK_THRESHOLD);
  bool objectNear = (distanceCm <= ALARM_DISTANCE_CM);

  bool newAlarmActive = (darkEnough && objectNear);

  if (newAlarmActive) {
    // 进入报警/保持报警
    if (!alarmActive) {
      // 刚触发：初始化闪烁与蜂鸣器时序
      ledLastToggleMs = millis();
      ledState = false;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);

      buzzerOn = false;
      alarmLastOnMs = millis();
      digitalWrite(BUZZER_PIN, LOW);

      // 串口输出 alarm
      Serial.println("alarm");
      lastAlarmSerialMs = millis();
    } else {
      // 报警中：根据时间维持输出
      unsigned long now = millis();

      // LED闪烁
      if (now - ledLastToggleMs >= LED_TOGGLE_INTERVAL_MS) {
        ledLastToggleMs = now;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      }

      // 蜂鸣器间歇鸣叫
      if (!buzzerOn) {
        // 当前是关的，等待到打开
        if (now - alarmLastOnMs >= BUZZ_OFF_INTERVAL_MS) {
          buzzerOn = true;
          alarmLastOnMs = now;
          // 用数字输出持续鸣叫（保持D6为高）
          digitalWrite(BUZZER_PIN, HIGH);
        }
      } else {
        // 当前是开着的，等待到关闭
        if (now - alarmLastOnMs >= BUZZ_ON_INTERVAL_MS) {
          buzzerOn = false;
          alarmLastOnMs = now;
          digitalWrite(BUZZER_PIN, LOW);
        }
      }

      // 维持串口输出 alarm（仅在长时间持续报警时重复）
      if (now - lastAlarmSerialMs >= ALARM_MIN_REPEAT_INTERVAL_MS) {
        Serial.println("alarm");
        lastAlarmSerialMs = now;
      }
    }

    alarmActive = true;
  } else {
    // 不报警：关闭LED与蜂鸣器，停止闪烁
    if (alarmActive) {
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      alarmActive = false;
    }

    // 保持不报警时输出为关闭态
    // （不需要额外delay，循环继续测距与光敏）
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // 小延时以降低频繁测距占用；不改变核心行为
  delay(20);
}