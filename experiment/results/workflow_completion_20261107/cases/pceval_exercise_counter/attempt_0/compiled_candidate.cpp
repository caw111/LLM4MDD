#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run6_native\\cases\\pceval_exercise_counter\\attempt_0\\arduino\\sketch\\sketch.ino"
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
extern double sysml_state_totalCount;
/* Global Variable define of  */
extern int32_t sysml_state_displayDigit;
/* Global Variable define of  */
extern bool sysml_state_motionArmed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_1, int32_t* io_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_totalCount = 0.0;
/* Global Variable define of  */
int32_t sysml_state_displayDigit = 0;
/* Global Variable define of  */
bool sysml_state_motionArmed = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run6_native\\cases\\pceval_exercise_counter\\attempt_0\\arduino\\sketch\\sketch.ino"
void setup();
#line 123 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run6_native\\cases\\pceval_exercise_counter\\attempt_0\\arduino\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run6_native\\cases\\pceval_exercise_counter\\attempt_0\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_totalCount = 0.0;
    sysml_state_displayDigit = 0;
    sysml_state_motionArmed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_1, int32_t* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_arithmetic_4 = 0;
    if (io_1) {
        sysml_lazy_0 = io_0 > (-2000);
        if (sysml_lazy_0) {
            sysml_lazy_0 = io_0 < 2000;
        }
        if (sysml_lazy_0) {
            sysml_state_motionArmed = true;
        }
        else {
            sysml_lazy_1 = sysml_state_motionArmed;
            if (sysml_lazy_1) {
                sysml_lazy_2 = io_0 > 15000;
                if (!sysml_lazy_2) {
                    sysml_lazy_2 = io_0 < (-15000);
                }
                sysml_lazy_1 = sysml_lazy_2;
            }
            if (sysml_lazy_1) {
                sysml_state_totalCount = (sysml_state_totalCount + 1.0);
                sysml_state_displayDigit = (sysml_state_displayDigit + 1);
                if ((((double)sysml_state_displayDigit) / 10) < ((double)((int32_t)(((double)sysml_state_displayDigit) / 10)))) {
                    sysml_conditional_3 = 1;
                }
                else {
                    sysml_conditional_3 = 0;
                }
                sysml_arithmetic_4 = ((int32_t)(((double)sysml_state_displayDigit) / 10)) - sysml_conditional_3;
                sysml_state_displayDigit = sysml_state_displayDigit - (10 * sysml_arithmetic_4);
                sysml_state_motionArmed = false;
            }
        }
    }
    int32_t sysml_value_0 = 0;
    sysml_value_0 = sysml_state_displayDigit;
    *io_2 = sysml_value_0;
}
// END COMPILED SYSML MODEL

#include <Wire.h>
#include <MPU6050.h>
MPU6050 hw_bus_0(104);
static bool hw_bus_0_ready=false, hw_bus_0_sampled=false;
static uint32_t hw_bus_0_last=0;
static const uint8_t hw_seven_io_2_segments[]={2,3,4,5,6,7,8};
static const uint8_t hw_seven_io_2_masks[]={63,6,91,79,102,109,125,7,127,111};
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

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Wire.begin();
  hw_bus_0.initialize();
  hw_bus_0.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  hw_bus_0_ready=hw_bus_0.testConnection();
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_bus_0_sampled || (uint32_t)(hw_now-hw_bus_0_last)>=5UL) {
    hw_bus_0_sampled=true; hw_bus_0_last=hw_now;
    bool sample_valid=false;
    if (hw_bus_0_ready) {
      sample_valid=hw_bus_0.testConnection();
      if (sample_valid) hw_value_io_0=hw_bus_0.getRotationY();
      hw_value_io_1=sample_valid;
    }
    hw_value_io_1=sample_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2);
  {
  uint8_t mask=(hw_value_io_2>=0 && hw_value_io_2<=9)?hw_seven_io_2_masks[hw_value_io_2]:0;
  for(uint8_t i=0;i<7;++i) digitalWrite(hw_seven_io_2_segments[i],((mask>>i)&1)?HIGH:LOW);
  }
}

