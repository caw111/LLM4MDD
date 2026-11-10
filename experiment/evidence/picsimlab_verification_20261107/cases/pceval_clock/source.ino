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
extern int32_t sysml_state_savedDisplayTime;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_1, int32_t* io_2, bool* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_savedDisplayTime = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_savedDisplayTime = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_1, int32_t* io_2, bool* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_arithmetic_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_arithmetic_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    int32_t sysml_arithmetic_15 = 0;
    int32_t sysml_conditional_16 = 0;
    int32_t sysml_arithmetic_17 = 0;
    if (io_1) {
        if ((((double)io_0) / 3600) < ((double)((int32_t)(((double)io_0) / 3600)))) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = 0;
        }
        if ((((double)io_0) / 3600) < ((double)((int32_t)(((double)io_0) / 3600)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        if ((((double)io_0) / 3600) < ((double)((int32_t)(((double)io_0) / 3600)))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        if ((((double)io_0) / 3600) < ((double)((int32_t)(((double)io_0) / 3600)))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        if ((((double)(((int32_t)(((double)io_0) / 3600)) - sysml_conditional_2)) / 24) < ((double)((int32_t)(((double)(((int32_t)(((double)io_0) / 3600)) - sysml_conditional_3)) / 24)))) {
            sysml_conditional_4 = 1;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_arithmetic_5 = ((int32_t)(((double)(((int32_t)(((double)io_0) / 3600)) - sysml_conditional_1)) / 24)) - sysml_conditional_4;
        if ((((double)io_0) / 60) < ((double)((int32_t)(((double)io_0) / 60)))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        if ((((double)io_0) / 60) < ((double)((int32_t)(((double)io_0) / 60)))) {
            sysml_conditional_7 = 1;
        }
        else {
            sysml_conditional_7 = 0;
        }
        if ((((double)io_0) / 60) < ((double)((int32_t)(((double)io_0) / 60)))) {
            sysml_conditional_8 = 1;
        }
        else {
            sysml_conditional_8 = 0;
        }
        if ((((double)io_0) / 60) < ((double)((int32_t)(((double)io_0) / 60)))) {
            sysml_conditional_9 = 1;
        }
        else {
            sysml_conditional_9 = 0;
        }
        if ((((double)(((int32_t)(((double)io_0) / 60)) - sysml_conditional_8)) / 60) < ((double)((int32_t)(((double)(((int32_t)(((double)io_0) / 60)) - sysml_conditional_9)) / 60)))) {
            sysml_conditional_10 = 1;
        }
        else {
            sysml_conditional_10 = 0;
        }
        sysml_arithmetic_11 = ((int32_t)(((double)(((int32_t)(((double)io_0) / 60)) - sysml_conditional_7)) / 60)) - sysml_conditional_10;
        sysml_state_savedDisplayTime = (((((int32_t)(((double)io_0) / 3600)) - sysml_conditional_0) - (24 * sysml_arithmetic_5)) * 100) + ((((int32_t)(((double)io_0) / 60)) - sysml_conditional_6) - (60 * sysml_arithmetic_11));
    }
    int32_t sysml_value_0 = 0;
    if ((((double)io_0) / 3600) < ((double)((int32_t)(((double)io_0) / 3600)))) {
        sysml_conditional_12 = 1;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_0 = ((int32_t)(((double)io_0) / 3600)) - sysml_conditional_12;
    int32_t sysml_value_1 = 0;
    if ((((double)io_0) / 60) < ((double)((int32_t)(((double)io_0) / 60)))) {
        sysml_conditional_13 = 1;
    }
    else {
        sysml_conditional_13 = 0;
    }
    sysml_value_1 = ((int32_t)(((double)io_0) / 60)) - sysml_conditional_13;
    int32_t sysml_value_2 = 0;
    sysml_value_2 = sysml_state_savedDisplayTime;
    bool sysml_value_3 = 0;
    sysml_value_3 = true;
    int32_t sysml_value_4 = 0;
    if ((((double)sysml_value_0) / 24) < ((double)((int32_t)(((double)sysml_value_0) / 24)))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    sysml_arithmetic_15 = ((int32_t)(((double)sysml_value_0) / 24)) - sysml_conditional_14;
    sysml_value_4 = sysml_value_0 - (24 * sysml_arithmetic_15);
    int32_t sysml_value_5 = 0;
    if ((((double)sysml_value_1) / 60) < ((double)((int32_t)(((double)sysml_value_1) / 60)))) {
        sysml_conditional_16 = 1;
    }
    else {
        sysml_conditional_16 = 0;
    }
    sysml_arithmetic_17 = ((int32_t)(((double)sysml_value_1) / 60)) - sysml_conditional_16;
    sysml_value_5 = sysml_value_1 - (60 * sysml_arithmetic_17);
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
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
static const uint8_t hw_seven_io_2_segments[]={6,7,8,9,10,11,12};
static const uint8_t hw_seven_io_2_masks[]={63,6,91,79,102,109,125,7,127,111};
static const uint8_t hw_seven_io_2_digits[]={2,3,4,5};
static uint8_t hw_seven_io_2_digit=0;
// LLM4MDD target-board: arduino:avr:uno
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_UNO)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static int32_t hw_value_io_0 = 0;
static bool hw_value_io_1 = false;
static int32_t hw_value_io_2 = 0;
static bool hw_value_io_3 = true;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_3 ? LOW : HIGH);
  Wire.begin();
  hw_bus_0_ready=hw_bus_0.begin();
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 2UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_bus_0_sampled || (uint32_t)(hw_now-hw_bus_0_last)>=100UL) {
    hw_bus_0_sampled=true; hw_bus_0_last=hw_now;
    bool sample_valid=false;
    if (hw_bus_0_ready) {
      uint32_t epoch=hw_bus_0.now().unixtime();
      sample_valid=epoch>=946684800UL && epoch<=2147483647UL;
      if (sample_valid) hw_value_io_0=(int32_t)epoch;
      hw_value_io_1=sample_valid;
    }
    hw_value_io_1=sample_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3);
  digitalWrite(13, hw_value_io_3 ? LOW : HIGH);
  {
  for(uint8_t i=0;i<4;++i) digitalWrite(hw_seven_io_2_digits[i],LOW);
  int32_t number=hw_value_io_2; uint8_t digit=hw_seven_io_2_digit;
  for(uint8_t i=digit;i<3;++i) number/=10;
  uint8_t mask=(hw_value_io_2>=0 && hw_value_io_2<=9999)?hw_seven_io_2_masks[number%10]:0;
  for(uint8_t i=0;i<7;++i) digitalWrite(hw_seven_io_2_segments[i],((mask>>i)&1)?LOW:HIGH);
  digitalWrite(hw_seven_io_2_digits[hw_seven_io_2_digit],HIGH);
  hw_seven_io_2_digit=(hw_seven_io_2_digit+1)%4;
  }
}
