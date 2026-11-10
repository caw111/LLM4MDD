#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_006\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_component_beep_phase;
/* Global Variable define of  */
extern bool sysml_state_component_beep_started;
/* Global Variable define of  */
extern bool sysml_state_component_beep_enabled;
/* Global Variable define of  */
extern int32_t sysml_state_component_beep_remainder;
/* Global Variable define of  */
extern bool sysml_state_component_beep_value;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, int32_t* io_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_component_beep_phase = 0;
/* Global Variable define of  */
bool sysml_state_component_beep_started = false;
/* Global Variable define of  */
bool sysml_state_component_beep_enabled = false;
/* Global Variable define of  */
int32_t sysml_state_component_beep_remainder = 0;
/* Global Variable define of  */
bool sysml_state_component_beep_value = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 173 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_006\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 184 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_006\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_006\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_component_beep_phase = 0;
    sysml_state_component_beep_started = false;
    sysml_state_component_beep_enabled = false;
    sysml_state_component_beep_remainder = 0;
    sysml_state_component_beep_value = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, int32_t* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    sysml_lazy_0 = sysml_state_component_beep_enabled;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0 > 0;
    }
    if (sysml_lazy_0) {
        if ((io_0 >= (2000 - sysml_state_component_beep_phase))) {
            sysml_state_component_beep_started = true;
        }
        sysml_state_component_beep_remainder = io_0;
        if ((sysml_state_component_beep_remainder >= 2097152000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 2097152000);
        }
        if ((sysml_state_component_beep_remainder >= 1048576000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 1048576000);
        }
        if ((sysml_state_component_beep_remainder >= 524288000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 524288000);
        }
        if ((sysml_state_component_beep_remainder >= 262144000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 262144000);
        }
        if ((sysml_state_component_beep_remainder >= 131072000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 131072000);
        }
        if ((sysml_state_component_beep_remainder >= 65536000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 65536000);
        }
        if ((sysml_state_component_beep_remainder >= 32768000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 32768000);
        }
        if ((sysml_state_component_beep_remainder >= 16384000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 16384000);
        }
        if ((sysml_state_component_beep_remainder >= 8192000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 8192000);
        }
        if ((sysml_state_component_beep_remainder >= 4096000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 4096000);
        }
        if ((sysml_state_component_beep_remainder >= 2048000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 2048000);
        }
        if ((sysml_state_component_beep_remainder >= 1024000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 1024000);
        }
        if ((sysml_state_component_beep_remainder >= 512000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 512000);
        }
        if ((sysml_state_component_beep_remainder >= 256000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 256000);
        }
        if ((sysml_state_component_beep_remainder >= 128000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 128000);
        }
        if ((sysml_state_component_beep_remainder >= 64000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 64000);
        }
        if ((sysml_state_component_beep_remainder >= 32000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 32000);
        }
        if ((sysml_state_component_beep_remainder >= 16000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 16000);
        }
        if ((sysml_state_component_beep_remainder >= 8000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 8000);
        }
        if ((sysml_state_component_beep_remainder >= 4000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 4000);
        }
        if ((sysml_state_component_beep_remainder >= 2000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 2000);
        }
        if ((sysml_state_component_beep_remainder >= (2000 - sysml_state_component_beep_phase))) {
            sysml_state_component_beep_phase = (sysml_state_component_beep_remainder - (2000 - sysml_state_component_beep_phase));
        }
        else {
            sysml_state_component_beep_phase = (sysml_state_component_beep_phase + sysml_state_component_beep_remainder);
        }
    }
    sysml_lazy_1 = true;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_component_beep_started;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_component_beep_phase < 200;
    }
    sysml_state_component_beep_value = sysml_lazy_2;
    sysml_state_component_beep_enabled = true;
    int32_t sysml_value_0 = 0;
    if (sysml_state_component_beep_value) {
        sysml_conditional_3 = 1000;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_0 = sysml_conditional_3;
    *io_1 = sysml_value_0;
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
static int32_t hw_value_io_0 = 0;
static int32_t hw_value_io_1 = 0;
static int32_t hw_tone_io_1 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_1 = (hw_value_io_1 >= 31 && hw_value_io_1 <= 65535) ? hw_value_io_1 : 0;
  if (hw_tone_command_io_1 != hw_tone_io_1) {
    if (hw_tone_command_io_1 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_1);
    hw_tone_io_1 = hw_tone_command_io_1;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_0 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1);
  int32_t hw_tone_command_io_1 = (hw_value_io_1 >= 31 && hw_value_io_1 <= 65535) ? hw_value_io_1 : 0;
  if (hw_tone_command_io_1 != hw_tone_io_1) {
    if (hw_tone_command_io_1 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_1);
    hw_tone_io_1 = hw_tone_command_io_1;
  }
}

