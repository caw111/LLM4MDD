#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_005\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_displayRoomIndex;
/* Global Variable define of  */
extern int32_t sysml_state_displayModeIndex;
/* Global Variable define of  */
extern int32_t sysml_state_reportAccumMs;
/* Global Variable define of  */
extern bool sysml_state_prevButton1;
/* Global Variable define of  */
extern bool sysml_state_prevButton2;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, double io_4, bool io_5, double io_6, double io_7, bool io_8, bool io_9, int32_t io_13, bool* io_10, bool* io_11, int32_t* io_12, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, double* serial_0_value_3, double* serial_0_value_4, double* serial_0_value_5, bool* serial_1_trigger, bool* serial_1_value_0, bool* serial_1_value_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_displayRoomIndex = 0;
/* Global Variable define of  */
int32_t sysml_state_displayModeIndex = 0;
/* Global Variable define of  */
int32_t sysml_state_reportAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_prevButton1 = false;
/* Global Variable define of  */
bool sysml_state_prevButton2 = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 374 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_005\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 395 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_005\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_005\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayRoomIndex = 0;
    sysml_state_displayModeIndex = 0;
    sysml_state_reportAccumMs = 0;
    sysml_state_prevButton1 = false;
    sysml_state_prevButton2 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, double io_4, bool io_5, double io_6, double io_7, bool io_8, bool io_9, int32_t io_13, bool* io_10, bool* io_11, int32_t* io_12, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, double* serial_0_value_3, double* serial_0_value_4, double* serial_0_value_5, bool* serial_1_trigger, bool* serial_1_value_0, bool* serial_1_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
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
    bool sysml_lazy_21 = 0;
    double sysml_conditional_22 = 0;
    double sysml_conditional_23 = 0;
    double sysml_conditional_24 = 0;
    double sysml_conditional_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_conditional_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_conditional_33 = 0;
    bool sysml_lazy_34 = 0;
    sysml_state_reportAccumMs = (sysml_state_reportAccumMs + io_13);
    sysml_lazy_0 = io_8;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevButton1;
    }
    if (sysml_lazy_0) {
        sysml_state_displayRoomIndex = (1 - sysml_state_displayRoomIndex);
    }
    sysml_lazy_1 = io_9;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevButton2;
    }
    if (sysml_lazy_1) {
        sysml_state_displayModeIndex = (1 - sysml_state_displayModeIndex);
    }
    if ((sysml_state_reportAccumMs >= 5000)) {
        sysml_state_reportAccumMs = (sysml_state_reportAccumMs - 5000);
    }
    sysml_state_prevButton1 = io_8;
    sysml_state_prevButton2 = io_9;
    bool sysml_value_0 = 0;
    sysml_lazy_2 = io_2;
    if (sysml_lazy_2) {
        sysml_lazy_3 = io_6 >= 600.0;
        if (sysml_lazy_3) {
            sysml_lazy_3 = io_0 >= 30.0;
        }
        sysml_lazy_2 = sysml_lazy_3;
    }
    if (sysml_lazy_2) {
        sysml_lazy_5 = io_0 >= 30.0;
        if (sysml_lazy_5) {
            sysml_lazy_5 = io_6 >= 600.0;
        }
        sysml_conditional_4 = sysml_lazy_5;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_0 = sysml_conditional_4;
    bool sysml_value_1 = 0;
    sysml_lazy_6 = io_5;
    if (sysml_lazy_6) {
        sysml_lazy_7 = io_7 >= 600.0;
        if (sysml_lazy_7) {
            sysml_lazy_7 = io_3 >= 30.0;
        }
        sysml_lazy_6 = sysml_lazy_7;
    }
    if (sysml_lazy_6) {
        sysml_lazy_9 = io_3 >= 30.0;
        if (sysml_lazy_9) {
            sysml_lazy_9 = io_7 >= 600.0;
        }
        sysml_conditional_8 = sysml_lazy_9;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_1 = sysml_conditional_8;
    int32_t sysml_value_2 = 0;
    if (sysml_state_displayRoomIndex == 0) {
        if (sysml_state_displayModeIndex == 0) {
            if (io_2) {
                if (io_0 < ((double)((int32_t)io_0))) {
                    sysml_conditional_13 = 1;
                }
                else {
                    sysml_conditional_13 = 0;
                }
                sysml_conditional_12 = ((int32_t)io_0) - sysml_conditional_13;
            }
            else {
                sysml_conditional_12 = 0;
            }
            sysml_conditional_11 = sysml_conditional_12;
        }
        else {
            if (io_2) {
                if (io_1 < ((double)((int32_t)io_1))) {
                    sysml_conditional_15 = 1;
                }
                else {
                    sysml_conditional_15 = 0;
                }
                sysml_conditional_14 = ((int32_t)io_1) - sysml_conditional_15;
            }
            else {
                sysml_conditional_14 = 0;
            }
            sysml_conditional_11 = sysml_conditional_14;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    else {
        if (sysml_state_displayModeIndex == 0) {
            if (io_5) {
                if (io_3 < ((double)((int32_t)io_3))) {
                    sysml_conditional_18 = 1;
                }
                else {
                    sysml_conditional_18 = 0;
                }
                sysml_conditional_17 = ((int32_t)io_3) - sysml_conditional_18;
            }
            else {
                sysml_conditional_17 = 0;
            }
            sysml_conditional_16 = sysml_conditional_17;
        }
        else {
            if (io_5) {
                if (io_4 < ((double)((int32_t)io_4))) {
                    sysml_conditional_20 = 1;
                }
                else {
                    sysml_conditional_20 = 0;
                }
                sysml_conditional_19 = ((int32_t)io_4) - sysml_conditional_20;
            }
            else {
                sysml_conditional_19 = 0;
            }
            sysml_conditional_16 = sysml_conditional_19;
        }
        sysml_conditional_10 = sysml_conditional_16;
    }
    sysml_value_2 = sysml_conditional_10;
    bool sysml_value_3 = 0;
    sysml_lazy_21 = sysml_state_reportAccumMs < 5000;
    if (sysml_lazy_21) {
        sysml_lazy_21 = (sysml_state_reportAccumMs + io_13) >= 5000;
    }
    sysml_value_3 = sysml_lazy_21;
    double sysml_value_4 = 0;
    if (io_2) {
        sysml_conditional_22 = io_0;
    }
    else {
        sysml_conditional_22 = 0.0;
    }
    sysml_value_4 = sysml_conditional_22;
    double sysml_value_5 = 0;
    if (io_2) {
        sysml_conditional_23 = io_1;
    }
    else {
        sysml_conditional_23 = 0.0;
    }
    sysml_value_5 = sysml_conditional_23;
    double sysml_value_6 = 0;
    sysml_value_6 = io_6;
    double sysml_value_7 = 0;
    if (io_5) {
        sysml_conditional_24 = io_3;
    }
    else {
        sysml_conditional_24 = 0.0;
    }
    sysml_value_7 = sysml_conditional_24;
    double sysml_value_8 = 0;
    if (io_5) {
        sysml_conditional_25 = io_4;
    }
    else {
        sysml_conditional_25 = 0.0;
    }
    sysml_value_8 = sysml_conditional_25;
    double sysml_value_9 = 0;
    sysml_value_9 = io_7;
    bool sysml_value_10 = 0;
    sysml_lazy_26 = io_2;
    if (sysml_lazy_26) {
        sysml_lazy_26 = io_0 >= 30.0;
    }
    sysml_lazy_27 = sysml_lazy_26;
    if (sysml_lazy_27) {
        sysml_lazy_27 = io_6 >= 600.0;
    }
    sysml_lazy_28 = sysml_lazy_27;
    if (!sysml_lazy_28) {
        sysml_lazy_29 = io_5;
        if (sysml_lazy_29) {
            sysml_lazy_29 = io_3 >= 30.0;
        }
        sysml_lazy_30 = sysml_lazy_29;
        if (sysml_lazy_30) {
            sysml_lazy_30 = io_7 >= 600.0;
        }
        sysml_lazy_28 = sysml_lazy_30;
    }
    sysml_value_10 = sysml_lazy_28;
    bool sysml_value_11 = 0;
    if (io_2) {
        sysml_lazy_32 = io_0 >= 30.0;
        if (sysml_lazy_32) {
            sysml_lazy_32 = io_6 >= 600.0;
        }
        sysml_conditional_31 = sysml_lazy_32;
    }
    else {
        sysml_conditional_31 = false;
    }
    sysml_value_11 = sysml_conditional_31;
    bool sysml_value_12 = 0;
    if (io_5) {
        sysml_lazy_34 = io_3 >= 30.0;
        if (sysml_lazy_34) {
            sysml_lazy_34 = io_7 >= 600.0;
        }
        sysml_conditional_33 = sysml_lazy_34;
    }
    else {
        sysml_conditional_33 = false;
    }
    sysml_value_12 = sysml_conditional_33;
    *io_10 = sysml_value_0;
    *io_11 = sysml_value_1;
    *io_12 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
    *serial_0_value_3 = sysml_value_7;
    *serial_0_value_4 = sysml_value_8;
    *serial_0_value_5 = sysml_value_9;
    *serial_1_trigger = sysml_value_10;
    *serial_1_value_0 = sysml_value_11;
    *serial_1_value_1 = sysml_value_12;
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
static double hw_value_io_0 = 0;
static double hw_value_io_1 = 0;
static bool hw_value_io_2 = false;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static bool hw_value_io_5 = false;
static double hw_value_io_6 = 0;
static uint32_t hw_sampled_io_6 = 0;
static bool hw_attempted_io_6 = false;
static double hw_value_io_7 = 0;
static uint32_t hw_sampled_io_7 = 0;
static bool hw_attempted_io_7 = false;
static bool hw_value_io_8 = false;
static uint32_t hw_sampled_io_8 = 0;
static bool hw_attempted_io_8 = false;
static bool hw_value_io_9 = false;
static uint32_t hw_sampled_io_9 = 0;
static bool hw_attempted_io_9 = false;
static bool hw_value_io_10 = false;
static bool hw_value_io_11 = false;
static int32_t hw_value_io_12 = 0;
TM1637Display hw_display_io_12(4, 5);
static int32_t hw_display_previous_io_12 = INT32_MIN;
static int32_t hw_value_io_13 = 0;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static double hw_value_serial_0_value_2 = 0;
static double hw_value_serial_0_value_3 = 0;
static double hw_value_serial_0_value_4 = 0;
static double hw_value_serial_0_value_5 = 0;
static bool hw_value_serial_1_trigger = false;
static bool hw_value_serial_1_value_0 = false;
static bool hw_value_serial_1_value_1 = false;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;
DHT hw_dht_1(8, DHT11);
static bool hw_dht_1_attempted = false;
static uint32_t hw_dht_1_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_10 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_11 ? HIGH : LOW);
  hw_display_io_12.setBrightness(5, true);
  if (hw_value_io_12 != hw_display_previous_io_12) {
    if (hw_value_io_12 < -999 || hw_value_io_12 > 9999) hw_display_io_12.clear();
    else hw_display_io_12.showNumberDec((int)hw_value_io_12, false, 4, 0);
    hw_display_previous_io_12 = hw_value_io_12;
  }
  hw_dht_0.begin();
  hw_dht_1.begin();
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_6 || (uint32_t)(hw_now - hw_sampled_io_6) >= 10UL) {
    hw_attempted_io_6 = true; hw_sampled_io_6 = hw_now;
    hw_value_io_6 = (analogRead(A0) * 1 + 0);
  }
  if (!hw_attempted_io_7 || (uint32_t)(hw_now - hw_sampled_io_7) >= 10UL) {
    hw_attempted_io_7 = true; hw_sampled_io_7 = hw_now;
    hw_value_io_7 = (analogRead(A1) * 1 + 0);
  }
  if (!hw_attempted_io_8 || (uint32_t)(hw_now - hw_sampled_io_8) >= 10UL) {
    hw_attempted_io_8 = true; hw_sampled_io_8 = hw_now;
    hw_value_io_8 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_9 || (uint32_t)(hw_now - hw_sampled_io_9) >= 10UL) {
    hw_attempted_io_9 = true; hw_sampled_io_9 = hw_now;
    hw_value_io_9 = (digitalRead(3) == HIGH);
  }
  uint32_t hw_dht_0_now = (uint32_t)millis();
  if (!hw_dht_0_attempted || (uint32_t)(hw_dht_0_now - hw_dht_0_sampled) >= 2000UL) {
    hw_dht_0_attempted = true; hw_dht_0_sampled = hw_dht_0_now;
    bool hw_dht_0_valid = hw_dht_0.read();
    double hw_dht_0_io_0 = hw_dht_0_valid ? hw_dht_0.readTemperature() : NAN;
    double hw_dht_0_io_1 = hw_dht_0_valid ? hw_dht_0.readHumidity() : NAN;
    hw_dht_0_valid = hw_dht_0_valid && isfinite(hw_dht_0_io_0) && isfinite(hw_dht_0_io_1);
    if (hw_dht_0_valid) {
      hw_value_io_0 = hw_dht_0_io_0;
      hw_value_io_1 = hw_dht_0_io_1;
    }
    hw_value_io_2 = hw_dht_0_valid;
  }
  uint32_t hw_dht_1_now = (uint32_t)millis();
  if (!hw_dht_1_attempted || (uint32_t)(hw_dht_1_now - hw_dht_1_sampled) >= 2000UL) {
    hw_dht_1_attempted = true; hw_dht_1_sampled = hw_dht_1_now;
    bool hw_dht_1_valid = hw_dht_1.read();
    double hw_dht_1_io_3 = hw_dht_1_valid ? hw_dht_1.readTemperature() : NAN;
    double hw_dht_1_io_4 = hw_dht_1_valid ? hw_dht_1.readHumidity() : NAN;
    hw_dht_1_valid = hw_dht_1_valid && isfinite(hw_dht_1_io_3) && isfinite(hw_dht_1_io_4);
    if (hw_dht_1_valid) {
      hw_value_io_3 = hw_dht_1_io_3;
      hw_value_io_4 = hw_dht_1_io_4;
    }
    hw_value_io_5 = hw_dht_1_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_13 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_5, hw_value_io_6, hw_value_io_7, hw_value_io_8, hw_value_io_9, hw_value_io_13, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2, &hw_value_serial_0_value_3, &hw_value_serial_0_value_4, &hw_value_serial_0_value_5, &hw_value_serial_1_trigger, &hw_value_serial_1_value_0, &hw_value_serial_1_value_1);
  digitalWrite(12, hw_value_io_10 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_11 ? HIGH : LOW);
  if (hw_value_io_12 != hw_display_previous_io_12) {
    if (hw_value_io_12 < -999 || hw_value_io_12 > 9999) hw_display_io_12.clear();
    else hw_display_io_12.showNumberDec((int)hw_value_io_12, false, 4, 0);
    hw_display_previous_io_12 = hw_value_io_12;
  }
  if (hw_value_serial_0_trigger) { Serial.print("living_room_temperature_c"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("living_room_humidity_percent"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("living_room_light_adc"); Serial.println(hw_value_serial_0_value_2); }
  if (hw_value_serial_0_trigger) { Serial.print("bedroom_temperature_c"); Serial.println(hw_value_serial_0_value_3); }
  if (hw_value_serial_0_trigger) { Serial.print("bedroom_humidity_percent"); Serial.println(hw_value_serial_0_value_4); }
  if (hw_value_serial_0_trigger) { Serial.print("bedroom_light_adc"); Serial.println(hw_value_serial_0_value_5); }
  if (hw_value_serial_1_trigger) { Serial.print("living_room_alarm"); Serial.println(hw_value_serial_1_value_0); }
  if (hw_value_serial_1_trigger) { Serial.print("bedroom_alarm"); Serial.println(hw_value_serial_1_value_1); }
}

