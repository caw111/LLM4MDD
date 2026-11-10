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
extern bool sysml_state_locked;
/* Global Variable define of  */
extern bool sysml_state_previousPressedAny;
/* Global Variable define of  */
extern bool sysml_state_previousPressedP1;
/* Global Variable define of  */
extern bool sysml_state_previousPressedP2;
/* Global Variable define of  */
extern bool sysml_state_previousPressedP3;
/* Global Variable define of  */
extern bool sysml_state_previousPressedP4;
/* Global Variable define of  */
extern bool sysml_state_resetHeld;
/* Global Variable define of  */
extern bool sysml_state_resetLatch;
/* Global Variable define of  */
extern int32_t sysml_state_playerId;
/* Global Variable define of  */
extern int32_t sysml_state_buzzerTone;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool io_4, bool* io_5, int32_t* io_6, int32_t* io_7, bool* serial_0_trigger, int32_t* serial_0_value_0, int32_t* serial_0_value_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_locked = false;
/* Global Variable define of  */
bool sysml_state_previousPressedAny = false;
/* Global Variable define of  */
bool sysml_state_previousPressedP1 = false;
/* Global Variable define of  */
bool sysml_state_previousPressedP2 = false;
/* Global Variable define of  */
bool sysml_state_previousPressedP3 = false;
/* Global Variable define of  */
bool sysml_state_previousPressedP4 = false;
/* Global Variable define of  */
bool sysml_state_resetHeld = false;
/* Global Variable define of  */
bool sysml_state_resetLatch = false;
/* Global Variable define of  */
int32_t sysml_state_playerId = 0;
/* Global Variable define of  */
int32_t sysml_state_buzzerTone = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_locked = false;
    sysml_state_previousPressedAny = false;
    sysml_state_previousPressedP1 = false;
    sysml_state_previousPressedP2 = false;
    sysml_state_previousPressedP3 = false;
    sysml_state_previousPressedP4 = false;
    sysml_state_resetHeld = false;
    sysml_state_resetLatch = false;
    sysml_state_playerId = 0;
    sysml_state_buzzerTone = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool io_4, bool* io_5, int32_t* io_6, int32_t* io_7, bool* serial_0_trigger, int32_t* serial_0_value_0, int32_t* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    if (io_4) {
        sysml_lazy_0 = io_4;
        if (sysml_lazy_0) {
            sysml_lazy_0 = !sysml_state_resetLatch;
        }
        if (sysml_lazy_0) {
            sysml_state_resetHeld = true;
        }
        sysml_state_resetLatch = true;
    }
    else {
        sysml_state_resetHeld = false;
        sysml_state_resetLatch = false;
    }
    sysml_lazy_1 = sysml_state_resetLatch;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_locked;
    }
    if (sysml_lazy_1) {
        sysml_state_buzzerTone = 0;
    }
    if (io_4) {
        sysml_state_locked = false;
        sysml_state_playerId = 0;
        sysml_state_buzzerTone = 0;
        sysml_state_previousPressedAny = false;
        sysml_state_previousPressedP1 = false;
        sysml_state_previousPressedP2 = false;
        sysml_state_previousPressedP3 = false;
        sysml_state_previousPressedP4 = false;
    }
    sysml_lazy_2 = !sysml_state_locked;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_0;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousPressedP1;
    }
    if (sysml_lazy_3) {
        sysml_state_locked = true;
        sysml_state_playerId = 1;
        sysml_state_buzzerTone = 1000;
    }
    sysml_lazy_4 = !sysml_state_locked;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_1;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_previousPressedP2;
    }
    if (sysml_lazy_5) {
        sysml_state_locked = true;
        sysml_state_playerId = 2;
        sysml_state_buzzerTone = 1000;
    }
    sysml_lazy_6 = !sysml_state_locked;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_2;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_previousPressedP3;
    }
    if (sysml_lazy_7) {
        sysml_state_locked = true;
        sysml_state_playerId = 3;
        sysml_state_buzzerTone = 1000;
    }
    sysml_lazy_8 = !sysml_state_locked;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_3;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_previousPressedP4;
    }
    if (sysml_lazy_9) {
        sysml_state_locked = true;
        sysml_state_playerId = 4;
        sysml_state_buzzerTone = 1000;
    }
    if (sysml_state_locked) {
        sysml_state_buzzerTone = 0;
    }
    sysml_lazy_10 = io_0;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = io_1;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = io_2;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = io_3;
    }
    sysml_state_previousPressedAny = sysml_lazy_12;
    sysml_state_previousPressedP1 = io_0;
    sysml_state_previousPressedP2 = io_1;
    sysml_state_previousPressedP3 = io_2;
    sysml_state_previousPressedP4 = io_3;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_locked;
    int32_t sysml_value_1 = 0;
    sysml_value_1 = sysml_state_buzzerTone;
    int32_t sysml_value_2 = 0;
    sysml_value_2 = sysml_state_playerId;
    bool sysml_value_3 = 0;
    sysml_lazy_13 = !sysml_state_locked;
    if (sysml_lazy_13) {
        sysml_lazy_14 = io_0;
        if (sysml_lazy_14) {
            sysml_lazy_14 = !sysml_state_previousPressedP1;
        }
        sysml_lazy_15 = sysml_lazy_14;
        if (!sysml_lazy_15) {
            sysml_lazy_16 = io_1;
            if (sysml_lazy_16) {
                sysml_lazy_16 = !sysml_state_previousPressedP2;
            }
            sysml_lazy_15 = sysml_lazy_16;
        }
        sysml_lazy_17 = sysml_lazy_15;
        if (!sysml_lazy_17) {
            sysml_lazy_18 = io_2;
            if (sysml_lazy_18) {
                sysml_lazy_18 = !sysml_state_previousPressedP3;
            }
            sysml_lazy_17 = sysml_lazy_18;
        }
        sysml_lazy_19 = sysml_lazy_17;
        if (!sysml_lazy_19) {
            sysml_lazy_20 = io_3;
            if (sysml_lazy_20) {
                sysml_lazy_20 = !sysml_state_previousPressedP4;
            }
            sysml_lazy_19 = sysml_lazy_20;
        }
        sysml_lazy_13 = sysml_lazy_19;
    }
    sysml_value_3 = sysml_lazy_13;
    int32_t sysml_value_4 = 0;
    sysml_value_4 = 0;
    int32_t sysml_value_5 = 0;
    sysml_value_5 = sysml_state_playerId;
    *io_5 = sysml_value_0;
    *io_6 = sysml_value_1;
    *io_7 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
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
static bool hw_value_io_0 = false;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
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
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static bool hw_value_io_5 = false;
static int32_t hw_value_io_6 = 0;
static int32_t hw_tone_io_6 = -1;
static int32_t hw_value_io_7 = 0;
TM1637Display hw_display_io_7(4, 5);
static int32_t hw_display_previous_io_7 = INT32_MIN;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static int32_t hw_value_serial_0_value_1 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_6 = (hw_value_io_6 >= 31 && hw_value_io_6 <= 65535) ? hw_value_io_6 : 0;
  if (hw_tone_command_io_6 != hw_tone_io_6) {
    if (hw_tone_command_io_6 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_6);
    hw_tone_io_6 = hw_tone_command_io_6;
  }
  hw_display_io_7.setBrightness(7, true);
  if (hw_value_io_7 != hw_display_previous_io_7) {
    if (hw_value_io_7 < -999 || hw_value_io_7 > 9999) hw_display_io_7.clear();
    else hw_display_io_7.showNumberDec((int)hw_value_io_7, false, 4, 0);
    hw_display_previous_io_7 = hw_value_io_7;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(7) == HIGH);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(8) == HIGH);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 5UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(9) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  int32_t hw_tone_command_io_6 = (hw_value_io_6 >= 31 && hw_value_io_6 <= 65535) ? hw_value_io_6 : 0;
  if (hw_tone_command_io_6 != hw_tone_io_6) {
    if (hw_tone_command_io_6 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_6);
    hw_tone_io_6 = hw_tone_command_io_6;
  }
  if (hw_value_io_7 != hw_display_previous_io_7) {
    if (hw_value_io_7 < -999 || hw_value_io_7 > 9999) hw_display_io_7.clear();
    else hw_display_io_7.showNumberDec((int)hw_value_io_7, false, 4, 0);
    hw_display_previous_io_7 = hw_value_io_7;
  }
  if (hw_value_serial_0_trigger) { Serial.print("timestamp_ms"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("player_id"); Serial.println(hw_value_serial_0_value_1); }
}
