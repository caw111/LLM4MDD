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
extern bool sysml_state_dhtValid;
/* Global Variable define of  */
extern bool sysml_state_lastTempValid;
/* Global Variable define of  */
extern bool sysml_state_lastHumValid;
/* Global Variable define of  */
extern bool sysml_state_showTemp;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool* io_0, int32_t* io_3, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_dhtValid = false;
/* Global Variable define of  */
bool sysml_state_lastTempValid = false;
/* Global Variable define of  */
bool sysml_state_lastHumValid = false;
/* Global Variable define of  */
bool sysml_state_showTemp = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_dhtValid = false;
    sysml_state_lastTempValid = false;
    sysml_state_lastHumValid = false;
    sysml_state_showTemp = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool* io_0, int32_t* io_3, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    int32_t sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    int32_t sysml_conditional_6 = 0;
    sysml_lazy_0 = sysml_state_lastTempValid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_lastHumValid;
    }
    if (sysml_lazy_0) {
        sysml_state_showTemp = (!sysml_state_showTemp);
    }
    else {
        sysml_state_showTemp = true;
    }
    sysml_state_lastTempValid = true;
    sysml_state_lastHumValid = true;
    sysml_lazy_1 = sysml_state_lastTempValid;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_lastHumValid;
    }
    sysml_state_dhtValid = sysml_lazy_1;
    bool sysml_value_0 = 0;
    if (sysml_state_dhtValid) {
        sysml_conditional_2 = false;
    }
    else {
        sysml_conditional_2 = true;
    }
    sysml_value_0 = sysml_conditional_2;
    int32_t sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_dhtValid;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_showTemp;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = 0;
    }
    else {
        sysml_lazy_5 = sysml_state_dhtValid;
        if (sysml_lazy_5) {
            sysml_lazy_5 = !sysml_state_showTemp;
        }
        if (sysml_lazy_5) {
            sysml_conditional_6 = 0;
        }
        else {
            sysml_conditional_6 = 0;
        }
        sysml_conditional_4 = sysml_conditional_6;
    }
    sysml_value_1 = sysml_conditional_4;
    bool sysml_value_2 = 0;
    sysml_value_2 = (!sysml_state_dhtValid);
    int32_t sysml_value_3 = 0;
    sysml_value_3 = 0;
    *io_0 = sysml_value_0;
    *io_3 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
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
static double hw_value_io_1 = 0;
static double hw_value_io_2 = 0;
static int32_t hw_value_io_3 = 0;
TM1637Display hw_display_io_3(4, 5);
static int32_t hw_display_previous_io_3 = INT32_MIN;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;
static bool hw_dht_0_ready = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
  hw_display_io_3.setBrightness(7, true);
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
  hw_dht_0.begin();
}

void loop() {
  static bool valid=false;
  static uint32_t lastRead=0,phaseStart=0;
  uint32_t now=millis();
  if (!hw_dht_0_attempted || (uint32_t)(now-lastRead)>=2000) {
    bool previous=valid;
    bool first=!hw_dht_0_attempted;
    hw_dht_0_attempted=true;
    lastRead=now;
    valid=hw_dht_0.read();
    if (valid) {
      hw_value_io_1=hw_dht_0.readTemperature();
      hw_value_io_2=hw_dht_0.readHumidity();
      valid=isfinite(hw_value_io_1) && isfinite(hw_value_io_2);
    }
    if (first || previous!=valid) {
      phaseStart=now;
      if (!valid) Serial.println("sensor error");
    }
  }
  uint32_t phase=(uint32_t)(now-phaseStart);
  digitalWrite(13,!valid && phase%1000<500 ? HIGH : LOW);
  int value=valid ? (int)(phase/2000%2 ? hw_value_io_2 : hw_value_io_1) : 0;
  if (value!=hw_display_previous_io_3 || (phase<20 && now==phaseStart)) {
    hw_display_io_3.showNumberDec(value,!valid,4,0);
    hw_display_previous_io_3=value;
  }
}
