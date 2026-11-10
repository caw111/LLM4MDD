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
extern int32_t sysml_state_cachedEpoch;
/* Global Variable define of  */
extern bool sysml_state_hasReading;
/* Global Variable define of  */
extern int32_t sysml_state_dayOffset;
/* Global Variable define of  */
extern bool sysml_state_previousBackPressed;
/* Global Variable define of  */
extern bool sysml_state_previousForwardPressed;
/* Global Variable define of  */
extern bool sysml_state_backPressEvent;
/* Global Variable define of  */
extern bool sysml_state_forwardPressEvent;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_1, bool io_2, bool io_3, int32_t* io_4, int32_t* io_5);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_cachedEpoch = 0;
/* Global Variable define of  */
bool sysml_state_hasReading = false;
/* Global Variable define of  */
int32_t sysml_state_dayOffset = 0;
/* Global Variable define of  */
bool sysml_state_previousBackPressed = false;
/* Global Variable define of  */
bool sysml_state_previousForwardPressed = false;
/* Global Variable define of  */
bool sysml_state_backPressEvent = false;
/* Global Variable define of  */
bool sysml_state_forwardPressEvent = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_cachedEpoch = 0;
    sysml_state_hasReading = false;
    sysml_state_dayOffset = 0;
    sysml_state_previousBackPressed = false;
    sysml_state_previousForwardPressed = false;
    sysml_state_backPressEvent = false;
    sysml_state_forwardPressEvent = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_1, bool io_2, bool io_3, int32_t* io_4, int32_t* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBackPressed;
    }
    sysml_state_backPressEvent = sysml_lazy_0;
    sysml_lazy_1 = io_3;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousForwardPressed;
    }
    sysml_state_forwardPressEvent = sysml_lazy_1;
    if (io_1) {
        sysml_state_cachedEpoch = io_0;
        sysml_state_hasReading = true;
    }
    sysml_lazy_2 = sysml_state_backPressEvent;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_forwardPressEvent;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = (sysml_state_cachedEpoch + (sysml_state_dayOffset * 86400.0)) >= (-2147397248.0);
    }
    if (sysml_lazy_3) {
        sysml_state_dayOffset = (sysml_state_dayOffset - 1);
    }
    else {
        sysml_lazy_4 = sysml_state_forwardPressEvent;
        if (sysml_lazy_4) {
            sysml_lazy_4 = !sysml_state_backPressEvent;
        }
        sysml_lazy_5 = sysml_lazy_4;
        if (sysml_lazy_5) {
            sysml_lazy_5 = (sysml_state_cachedEpoch + (sysml_state_dayOffset * 86400.0)) <= 2147397247.0;
        }
        if (sysml_lazy_5) {
            sysml_state_dayOffset = (sysml_state_dayOffset + 1);
        }
    }
    sysml_state_previousBackPressed = io_2;
    sysml_state_previousForwardPressed = io_3;
    double sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_cachedEpoch + (sysml_state_dayOffset * 86400.0));
    int32_t sysml_value_1 = 0;
    if (sysml_value_0 < (-2147483648.0)) {
        sysml_conditional_6 = (-2147483647) - 1;
    }
    else {
        if (sysml_value_0 > 2147483647.0) {
            sysml_conditional_7 = 2147483647;
        }
        else {
            if (sysml_value_0 < ((double)((int32_t)sysml_value_0))) {
                sysml_conditional_8 = 1;
            }
            else {
                sysml_conditional_8 = 0;
            }
            sysml_conditional_7 = ((int32_t)sysml_value_0) - sysml_conditional_8;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_1 = sysml_conditional_6;
    int32_t sysml_value_2 = 0;
    if (sysml_state_hasReading) {
        sysml_conditional_9 = sysml_value_1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_2 = sysml_conditional_9;
    int32_t sysml_value_3 = 0;
    if (sysml_state_hasReading) {
        sysml_conditional_10 = sysml_value_1;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_3 = sysml_conditional_10;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
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
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
Adafruit_ILI9341 hw_lcd_0(10, 9, 8);
static bool hw_lcd_0_drawn=false;
static int32_t hw_previous_io_4=0;
static int32_t hw_previous_io_5=0;
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
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static int32_t hw_value_io_4 = 0;
static int32_t hw_value_io_5 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  Wire.begin();
  hw_bus_0_ready=hw_bus_0.begin();
  hw_lcd_0.begin();
  hw_lcd_0.setRotation(1);
  hw_lcd_0.fillScreen(0);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(2) == HIGH);
  }
  if (!hw_bus_0_sampled || (uint32_t)(hw_now-hw_bus_0_last)>=5UL) {
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
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, &hw_value_io_4, &hw_value_io_5);
  if (!hw_lcd_0_drawn || hw_value_io_4!=hw_previous_io_4 || hw_value_io_5!=hw_previous_io_5) {
  hw_lcd_0_drawn=true;
  hw_lcd_0.fillScreen(0);
  hw_lcd_0.setCursor(10,30);
  hw_lcd_0.setTextColor(65535);
  hw_lcd_0.setTextSize(3);
  { char text[64]; hw_datetime_text((int32_t)(hw_value_io_4), "date", text); hw_lcd_0.print(text); }
  hw_previous_io_4=hw_value_io_4;
  hw_lcd_0.setCursor(10,80);
  hw_lcd_0.setTextColor(65535);
  hw_lcd_0.setTextSize(3);
  { char text[64]; hw_datetime_text((int32_t)(hw_value_io_5), "weekday", text); hw_lcd_0.print(text); }
  hw_previous_io_5=hw_value_io_5;
  }
}
