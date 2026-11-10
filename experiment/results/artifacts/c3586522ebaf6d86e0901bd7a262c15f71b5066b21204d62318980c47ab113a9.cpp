#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
#ifdef ARDUINO
#include <Arduino.h>
#endif
// BEGIN COMPILED SYSML MODEL
#include <stdint.h>
#include <stdbool.h>
#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int32_t sysml_state_targetPerMinute;
/* Global Variable define of  */
extern int32_t sysml_state_productCountTotal;
/* Global Variable define of  */
extern int32_t sysml_state_productCountActual;
/* Global Variable define of  */
extern int32_t sysml_state_productCountAtLastToggle;
/* Global Variable define of  */
extern int32_t sysml_state_stopCount;
/* Global Variable define of  */
extern int32_t sysml_state_missedInactivityMs;
/* Global Variable define of  */
extern bool sysml_state_alarmLatched;
/* Global Variable define of  */
extern bool sysml_state_prevProductPulse;
/* Global Variable define of  */
extern bool sysml_state_prevStopButtonPressed;
/* Global Variable define of  */
extern int32_t sysml_state_displayMode;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_1, bool io_2, bool io_3, bool* io_4, int32_t* io_5, int32_t* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0, double* serial_0_value_1, int32_t* serial_0_value_2, int32_t* serial_0_value_3, bool* serial_0_value_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_targetPerMinute = 60;
/* Global Variable define of  */
int32_t sysml_state_productCountTotal = 0;
/* Global Variable define of  */
int32_t sysml_state_productCountActual = 0;
/* Global Variable define of  */
int32_t sysml_state_productCountAtLastToggle = 0;
/* Global Variable define of  */
int32_t sysml_state_stopCount = 0;
/* Global Variable define of  */
int32_t sysml_state_missedInactivityMs = 0;
/* Global Variable define of  */
bool sysml_state_alarmLatched = false;
/* Global Variable define of  */
bool sysml_state_prevProductPulse = false;
/* Global Variable define of  */
bool sysml_state_prevStopButtonPressed = false;
/* Global Variable define of  */
int32_t sysml_state_displayMode = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 232 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 255 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_targetPerMinute = 60;
    sysml_state_productCountTotal = 0;
    sysml_state_productCountActual = 0;
    sysml_state_productCountAtLastToggle = 0;
    sysml_state_stopCount = 0;
    sysml_state_missedInactivityMs = 0;
    sysml_state_alarmLatched = false;
    sysml_state_prevProductPulse = false;
    sysml_state_prevStopButtonPressed = false;
    sysml_state_displayMode = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_1, bool io_2, bool io_3, bool* io_4, int32_t* io_5, int32_t* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0, double* serial_0_value_1, int32_t* serial_0_value_2, int32_t* serial_0_value_3, bool* serial_0_value_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevStopButtonPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_displayMode == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_displayMode + 1;
        }
        sysml_state_displayMode = sysml_conditional_1;
    }
    if (io_2) {
        sysml_state_missedInactivityMs = 0;
        if ((!sysml_state_alarmLatched)) {
            sysml_state_alarmLatched = false;
        }
    }
    if ((!io_2)) {
        sysml_state_missedInactivityMs = (sysml_state_missedInactivityMs + io_0);
        sysml_lazy_2 = !sysml_state_alarmLatched;
        if (sysml_lazy_2) {
            sysml_lazy_2 = sysml_state_missedInactivityMs >= 30000;
        }
        if (sysml_lazy_2) {
            sysml_state_alarmLatched = true;
            sysml_state_stopCount = (sysml_state_stopCount + 1);
        }
    }
    sysml_lazy_3 = io_1;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevProductPulse;
    }
    if (sysml_lazy_3) {
        sysml_state_productCountTotal = (sysml_state_productCountTotal + 1);
        sysml_state_productCountActual = (sysml_state_productCountActual + 1);
        sysml_state_missedInactivityMs = 0;
        sysml_state_alarmLatched = false;
    }
    sysml_state_prevProductPulse = io_1;
    sysml_state_prevStopButtonPressed = io_2;
    sysml_lazy_4 = io_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevStopButtonPressed;
    }
    if (sysml_lazy_4) {
        sysml_state_productCountAtLastToggle = sysml_state_productCountTotal;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_alarmLatched;
    int32_t sysml_value_1 = 0;
    if (sysml_state_alarmLatched) {
        sysml_conditional_5 = 2000;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_1 = sysml_conditional_5;
    int32_t sysml_value_2 = 0;
    if (sysml_state_displayMode == 0) {
        sysml_conditional_6 = sysml_state_productCountTotal;
    }
    else {
        if (sysml_state_displayMode == 1) {
            if ((((double)(sysml_state_productCountActual * 100.0)) / sysml_state_targetPerMinute) < ((double)((int32_t)(((double)(sysml_state_productCountActual * 100.0)) / sysml_state_targetPerMinute)))) {
                sysml_conditional_8 = 1;
            }
            else {
                sysml_conditional_8 = 0;
            }
            sysml_conditional_7 = ((int32_t)(((double)(sysml_state_productCountActual * 100.0)) / sysml_state_targetPerMinute)) - sysml_conditional_8;
        }
        else {
            sysml_conditional_7 = sysml_state_stopCount;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_2 = sysml_conditional_6;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_alarmLatched;
    int32_t sysml_value_4 = 0;
    sysml_value_4 = sysml_state_productCountTotal;
    double sysml_value_5 = 0;
    if (sysml_state_targetPerMinute == 0) {
        sysml_conditional_9 = 0.0;
    }
    else {
        sysml_conditional_9 = ((double)(sysml_state_productCountActual * 100.0)) / sysml_state_targetPerMinute;
    }
    sysml_value_5 = sysml_conditional_9;
    int32_t sysml_value_6 = 0;
    sysml_value_6 = sysml_state_stopCount;
    int32_t sysml_value_7 = 0;
    sysml_value_7 = sysml_state_productCountActual;
    bool sysml_value_8 = 0;
    sysml_value_8 = sysml_state_alarmLatched;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
    *serial_0_value_3 = sysml_value_7;
    *serial_0_value_4 = sysml_value_8;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:uno
#include <TM1637Display.h>
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_UNO)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static int32_t hw_value_io_0 = 0;
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_io_4 = false;
static int32_t hw_value_io_5 = 0;
static int32_t hw_tone_io_5 = -1;
static int32_t hw_value_io_6 = 0;
TM1637Display hw_display_io_6(4, 5);
static int32_t hw_display_previous_io_6 = INT32_MIN;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static int32_t hw_value_serial_0_value_2 = 0;
static int32_t hw_value_serial_0_value_3 = 0;
static bool hw_value_serial_0_value_4 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(7, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_5 = (hw_value_io_5 >= 31 && hw_value_io_5 <= 65535) ? hw_value_io_5 : 0;
  if (hw_tone_command_io_5 != hw_tone_io_5) {
    if (hw_tone_command_io_5 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_5);
    hw_tone_io_5 = hw_tone_command_io_5;
  }
  hw_display_io_6.setBrightness(7, true);
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < -999 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, false, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(7) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_0 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2, &hw_value_serial_0_value_3, &hw_value_serial_0_value_4);
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  int32_t hw_tone_command_io_5 = (hw_value_io_5 >= 31 && hw_value_io_5 <= 65535) ? hw_value_io_5 : 0;
  if (hw_tone_command_io_5 != hw_tone_io_5) {
    if (hw_tone_command_io_5 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_5);
    hw_tone_io_5 = hw_tone_command_io_5;
  }
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < -999 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, false, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
  if (hw_value_serial_0_trigger) { Serial.print("累计产量"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("达成率"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("停机次数"); Serial.println(hw_value_serial_0_value_2); }
  if (hw_value_serial_0_trigger) { Serial.print("实际计数或所需计数"); Serial.println(hw_value_serial_0_value_3); }
  if (hw_value_serial_0_trigger) { Serial.print("报警状态"); Serial.println(hw_value_serial_0_value_4); }
}

