#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run2\\cases\\pceval_button_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
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
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int32_t* io_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run2\\cases\\pceval_button_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
void setup();
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run2\\cases\\pceval_button_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run2\\cases\\pceval_button_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int32_t* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_value_0 = 0;
    int32_t sysml_conditional_0 = 0;
    if (io_0) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_0 = sysml_conditional_0;
    *io_1 = sysml_value_0;
}
// END COMPILED SYSML MODEL

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
Adafruit_ILI9341 hw_lcd_0(10, 9, 8);
static bool hw_lcd_0_drawn=false;
static int32_t hw_previous_io_1=0;
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

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT);
  hw_lcd_0.begin();
  hw_lcd_0.setRotation(1);
  hw_lcd_0.fillScreen(0);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1);
  if (!hw_lcd_0_drawn || hw_value_io_1!=hw_previous_io_1) {
  hw_lcd_0_drawn=true;
  hw_lcd_0.fillScreen(0);
  hw_lcd_0.setCursor(0,0);
  hw_lcd_0.setTextColor(65535);
  hw_lcd_0.setTextSize(2);
  switch (hw_value_io_1) {
  case 0: hw_lcd_0.print("Not pressed!"); break;
  case 1: hw_lcd_0.print("Pressed!"); break;
  default: break; }
  hw_previous_io_1=hw_value_io_1;
  }
}

