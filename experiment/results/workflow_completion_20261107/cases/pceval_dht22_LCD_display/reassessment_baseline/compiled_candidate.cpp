#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run2\\cases\\pceval_dht22_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
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
extern int32_t sysml_state_loopWaitRemaining;
/* Global Variable define of  */
extern double sysml_state_displayedHumidity;
/* Global Variable define of  */
extern double sysml_state_displayedTemperature;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int32_t io_5, double* io_3, double* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_loopWaitRemaining = 0;
/* Global Variable define of  */
double sysml_state_displayedHumidity = 0.0;
/* Global Variable define of  */
double sysml_state_displayedTemperature = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 106 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run2\\cases\\pceval_dht22_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
void setup();
#line 114 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run2\\cases\\pceval_dht22_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run2\\cases\\pceval_dht22_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_loopWaitRemaining = 0;
    sysml_state_displayedHumidity = 0.0;
    sysml_state_displayedTemperature = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int32_t io_5, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    if (io_5 >= sysml_state_loopWaitRemaining) {
        sysml_conditional_0 = 0;
    }
    else {
        sysml_conditional_0 = sysml_state_loopWaitRemaining - io_5;
    }
    sysml_state_loopWaitRemaining = sysml_conditional_0;
    if ((sysml_state_loopWaitRemaining == 0)) {
        sysml_lazy_1 = io_2;
        if (sysml_lazy_1) {
            sysml_lazy_2 = io_1 != sysml_state_displayedHumidity;
            if (!sysml_lazy_2) {
                sysml_lazy_2 = io_0 != sysml_state_displayedTemperature;
            }
            sysml_lazy_1 = sysml_lazy_2;
        }
        if (sysml_lazy_1) {
            sysml_state_displayedHumidity = io_1;
            sysml_state_displayedTemperature = io_0;
        }
        sysml_state_loopWaitRemaining = 100;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_displayedHumidity;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_displayedTemperature;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
}
// END COMPILED SYSML MODEL

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
Adafruit_ILI9341 hw_lcd_0(10, 9, 8);
static bool hw_lcd_0_drawn=false;
static double hw_previous_io_3=0;
static double hw_previous_io_4=0;
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
static bool hw_value_io_2 = false;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static int32_t hw_value_io_5 = 0;
#include <DHT.h>
DHT hw_dht_0(2, DHT22);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  hw_lcd_0.begin();
  hw_lcd_0.setRotation(1);
  hw_lcd_0.fillScreen(0);
  hw_dht_0.begin();
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
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
  hw_value_io_5 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_5, &hw_value_io_3, &hw_value_io_4);
  if (!hw_lcd_0_drawn || hw_value_io_3!=hw_previous_io_3 || hw_value_io_4!=hw_previous_io_4) {
  hw_lcd_0_drawn=true;
  hw_lcd_0.fillScreen(0);
  hw_lcd_0.setCursor(0,0);
  hw_lcd_0.setTextColor(65535);
  hw_lcd_0.setTextSize(2);
  hw_lcd_0.print("Humidity: ");
  hw_lcd_0.print(hw_value_io_3, 2);
  hw_previous_io_3=hw_value_io_3;
  hw_lcd_0.setCursor(0,100);
  hw_lcd_0.setTextColor(65535);
  hw_lcd_0.setTextSize(2);
  hw_lcd_0.print("Temperature: ");
  hw_lcd_0.print(hw_value_io_4, 2);
  hw_previous_io_4=hw_value_io_4;
  }
}

