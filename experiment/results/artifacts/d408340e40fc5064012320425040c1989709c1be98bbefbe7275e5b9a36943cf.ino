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
extern bool sysml_state_modeIsAuto;
/* Global Variable define of  */
extern bool sysml_state_manualTargetIsFan;
/* Global Variable define of  */
extern bool sysml_state_previousD2Pressed;
/* Global Variable define of  */
extern bool sysml_state_previousD3Pressed;
/* Global Variable define of  */
extern double sysml_state_fanPwmManual;
/* Global Variable define of  */
extern bool sysml_state_ledOnManual;
/* Global Variable define of  */
extern bool sysml_state_pumpOnManual;
/* Global Variable define of  */
extern int32_t sysml_state_tmItemIndex;
/* Global Variable define of  */
extern int32_t sysml_state_tmElapsedMs;
/* Global Variable define of  */
extern double sysml_state_lastTempValid;
/* Global Variable define of  */
extern double sysml_state_lastHumValid;
/* Global Variable define of  */
extern double sysml_state_lastIo3;
/* Global Variable define of  */
extern double sysml_state_prevSoilHumValid;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, double io_4, double io_5, bool io_11, bool io_12, double* io_6, bool* io_7, bool* io_8, int32_t* io_9, int32_t* io_10, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_1_trigger, double* serial_1_value_0, double* serial_1_value_1, double* serial_1_value_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeIsAuto = true;
/* Global Variable define of  */
bool sysml_state_manualTargetIsFan = true;
/* Global Variable define of  */
bool sysml_state_previousD2Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousD3Pressed = false;
/* Global Variable define of  */
double sysml_state_fanPwmManual = 0.0;
/* Global Variable define of  */
bool sysml_state_ledOnManual = false;
/* Global Variable define of  */
bool sysml_state_pumpOnManual = false;
/* Global Variable define of  */
int32_t sysml_state_tmItemIndex = 0;
/* Global Variable define of  */
int32_t sysml_state_tmElapsedMs = 0;
/* Global Variable define of  */
double sysml_state_lastTempValid = 0.0;
/* Global Variable define of  */
double sysml_state_lastHumValid = 0.0;
/* Global Variable define of  */
double sysml_state_lastIo3 = 0.0;
/* Global Variable define of  */
double sysml_state_prevSoilHumValid = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeIsAuto = true;
    sysml_state_manualTargetIsFan = true;
    sysml_state_previousD2Pressed = false;
    sysml_state_previousD3Pressed = false;
    sysml_state_fanPwmManual = 0.0;
    sysml_state_ledOnManual = false;
    sysml_state_pumpOnManual = false;
    sysml_state_tmItemIndex = 0;
    sysml_state_tmElapsedMs = 0;
    sysml_state_lastTempValid = 0.0;
    sysml_state_lastHumValid = 0.0;
    sysml_state_lastIo3 = 0.0;
    sysml_state_prevSoilHumValid = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, double io_4, double io_5, bool io_11, bool io_12, double* io_6, bool* io_7, bool* io_8, int32_t* io_9, int32_t* io_10, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_1_trigger, double* serial_1_value_0, double* serial_1_value_1, double* serial_1_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    double sysml_conditional_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_conditional_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_conditional_27 = 0;
    bool sysml_conditional_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_conditional_30 = 0;
    bool sysml_lazy_31 = 0;
    bool sysml_lazy_32 = 0;
    int32_t sysml_conditional_33 = 0;
    int32_t sysml_conditional_34 = 0;
    int32_t sysml_conditional_35 = 0;
    int32_t sysml_conditional_36 = 0;
    int32_t sysml_conditional_37 = 0;
    int32_t sysml_conditional_38 = 0;
    bool sysml_lazy_39 = 0;
    bool sysml_lazy_40 = 0;
    bool sysml_lazy_41 = 0;
    bool sysml_lazy_42 = 0;
    double sysml_conditional_43 = 0;
    if (io_2) {
        sysml_state_lastTempValid = io_0;
        sysml_state_lastHumValid = io_1;
    }
    if (true) {
        sysml_state_lastIo3 = io_3;
        sysml_state_prevSoilHumValid = io_4;
    }
    sysml_lazy_0 = io_11;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousD2Pressed;
    }
    if (sysml_lazy_0) {
        sysml_state_modeIsAuto = (!sysml_state_modeIsAuto);
    }
    sysml_lazy_1 = io_12;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousD3Pressed;
    }
    if (sysml_lazy_1) {
        sysml_state_manualTargetIsFan = (!sysml_state_manualTargetIsFan);
    }
    sysml_state_previousD2Pressed = io_11;
    sysml_state_previousD3Pressed = io_12;
    if (sysml_state_manualTargetIsFan) {
        if (io_5 < 0.0) {
            sysml_conditional_3 = 0.0;
        }
        else {
            if (io_5 > 1023.0) {
                sysml_conditional_4 = 1023.0;
            }
            else {
                sysml_conditional_4 = io_5;
            }
            sysml_conditional_3 = sysml_conditional_4;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    else {
        sysml_conditional_2 = 0.0;
    }
    sysml_state_fanPwmManual = sysml_conditional_2;
    if (sysml_state_manualTargetIsFan) {
        sysml_conditional_5 = false;
    }
    else {
        if (io_5 >= 512.0) {
            sysml_conditional_6 = true;
        }
        else {
            sysml_conditional_6 = false;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_state_ledOnManual = sysml_conditional_5;
    if (sysml_state_manualTargetIsFan) {
        sysml_conditional_7 = false;
    }
    else {
        if (io_5 >= 256.0) {
            sysml_conditional_8 = true;
        }
        else {
            sysml_conditional_8 = false;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    sysml_state_pumpOnManual = sysml_conditional_7;
    sysml_lazy_9 = !sysml_state_modeIsAuto;
    if (sysml_lazy_9) {
        sysml_lazy_9 = true;
    }
    if (sysml_lazy_9) {
        sysml_state_tmItemIndex = sysml_state_tmItemIndex;
    }
    if (true) {
        sysml_state_tmElapsedMs = (sysml_state_tmElapsedMs + 10);
        if (sysml_state_tmElapsedMs >= 2000) {
            if (sysml_state_tmItemIndex == 2) {
                sysml_conditional_11 = 0;
            }
            else {
                sysml_conditional_11 = sysml_state_tmItemIndex + 1;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        else {
            sysml_conditional_10 = sysml_state_tmItemIndex;
        }
        sysml_state_tmItemIndex = sysml_conditional_10;
        if (sysml_state_tmElapsedMs >= 2000) {
            sysml_conditional_12 = 0;
        }
        else {
            sysml_conditional_12 = sysml_state_tmElapsedMs;
        }
        sysml_state_tmElapsedMs = sysml_conditional_12;
    }
    if (true) {
        sysml_state_tmItemIndex = sysml_state_tmItemIndex;
    }
    double sysml_value_0 = 0;
    sysml_lazy_13 = sysml_state_modeIsAuto;
    if (sysml_lazy_13) {
        sysml_lazy_13 = io_2;
    }
    if (sysml_lazy_13) {
        if (sysml_state_lastTempValid >= 30.0) {
            if (sysml_state_lastTempValid >= 40.0) {
                sysml_conditional_16 = 1023.0;
            }
            else {
                sysml_conditional_16 = ((double)((sysml_state_lastTempValid - 30.0) * 1023.0)) / 10.0;
            }
            sysml_conditional_15 = sysml_conditional_16;
        }
        else {
            sysml_conditional_15 = 0.0;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    else {
        sysml_lazy_17 = !sysml_state_modeIsAuto;
        if (sysml_lazy_17) {
            sysml_lazy_17 = sysml_state_manualTargetIsFan;
        }
        if (sysml_lazy_17) {
            if (sysml_state_fanPwmManual < 0.0) {
                sysml_conditional_19 = 0.0;
            }
            else {
                if (sysml_state_fanPwmManual > 1023.0) {
                    sysml_conditional_20 = 1023.0;
                }
                else {
                    sysml_conditional_20 = sysml_state_fanPwmManual;
                }
                sysml_conditional_19 = sysml_conditional_20;
            }
            sysml_conditional_18 = sysml_conditional_19;
        }
        else {
            sysml_conditional_18 = 0.0;
        }
        sysml_conditional_14 = sysml_conditional_18;
    }
    sysml_value_0 = sysml_conditional_14;
    bool sysml_value_1 = 0;
    sysml_lazy_21 = sysml_state_modeIsAuto;
    if (sysml_lazy_21) {
        sysml_lazy_21 = io_2;
    }
    if (sysml_lazy_21) {
        if (sysml_state_lastIo3 >= 600.0) {
            sysml_conditional_23 = true;
        }
        else {
            sysml_conditional_23 = false;
        }
        sysml_conditional_22 = sysml_conditional_23;
    }
    else {
        sysml_lazy_24 = !sysml_state_modeIsAuto;
        if (sysml_lazy_24) {
            sysml_lazy_24 = !sysml_state_manualTargetIsFan;
        }
        if (sysml_lazy_24) {
            sysml_conditional_25 = sysml_state_ledOnManual;
        }
        else {
            sysml_conditional_25 = false;
        }
        sysml_conditional_22 = sysml_conditional_25;
    }
    sysml_value_1 = sysml_conditional_22;
    bool sysml_value_2 = 0;
    sysml_lazy_26 = sysml_state_modeIsAuto;
    if (sysml_lazy_26) {
        sysml_lazy_26 = io_2;
    }
    if (sysml_lazy_26) {
        if (sysml_state_prevSoilHumValid <= 400.0) {
            sysml_conditional_28 = true;
        }
        else {
            sysml_conditional_28 = false;
        }
        sysml_conditional_27 = sysml_conditional_28;
    }
    else {
        sysml_lazy_29 = !sysml_state_modeIsAuto;
        if (sysml_lazy_29) {
            sysml_lazy_29 = !sysml_state_manualTargetIsFan;
        }
        if (sysml_lazy_29) {
            sysml_conditional_30 = sysml_state_pumpOnManual;
        }
        else {
            sysml_conditional_30 = false;
        }
        sysml_conditional_27 = sysml_conditional_30;
    }
    sysml_value_2 = sysml_conditional_27;
    int32_t sysml_value_3 = 0;
    sysml_lazy_31 = !io_2;
    if (!sysml_lazy_31) {
        sysml_lazy_31 = io_3 < 0.0;
    }
    sysml_lazy_32 = sysml_lazy_31;
    if (!sysml_lazy_32) {
        sysml_lazy_32 = io_4 < 0.0;
    }
    if (sysml_lazy_32) {
        sysml_conditional_33 = 1000;
    }
    else {
        sysml_conditional_33 = 0;
    }
    sysml_value_3 = sysml_conditional_33;
    int32_t sysml_value_4 = 0;
    if (sysml_state_tmItemIndex == 0) {
        if (sysml_state_lastTempValid < ((double)((int32_t)sysml_state_lastTempValid))) {
            sysml_conditional_35 = 1;
        }
        else {
            sysml_conditional_35 = 0;
        }
        sysml_conditional_34 = ((int32_t)sysml_state_lastTempValid) - sysml_conditional_35;
    }
    else {
        if (sysml_state_tmItemIndex == 1) {
            if (sysml_state_lastIo3 < ((double)((int32_t)sysml_state_lastIo3))) {
                sysml_conditional_37 = 1;
            }
            else {
                sysml_conditional_37 = 0;
            }
            sysml_conditional_36 = ((int32_t)sysml_state_lastIo3) - sysml_conditional_37;
        }
        else {
            if (sysml_state_prevSoilHumValid < ((double)((int32_t)sysml_state_prevSoilHumValid))) {
                sysml_conditional_38 = 1;
            }
            else {
                sysml_conditional_38 = 0;
            }
            sysml_conditional_36 = ((int32_t)sysml_state_prevSoilHumValid) - sysml_conditional_38;
        }
        sysml_conditional_34 = sysml_conditional_36;
    }
    sysml_value_4 = sysml_conditional_34;
    bool sysml_value_5 = 0;
    sysml_lazy_39 = !io_2;
    if (!sysml_lazy_39) {
        sysml_lazy_39 = io_3 < 0.0;
    }
    sysml_lazy_40 = sysml_lazy_39;
    if (!sysml_lazy_40) {
        sysml_lazy_40 = io_4 < 0.0;
    }
    sysml_value_5 = sysml_lazy_40;
    bool sysml_value_6 = 0;
    sysml_lazy_41 = !io_2;
    if (!sysml_lazy_41) {
        sysml_lazy_41 = io_3 < 0.0;
    }
    sysml_lazy_42 = sysml_lazy_41;
    if (!sysml_lazy_42) {
        sysml_lazy_42 = io_4 < 0.0;
    }
    sysml_value_6 = sysml_lazy_42;
    bool sysml_value_7 = 0;
    sysml_value_7 = io_2;
    double sysml_value_8 = 0;
    if (io_2) {
        sysml_conditional_43 = sysml_state_lastTempValid;
    }
    else {
        sysml_conditional_43 = sysml_state_lastTempValid;
    }
    sysml_value_8 = sysml_conditional_43;
    double sysml_value_9 = 0;
    sysml_value_9 = sysml_state_lastIo3;
    double sysml_value_10 = 0;
    sysml_value_10 = sysml_state_prevSoilHumValid;
    *io_6 = sysml_value_0;
    *io_7 = sysml_value_1;
    *io_8 = sysml_value_2;
    *io_9 = sysml_value_3;
    *io_10 = sysml_value_4;
    *serial_0_trigger = sysml_value_5;
    *serial_0_value_0 = sysml_value_6;
    *serial_1_trigger = sysml_value_7;
    *serial_1_value_0 = sysml_value_8;
    *serial_1_value_1 = sysml_value_9;
    *serial_1_value_2 = sysml_value_10;
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
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static double hw_value_io_4 = 0;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static double hw_value_io_5 = 0;
static uint32_t hw_sampled_io_5 = 0;
static bool hw_attempted_io_5 = false;
static double hw_value_io_6 = 0;
static bool hw_value_io_7 = false;
static bool hw_value_io_8 = false;
static int32_t hw_value_io_9 = 0;
static int32_t hw_tone_io_9 = -1;
static int32_t hw_value_io_10 = 0;
TM1637Display hw_display_io_10(4, 8);
static int32_t hw_display_previous_io_10 = INT32_MIN;
static bool hw_value_io_11 = false;
static uint32_t hw_sampled_io_11 = 0;
static bool hw_attempted_io_11 = false;
static bool hw_value_io_12 = false;
static uint32_t hw_sampled_io_12 = 0;
static bool hw_attempted_io_12 = false;
static bool hw_value_serial_0_trigger = false;
static bool hw_value_serial_0_value_0 = false;
static bool hw_value_serial_1_trigger = false;
static double hw_value_serial_1_value_0 = 0;
static double hw_value_serial_1_value_1 = 0;
static double hw_value_serial_1_value_2 = 0;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_6 * 1 + 0), 0.0, 255.0));
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_7 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_8 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_9 = (hw_value_io_9 >= 31 && hw_value_io_9 <= 65535) ? hw_value_io_9 : 0;
  if (hw_tone_command_io_9 != hw_tone_io_9) {
    if (hw_tone_command_io_9 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_9);
    hw_tone_io_9 = hw_tone_command_io_9;
  }
  hw_display_io_10.setBrightness(7, true);
  if (hw_value_io_10 != hw_display_previous_io_10) {
    if (hw_value_io_10 < -999 || hw_value_io_10 > 9999) hw_display_io_10.clear();
    else hw_display_io_10.showNumberDec((int)hw_value_io_10, false, 4, 0);
    hw_display_previous_io_10 = hw_value_io_10;
  }
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  hw_dht_0.begin();
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (analogRead(A0) * 1 + 0);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 10UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (analogRead(A1) * 1 + 0);
  }
  if (!hw_attempted_io_5 || (uint32_t)(hw_now - hw_sampled_io_5) >= 10UL) {
    hw_attempted_io_5 = true; hw_sampled_io_5 = hw_now;
    hw_value_io_5 = (analogRead(A2) * 1 + 0);
  }
  if (!hw_attempted_io_11 || (uint32_t)(hw_now - hw_sampled_io_11) >= 10UL) {
    hw_attempted_io_11 = true; hw_sampled_io_11 = hw_now;
    hw_value_io_11 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_12 || (uint32_t)(hw_now - hw_sampled_io_12) >= 10UL) {
    hw_attempted_io_12 = true; hw_sampled_io_12 = hw_now;
    hw_value_io_12 = (digitalRead(3) == HIGH);
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
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_5, hw_value_io_11, hw_value_io_12, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_1_trigger, &hw_value_serial_1_value_0, &hw_value_serial_1_value_1, &hw_value_serial_1_value_2);
  analogWrite(5, (int)constrain((hw_value_io_6 * 1 + 0), 0.0, 255.0));
  digitalWrite(9, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_8 ? HIGH : LOW);
  int32_t hw_tone_command_io_9 = (hw_value_io_9 >= 31 && hw_value_io_9 <= 65535) ? hw_value_io_9 : 0;
  if (hw_tone_command_io_9 != hw_tone_io_9) {
    if (hw_tone_command_io_9 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_9);
    hw_tone_io_9 = hw_tone_command_io_9;
  }
  if (hw_value_io_10 != hw_display_previous_io_10) {
    if (hw_value_io_10 < -999 || hw_value_io_10 > 9999) hw_display_io_10.clear();
    else hw_display_io_10.showNumberDec((int)hw_value_io_10, false, 4, 0);
    hw_display_previous_io_10 = hw_value_io_10;
  }
  if (hw_value_serial_0_trigger) { Serial.print("dht_valid"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_1_trigger) { Serial.print("temperature_c"); Serial.println(hw_value_serial_1_value_0); }
  if (hw_value_serial_1_trigger) { Serial.print("light_adc"); Serial.println(hw_value_serial_1_value_1); }
  if (hw_value_serial_1_trigger) { Serial.print("soil_adc"); Serial.println(hw_value_serial_1_value_2); }
}
