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
extern int32_t sysml_state_digit;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_8, bool io_8_valid, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_digit = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_digit = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_8, bool io_8_valid, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_conditional_25 = 0;
    bool sysml_conditional_26 = 0;
    bool sysml_conditional_27 = 0;
    bool sysml_conditional_28 = 0;
    bool sysml_conditional_29 = 0;
    bool sysml_conditional_30 = 0;
    bool sysml_conditional_31 = 0;
    bool sysml_conditional_32 = 0;
    bool sysml_conditional_33 = 0;
    bool sysml_conditional_34 = 0;
    bool sysml_conditional_35 = 0;
    bool sysml_conditional_36 = 0;
    bool sysml_conditional_37 = 0;
    bool sysml_conditional_38 = 0;
    bool sysml_conditional_39 = 0;
    bool sysml_conditional_40 = 0;
    bool sysml_conditional_41 = 0;
    bool sysml_conditional_42 = 0;
    bool sysml_conditional_43 = 0;
    bool sysml_conditional_44 = 0;
    bool sysml_conditional_45 = 0;
    bool sysml_conditional_46 = 0;
    bool sysml_conditional_47 = 0;
    bool sysml_conditional_48 = 0;
    bool sysml_conditional_49 = 0;
    bool sysml_conditional_50 = 0;
    bool sysml_conditional_51 = 0;
    bool sysml_conditional_52 = 0;
    bool sysml_conditional_53 = 0;
    bool sysml_conditional_54 = 0;
    bool sysml_conditional_55 = 0;
    bool sysml_conditional_56 = 0;
    bool sysml_conditional_57 = 0;
    bool sysml_conditional_58 = 0;
    bool sysml_conditional_59 = 0;
    bool sysml_conditional_60 = 0;
    bool sysml_conditional_61 = 0;
    bool sysml_conditional_62 = 0;
    bool sysml_conditional_63 = 0;
    bool sysml_conditional_64 = 0;
    bool sysml_conditional_65 = 0;
    bool sysml_conditional_66 = 0;
    bool sysml_conditional_67 = 0;
    bool sysml_conditional_68 = 0;
    bool sysml_conditional_69 = 0;
    if (io_8_valid) {
        sysml_state_digit = io_8;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_0 = true;
    }
    else {
        if (sysml_state_digit == 1) {
            sysml_conditional_1 = false;
        }
        else {
            if (sysml_state_digit == 2) {
                sysml_conditional_2 = false;
            }
            else {
                if (sysml_state_digit == 3) {
                    sysml_conditional_3 = false;
                }
                else {
                    if (sysml_state_digit == 4) {
                        sysml_conditional_4 = true;
                    }
                    else {
                        if (sysml_state_digit == 5) {
                            sysml_conditional_5 = true;
                        }
                        else {
                            if (sysml_state_digit == 6) {
                                sysml_conditional_6 = true;
                            }
                            else {
                                if (sysml_state_digit == 7) {
                                    sysml_conditional_7 = false;
                                }
                                else {
                                    if (sysml_state_digit == 8) {
                                        sysml_conditional_8 = true;
                                    }
                                    else {
                                        if (sysml_state_digit == 9) {
                                            sysml_conditional_9 = true;
                                        }
                                        else {
                                            sysml_conditional_9 = false;
                                        }
                                        sysml_conditional_8 = sysml_conditional_9;
                                    }
                                    sysml_conditional_7 = sysml_conditional_8;
                                }
                                sysml_conditional_6 = sysml_conditional_7;
                            }
                            sysml_conditional_5 = sysml_conditional_6;
                        }
                        sysml_conditional_4 = sysml_conditional_5;
                    }
                    sysml_conditional_3 = sysml_conditional_4;
                }
                sysml_conditional_2 = sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_10 = true;
    }
    else {
        if (sysml_state_digit == 1) {
            sysml_conditional_11 = true;
        }
        else {
            if (sysml_state_digit == 2) {
                sysml_conditional_12 = false;
            }
            else {
                if (sysml_state_digit == 3) {
                    sysml_conditional_13 = false;
                }
                else {
                    if (sysml_state_digit == 4) {
                        sysml_conditional_14 = true;
                    }
                    else {
                        if (sysml_state_digit == 5) {
                            sysml_conditional_15 = false;
                        }
                        else {
                            if (sysml_state_digit == 6) {
                                sysml_conditional_16 = false;
                            }
                            else {
                                if (sysml_state_digit == 7) {
                                    sysml_conditional_17 = true;
                                }
                                else {
                                    if (sysml_state_digit == 8) {
                                        sysml_conditional_18 = true;
                                    }
                                    else {
                                        if (sysml_state_digit == 9) {
                                            sysml_conditional_19 = true;
                                        }
                                        else {
                                            sysml_conditional_19 = false;
                                        }
                                        sysml_conditional_18 = sysml_conditional_19;
                                    }
                                    sysml_conditional_17 = sysml_conditional_18;
                                }
                                sysml_conditional_16 = sysml_conditional_17;
                            }
                            sysml_conditional_15 = sysml_conditional_16;
                        }
                        sysml_conditional_14 = sysml_conditional_15;
                    }
                    sysml_conditional_13 = sysml_conditional_14;
                }
                sysml_conditional_12 = sysml_conditional_13;
            }
            sysml_conditional_11 = sysml_conditional_12;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    sysml_value_1 = sysml_conditional_10;
    bool sysml_value_2 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_20 = true;
    }
    else {
        if (sysml_state_digit == 1) {
            sysml_conditional_21 = true;
        }
        else {
            if (sysml_state_digit == 2) {
                sysml_conditional_22 = true;
            }
            else {
                if (sysml_state_digit == 3) {
                    sysml_conditional_23 = true;
                }
                else {
                    if (sysml_state_digit == 4) {
                        sysml_conditional_24 = false;
                    }
                    else {
                        if (sysml_state_digit == 5) {
                            sysml_conditional_25 = false;
                        }
                        else {
                            if (sysml_state_digit == 6) {
                                sysml_conditional_26 = true;
                            }
                            else {
                                if (sysml_state_digit == 7) {
                                    sysml_conditional_27 = true;
                                }
                                else {
                                    if (sysml_state_digit == 8) {
                                        sysml_conditional_28 = true;
                                    }
                                    else {
                                        if (sysml_state_digit == 9) {
                                            sysml_conditional_29 = true;
                                        }
                                        else {
                                            sysml_conditional_29 = false;
                                        }
                                        sysml_conditional_28 = sysml_conditional_29;
                                    }
                                    sysml_conditional_27 = sysml_conditional_28;
                                }
                                sysml_conditional_26 = sysml_conditional_27;
                            }
                            sysml_conditional_25 = sysml_conditional_26;
                        }
                        sysml_conditional_24 = sysml_conditional_25;
                    }
                    sysml_conditional_23 = sysml_conditional_24;
                }
                sysml_conditional_22 = sysml_conditional_23;
            }
            sysml_conditional_21 = sysml_conditional_22;
        }
        sysml_conditional_20 = sysml_conditional_21;
    }
    sysml_value_2 = sysml_conditional_20;
    bool sysml_value_3 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_30 = true;
    }
    else {
        if (sysml_state_digit == 1) {
            sysml_conditional_31 = false;
        }
        else {
            if (sysml_state_digit == 2) {
                sysml_conditional_32 = true;
            }
            else {
                if (sysml_state_digit == 3) {
                    sysml_conditional_33 = true;
                }
                else {
                    if (sysml_state_digit == 4) {
                        sysml_conditional_34 = true;
                    }
                    else {
                        if (sysml_state_digit == 5) {
                            sysml_conditional_35 = true;
                        }
                        else {
                            if (sysml_state_digit == 6) {
                                sysml_conditional_36 = true;
                            }
                            else {
                                if (sysml_state_digit == 7) {
                                    sysml_conditional_37 = false;
                                }
                                else {
                                    if (sysml_state_digit == 8) {
                                        sysml_conditional_38 = true;
                                    }
                                    else {
                                        if (sysml_state_digit == 9) {
                                            sysml_conditional_39 = true;
                                        }
                                        else {
                                            sysml_conditional_39 = false;
                                        }
                                        sysml_conditional_38 = sysml_conditional_39;
                                    }
                                    sysml_conditional_37 = sysml_conditional_38;
                                }
                                sysml_conditional_36 = sysml_conditional_37;
                            }
                            sysml_conditional_35 = sysml_conditional_36;
                        }
                        sysml_conditional_34 = sysml_conditional_35;
                    }
                    sysml_conditional_33 = sysml_conditional_34;
                }
                sysml_conditional_32 = sysml_conditional_33;
            }
            sysml_conditional_31 = sysml_conditional_32;
        }
        sysml_conditional_30 = sysml_conditional_31;
    }
    sysml_value_3 = sysml_conditional_30;
    bool sysml_value_4 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_40 = true;
    }
    else {
        if (sysml_state_digit == 1) {
            sysml_conditional_41 = false;
        }
        else {
            if (sysml_state_digit == 2) {
                sysml_conditional_42 = false;
            }
            else {
                if (sysml_state_digit == 3) {
                    sysml_conditional_43 = true;
                }
                else {
                    if (sysml_state_digit == 4) {
                        sysml_conditional_44 = true;
                    }
                    else {
                        if (sysml_state_digit == 5) {
                            sysml_conditional_45 = false;
                        }
                        else {
                            if (sysml_state_digit == 6) {
                                sysml_conditional_46 = true;
                            }
                            else {
                                if (sysml_state_digit == 7) {
                                    sysml_conditional_47 = false;
                                }
                                else {
                                    if (sysml_state_digit == 8) {
                                        sysml_conditional_48 = true;
                                    }
                                    else {
                                        if (sysml_state_digit == 9) {
                                            sysml_conditional_49 = false;
                                        }
                                        else {
                                            sysml_conditional_49 = false;
                                        }
                                        sysml_conditional_48 = sysml_conditional_49;
                                    }
                                    sysml_conditional_47 = sysml_conditional_48;
                                }
                                sysml_conditional_46 = sysml_conditional_47;
                            }
                            sysml_conditional_45 = sysml_conditional_46;
                        }
                        sysml_conditional_44 = sysml_conditional_45;
                    }
                    sysml_conditional_43 = sysml_conditional_44;
                }
                sysml_conditional_42 = sysml_conditional_43;
            }
            sysml_conditional_41 = sysml_conditional_42;
        }
        sysml_conditional_40 = sysml_conditional_41;
    }
    sysml_value_4 = sysml_conditional_40;
    bool sysml_value_5 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_50 = true;
    }
    else {
        if (sysml_state_digit == 1) {
            sysml_conditional_51 = false;
        }
        else {
            if (sysml_state_digit == 2) {
                sysml_conditional_52 = true;
            }
            else {
                if (sysml_state_digit == 3) {
                    sysml_conditional_53 = false;
                }
                else {
                    if (sysml_state_digit == 4) {
                        sysml_conditional_54 = true;
                    }
                    else {
                        if (sysml_state_digit == 5) {
                            sysml_conditional_55 = true;
                        }
                        else {
                            if (sysml_state_digit == 6) {
                                sysml_conditional_56 = false;
                            }
                            else {
                                if (sysml_state_digit == 7) {
                                    sysml_conditional_57 = false;
                                }
                                else {
                                    if (sysml_state_digit == 8) {
                                        sysml_conditional_58 = true;
                                    }
                                    else {
                                        if (sysml_state_digit == 9) {
                                            sysml_conditional_59 = true;
                                        }
                                        else {
                                            sysml_conditional_59 = false;
                                        }
                                        sysml_conditional_58 = sysml_conditional_59;
                                    }
                                    sysml_conditional_57 = sysml_conditional_58;
                                }
                                sysml_conditional_56 = sysml_conditional_57;
                            }
                            sysml_conditional_55 = sysml_conditional_56;
                        }
                        sysml_conditional_54 = sysml_conditional_55;
                    }
                    sysml_conditional_53 = sysml_conditional_54;
                }
                sysml_conditional_52 = sysml_conditional_53;
            }
            sysml_conditional_51 = sysml_conditional_52;
        }
        sysml_conditional_50 = sysml_conditional_51;
    }
    sysml_value_5 = sysml_conditional_50;
    bool sysml_value_6 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_60 = false;
    }
    else {
        if (sysml_state_digit == 1) {
            sysml_conditional_61 = false;
        }
        else {
            if (sysml_state_digit == 2) {
                sysml_conditional_62 = true;
            }
            else {
                if (sysml_state_digit == 3) {
                    sysml_conditional_63 = true;
                }
                else {
                    if (sysml_state_digit == 4) {
                        sysml_conditional_64 = true;
                    }
                    else {
                        if (sysml_state_digit == 5) {
                            sysml_conditional_65 = true;
                        }
                        else {
                            if (sysml_state_digit == 6) {
                                sysml_conditional_66 = true;
                            }
                            else {
                                if (sysml_state_digit == 7) {
                                    sysml_conditional_67 = true;
                                }
                                else {
                                    if (sysml_state_digit == 8) {
                                        sysml_conditional_68 = true;
                                    }
                                    else {
                                        if (sysml_state_digit == 9) {
                                            sysml_conditional_69 = true;
                                        }
                                        else {
                                            sysml_conditional_69 = false;
                                        }
                                        sysml_conditional_68 = sysml_conditional_69;
                                    }
                                    sysml_conditional_67 = sysml_conditional_68;
                                }
                                sysml_conditional_66 = sysml_conditional_67;
                            }
                            sysml_conditional_65 = sysml_conditional_66;
                        }
                        sysml_conditional_64 = sysml_conditional_65;
                    }
                    sysml_conditional_63 = sysml_conditional_64;
                }
                sysml_conditional_62 = sysml_conditional_63;
            }
            sysml_conditional_61 = sysml_conditional_62;
        }
        sysml_conditional_60 = sysml_conditional_61;
    }
    sysml_value_6 = sysml_conditional_60;
    bool sysml_value_7 = 0;
    sysml_value_7 = false;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
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
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static int32_t hw_value_io_8 = 0;
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
  char* cursor = begin;
  bool negative = *cursor == '-';
  if (*cursor == '-' || *cursor == '+') ++cursor;
  if (!*cursor) return false;
  uint32_t magnitude = 0;
  const uint32_t limit = negative ? 2147483648UL : 2147483647UL;
  while (*cursor) {
    if (*cursor < '0' || *cursor > '9') return false;
    uint32_t digit = (uint32_t)(*cursor++ - '0');
    if (magnitude > (limit - digit) / 10UL) return false;
    magnitude = magnitude * 10UL + digit;
  }
  int32_t parsed = negative ? (magnitude == 2147483648UL ? INT32_MIN : -(int32_t)magnitude) : (int32_t)magnitude;
  if (parsed < 0L || parsed > 9L) return false;
  *value = parsed;
  return true;
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
static bool hw_value_io_8_valid = false;

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
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_4 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_5 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_6 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_7 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_8_valid = hw_rx_poll(hw_now, &hw_value_io_8);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_8, hw_value_io_8_valid, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_7 ? HIGH : LOW);
}
