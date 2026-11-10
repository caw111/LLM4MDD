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
extern int32_t sysml_state_displayMode;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern int32_t sysml_state_serialAccMs;
/* Global Variable define of  */
extern bool sysml_state_dhtTempValidLatched;
/* Global Variable define of  */
extern bool sysml_state_dhtHumValidLatched;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, double io_3, bool io_4, int32_t io_6, int32_t* io_5, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, bool* serial_1_trigger, bool* serial_1_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_displayMode = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int32_t sysml_state_serialAccMs = 0;
/* Global Variable define of  */
bool sysml_state_dhtTempValidLatched = false;
/* Global Variable define of  */
bool sysml_state_dhtHumValidLatched = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayMode = 0;
    sysml_state_previousPressed = false;
    sysml_state_serialAccMs = 0;
    sysml_state_dhtTempValidLatched = false;
    sysml_state_dhtHumValidLatched = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, double io_3, bool io_4, int32_t io_6, int32_t* io_5, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, bool* serial_1_trigger, bool* serial_1_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
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
    int32_t sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    double sysml_conditional_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_conditional_26 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_displayMode == 0) {
            sysml_conditional_1 = 1;
        }
        else {
            if (sysml_state_displayMode == 1) {
                sysml_conditional_2 = 2;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_state_displayMode = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_1;
    sysml_state_serialAccMs = (sysml_state_serialAccMs + io_6);
    if ((sysml_state_serialAccMs >= 2000)) {
        sysml_state_serialAccMs = (sysml_state_serialAccMs - 2000);
    }
    if (io_4) {
        sysml_state_dhtTempValidLatched = true;
        sysml_state_dhtHumValidLatched = true;
    }
    int32_t sysml_value_0 = 0;
    if (sysml_state_displayMode == 0) {
        if (io_0 < 0.0) {
            sysml_conditional_4 = 0;
        }
        else {
            if (io_0 > 2147483647.0) {
                sysml_conditional_5 = 2147483647;
            }
            else {
                if (io_0 < ((double)((int32_t)io_0))) {
                    sysml_conditional_6 = 1;
                }
                else {
                    sysml_conditional_6 = 0;
                }
                sysml_conditional_5 = ((int32_t)io_0) - sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        if (sysml_state_displayMode == 1) {
            if (sysml_state_dhtTempValidLatched) {
                if (io_2 < 0.0) {
                    sysml_conditional_9 = 0;
                }
                else {
                    if (io_2 > 2147483647.0) {
                        sysml_conditional_10 = 2147483647;
                    }
                    else {
                        if (io_2 < ((double)((int32_t)io_2))) {
                            sysml_conditional_11 = 1;
                        }
                        else {
                            sysml_conditional_11 = 0;
                        }
                        sysml_conditional_10 = ((int32_t)io_2) - sysml_conditional_11;
                    }
                    sysml_conditional_9 = sysml_conditional_10;
                }
                sysml_conditional_8 = sysml_conditional_9;
            }
            else {
                sysml_conditional_8 = 0;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        else {
            if (sysml_state_dhtHumValidLatched) {
                if (io_3 < 0.0) {
                    sysml_conditional_13 = 0;
                }
                else {
                    if (io_3 > 2147483647.0) {
                        sysml_conditional_14 = 2147483647;
                    }
                    else {
                        if (io_3 < ((double)((int32_t)io_3))) {
                            sysml_conditional_15 = 1;
                        }
                        else {
                            sysml_conditional_15 = 0;
                        }
                        sysml_conditional_14 = ((int32_t)io_3) - sysml_conditional_15;
                    }
                    sysml_conditional_13 = sysml_conditional_14;
                }
                sysml_conditional_12 = sysml_conditional_13;
            }
            else {
                sysml_conditional_12 = 0;
            }
            sysml_conditional_7 = sysml_conditional_12;
        }
        sysml_conditional_3 = sysml_conditional_7;
    }
    sysml_value_0 = sysml_conditional_3;
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_serialAccMs >= 0);
    double sysml_value_2 = 0;
    if (sysml_state_serialAccMs >= 0) {
        sysml_conditional_16 = io_0;
    }
    else {
        sysml_conditional_16 = 0.0;
    }
    sysml_value_2 = sysml_conditional_16;
    double sysml_value_3 = 0;
    if (sysml_state_serialAccMs >= 0) {
        if (sysml_state_dhtTempValidLatched) {
            sysml_conditional_18 = io_2;
        }
        else {
            sysml_conditional_18 = 0.0;
        }
        sysml_conditional_17 = sysml_conditional_18;
    }
    else {
        sysml_conditional_17 = 0.0;
    }
    sysml_value_3 = sysml_conditional_17;
    double sysml_value_4 = 0;
    if (sysml_state_serialAccMs >= 0) {
        if (sysml_state_dhtHumValidLatched) {
            sysml_conditional_20 = io_3;
        }
        else {
            sysml_conditional_20 = 0.0;
        }
        sysml_conditional_19 = sysml_conditional_20;
    }
    else {
        sysml_conditional_19 = 0.0;
    }
    sysml_value_4 = sysml_conditional_19;
    bool sysml_value_5 = 0;
    sysml_lazy_21 = !sysml_state_dhtTempValidLatched;
    if (!sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_dhtTempValidLatched;
    }
    sysml_lazy_22 = sysml_lazy_21;
    if (sysml_lazy_22) {
        sysml_lazy_23 = !sysml_state_dhtHumValidLatched;
        if (!sysml_lazy_23) {
            sysml_lazy_23 = sysml_state_dhtHumValidLatched;
        }
        sysml_lazy_22 = sysml_lazy_23;
    }
    sysml_lazy_24 = sysml_lazy_22;
    if (sysml_lazy_24) {
        sysml_lazy_25 = io_0 < 300.0;
        if (sysml_lazy_25) {
            sysml_lazy_25 = io_2 > 30.0;
        }
        sysml_lazy_24 = sysml_lazy_25;
    }
    sysml_value_5 = sysml_lazy_24;
    bool sysml_value_6 = 0;
    if (sysml_value_5) {
        sysml_conditional_26 = true;
    }
    else {
        sysml_conditional_26 = false;
    }
    sysml_value_6 = sysml_conditional_26;
    *io_5 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
    *serial_1_trigger = sysml_value_5;
    *serial_1_value_0 = sysml_value_6;
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
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static double hw_value_io_3 = 0;
static bool hw_value_io_4 = false;
static int32_t hw_value_io_5 = 0;
TM1637Display hw_display_io_5(4, 5);
static int32_t hw_display_previous_io_5 = INT32_MIN;
static int32_t hw_value_io_6 = 0;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static double hw_value_serial_0_value_2 = 0;
static bool hw_value_serial_1_trigger = false;
static bool hw_value_serial_1_value_0 = false;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(2, INPUT);
  hw_display_io_5.setBrightness(7, true);
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
  hw_dht_0.begin();
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
    hw_value_io_1 = (digitalRead(2) == HIGH);
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
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_6 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_6, &hw_value_io_5, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2, &hw_value_serial_1_trigger, &hw_value_serial_1_value_0);
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
  if (hw_value_serial_0_trigger) { Serial.print("light"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("temperature"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("humidity"); Serial.println(hw_value_serial_0_value_2); }
  if (hw_value_serial_1_trigger) { Serial.print("environment_alarm"); Serial.println(hw_value_serial_1_value_0); }
}
