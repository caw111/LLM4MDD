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
extern bool sysml_state_component_run_value;
/* Global Variable define of  */
extern bool sysml_state_component_run_previous;
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
void ProgramPage1(bool buttonPauseToggle, int32_t elapsedMilliseconds, int32_t* buzzerToneHz);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_component_run_value = true;
/* Global Variable define of  */
bool sysml_state_component_run_previous = false;
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
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_component_run_value = true;
    sysml_state_component_run_previous = false;
    sysml_state_component_beep_phase = 0;
    sysml_state_component_beep_started = false;
    sysml_state_component_beep_enabled = false;
    sysml_state_component_beep_remainder = 0;
    sysml_state_component_beep_value = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool buttonPauseToggle, int32_t elapsedMilliseconds, int32_t* buzzerToneHz) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int32_t sysml_conditional_4 = 0;
    sysml_lazy_0 = buttonPauseToggle;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_component_run_previous;
    }
    if (sysml_lazy_0) {
        sysml_state_component_run_value = (!sysml_state_component_run_value);
    }
    sysml_state_component_run_previous = buttonPauseToggle;
    sysml_lazy_1 = sysml_state_component_beep_enabled;
    if (sysml_lazy_1) {
        sysml_lazy_1 = elapsedMilliseconds > 0;
    }
    if (sysml_lazy_1) {
        if ((elapsedMilliseconds >= (3000 - sysml_state_component_beep_phase))) {
            sysml_state_component_beep_started = true;
        }
        sysml_state_component_beep_remainder = elapsedMilliseconds;
        if ((sysml_state_component_beep_remainder >= 1572864000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 1572864000);
        }
        if ((sysml_state_component_beep_remainder >= 786432000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 786432000);
        }
        if ((sysml_state_component_beep_remainder >= 393216000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 393216000);
        }
        if ((sysml_state_component_beep_remainder >= 196608000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 196608000);
        }
        if ((sysml_state_component_beep_remainder >= 98304000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 98304000);
        }
        if ((sysml_state_component_beep_remainder >= 49152000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 49152000);
        }
        if ((sysml_state_component_beep_remainder >= 24576000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 24576000);
        }
        if ((sysml_state_component_beep_remainder >= 12288000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 12288000);
        }
        if ((sysml_state_component_beep_remainder >= 6144000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 6144000);
        }
        if ((sysml_state_component_beep_remainder >= 3072000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 3072000);
        }
        if ((sysml_state_component_beep_remainder >= 1536000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 1536000);
        }
        if ((sysml_state_component_beep_remainder >= 768000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 768000);
        }
        if ((sysml_state_component_beep_remainder >= 384000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 384000);
        }
        if ((sysml_state_component_beep_remainder >= 192000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 192000);
        }
        if ((sysml_state_component_beep_remainder >= 96000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 96000);
        }
        if ((sysml_state_component_beep_remainder >= 48000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 48000);
        }
        if ((sysml_state_component_beep_remainder >= 24000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 24000);
        }
        if ((sysml_state_component_beep_remainder >= 12000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 12000);
        }
        if ((sysml_state_component_beep_remainder >= 6000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 6000);
        }
        if ((sysml_state_component_beep_remainder >= 3000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 3000);
        }
        if ((sysml_state_component_beep_remainder >= (3000 - sysml_state_component_beep_phase))) {
            sysml_state_component_beep_phase = (sysml_state_component_beep_remainder - (3000 - sysml_state_component_beep_phase));
        }
        else {
            sysml_state_component_beep_phase = (sysml_state_component_beep_phase + sysml_state_component_beep_remainder);
        }
    }
    sysml_lazy_2 = sysml_state_component_run_value;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_component_beep_started;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_component_beep_phase < 100;
    }
    sysml_state_component_beep_value = sysml_lazy_3;
    sysml_state_component_beep_enabled = sysml_state_component_run_value;
    int32_t sysml_value_0 = 0;
    if (sysml_state_component_beep_value) {
        sysml_conditional_4 = 1000;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_0 = sysml_conditional_4;
    *buzzerToneHz = sysml_value_0;
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
static bool hw_value_buttonPauseToggle = false;
static uint32_t hw_sampled_buttonPauseToggle = 0;
static bool hw_attempted_buttonPauseToggle = false;
static int32_t hw_value_elapsedMilliseconds = 0;
static int32_t hw_value_buzzerToneHz = 0;
static int32_t hw_tone_buzzerToneHz = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_buzzerToneHz = (hw_value_buzzerToneHz >= 31 && hw_value_buzzerToneHz <= 65535) ? hw_value_buzzerToneHz : 0;
  if (hw_tone_command_buzzerToneHz != hw_tone_buzzerToneHz) {
    if (hw_tone_command_buzzerToneHz == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_buzzerToneHz);
    hw_tone_buzzerToneHz = hw_tone_command_buzzerToneHz;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_buttonPauseToggle || (uint32_t)(hw_now - hw_sampled_buttonPauseToggle) >= 10UL) {
    hw_attempted_buttonPauseToggle = true; hw_sampled_buttonPauseToggle = hw_now;
    hw_value_buttonPauseToggle = (digitalRead(2) == LOW);
  }
  hw_value_elapsedMilliseconds = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_buttonPauseToggle, hw_value_elapsedMilliseconds, &hw_value_buzzerToneHz);
  int32_t hw_tone_command_buzzerToneHz = (hw_value_buzzerToneHz >= 31 && hw_value_buzzerToneHz <= 65535) ? hw_value_buzzerToneHz : 0;
  if (hw_tone_command_buzzerToneHz != hw_tone_buzzerToneHz) {
    if (hw_tone_command_buzzerToneHz == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_buzzerToneHz);
    hw_tone_buzzerToneHz = hw_tone_command_buzzerToneHz;
  }
}
