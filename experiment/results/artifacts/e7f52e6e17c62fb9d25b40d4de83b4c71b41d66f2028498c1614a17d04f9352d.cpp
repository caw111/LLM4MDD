#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_7_segment_display_serial\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_0_valid, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
#line 401 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_7_segment_display_serial\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_parse(int32_t* value);
#line 425 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_7_segment_display_serial\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_finish(int32_t* value);
#line 430 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_7_segment_display_serial\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_poll(uint32_t hw_now, int32_t* value);
#line 457 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_7_segment_display_serial\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 478 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_7_segment_display_serial\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_7_segment_display_serial\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_0_valid, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_value_0 = 0;
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
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_lazy_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_lazy_33 = 0;
    bool sysml_lazy_34 = 0;
    bool sysml_lazy_35 = 0;
    bool sysml_lazy_36 = 0;
    bool sysml_lazy_37 = 0;
    bool sysml_lazy_38 = 0;
    bool sysml_lazy_39 = 0;
    bool sysml_lazy_40 = 0;
    bool sysml_lazy_41 = 0;
    bool sysml_lazy_42 = 0;
    bool sysml_lazy_43 = 0;
    bool sysml_lazy_44 = 0;
    bool sysml_lazy_45 = 0;
    bool sysml_lazy_46 = 0;
    bool sysml_lazy_47 = 0;
    bool sysml_lazy_48 = 0;
    bool sysml_lazy_49 = 0;
    bool sysml_lazy_50 = 0;
    bool sysml_lazy_51 = 0;
    bool sysml_lazy_52 = 0;
    bool sysml_lazy_53 = 0;
    bool sysml_lazy_54 = 0;
    bool sysml_lazy_55 = 0;
    bool sysml_lazy_56 = 0;
    bool sysml_lazy_57 = 0;
    bool sysml_lazy_58 = 0;
    bool sysml_lazy_59 = 0;
    bool sysml_lazy_60 = 0;
    bool sysml_lazy_61 = 0;
    bool sysml_lazy_62 = 0;
    bool sysml_lazy_63 = 0;
    bool sysml_lazy_64 = 0;
    bool sysml_lazy_65 = 0;
    sysml_lazy_0 = io_0_valid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0 == 0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (!sysml_lazy_1) {
        sysml_lazy_2 = io_0_valid;
        if (sysml_lazy_2) {
            sysml_lazy_2 = io_0 == 2;
        }
        sysml_lazy_1 = sysml_lazy_2;
    }
    sysml_lazy_3 = sysml_lazy_1;
    if (!sysml_lazy_3) {
        sysml_lazy_4 = io_0_valid;
        if (sysml_lazy_4) {
            sysml_lazy_4 = io_0 == 3;
        }
        sysml_lazy_3 = sysml_lazy_4;
    }
    sysml_lazy_5 = sysml_lazy_3;
    if (!sysml_lazy_5) {
        sysml_lazy_6 = io_0_valid;
        if (sysml_lazy_6) {
            sysml_lazy_6 = io_0 == 5;
        }
        sysml_lazy_5 = sysml_lazy_6;
    }
    sysml_lazy_7 = sysml_lazy_5;
    if (!sysml_lazy_7) {
        sysml_lazy_8 = io_0_valid;
        if (sysml_lazy_8) {
            sysml_lazy_8 = io_0 == 6;
        }
        sysml_lazy_7 = sysml_lazy_8;
    }
    sysml_lazy_9 = sysml_lazy_7;
    if (!sysml_lazy_9) {
        sysml_lazy_10 = io_0_valid;
        if (sysml_lazy_10) {
            sysml_lazy_10 = io_0 == 7;
        }
        sysml_lazy_9 = sysml_lazy_10;
    }
    sysml_lazy_11 = sysml_lazy_9;
    if (!sysml_lazy_11) {
        sysml_lazy_12 = io_0_valid;
        if (sysml_lazy_12) {
            sysml_lazy_12 = io_0 == 8;
        }
        sysml_lazy_11 = sysml_lazy_12;
    }
    sysml_lazy_13 = sysml_lazy_11;
    if (!sysml_lazy_13) {
        sysml_lazy_14 = io_0_valid;
        if (sysml_lazy_14) {
            sysml_lazy_14 = io_0 == 9;
        }
        sysml_lazy_13 = sysml_lazy_14;
    }
    sysml_value_0 = sysml_lazy_13;
    bool sysml_value_1 = 0;
    sysml_lazy_15 = io_0_valid;
    if (sysml_lazy_15) {
        sysml_lazy_16 = io_0 == 0;
        if (!sysml_lazy_16) {
            sysml_lazy_16 = io_0 == 1;
        }
        sysml_lazy_17 = sysml_lazy_16;
        if (!sysml_lazy_17) {
            sysml_lazy_17 = io_0 == 2;
        }
        sysml_lazy_18 = sysml_lazy_17;
        if (!sysml_lazy_18) {
            sysml_lazy_18 = io_0 == 3;
        }
        sysml_lazy_19 = sysml_lazy_18;
        if (!sysml_lazy_19) {
            sysml_lazy_19 = io_0 == 4;
        }
        sysml_lazy_20 = sysml_lazy_19;
        if (!sysml_lazy_20) {
            sysml_lazy_20 = io_0 == 7;
        }
        sysml_lazy_21 = sysml_lazy_20;
        if (!sysml_lazy_21) {
            sysml_lazy_21 = io_0 == 8;
        }
        sysml_lazy_22 = sysml_lazy_21;
        if (!sysml_lazy_22) {
            sysml_lazy_22 = io_0 == 9;
        }
        sysml_lazy_15 = sysml_lazy_22;
    }
    sysml_value_1 = sysml_lazy_15;
    bool sysml_value_2 = 0;
    sysml_lazy_23 = io_0_valid;
    if (sysml_lazy_23) {
        sysml_lazy_24 = io_0 == 0;
        if (!sysml_lazy_24) {
            sysml_lazy_24 = io_0 == 1;
        }
        sysml_lazy_25 = sysml_lazy_24;
        if (!sysml_lazy_25) {
            sysml_lazy_25 = io_0 == 3;
        }
        sysml_lazy_26 = sysml_lazy_25;
        if (!sysml_lazy_26) {
            sysml_lazy_26 = io_0 == 4;
        }
        sysml_lazy_27 = sysml_lazy_26;
        if (!sysml_lazy_27) {
            sysml_lazy_27 = io_0 == 5;
        }
        sysml_lazy_28 = sysml_lazy_27;
        if (!sysml_lazy_28) {
            sysml_lazy_28 = io_0 == 6;
        }
        sysml_lazy_29 = sysml_lazy_28;
        if (!sysml_lazy_29) {
            sysml_lazy_29 = io_0 == 7;
        }
        sysml_lazy_30 = sysml_lazy_29;
        if (!sysml_lazy_30) {
            sysml_lazy_30 = io_0 == 8;
        }
        sysml_lazy_31 = sysml_lazy_30;
        if (!sysml_lazy_31) {
            sysml_lazy_31 = io_0 == 9;
        }
        sysml_lazy_23 = sysml_lazy_31;
    }
    sysml_value_2 = sysml_lazy_23;
    bool sysml_value_3 = 0;
    sysml_lazy_32 = io_0_valid;
    if (sysml_lazy_32) {
        sysml_lazy_33 = io_0 == 0;
        if (!sysml_lazy_33) {
            sysml_lazy_33 = io_0 == 2;
        }
        sysml_lazy_34 = sysml_lazy_33;
        if (!sysml_lazy_34) {
            sysml_lazy_34 = io_0 == 3;
        }
        sysml_lazy_35 = sysml_lazy_34;
        if (!sysml_lazy_35) {
            sysml_lazy_35 = io_0 == 5;
        }
        sysml_lazy_36 = sysml_lazy_35;
        if (!sysml_lazy_36) {
            sysml_lazy_36 = io_0 == 6;
        }
        sysml_lazy_37 = sysml_lazy_36;
        if (!sysml_lazy_37) {
            sysml_lazy_37 = io_0 == 8;
        }
        sysml_lazy_38 = sysml_lazy_37;
        if (!sysml_lazy_38) {
            sysml_lazy_38 = io_0 == 9;
        }
        sysml_lazy_32 = sysml_lazy_38;
    }
    sysml_value_3 = sysml_lazy_32;
    bool sysml_value_4 = 0;
    sysml_lazy_39 = io_0_valid;
    if (sysml_lazy_39) {
        sysml_lazy_40 = io_0 == 0;
        if (!sysml_lazy_40) {
            sysml_lazy_40 = io_0 == 2;
        }
        sysml_lazy_41 = sysml_lazy_40;
        if (!sysml_lazy_41) {
            sysml_lazy_41 = io_0 == 6;
        }
        sysml_lazy_42 = sysml_lazy_41;
        if (!sysml_lazy_42) {
            sysml_lazy_42 = io_0 == 8;
        }
        sysml_lazy_39 = sysml_lazy_42;
    }
    sysml_value_4 = sysml_lazy_39;
    bool sysml_value_5 = 0;
    sysml_lazy_43 = io_0_valid;
    if (sysml_lazy_43) {
        sysml_lazy_44 = io_0 == 0;
        if (!sysml_lazy_44) {
            sysml_lazy_44 = io_0 == 4;
        }
        sysml_lazy_45 = sysml_lazy_44;
        if (!sysml_lazy_45) {
            sysml_lazy_45 = io_0 == 5;
        }
        sysml_lazy_46 = sysml_lazy_45;
        if (!sysml_lazy_46) {
            sysml_lazy_46 = io_0 == 6;
        }
        sysml_lazy_47 = sysml_lazy_46;
        if (!sysml_lazy_47) {
            sysml_lazy_47 = io_0 == 8;
        }
        sysml_lazy_48 = sysml_lazy_47;
        if (!sysml_lazy_48) {
            sysml_lazy_48 = io_0 == 9;
        }
        sysml_lazy_43 = sysml_lazy_48;
    }
    sysml_value_5 = sysml_lazy_43;
    bool sysml_value_6 = 0;
    sysml_lazy_49 = io_0_valid;
    if (sysml_lazy_49) {
        sysml_lazy_50 = io_0 == 0;
        if (!sysml_lazy_50) {
            sysml_lazy_50 = io_0 == 2;
        }
        sysml_lazy_51 = sysml_lazy_50;
        if (!sysml_lazy_51) {
            sysml_lazy_51 = io_0 == 3;
        }
        sysml_lazy_52 = sysml_lazy_51;
        if (!sysml_lazy_52) {
            sysml_lazy_52 = io_0 == 5;
        }
        sysml_lazy_53 = sysml_lazy_52;
        if (!sysml_lazy_53) {
            sysml_lazy_53 = io_0 == 6;
        }
        sysml_lazy_54 = sysml_lazy_53;
        if (!sysml_lazy_54) {
            sysml_lazy_54 = io_0 == 8;
        }
        sysml_lazy_55 = sysml_lazy_54;
        if (!sysml_lazy_55) {
            sysml_lazy_55 = io_0 == 9;
        }
        sysml_lazy_49 = sysml_lazy_55;
    }
    sysml_value_6 = sysml_lazy_49;
    bool sysml_value_7 = 0;
    sysml_lazy_56 = io_0_valid;
    if (sysml_lazy_56) {
        sysml_lazy_57 = io_0 == 0;
        if (!sysml_lazy_57) {
            sysml_lazy_57 = io_0 == 1;
        }
        sysml_lazy_58 = sysml_lazy_57;
        if (!sysml_lazy_58) {
            sysml_lazy_58 = io_0 == 2;
        }
        sysml_lazy_59 = sysml_lazy_58;
        if (!sysml_lazy_59) {
            sysml_lazy_59 = io_0 == 3;
        }
        sysml_lazy_60 = sysml_lazy_59;
        if (!sysml_lazy_60) {
            sysml_lazy_60 = io_0 == 4;
        }
        sysml_lazy_61 = sysml_lazy_60;
        if (!sysml_lazy_61) {
            sysml_lazy_61 = io_0 == 5;
        }
        sysml_lazy_62 = sysml_lazy_61;
        if (!sysml_lazy_62) {
            sysml_lazy_62 = io_0 == 6;
        }
        sysml_lazy_63 = sysml_lazy_62;
        if (!sysml_lazy_63) {
            sysml_lazy_63 = io_0 == 7;
        }
        sysml_lazy_64 = sysml_lazy_63;
        if (!sysml_lazy_64) {
            sysml_lazy_64 = io_0 == 8;
        }
        sysml_lazy_65 = sysml_lazy_64;
        if (!sysml_lazy_65) {
            sysml_lazy_65 = io_0 == 9;
        }
        sysml_lazy_56 = sysml_lazy_65;
    }
    sysml_value_7 = sysml_lazy_56;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_8 = sysml_value_7;
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
  
  return false;
}
static bool hw_value_io_0_valid = false;
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static bool hw_value_io_8 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_4 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_5 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_6 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_7 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_8 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_0_valid = hw_rx_poll(hw_now, &hw_value_io_0);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_0_valid, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8);
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_8 ? HIGH : LOW);
}

