#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run7_clarified\\cases\\pceval_dday_counter\\attempt_0\\arduino\\sketch\\sketch.ino"
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
extern int32_t sysml_state_reportedDays;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_1, int32_t io_2, bool io_2_valid, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_reportedDays = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run7_clarified\\cases\\pceval_dday_counter\\attempt_0\\arduino\\sketch\\sketch.ino"
static void hw_datetime_text(int32_t epoch, const char* style, char* text);
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run7_clarified\\cases\\pceval_dday_counter\\attempt_0\\arduino\\sketch\\sketch.ino"
static bool hw_rx_parse(int32_t* value);
#line 131 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run7_clarified\\cases\\pceval_dday_counter\\attempt_0\\arduino\\sketch\\sketch.ino"
static bool hw_rx_finish(int32_t* value);
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run7_clarified\\cases\\pceval_dday_counter\\attempt_0\\arduino\\sketch\\sketch.ino"
static bool hw_rx_poll(uint32_t hw_now, int32_t* value);
#line 157 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run7_clarified\\cases\\pceval_dday_counter\\attempt_0\\arduino\\sketch\\sketch.ino"
void setup();
#line 170 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run7_clarified\\cases\\pceval_dday_counter\\attempt_0\\arduino\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run7_clarified\\cases\\pceval_dday_counter\\attempt_0\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_reportedDays = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_1, int32_t io_2, bool io_2_valid, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_2_valid;
    }
    if (sysml_lazy_0) {
        if (((io_2 * 1.0) - io_0) >= 0.0) {
            if ((((double)((io_2 * 1.0) - io_0)) / 86400.0) < ((double)((int32_t)(((double)((io_2 * 1.0) - io_0)) / 86400.0)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int32_t)(((double)((io_2 * 1.0) - io_0)) / 86400.0)) - sysml_conditional_2;
        }
        else {
            if ((((double)(-((io_2 * 1.0) - io_0))) / 86400.0) < ((double)((int32_t)(((double)(-((io_2 * 1.0) - io_0))) / 86400.0)))) {
                sysml_conditional_3 = 1;
            }
            else {
                sysml_conditional_3 = 0;
            }
            sysml_conditional_1 = -(((int32_t)(((double)(-((io_2 * 1.0) - io_0))) / 86400.0)) - sysml_conditional_3);
        }
        sysml_state_reportedDays = sysml_conditional_1;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = ((io_2 * 1.0) - io_0);
    bool sysml_value_1 = 0;
    sysml_lazy_4 = io_1;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_2_valid;
    }
    sysml_value_1 = sysml_lazy_4;
    int32_t sysml_value_2 = 0;
    sysml_value_2 = sysml_state_reportedDays;
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
static int32_t hw_value_io_0 = 0;
static bool hw_value_io_1 = false;
static int32_t hw_value_io_2 = 0;
#include <string.h>
static char hw_rx_buffer[64];
static char hw_rx_text[64];
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
  if(strlen(begin)!=19 || begin[4]!='-' || begin[7]!='-' || begin[10]!='T' || begin[13]!=':' || begin[16]!=':') return false;
  for(uint8_t i=0;i<19;++i) if(i!=4&&i!=7&&i!=10&&i!=13&&i!=16 && (begin[i]<'0'||begin[i]>'9')) return false;
  unsigned y=0,m=0,d=0,h=0,mi=0,s=0;
  if(sscanf(begin,"%u-%u-%uT%u:%u:%u",&y,&m,&d,&h,&mi,&s)!=6 || y<2000 || y>2037 || m<1 || m>12 || d<1 || h>23 || mi>59 || s>59) return false;
  static const uint8_t days[]={31,28,31,30,31,30,31,31,30,31,30,31};
  unsigned maximum=days[m-1]+(m==2 && (y%4==0 && (y%100!=0||y%400==0)));
  if(d>maximum) return false;
  *value=(int32_t)DateTime(y,m,d,h,mi,s).unixtime();return true;
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
  if ((hw_rx_size || hw_rx_overflow) && (uint32_t)(hw_now - hw_rx_last_byte) >= 20UL) return hw_rx_finish(value);
  return false;
}
static bool hw_value_io_2_valid = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  Wire.begin();
  hw_bus_0_ready=hw_bus_0.begin();
  if (hw_bus_0_ready) {
  uint32_t epoch=hw_bus_0.now().unixtime();
  hw_bus_0_ready=epoch>=946684800UL && epoch<=2147483647UL;
  if (hw_bus_0_ready) hw_value_io_0=(int32_t)epoch;
  }
  hw_value_io_1=hw_bus_0_ready;
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_2_valid = hw_rx_poll(hw_now, &hw_value_io_2);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_2_valid, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  if (hw_value_serial_0_trigger) {
  Serial.print("D-");
  Serial.print((long)(hw_value_serial_0_value_0));
  Serial.println();
  }
}

