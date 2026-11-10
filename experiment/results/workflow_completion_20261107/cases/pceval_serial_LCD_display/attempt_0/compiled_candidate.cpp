#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run13_offline_recovery\\cases\\pceval_serial_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
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
extern int32_t sysml_state_acceptedFrame;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_0_valid, int32_t* io_1, int32_t* io_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_acceptedFrame = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run13_offline_recovery\\cases\\pceval_serial_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
static bool hw_rx_parse(int32_t* value);
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run13_offline_recovery\\cases\\pceval_serial_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
static bool hw_rx_finish(int32_t* value);
#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run13_offline_recovery\\cases\\pceval_serial_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
static bool hw_rx_poll(uint32_t hw_now, int32_t* value);
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run13_offline_recovery\\cases\\pceval_serial_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
void setup();
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run13_offline_recovery\\cases\\pceval_serial_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run13_offline_recovery\\cases\\pceval_serial_LCD_display\\attempt_0\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_acceptedFrame = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_0_valid, int32_t* io_1, int32_t* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    if (io_0_valid) {
        sysml_state_acceptedFrame = io_0;
    }
    int32_t sysml_value_0 = 0;
    sysml_value_0 = sysml_state_acceptedFrame;
    int32_t sysml_value_1 = 0;
    sysml_value_1 = sysml_state_acceptedFrame;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
}
// END COMPILED SYSML MODEL

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
Adafruit_ILI9341 hw_lcd_0(10, 9, -1);
static bool hw_lcd_0_drawn=false;
static int32_t hw_previous_io_1=0;
static int32_t hw_previous_io_2=0;
// LLM4MDD target-board: arduino:avr:uno
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_UNO)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static int32_t hw_value_io_0 = 0;
#include <string.h>
static char hw_rx_buffer[64];
static char hw_rx_text[64];
static uint8_t hw_rx_size = 0;
static bool hw_rx_overflow = false;
static uint32_t hw_rx_last_byte = 0;
static bool hw_rx_parse(int32_t* value) {
  hw_rx_buffer[hw_rx_size] = '\0';
  char* begin = hw_rx_buffer;

  if (!*begin) return false;
  strncpy(hw_rx_text,begin,63);hw_rx_text[63]='\0';
  *value=(*value>=2147483647L)?1:*value+1;
  return true;
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
static bool hw_value_io_0_valid = false;
static int32_t hw_value_io_1 = 0;
static int32_t hw_value_io_2 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  hw_lcd_0.begin();
  hw_lcd_0.setRotation(1);
  hw_lcd_0.fillScreen(0);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_0_valid = hw_rx_poll(hw_now, &hw_value_io_0);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_0_valid, &hw_value_io_1, &hw_value_io_2);
  if (!hw_lcd_0_drawn || hw_value_io_1!=hw_previous_io_1) {
  hw_lcd_0_drawn=true;
  hw_lcd_0.fillScreen(0);
  hw_lcd_0.setCursor(0,0);
  hw_lcd_0.setTextColor(65535);
  hw_lcd_0.setTextSize(2);
  if (hw_value_io_1>0) hw_lcd_0.print(hw_rx_text);
  hw_previous_io_1=hw_value_io_1;
  }
  if (hw_value_io_2>0 && hw_value_io_2!=hw_previous_io_2) {Serial.println(hw_rx_text);hw_previous_io_2=hw_value_io_2;}
}

