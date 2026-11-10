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
extern int32_t sysml_state_reportAge;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern bool sysml_state_previousValid;
/* Global Variable define of  */
extern bool sysml_state_reportNow;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int32_t io_1, bool io_2, int32_t io_3, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_reportAge = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_previousValid = false;
/* Global Variable define of  */
bool sysml_state_reportNow = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_reportAge = 0;
    sysml_state_previousPressed = false;
    sysml_state_previousValid = false;
    sysml_state_reportNow = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int32_t io_1, bool io_2, int32_t io_3, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_arithmetic_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_arithmetic_4 = 0;
    int32_t sysml_conditional_5 = 0;
    sysml_state_reportNow = false;
    if (io_2) {
        sysml_lazy_0 = !sysml_state_previousValid;
        if (!sysml_lazy_0) {
            sysml_lazy_0 = io_0 != sysml_state_previousPressed;
        }
        if (sysml_lazy_0) {
            sysml_state_reportNow = true;
            sysml_state_reportAge = 0;
        }
        else {
            if ((io_3 >= (100 - sysml_state_reportAge))) {
                sysml_state_reportNow = true;
            }
            if ((((double)io_3) / 100) < ((double)((int32_t)(((double)io_3) / 100)))) {
                sysml_conditional_1 = 1;
            }
            else {
                sysml_conditional_1 = 0;
            }
            sysml_arithmetic_2 = ((int32_t)(((double)io_3) / 100)) - sysml_conditional_1;
            sysml_state_reportAge = sysml_state_reportAge + (io_3 - (100 * sysml_arithmetic_2));
            if ((sysml_state_reportAge >= 100)) {
                sysml_state_reportAge = (sysml_state_reportAge - 100);
            }
        }
    }
    else {
        sysml_state_reportAge = 0;
    }
    sysml_state_previousValid = io_2;
    sysml_state_previousPressed = io_0;
    int32_t sysml_value_0 = 0;
    if ((((double)io_3) / 100) < ((double)((int32_t)(((double)io_3) / 100)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_arithmetic_4 = ((int32_t)(((double)io_3) / 100)) - sysml_conditional_3;
    sysml_value_0 = io_3 - (100 * sysml_arithmetic_4);
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_reportNow;
    int32_t sysml_value_2 = 0;
    if (io_0) {
        sysml_conditional_5 = io_1 + 32400;
    }
    else {
        sysml_conditional_5 = io_1;
    }
    sysml_value_2 = sysml_conditional_5;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}
// END COMPILED SYSML MODEL

#include <Wire.h>

#include <RTClib.h>
#include <stdio.h>
#include <string.h>
static void hw_datetime_text(int32_t epoch, const char* style, char* text) {
  static const char* days[]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
  DateTime t((uint32_t)epoch);
  if(strcmp(style,"weekday")==0) {strcpy(text,days[t.dayOfTheWeek()]);return;}
  if(strcmp(style,"rtc")==0) snprintf(text,64,"%u/%u/%u (%s) %u:%u:%u",t.year(),t.month(),t.day(),days[t.dayOfTheWeek()],t.hour(),t.minute(),t.second());
  else if(strcmp(style,"date")==0) snprintf(text,64,"%04u-%02u-%02u",t.year(),t.month(),t.day());
  else snprintf(text,64,"%04u-%02u-%02uT%02u:%02u:%02u",t.year(),t.month(),t.day(),t.hour(),t.minute(),t.second());
}

RTC_DS1307 hw_bus_0;
static bool hw_bus_0_ready=false, hw_bus_0_sampled=false;
static uint32_t hw_bus_0_last=0;
// LLM4MDD target-board: arduino:avr:uno
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
static int32_t hw_value_io_1 = 0;
static bool hw_value_io_2 = false;
static int32_t hw_value_io_3 = 0;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  Wire.begin();
  hw_bus_0_ready=hw_bus_0.begin();
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  if (!hw_bus_0_sampled || (uint32_t)(hw_now-hw_bus_0_last)>=10UL) {
    hw_bus_0_sampled=true; hw_bus_0_last=hw_now;
    bool sample_valid=false;
    if (hw_bus_0_ready) {
      uint32_t epoch=hw_bus_0.now().unixtime();
      sample_valid=epoch>=946684800UL && epoch<=2147483647UL;
      if (sample_valid) hw_value_io_1=(int32_t)epoch;
      hw_value_io_2=sample_valid;
    }
    hw_value_io_2=sample_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_3 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  if (hw_value_serial_0_trigger) {
  { char text[64]; hw_datetime_text((int32_t)(hw_value_serial_0_value_0), "iso", text); Serial.print(text); }
  Serial.println();
  }
}
