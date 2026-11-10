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
extern bool sysml_state_readySent;
/* Global Variable define of  */
extern bool sysml_state_readyPulse;
/* Global Variable define of  */
extern bool sysml_state_reportPulse;
/* Global Variable define of  */
extern double sysml_state_reportPhase;
/* Global Variable define of  */
extern double sysml_state_cachedX;
/* Global Variable define of  */
extern double sysml_state_cachedY;
/* Global Variable define of  */
extern double sysml_state_cachedZ;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_3, int32_t io_5, bool* io_4, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_readySent = false;
/* Global Variable define of  */
bool sysml_state_readyPulse = false;
/* Global Variable define of  */
bool sysml_state_reportPulse = false;
/* Global Variable define of  */
double sysml_state_reportPhase = 0.0;
/* Global Variable define of  */
double sysml_state_cachedX = 0.0;
/* Global Variable define of  */
double sysml_state_cachedY = 0.0;
/* Global Variable define of  */
double sysml_state_cachedZ = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_readySent = false;
    sysml_state_readyPulse = false;
    sysml_state_reportPulse = false;
    sysml_state_reportPhase = 0.0;
    sysml_state_cachedX = 0.0;
    sysml_state_cachedY = 0.0;
    sysml_state_cachedZ = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_3, int32_t io_5, bool* io_4, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_arithmetic_1 = 0;
    sysml_state_readyPulse = false;
    sysml_state_reportPulse = false;
    if (io_3) {
        sysml_state_cachedX = io_0;
        sysml_state_cachedY = io_1;
        sysml_state_cachedZ = io_2;
    }
    if (sysml_state_readySent) {
        sysml_state_reportPhase = (sysml_state_reportPhase + io_5);
        if ((sysml_state_reportPhase >= 500.0)) {
            sysml_state_reportPulse = io_3;
            if ((((double)sysml_state_reportPhase) / 500.0) < ((double)((int32_t)(((double)sysml_state_reportPhase) / 500.0)))) {
                sysml_conditional_0 = 1;
            }
            else {
                sysml_conditional_0 = 0;
            }
            sysml_arithmetic_1 = ((int32_t)(((double)sysml_state_reportPhase) / 500.0)) - sysml_conditional_0;
            sysml_state_reportPhase = sysml_state_reportPhase - (500.0 * sysml_arithmetic_1);
        }
    }
    else {
        if (io_3) {
            sysml_state_readyPulse = true;
            sysml_state_readySent = true;
            sysml_state_reportPhase = 0.0;
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_readyPulse;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_reportPulse;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_cachedX;
    double sysml_value_3 = 0;
    sysml_value_3 = sysml_state_cachedY;
    double sysml_value_4 = 0;
    sysml_value_4 = sysml_state_cachedZ;
    *io_4 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
}
// END COMPILED SYSML MODEL

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
Adafruit_MPU6050 hw_bus_0;
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
static double hw_value_io_0 = 0;
static double hw_value_io_1 = 0;
static double hw_value_io_2 = 0;
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
static int32_t hw_value_io_5 = 0;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static double hw_value_serial_0_value_2 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  Wire.begin();
  hw_bus_0_ready=hw_bus_0.begin(104, &Wire);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_bus_0_sampled || (uint32_t)(hw_now-hw_bus_0_last)>=500UL) {
    hw_bus_0_sampled=true; hw_bus_0_last=hw_now;
    bool sample_valid=false;
    if (hw_bus_0_ready) {
      sensors_event_t acceleration,gyro,temperature;
      sample_valid=hw_bus_0.getEvent(&acceleration,&gyro,&temperature);
      if (sample_valid) hw_value_io_0=gyro.gyro.x;
      if (sample_valid) hw_value_io_1=gyro.gyro.y;
      if (sample_valid) hw_value_io_2=gyro.gyro.z;
      hw_value_io_3=sample_valid;
    }
    hw_value_io_3=sample_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_5 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_5, &hw_value_io_4, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2);
  if (hw_value_io_4) Serial.println("MPU6050 ready!");
  if (hw_value_serial_0_trigger) {
  Serial.print("X: ");
  Serial.print(hw_value_serial_0_value_0, 2);
  Serial.print(", Y: ");
  Serial.print(hw_value_serial_0_value_1, 2);
  Serial.print(", Z: ");
  Serial.print(hw_value_serial_0_value_2, 2);
  Serial.print(" rad/s");
  Serial.println();
  }
}
