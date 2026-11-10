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
extern int32_t sysml_state_peopleCount;
/* Global Variable define of  */
extern bool sysml_state_previousEntryPressed;
/* Global Variable define of  */
extern bool sysml_state_displayTemperature;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, double io_4, double io_5, bool io_6, bool io_9, double* io_7, int32_t* io_8, bool* serial_0_trigger, int32_t* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_peopleCount = 0;
/* Global Variable define of  */
bool sysml_state_previousEntryPressed = false;
/* Global Variable define of  */
bool sysml_state_displayTemperature = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_peopleCount = 0;
    sysml_state_previousEntryPressed = false;
    sysml_state_displayTemperature = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, double io_4, double io_5, bool io_6, bool io_9, double* io_7, int32_t* io_8, bool* serial_0_trigger, int32_t* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    sysml_lazy_0 = io_9;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousEntryPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_displayTemperature = (!sysml_state_displayTemperature);
        sysml_state_previousEntryPressed = io_9;
    }
    else {
        sysml_state_previousEntryPressed = io_9;
    }
    if (io_1) {
        if (io_3) {
            if (io_0 <= 0.0) {
                sysml_conditional_1 = 0;
            }
            else {
                sysml_conditional_1 = 0;
            }
            sysml_state_peopleCount = sysml_conditional_1;
        }
        else {
            if (io_0 <= 0.0) {
                sysml_conditional_2 = 0;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_state_peopleCount = sysml_conditional_2;
        }
    }
    if ((!io_1)) {
        if (io_3) {
            sysml_state_peopleCount = 0;
        }
        else {
            sysml_state_peopleCount = 0;
        }
    }
    if ((!io_6)) {
    }
    double sysml_value_0 = 0;
    if (sysml_state_peopleCount == 0) {
        sysml_conditional_3 = 0.0;
    }
    else {
        if (io_4 > 30.0) {
            sysml_conditional_4 = 255.0;
        }
        else {
            if (io_4 > 25.0) {
                sysml_conditional_5 = 180.0;
            }
            else {
                if (io_4 > 20.0) {
                    sysml_conditional_6 = 120.0;
                }
                else {
                    sysml_conditional_6 = 60.0;
                }
                sysml_conditional_5 = sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    sysml_value_0 = sysml_conditional_3;
    int32_t sysml_value_1 = 0;
    if (sysml_state_displayTemperature) {
        if (io_6) {
            if (io_4 < ((double)((int32_t)io_4))) {
                sysml_conditional_9 = 1;
            }
            else {
                sysml_conditional_9 = 0;
            }
            sysml_conditional_8 = ((int32_t)io_4) - sysml_conditional_9;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    else {
        sysml_conditional_7 = sysml_state_peopleCount;
    }
    sysml_value_1 = sysml_conditional_7;
    bool sysml_value_2 = 0;
    sysml_value_2 = true;
    int32_t sysml_value_3 = 0;
    sysml_value_3 = sysml_state_peopleCount;
    double sysml_value_4 = 0;
    if (io_6) {
        sysml_conditional_10 = io_4;
    }
    else {
        sysml_conditional_10 = 0.0;
    }
    sysml_value_4 = sysml_conditional_10;
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_peopleCount == 0);
    *io_7 = sysml_value_0;
    *io_8 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
    *serial_0_value_1 = sysml_value_4;
    *serial_0_value_2 = sysml_value_5;
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
static bool hw_value_io_1 = false;
static double hw_value_io_2 = 0;
static bool hw_value_io_3 = false;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;
static bool hw_value_io_6 = false;
static double hw_value_io_7 = 0;
static int32_t hw_value_io_8 = 0;
TM1637Display hw_display_io_8(4, 3);
static int32_t hw_display_previous_io_8 = INT32_MIN;
static bool hw_value_io_9 = false;
static uint32_t hw_sampled_io_9 = 0;
static bool hw_attempted_io_9 = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static bool hw_value_serial_0_value_2 = false;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;
static uint32_t hw_ultrasonic_sampled_1 = 0;
static bool hw_ultrasonic_attempted_1 = false;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_7 * 1 + 0), 0.0, 255.0));
  hw_display_io_8.setBrightness(7, true);
  if (hw_value_io_8 != hw_display_previous_io_8) {
    if (hw_value_io_8 < -999 || hw_value_io_8 > 9999) hw_display_io_8.clear();
    else hw_display_io_8.showNumberDec((int)hw_value_io_8, false, 4, 0);
    hw_display_previous_io_8 = hw_value_io_8;
  }
  pinMode(2, INPUT_PULLUP);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, INPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  pinMode(11, INPUT);
  hw_dht_0.begin();
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_9 || (uint32_t)(hw_now - hw_sampled_io_9) >= 10UL) {
    hw_attempted_io_9 = true; hw_sampled_io_9 = hw_now;
    hw_value_io_9 = (digitalRead(2) == HIGH);
  }
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 60UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(8, LOW); delayMicroseconds(2);
    digitalWrite(8, HIGH); delayMicroseconds(10); digitalWrite(8, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(9, HIGH, 30000UL);
    hw_value_io_1 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_1) hw_value_io_0 = hw_ultrasonic_pulse_0 / 58.0;
  }
  uint32_t hw_ultrasonic_now_1 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_1 || (uint32_t)(hw_ultrasonic_now_1 - hw_ultrasonic_sampled_1) >= 60UL) {
    hw_ultrasonic_attempted_1 = true; hw_ultrasonic_sampled_1 = hw_ultrasonic_now_1;
    digitalWrite(10, LOW); delayMicroseconds(2);
    digitalWrite(10, HIGH); delayMicroseconds(10); digitalWrite(10, LOW);
    unsigned long hw_ultrasonic_pulse_1 = pulseIn(11, HIGH, 30000UL);
    hw_value_io_3 = (hw_ultrasonic_pulse_1 >= 116UL && hw_ultrasonic_pulse_1 <= 23200UL);
    if (hw_value_io_3) hw_value_io_2 = hw_ultrasonic_pulse_1 / 58.0;
  }
  uint32_t hw_dht_0_now = (uint32_t)millis();
  if (!hw_dht_0_attempted || (uint32_t)(hw_dht_0_now - hw_dht_0_sampled) >= 2000UL) {
    hw_dht_0_attempted = true; hw_dht_0_sampled = hw_dht_0_now;
    bool hw_dht_0_valid = hw_dht_0.read();
    double hw_dht_0_io_4 = hw_dht_0_valid ? hw_dht_0.readTemperature() : NAN;
    double hw_dht_0_io_5 = hw_dht_0_valid ? hw_dht_0.readHumidity() : NAN;
    hw_dht_0_valid = hw_dht_0_valid && isfinite(hw_dht_0_io_4) && isfinite(hw_dht_0_io_5);
    if (hw_dht_0_valid) {
      hw_value_io_4 = hw_dht_0_io_4;
      hw_value_io_5 = hw_dht_0_io_5;
    }
    hw_value_io_6 = hw_dht_0_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_5, hw_value_io_6, hw_value_io_9, &hw_value_io_7, &hw_value_io_8, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2);
  analogWrite(5, (int)constrain((hw_value_io_7 * 1 + 0), 0.0, 255.0));
  if (hw_value_io_8 != hw_display_previous_io_8) {
    if (hw_value_io_8 < -999 || hw_value_io_8 > 9999) hw_display_io_8.clear();
    else hw_display_io_8.showNumberDec((int)hw_value_io_8, false, 4, 0);
    hw_display_previous_io_8 = hw_value_io_8;
  }
  if (hw_value_serial_0_trigger) { Serial.print("people"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("temperature_c"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("eco_mode"); Serial.println(hw_value_serial_0_value_2); }
}
