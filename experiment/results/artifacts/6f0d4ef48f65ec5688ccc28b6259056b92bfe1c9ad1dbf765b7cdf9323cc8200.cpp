#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision20\\cases\\pceval_multiplexer_photoresistor\\arduino\\sketch\\sketch.ino"
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
extern int32_t sysml_state_ch;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_ch = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 255 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision20\\cases\\pceval_multiplexer_photoresistor\\arduino\\sketch\\sketch.ino"
void setup();
#line 269 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision20\\cases\\pceval_multiplexer_photoresistor\\arduino\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision20\\cases\\pceval_multiplexer_photoresistor\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ch = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* serial_0_trigger, int32_t* serial_0_value_0) {
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
    sysml_lazy_0 = sysml_state_ch >= 0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_ch <= 5;
    }
    if (sysml_lazy_0) {
        sysml_state_ch = (sysml_state_ch + 1);
    }
    else {
        sysml_state_ch = 0;
    }
    if ((sysml_state_ch >= 6)) {
        sysml_state_ch = 0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_1 = sysml_state_ch == 0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_ch == 2;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_ch == 4;
    }
    sysml_value_0 = sysml_lazy_2;
    bool sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_ch == 1;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_ch == 2;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_ch == 5;
    }
    sysml_value_1 = sysml_lazy_4;
    bool sysml_value_2 = 0;
    sysml_lazy_5 = sysml_state_ch == 2;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_ch == 3;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_ch == 6;
    }
    sysml_value_2 = sysml_lazy_6;
    bool sysml_value_3 = 0;
    sysml_lazy_7 = sysml_state_ch == 4;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_ch == 5;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_ch == 6;
    }
    sysml_value_3 = sysml_lazy_8;
    bool sysml_value_4 = 0;
    sysml_value_4 = true;
    int32_t sysml_value_5 = 0;
    if (sysml_state_ch == 0) {
        if (true) {
            if (io_0 < ((double)((int32_t)io_0))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_10 = ((int32_t)io_0) - sysml_conditional_11;
        }
        else {
            sysml_conditional_10 = 0;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    else {
        if (sysml_state_ch == 1) {
            if (true) {
                if (io_0 < ((double)((int32_t)io_0))) {
                    sysml_conditional_14 = 1;
                }
                else {
                    sysml_conditional_14 = 0;
                }
                sysml_conditional_13 = ((int32_t)io_0) - sysml_conditional_14;
            }
            else {
                sysml_conditional_13 = 0;
            }
            sysml_conditional_12 = sysml_conditional_13;
        }
        else {
            if (sysml_state_ch == 2) {
                if (true) {
                    if (io_0 < ((double)((int32_t)io_0))) {
                        sysml_conditional_17 = 1;
                    }
                    else {
                        sysml_conditional_17 = 0;
                    }
                    sysml_conditional_16 = ((int32_t)io_0) - sysml_conditional_17;
                }
                else {
                    sysml_conditional_16 = 0;
                }
                sysml_conditional_15 = sysml_conditional_16;
            }
            else {
                if (sysml_state_ch == 3) {
                    if (true) {
                        if (io_0 < ((double)((int32_t)io_0))) {
                            sysml_conditional_20 = 1;
                        }
                        else {
                            sysml_conditional_20 = 0;
                        }
                        sysml_conditional_19 = ((int32_t)io_0) - sysml_conditional_20;
                    }
                    else {
                        sysml_conditional_19 = 0;
                    }
                    sysml_conditional_18 = sysml_conditional_19;
                }
                else {
                    if (sysml_state_ch == 4) {
                        if (true) {
                            if (io_0 < ((double)((int32_t)io_0))) {
                                sysml_conditional_23 = 1;
                            }
                            else {
                                sysml_conditional_23 = 0;
                            }
                            sysml_conditional_22 = ((int32_t)io_0) - sysml_conditional_23;
                        }
                        else {
                            sysml_conditional_22 = 0;
                        }
                        sysml_conditional_21 = sysml_conditional_22;
                    }
                    else {
                        if (sysml_state_ch == 5) {
                            if (true) {
                                if (io_0 < ((double)((int32_t)io_0))) {
                                    sysml_conditional_26 = 1;
                                }
                                else {
                                    sysml_conditional_26 = 0;
                                }
                                sysml_conditional_25 = ((int32_t)io_0) - sysml_conditional_26;
                            }
                            else {
                                sysml_conditional_25 = 0;
                            }
                            sysml_conditional_24 = sysml_conditional_25;
                        }
                        else {
                            sysml_conditional_24 = 0;
                        }
                        sysml_conditional_21 = sysml_conditional_24;
                    }
                    sysml_conditional_18 = sysml_conditional_21;
                }
                sysml_conditional_15 = sysml_conditional_18;
            }
            sysml_conditional_12 = sysml_conditional_15;
        }
        sysml_conditional_9 = sysml_conditional_12;
    }
    sysml_value_5 = sysml_conditional_9;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
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
static double hw_value_io_0 = 0;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_4 ? HIGH : LOW);
}

void loop() {
  static bool started=false;
  static uint32_t last=0;
  uint32_t now=millis();
  if (started && (uint32_t)(now-last)<250) return;
  started=true;
  last=now;
  for (uint8_t channel=0; channel<6; ++channel) {
    for (uint8_t bit=0; bit<4; ++bit) digitalWrite(2+bit,channel&(1U<<bit) ? HIGH : LOW);
    delayMicroseconds(20);
    int value=analogRead(A0);
    Serial.print("Sensor ");
    Serial.print(channel);
    Serial.print(": ");
    Serial.println(value);
  }
}

