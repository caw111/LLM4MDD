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
extern int32_t sysml_state_roomIndex;
/* Global Variable define of  */
extern int32_t sysml_state_displayMode;
/* Global Variable define of  */
extern bool sysml_state_prevBtnRoom;
/* Global Variable define of  */
extern bool sysml_state_prevBtnMode;
/* Global Variable define of  */
extern int32_t sysml_state_serialAccumMs;
/* Global Variable define of  */
extern bool sysml_state_nextSerialReady;
/* Global Variable define of  */
extern int32_t sysml_state_tm1637NumberLatched;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, double io_3, bool io_4, double io_5, double io_6, bool io_7, bool io_8, bool io_9, int32_t io_13, bool* io_10, bool* io_11, int32_t* io_12, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, double* serial_0_value_3, double* serial_0_value_4, double* serial_0_value_5, bool* serial_0_value_6, bool* serial_0_value_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_roomIndex = 0;
/* Global Variable define of  */
int32_t sysml_state_displayMode = 0;
/* Global Variable define of  */
bool sysml_state_prevBtnRoom = false;
/* Global Variable define of  */
bool sysml_state_prevBtnMode = false;
/* Global Variable define of  */
int32_t sysml_state_serialAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_nextSerialReady = false;
/* Global Variable define of  */
int32_t sysml_state_tm1637NumberLatched = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_roomIndex = 0;
    sysml_state_displayMode = 0;
    sysml_state_prevBtnRoom = false;
    sysml_state_prevBtnMode = false;
    sysml_state_serialAccumMs = 0;
    sysml_state_nextSerialReady = false;
    sysml_state_tm1637NumberLatched = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, double io_3, bool io_4, double io_5, double io_6, bool io_7, bool io_8, bool io_9, int32_t io_13, bool* io_10, bool* io_11, int32_t* io_12, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, double* serial_0_value_3, double* serial_0_value_4, double* serial_0_value_5, bool* serial_0_value_6, bool* serial_0_value_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    double sysml_conditional_21 = 0;
    double sysml_conditional_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    sysml_lazy_0 = io_8;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtnRoom;
    }
    if (sysml_lazy_0) {
        if (sysml_state_roomIndex == 0) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_state_roomIndex = sysml_conditional_1;
    }
    sysml_lazy_2 = io_9;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevBtnMode;
    }
    if (sysml_lazy_2) {
        if (sysml_state_displayMode == 0) {
            sysml_conditional_3 = 1;
        }
        else {
            if (sysml_state_displayMode == 1) {
                sysml_conditional_4 = 2;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_conditional_3 = sysml_conditional_4;
        }
        sysml_state_displayMode = sysml_conditional_3;
    }
    sysml_state_prevBtnRoom = io_8;
    sysml_state_prevBtnMode = io_9;
    sysml_state_serialAccumMs = (sysml_state_serialAccumMs + io_13);
    if ((sysml_state_serialAccumMs >= 5000)) {
        sysml_state_serialAccumMs = (sysml_state_serialAccumMs - 5000);
        sysml_state_nextSerialReady = true;
    }
    else {
        sysml_state_nextSerialReady = false;
    }
    if ((sysml_state_roomIndex == 0)) {
        if (sysml_state_displayMode == 0) {
            if (io_2 < ((double)((int32_t)io_2))) {
                sysml_conditional_6 = 1;
            }
            else {
                sysml_conditional_6 = 0;
            }
            sysml_conditional_5 = ((int32_t)io_2) - sysml_conditional_6;
        }
        else {
            if (sysml_state_displayMode == 1) {
                if (io_3 < ((double)((int32_t)io_3))) {
                    sysml_conditional_8 = 1;
                }
                else {
                    sysml_conditional_8 = 0;
                }
                sysml_conditional_7 = ((int32_t)io_3) - sysml_conditional_8;
            }
            else {
                if (io_0 < ((double)((int32_t)io_0))) {
                    sysml_conditional_9 = 1;
                }
                else {
                    sysml_conditional_9 = 0;
                }
                sysml_conditional_7 = ((int32_t)io_0) - sysml_conditional_9;
            }
            sysml_conditional_5 = sysml_conditional_7;
        }
        sysml_state_tm1637NumberLatched = sysml_conditional_5;
    }
    else {
        if (sysml_state_displayMode == 0) {
            if (io_5 < ((double)((int32_t)io_5))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_10 = ((int32_t)io_5) - sysml_conditional_11;
        }
        else {
            if (sysml_state_displayMode == 1) {
                if (io_6 < ((double)((int32_t)io_6))) {
                    sysml_conditional_13 = 1;
                }
                else {
                    sysml_conditional_13 = 0;
                }
                sysml_conditional_12 = ((int32_t)io_6) - sysml_conditional_13;
            }
            else {
                if (io_1 < ((double)((int32_t)io_1))) {
                    sysml_conditional_14 = 1;
                }
                else {
                    sysml_conditional_14 = 0;
                }
                sysml_conditional_12 = ((int32_t)io_1) - sysml_conditional_14;
            }
            sysml_conditional_10 = sysml_conditional_12;
        }
        sysml_state_tm1637NumberLatched = sysml_conditional_10;
    }
    bool sysml_value_0 = 0;
    if (io_4) {
        sysml_lazy_16 = io_0 >= 700.0;
        if (sysml_lazy_16) {
            sysml_lazy_16 = io_2 >= 30.0;
        }
        sysml_conditional_15 = sysml_lazy_16;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_0 = sysml_conditional_15;
    bool sysml_value_1 = 0;
    if (io_7) {
        sysml_lazy_18 = io_1 >= 700.0;
        if (sysml_lazy_18) {
            sysml_lazy_18 = io_5 >= 30.0;
        }
        sysml_conditional_17 = sysml_lazy_18;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_1 = sysml_conditional_17;
    int32_t sysml_value_2 = 0;
    sysml_value_2 = sysml_state_tm1637NumberLatched;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_nextSerialReady;
    double sysml_value_4 = 0;
    if (io_4) {
        sysml_conditional_19 = io_2;
    }
    else {
        sysml_conditional_19 = io_2;
    }
    sysml_value_4 = sysml_conditional_19;
    double sysml_value_5 = 0;
    if (io_4) {
        sysml_conditional_20 = io_3;
    }
    else {
        sysml_conditional_20 = io_3;
    }
    sysml_value_5 = sysml_conditional_20;
    double sysml_value_6 = 0;
    sysml_value_6 = io_0;
    double sysml_value_7 = 0;
    if (io_7) {
        sysml_conditional_21 = io_5;
    }
    else {
        sysml_conditional_21 = io_5;
    }
    sysml_value_7 = sysml_conditional_21;
    double sysml_value_8 = 0;
    if (io_7) {
        sysml_conditional_22 = io_6;
    }
    else {
        sysml_conditional_22 = io_6;
    }
    sysml_value_8 = sysml_conditional_22;
    double sysml_value_9 = 0;
    sysml_value_9 = io_1;
    bool sysml_value_10 = 0;
    sysml_lazy_23 = io_4;
    if (sysml_lazy_23) {
        sysml_lazy_24 = io_2 >= 30.0;
        if (sysml_lazy_24) {
            sysml_lazy_24 = io_0 >= 700.0;
        }
        sysml_lazy_23 = sysml_lazy_24;
    }
    sysml_value_10 = sysml_lazy_23;
    bool sysml_value_11 = 0;
    sysml_lazy_25 = io_7;
    if (sysml_lazy_25) {
        sysml_lazy_26 = io_5 >= 30.0;
        if (sysml_lazy_26) {
            sysml_lazy_26 = io_1 >= 700.0;
        }
        sysml_lazy_25 = sysml_lazy_26;
    }
    sysml_value_11 = sysml_lazy_25;
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
static double hw_value_io_0 = 0;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static double hw_value_io_3 = 0;
static bool hw_value_io_4 = false;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
static bool hw_value_io_7 = false;
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
static bool hw_value_serial_0_value_6 = false;
static bool hw_value_serial_0_value_7 = false;
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
  hw_display_io_12.setBrightness(7, true);
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
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A0) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A1) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_8 || (uint32_t)(hw_now - hw_sampled_io_8) >= 10UL) {
    hw_attempted_io_8 = true; hw_sampled_io_8 = hw_now;
    hw_value_io_8 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_9 || (uint32_t)(hw_now - hw_sampled_io_9) >= 10UL) {
    hw_attempted_io_9 = true; hw_sampled_io_9 = hw_now;
    hw_value_io_9 = (digitalRead(3) == LOW);
  }
  uint32_t hw_dht_0_now = (uint32_t)millis();
  if (!hw_dht_0_attempted || (uint32_t)(hw_dht_0_now - hw_dht_0_sampled) >= 2000UL) {
    hw_dht_0_attempted = true; hw_dht_0_sampled = hw_dht_0_now;
    bool hw_dht_0_valid = hw_dht_0.read();
    double hw_dht_0_io_2 = hw_dht_0_valid ? hw_dht_0.readTemperature() : NAN;
    double hw_dht_0_io_3 = hw_dht_0_valid ? hw_dht_0.readHumidity() : NAN;
    hw_dht_0_valid = hw_dht_0_valid && isfinite(hw_dht_0_io_2) && isfinite(hw_dht_0_io_3);
    if (hw_dht_0_valid) {
      hw_value_io_2 = hw_dht_0_io_2;
      hw_value_io_3 = hw_dht_0_io_3;
    }
    hw_value_io_4 = hw_dht_0_valid;
  }
  uint32_t hw_dht_1_now = (uint32_t)millis();
  if (!hw_dht_1_attempted || (uint32_t)(hw_dht_1_now - hw_dht_1_sampled) >= 2000UL) {
    hw_dht_1_attempted = true; hw_dht_1_sampled = hw_dht_1_now;
    bool hw_dht_1_valid = hw_dht_1.read();
    double hw_dht_1_io_5 = hw_dht_1_valid ? hw_dht_1.readTemperature() : NAN;
    double hw_dht_1_io_6 = hw_dht_1_valid ? hw_dht_1.readHumidity() : NAN;
    hw_dht_1_valid = hw_dht_1_valid && isfinite(hw_dht_1_io_5) && isfinite(hw_dht_1_io_6);
    if (hw_dht_1_valid) {
      hw_value_io_5 = hw_dht_1_io_5;
      hw_value_io_6 = hw_dht_1_io_6;
    }
    hw_value_io_7 = hw_dht_1_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_13 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_5, hw_value_io_6, hw_value_io_7, hw_value_io_8, hw_value_io_9, hw_value_io_13, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2, &hw_value_serial_0_value_3, &hw_value_serial_0_value_4, &hw_value_serial_0_value_5, &hw_value_serial_0_value_6, &hw_value_serial_0_value_7);
  digitalWrite(12, hw_value_io_10 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_11 ? HIGH : LOW);
  if (hw_value_io_12 != hw_display_previous_io_12) {
    if (hw_value_io_12 < -999 || hw_value_io_12 > 9999) hw_display_io_12.clear();
    else hw_display_io_12.showNumberDec((int)hw_value_io_12, false, 4, 0);
    hw_display_previous_io_12 = hw_value_io_12;
  }
  if (hw_value_serial_0_trigger) { Serial.print("living_room_temperature_c"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("living_room_humidity_rh"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("living_room_light_adc"); Serial.println(hw_value_serial_0_value_2); }
  if (hw_value_serial_0_trigger) { Serial.print("bedroom_temperature_c"); Serial.println(hw_value_serial_0_value_3); }
  if (hw_value_serial_0_trigger) { Serial.print("bedroom_humidity_rh"); Serial.println(hw_value_serial_0_value_4); }
  if (hw_value_serial_0_trigger) { Serial.print("bedroom_light_adc"); Serial.println(hw_value_serial_0_value_5); }
  if (hw_value_serial_0_trigger) { Serial.print("living_room_alarm_active"); Serial.println(hw_value_serial_0_value_6); }
  if (hw_value_serial_0_trigger) { Serial.print("bedroom_alarm_active"); Serial.println(hw_value_serial_0_value_7); }
}
