#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_photoresistor\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_channelIndex;
/* Global Variable define of  */
extern int32_t sysml_state_prevChannelIndex;
/* Global Variable define of  */
extern bool sysml_state_initialized;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_4, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_channelIndex = 0;
/* Global Variable define of  */
int32_t sysml_state_prevChannelIndex = 0;
/* Global Variable define of  */
bool sysml_state_initialized = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 315 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_photoresistor\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 329 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_photoresistor\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_photoresistor\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_channelIndex = 0;
    sysml_state_prevChannelIndex = 0;
    sysml_state_initialized = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_4, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    int32_t sysml_conditional_15 = 0;
    int32_t sysml_conditional_16 = 0;
    int32_t sysml_conditional_17 = 0;
    int32_t sysml_conditional_18 = 0;
    int32_t sysml_conditional_19 = 0;
    int32_t sysml_conditional_20 = 0;
    int32_t sysml_conditional_21 = 0;
    int32_t sysml_conditional_22 = 0;
    int32_t sysml_conditional_23 = 0;
    int32_t sysml_conditional_24 = 0;
    int32_t sysml_conditional_25 = 0;
    int32_t sysml_conditional_26 = 0;
    int32_t sysml_conditional_27 = 0;
    int32_t sysml_conditional_28 = 0;
    int32_t sysml_conditional_29 = 0;
    int32_t sysml_conditional_30 = 0;
    int32_t sysml_conditional_31 = 0;
    if ((!sysml_state_initialized)) {
        sysml_state_initialized = true;
        sysml_state_prevChannelIndex = 0;
        sysml_state_channelIndex = 0;
    }
    else {
        sysml_state_prevChannelIndex = sysml_state_channelIndex;
        if ((sysml_state_channelIndex == 5)) {
            sysml_state_channelIndex = 0;
        }
        else {
            sysml_state_channelIndex = (sysml_state_channelIndex + 1);
        }
    }
    bool sysml_value_0 = 0;
    sysml_lazy_0 = sysml_state_channelIndex == 1;
    if (!sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_channelIndex == 3;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_channelIndex == 5;
    }
    if (sysml_lazy_1) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_0 = sysml_conditional_2;
    bool sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_channelIndex >= 2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_channelIndex <= 5;
    }
    if (sysml_lazy_3) {
        sysml_lazy_5 = sysml_state_channelIndex == 2;
        if (!sysml_lazy_5) {
            sysml_lazy_5 = sysml_state_channelIndex == 3;
        }
        if (sysml_lazy_5) {
            sysml_conditional_6 = false;
        }
        else {
            sysml_conditional_6 = true;
        }
        sysml_conditional_4 = sysml_conditional_6;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_1 = sysml_conditional_4;
    bool sysml_value_2 = 0;
    if (sysml_state_channelIndex >= 4) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_2 = sysml_conditional_7;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    bool sysml_value_4 = 0;
    if (sysml_state_initialized) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_4 = sysml_conditional_8;
    int32_t sysml_value_5 = 0;
    if (sysml_state_channelIndex == 0) {
        if (io_4 >= 0) {
            if (io_4 < ((double)((int32_t)io_4))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_10 = ((int32_t)io_4) - sysml_conditional_11;
        }
        else {
            if (io_4 < ((double)((int32_t)io_4))) {
                sysml_conditional_12 = 1;
            }
            else {
                sysml_conditional_12 = 0;
            }
            sysml_conditional_10 = ((int32_t)io_4) - sysml_conditional_12;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    else {
        if (sysml_state_channelIndex == 1) {
            if (io_4 >= 0) {
                if (io_4 < ((double)((int32_t)io_4))) {
                    sysml_conditional_15 = 1;
                }
                else {
                    sysml_conditional_15 = 0;
                }
                sysml_conditional_14 = ((int32_t)io_4) - sysml_conditional_15;
            }
            else {
                if (io_4 < ((double)((int32_t)io_4))) {
                    sysml_conditional_16 = 1;
                }
                else {
                    sysml_conditional_16 = 0;
                }
                sysml_conditional_14 = ((int32_t)io_4) - sysml_conditional_16;
            }
            sysml_conditional_13 = sysml_conditional_14;
        }
        else {
            if (sysml_state_channelIndex == 2) {
                if (io_4 >= 0) {
                    if (io_4 < ((double)((int32_t)io_4))) {
                        sysml_conditional_19 = 1;
                    }
                    else {
                        sysml_conditional_19 = 0;
                    }
                    sysml_conditional_18 = ((int32_t)io_4) - sysml_conditional_19;
                }
                else {
                    if (io_4 < ((double)((int32_t)io_4))) {
                        sysml_conditional_20 = 1;
                    }
                    else {
                        sysml_conditional_20 = 0;
                    }
                    sysml_conditional_18 = ((int32_t)io_4) - sysml_conditional_20;
                }
                sysml_conditional_17 = sysml_conditional_18;
            }
            else {
                if (sysml_state_channelIndex == 3) {
                    if (io_4 >= 0) {
                        if (io_4 < ((double)((int32_t)io_4))) {
                            sysml_conditional_23 = 1;
                        }
                        else {
                            sysml_conditional_23 = 0;
                        }
                        sysml_conditional_22 = ((int32_t)io_4) - sysml_conditional_23;
                    }
                    else {
                        if (io_4 < ((double)((int32_t)io_4))) {
                            sysml_conditional_24 = 1;
                        }
                        else {
                            sysml_conditional_24 = 0;
                        }
                        sysml_conditional_22 = ((int32_t)io_4) - sysml_conditional_24;
                    }
                    sysml_conditional_21 = sysml_conditional_22;
                }
                else {
                    if (sysml_state_channelIndex == 4) {
                        if (io_4 >= 0) {
                            if (io_4 < ((double)((int32_t)io_4))) {
                                sysml_conditional_27 = 1;
                            }
                            else {
                                sysml_conditional_27 = 0;
                            }
                            sysml_conditional_26 = ((int32_t)io_4) - sysml_conditional_27;
                        }
                        else {
                            if (io_4 < ((double)((int32_t)io_4))) {
                                sysml_conditional_28 = 1;
                            }
                            else {
                                sysml_conditional_28 = 0;
                            }
                            sysml_conditional_26 = ((int32_t)io_4) - sysml_conditional_28;
                        }
                        sysml_conditional_25 = sysml_conditional_26;
                    }
                    else {
                        if (io_4 >= 0) {
                            if (io_4 < ((double)((int32_t)io_4))) {
                                sysml_conditional_30 = 1;
                            }
                            else {
                                sysml_conditional_30 = 0;
                            }
                            sysml_conditional_29 = ((int32_t)io_4) - sysml_conditional_30;
                        }
                        else {
                            if (io_4 < ((double)((int32_t)io_4))) {
                                sysml_conditional_31 = 1;
                            }
                            else {
                                sysml_conditional_31 = 0;
                            }
                            sysml_conditional_29 = ((int32_t)io_4) - sysml_conditional_31;
                        }
                        sysml_conditional_25 = sysml_conditional_29;
                    }
                    sysml_conditional_21 = sysml_conditional_25;
                }
                sysml_conditional_17 = sysml_conditional_21;
            }
            sysml_conditional_13 = sysml_conditional_17;
        }
        sysml_conditional_9 = sysml_conditional_13;
    }
    sysml_value_5 = sysml_conditional_9;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
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
static bool hw_value_io_0 = false;
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static double hw_value_io_4 = 0;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  pinMode(A0, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 5UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (analogRead(A0) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_4, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("Sensor {channel}"); Serial.println(hw_value_serial_0_value_0); }
}

