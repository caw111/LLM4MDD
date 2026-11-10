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
extern bool sysml_state_previousAllFull;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int32_t* io_3, bool* io_4, int32_t* io_5, bool* serial_0_trigger, int32_t* serial_0_value_0, bool* serial_0_value_1, bool* serial_0_value_2, bool* serial_0_value_3, int32_t* serial_0_value_4, bool* serial_0_value_5, bool* serial_0_value_6, bool* serial_0_value_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousAllFull = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousAllFull = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int32_t* io_3, bool* io_4, int32_t* io_5, bool* serial_0_trigger, int32_t* serial_0_value_0, bool* serial_0_value_1, bool* serial_0_value_2, bool* serial_0_value_3, int32_t* serial_0_value_4, bool* serial_0_value_5, bool* serial_0_value_6, bool* serial_0_value_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    int32_t sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    int32_t sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    int32_t sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    int32_t sysml_conditional_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    int32_t sysml_conditional_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    int32_t sysml_conditional_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    int32_t sysml_conditional_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    int32_t sysml_conditional_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_lazy_33 = 0;
    bool sysml_lazy_34 = 0;
    bool sysml_lazy_35 = 0;
    bool sysml_lazy_36 = 0;
    int32_t sysml_conditional_37 = 0;
    bool sysml_lazy_38 = 0;
    bool sysml_lazy_39 = 0;
    bool sysml_lazy_40 = 0;
    bool sysml_lazy_41 = 0;
    int32_t sysml_conditional_42 = 0;
    bool sysml_lazy_43 = 0;
    bool sysml_lazy_44 = 0;
    int32_t sysml_conditional_45 = 0;
    bool sysml_lazy_46 = 0;
    bool sysml_lazy_47 = 0;
    bool sysml_lazy_48 = 0;
    bool sysml_lazy_49 = 0;
    bool sysml_lazy_50 = 0;
    bool sysml_lazy_51 = 0;
    bool sysml_lazy_52 = 0;
    bool sysml_lazy_53 = 0;
    int32_t sysml_conditional_54 = 0;
    bool sysml_lazy_55 = 0;
    bool sysml_lazy_56 = 0;
    bool sysml_lazy_57 = 0;
    bool sysml_lazy_58 = 0;
    bool sysml_lazy_59 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_2;
    }
    sysml_state_previousAllFull = sysml_lazy_1;
    int32_t sysml_value_0 = 0;
    sysml_lazy_2 = io_0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_1;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_2;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousAllFull;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = 1000;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_0 = sysml_conditional_5;
    bool sysml_value_1 = 0;
    sysml_lazy_6 = io_0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_1;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = io_2;
    }
    sysml_value_1 = sysml_lazy_7;
    int32_t sysml_value_2 = 0;
    sysml_lazy_8 = io_0;
    if (sysml_lazy_8) {
        sysml_lazy_8 = !io_1;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !io_2;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 1;
    }
    else {
        sysml_lazy_11 = io_0;
        if (sysml_lazy_11) {
            sysml_lazy_11 = io_1;
        }
        sysml_lazy_12 = sysml_lazy_11;
        if (sysml_lazy_12) {
            sysml_lazy_12 = !io_2;
        }
        if (sysml_lazy_12) {
            sysml_conditional_13 = 2;
        }
        else {
            sysml_lazy_14 = io_0;
            if (sysml_lazy_14) {
                sysml_lazy_14 = !io_1;
            }
            sysml_lazy_15 = sysml_lazy_14;
            if (sysml_lazy_15) {
                sysml_lazy_15 = io_2;
            }
            if (sysml_lazy_15) {
                sysml_conditional_16 = 2;
            }
            else {
                sysml_lazy_17 = !io_0;
                if (sysml_lazy_17) {
                    sysml_lazy_17 = io_1;
                }
                sysml_lazy_18 = sysml_lazy_17;
                if (sysml_lazy_18) {
                    sysml_lazy_18 = io_2;
                }
                if (sysml_lazy_18) {
                    sysml_conditional_19 = 2;
                }
                else {
                    sysml_lazy_20 = io_0;
                    if (sysml_lazy_20) {
                        sysml_lazy_20 = io_1;
                    }
                    sysml_lazy_21 = sysml_lazy_20;
                    if (sysml_lazy_21) {
                        sysml_lazy_21 = io_2;
                    }
                    if (sysml_lazy_21) {
                        sysml_conditional_22 = 3;
                    }
                    else {
                        sysml_lazy_23 = io_0;
                        if (sysml_lazy_23) {
                            sysml_lazy_23 = !io_1;
                        }
                        sysml_lazy_24 = sysml_lazy_23;
                        if (sysml_lazy_24) {
                            sysml_lazy_24 = !io_2;
                        }
                        if (sysml_lazy_24) {
                            sysml_conditional_25 = 1;
                        }
                        else {
                            sysml_lazy_26 = !io_0;
                            if (sysml_lazy_26) {
                                sysml_lazy_26 = io_1;
                            }
                            sysml_lazy_27 = sysml_lazy_26;
                            if (sysml_lazy_27) {
                                sysml_lazy_27 = !io_2;
                            }
                            if (sysml_lazy_27) {
                                sysml_conditional_28 = 1;
                            }
                            else {
                                sysml_lazy_29 = !io_0;
                                if (sysml_lazy_29) {
                                    sysml_lazy_29 = !io_1;
                                }
                                sysml_lazy_30 = sysml_lazy_29;
                                if (sysml_lazy_30) {
                                    sysml_lazy_30 = io_2;
                                }
                                if (sysml_lazy_30) {
                                    sysml_conditional_31 = 1;
                                }
                                else {
                                    sysml_conditional_31 = 0;
                                }
                                sysml_conditional_28 = sysml_conditional_31;
                            }
                            sysml_conditional_25 = sysml_conditional_28;
                        }
                        sysml_conditional_22 = sysml_conditional_25;
                    }
                    sysml_conditional_19 = sysml_conditional_22;
                }
                sysml_conditional_16 = sysml_conditional_19;
            }
            sysml_conditional_13 = sysml_conditional_16;
        }
        sysml_conditional_10 = sysml_conditional_13;
    }
    sysml_value_2 = sysml_conditional_10;
    bool sysml_value_3 = 0;
    sysml_lazy_32 = io_0;
    if (!sysml_lazy_32) {
        sysml_lazy_32 = io_1;
    }
    sysml_lazy_33 = sysml_lazy_32;
    if (!sysml_lazy_33) {
        sysml_lazy_33 = io_2;
    }
    sysml_lazy_34 = sysml_lazy_33;
    if (sysml_lazy_34) {
        sysml_lazy_34 = !sysml_state_previousAllFull;
    }
    sysml_value_3 = sysml_lazy_34;
    int32_t sysml_value_4 = 0;
    sysml_lazy_35 = io_0;
    if (sysml_lazy_35) {
        sysml_lazy_35 = io_1;
    }
    sysml_lazy_36 = sysml_lazy_35;
    if (sysml_lazy_36) {
        sysml_lazy_36 = io_2;
    }
    if (sysml_lazy_36) {
        sysml_conditional_37 = 3;
    }
    else {
        sysml_lazy_38 = io_0;
        if (sysml_lazy_38) {
            sysml_lazy_39 = io_1;
            if (!sysml_lazy_39) {
                sysml_lazy_39 = io_2;
            }
            sysml_lazy_38 = sysml_lazy_39;
        }
        sysml_lazy_40 = sysml_lazy_38;
        if (sysml_lazy_40) {
            sysml_lazy_41 = io_1;
            if (sysml_lazy_41) {
                sysml_lazy_41 = io_2;
            }
            sysml_lazy_40 = !sysml_lazy_41;
        }
        if (sysml_lazy_40) {
            sysml_conditional_42 = 2;
        }
        else {
            sysml_lazy_43 = !io_0;
            if (sysml_lazy_43) {
                sysml_lazy_43 = io_1;
            }
            sysml_lazy_44 = sysml_lazy_43;
            if (sysml_lazy_44) {
                sysml_lazy_44 = io_2;
            }
            if (sysml_lazy_44) {
                sysml_conditional_45 = 2;
            }
            else {
                sysml_lazy_46 = io_0;
                if (sysml_lazy_46) {
                    sysml_lazy_46 = !io_1;
                }
                sysml_lazy_47 = sysml_lazy_46;
                if (sysml_lazy_47) {
                    sysml_lazy_47 = !io_2;
                }
                sysml_lazy_48 = sysml_lazy_47;
                if (!sysml_lazy_48) {
                    sysml_lazy_49 = !io_0;
                    if (sysml_lazy_49) {
                        sysml_lazy_49 = io_1;
                    }
                    sysml_lazy_50 = sysml_lazy_49;
                    if (sysml_lazy_50) {
                        sysml_lazy_50 = !io_2;
                    }
                    sysml_lazy_48 = sysml_lazy_50;
                }
                sysml_lazy_51 = sysml_lazy_48;
                if (!sysml_lazy_51) {
                    sysml_lazy_52 = !io_0;
                    if (sysml_lazy_52) {
                        sysml_lazy_52 = !io_1;
                    }
                    sysml_lazy_53 = sysml_lazy_52;
                    if (sysml_lazy_53) {
                        sysml_lazy_53 = io_2;
                    }
                    sysml_lazy_51 = sysml_lazy_53;
                }
                if (sysml_lazy_51) {
                    sysml_conditional_54 = 1;
                }
                else {
                    sysml_conditional_54 = 0;
                }
                sysml_conditional_45 = sysml_conditional_54;
            }
            sysml_conditional_42 = sysml_conditional_45;
        }
        sysml_conditional_37 = sysml_conditional_42;
    }
    sysml_value_4 = sysml_conditional_37;
    bool sysml_value_5 = 0;
    sysml_value_5 = io_0;
    bool sysml_value_6 = 0;
    sysml_value_6 = io_1;
    bool sysml_value_7 = 0;
    sysml_value_7 = io_2;
    int32_t sysml_value_8 = 0;
    sysml_value_8 = 0;
    bool sysml_value_9 = 0;
    sysml_value_9 = false;
    bool sysml_value_10 = 0;
    sysml_lazy_55 = io_0;
    if (sysml_lazy_55) {
        sysml_lazy_55 = io_1;
    }
    sysml_lazy_56 = sysml_lazy_55;
    if (sysml_lazy_56) {
        sysml_lazy_56 = io_2;
    }
    sysml_value_10 = sysml_lazy_56;
    bool sysml_value_11 = 0;
    sysml_lazy_57 = io_0;
    if (sysml_lazy_57) {
        sysml_lazy_57 = io_1;
    }
    sysml_lazy_58 = sysml_lazy_57;
    if (sysml_lazy_58) {
        sysml_lazy_58 = io_2;
    }
    sysml_lazy_59 = sysml_lazy_58;
    if (sysml_lazy_59) {
        sysml_lazy_59 = !sysml_state_previousAllFull;
    }
    sysml_value_11 = sysml_lazy_59;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
    *serial_0_value_3 = sysml_value_7;
    *serial_0_value_4 = sysml_value_8;
    *serial_0_value_5 = sysml_value_9;
    *serial_0_value_6 = sysml_value_10;
    *serial_0_value_7 = sysml_value_11;
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
static int32_t hw_value_io_3 = 0;
static int32_t hw_tone_io_3 = -1;
static bool hw_value_io_4 = false;
static int32_t hw_value_io_5 = 0;
TM1637Display hw_display_io_5(4, 5);
static int32_t hw_display_previous_io_5 = INT32_MIN;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static bool hw_value_serial_0_value_1 = false;
static bool hw_value_serial_0_value_2 = false;
static bool hw_value_serial_0_value_3 = false;
static int32_t hw_value_serial_0_value_4 = 0;
static bool hw_value_serial_0_value_5 = false;
static bool hw_value_serial_0_value_6 = false;
static bool hw_value_serial_0_value_7 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(7, INPUT);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  hw_display_io_5.setBrightness(7, true);
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
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
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2, &hw_value_serial_0_value_3, &hw_value_serial_0_value_4, &hw_value_serial_0_value_5, &hw_value_serial_0_value_6, &hw_value_serial_0_value_7);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
  if (hw_value_serial_0_trigger) { Serial.print("layer_count"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("layer1_occupied"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("layer2_occupied"); Serial.println(hw_value_serial_0_value_2); }
  if (hw_value_serial_0_trigger) { Serial.print("layer3_occupied"); Serial.println(hw_value_serial_0_value_3); }
  if (hw_value_serial_0_trigger) { Serial.print("query_elapsed_ms"); Serial.println(hw_value_serial_0_value_4); }
  if (hw_value_serial_0_trigger) { Serial.print("restock_needed"); Serial.println(hw_value_serial_0_value_5); }
  if (hw_value_serial_0_trigger) { Serial.print("all_full"); Serial.println(hw_value_serial_0_value_6); }
  if (hw_value_serial_0_trigger) { Serial.print("lock_new_records"); Serial.println(hw_value_serial_0_value_7); }
}
