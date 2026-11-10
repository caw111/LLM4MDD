#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern double sysml_state_targetTempC;
/* Global Variable define of  */
extern int32_t sysml_state_minFanPwm;
/* Global Variable define of  */
extern double sysml_state_serialSampleTempC;
/* Global Variable define of  */
extern bool sysml_state_ledOverheatLatched;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int32_t io_3, bool io_3_valid, double* io_1, bool* io_2, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, int32_t* serial_0_value_2, bool* serial_0_value_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_targetTempC = 25.0;
/* Global Variable define of  */
int32_t sysml_state_minFanPwm = 30;
/* Global Variable define of  */
double sysml_state_serialSampleTempC = 0.0;
/* Global Variable define of  */
bool sysml_state_ledOverheatLatched = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 201 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_parse(int32_t* value);
#line 242 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_finish(int32_t* value);
#line 247 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_poll(uint32_t hw_now, int32_t* value);
#line 271 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 281 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_targetTempC = 25.0;
    sysml_state_minFanPwm = 30;
    sysml_state_serialSampleTempC = 0.0;
    sysml_state_ledOverheatLatched = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int32_t io_3, bool io_3_valid, double* io_1, bool* io_2, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, int32_t* serial_0_value_2, bool* serial_0_value_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    if (io_3_valid) {
        sysml_state_serialSampleTempC = io_0;
        if ((io_3 == 1)) {
            sysml_state_targetTempC = (sysml_state_targetTempC + 1.0);
        }
        if ((io_3 == 2)) {
            sysml_state_targetTempC = (sysml_state_targetTempC + 2.0);
        }
        if ((io_3 == 3)) {
            sysml_state_targetTempC = (sysml_state_targetTempC + 3.0);
        }
        if ((io_3 == 4)) {
            sysml_state_targetTempC = (sysml_state_targetTempC + 4.0);
        }
        if ((io_3 == 5)) {
            sysml_state_targetTempC = (sysml_state_targetTempC + 5.0);
        }
        if ((io_3 == 21)) {
            sysml_state_targetTempC = (sysml_state_targetTempC - 1.0);
        }
        if ((io_3 == 22)) {
            sysml_state_targetTempC = (sysml_state_targetTempC - 2.0);
        }
        if ((io_3 == 23)) {
            sysml_state_targetTempC = (sysml_state_targetTempC - 3.0);
        }
        if ((io_3 == 24)) {
            sysml_state_targetTempC = (sysml_state_targetTempC - 4.0);
        }
        if ((io_3 == 25)) {
            sysml_state_targetTempC = (sysml_state_targetTempC - 5.0);
        }
        if ((io_3 == 26)) {
            sysml_state_minFanPwm = (sysml_state_minFanPwm + 1);
        }
        if ((io_3 == 27)) {
            sysml_state_minFanPwm = (sysml_state_minFanPwm + 2);
        }
        if ((io_3 == 28)) {
            sysml_state_minFanPwm = (sysml_state_minFanPwm + 3);
        }
        if ((io_3 == 29)) {
            sysml_state_minFanPwm = (sysml_state_minFanPwm + 4);
        }
        if ((io_3 == 30)) {
            sysml_state_minFanPwm = (sysml_state_minFanPwm + 5);
        }
        if ((io_3 == 31)) {
            sysml_state_targetTempC = 25.0;
            sysml_state_minFanPwm = 30;
        }
    }
    if ((sysml_state_serialSampleTempC >= (sysml_state_targetTempC + 5.0))) {
        sysml_state_ledOverheatLatched = true;
    }
    else {
        sysml_state_ledOverheatLatched = false;
    }
    double sysml_value_0 = 0;
    if (sysml_state_serialSampleTempC >= sysml_state_targetTempC) {
        if (sysml_state_serialSampleTempC >= (sysml_state_targetTempC + 5.0)) {
            sysml_conditional_1 = 255.0;
        }
        else {
            if (sysml_state_minFanPwm >= 255) {
                sysml_conditional_2 = 255.0;
            }
            else {
                if (sysml_state_minFanPwm != 0) {
                    if (((sysml_state_serialSampleTempC - sysml_state_targetTempC) * 10.0) < ((double)((int32_t)((sysml_state_serialSampleTempC - sysml_state_targetTempC) * 10.0)))) {
                        sysml_conditional_4 = 1;
                    }
                    else {
                        sysml_conditional_4 = 0;
                    }
                    sysml_conditional_3 = ((int32_t)((sysml_state_serialSampleTempC - sysml_state_targetTempC) * 10.0)) - sysml_conditional_4;
                }
                else {
                    sysml_conditional_3 = 0;
                }
                sysml_conditional_2 = sysml_state_minFanPwm + sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        if (sysml_state_minFanPwm >= 0) {
            if (sysml_state_minFanPwm < ((double)((int32_t)sysml_state_minFanPwm))) {
                sysml_conditional_6 = 1;
            }
            else {
                sysml_conditional_6 = 0;
            }
            sysml_conditional_5 = ((int32_t)sysml_state_minFanPwm) - sysml_conditional_6;
        }
        else {
            sysml_conditional_5 = 0.0;
        }
        sysml_conditional_0 = sysml_conditional_5;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_ledOverheatLatched;
    bool sysml_value_2 = 0;
    sysml_value_2 = io_3_valid;
    double sysml_value_3 = 0;
    sysml_value_3 = sysml_state_serialSampleTempC;
    double sysml_value_4 = 0;
    sysml_value_4 = sysml_state_targetTempC;
    int32_t sysml_value_5 = 0;
    sysml_value_5 = sysml_state_minFanPwm;
    bool sysml_value_6 = 0;
    sysml_value_6 = sysml_state_ledOverheatLatched;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
    *serial_0_value_1 = sysml_value_4;
    *serial_0_value_2 = sysml_value_5;
    *serial_0_value_3 = sysml_value_6;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:uno
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_UNO)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static double hw_value_io_0 = 0;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
static bool hw_value_io_2 = false;
static int32_t hw_value_io_3 = 0;
#include <string.h>
static char hw_rx_buffer[64];
static uint8_t hw_rx_size = 0;
static bool hw_rx_overflow = false;
static uint32_t hw_rx_last_byte = 0;
static bool hw_rx_parse(int32_t* value) {
  hw_rx_buffer[hw_rx_size] = '\0';
  char* begin = hw_rx_buffer;
  while (*begin == ' ' || *begin == '\t') ++begin;
  char* end = hw_rx_buffer + hw_rx_size;
  while (end > begin && (end[-1] == ' ' || end[-1] == '\t')) *--end = '\0';
  if (!*begin) return false;
  if (strcmp(begin, "T+1") == 0) { *value = 1L; return true; }
  if (strcmp(begin, "T+2") == 0) { *value = 2L; return true; }
  if (strcmp(begin, "T+3") == 0) { *value = 3L; return true; }
  if (strcmp(begin, "T+4") == 0) { *value = 4L; return true; }
  if (strcmp(begin, "T+5") == 0) { *value = 5L; return true; }
  if (strcmp(begin, "T+6") == 0) { *value = 6L; return true; }
  if (strcmp(begin, "T+7") == 0) { *value = 7L; return true; }
  if (strcmp(begin, "T+8") == 0) { *value = 8L; return true; }
  if (strcmp(begin, "T+9") == 0) { *value = 9L; return true; }
  if (strcmp(begin, "T+10") == 0) { *value = 10L; return true; }
  if (strcmp(begin, "T+11") == 0) { *value = 11L; return true; }
  if (strcmp(begin, "T+12") == 0) { *value = 12L; return true; }
  if (strcmp(begin, "T+13") == 0) { *value = 13L; return true; }
  if (strcmp(begin, "T+14") == 0) { *value = 14L; return true; }
  if (strcmp(begin, "T+15") == 0) { *value = 15L; return true; }
  if (strcmp(begin, "T+16") == 0) { *value = 16L; return true; }
  if (strcmp(begin, "T+17") == 0) { *value = 17L; return true; }
  if (strcmp(begin, "T+18") == 0) { *value = 18L; return true; }
  if (strcmp(begin, "T+19") == 0) { *value = 19L; return true; }
  if (strcmp(begin, "T+20") == 0) { *value = 20L; return true; }
  if (strcmp(begin, "T-1") == 0) { *value = 21L; return true; }
  if (strcmp(begin, "T-2") == 0) { *value = 22L; return true; }
  if (strcmp(begin, "T-3") == 0) { *value = 23L; return true; }
  if (strcmp(begin, "T-4") == 0) { *value = 24L; return true; }
  if (strcmp(begin, "T-5") == 0) { *value = 25L; return true; }
  if (strcmp(begin, "F+1") == 0) { *value = 26L; return true; }
  if (strcmp(begin, "F+2") == 0) { *value = 27L; return true; }
  if (strcmp(begin, "F+3") == 0) { *value = 28L; return true; }
  if (strcmp(begin, "F+4") == 0) { *value = 29L; return true; }
  if (strcmp(begin, "F+5") == 0) { *value = 30L; return true; }
  if (strcmp(begin, "RESET") == 0) { *value = 31L; return true; }
  if (strcmp(begin, "SAVE") == 0) { *value = 32L; return true; }
  return false;
}
static bool hw_rx_finish(int32_t* value) {
  bool accepted = !hw_rx_overflow && hw_rx_parse(value);
  hw_rx_size = 0; hw_rx_overflow = false;
  return accepted;
}
static bool hw_rx_poll(uint32_t hw_now, int32_t* value) {
  for (uint8_t budget = 0; budget < 32 && Serial.available() > 0; ++budget) {
    int byte = Serial.read();
    if (byte < 0) break;
    hw_rx_last_byte = hw_now;
    if (byte == '\r' || byte == '\n') {
      if (!hw_rx_size && !hw_rx_overflow) continue;
      return hw_rx_finish(value);
    }
    if (byte == 0 || byte > 127) hw_rx_overflow = true;
    if (hw_rx_size < 63) hw_rx_buffer[hw_rx_size++] = (char)byte;
    else hw_rx_overflow = true;
  }
  if (Serial.available() > 0) return false;
  if ((hw_rx_size || hw_rx_overflow) && (uint32_t)(hw_now - hw_rx_last_byte) >= 20UL) return hw_rx_finish(value);
  return false;
}
static bool hw_value_io_3_valid = false;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static int32_t hw_value_serial_0_value_2 = 0;
static bool hw_value_serial_0_value_3 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_1 * 1 + 0), 0.0, 255.0));
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A1) * 1 + 0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_3_valid = hw_rx_poll(hw_now, &hw_value_io_3);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_3, hw_value_io_3_valid, &hw_value_io_1, &hw_value_io_2, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2, &hw_value_serial_0_value_3);
  analogWrite(5, (int)constrain((hw_value_io_1 * 1 + 0), 0.0, 255.0));
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("temperature_degC"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("target_temperature_degC"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("fan_pwm_command"); Serial.println(hw_value_serial_0_value_2); }
  if (hw_value_serial_0_trigger) { Serial.print("overheat_warning"); Serial.println(hw_value_serial_0_value_3); }
}

