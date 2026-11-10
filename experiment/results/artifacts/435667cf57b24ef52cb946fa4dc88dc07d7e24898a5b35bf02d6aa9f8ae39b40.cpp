#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision35\\cases\\advanced_005\\arduino\\sketch\\sketch.ino"
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
extern int32_t sysml_state_displayRoomIndex;
/* Global Variable define of  */
extern int32_t sysml_state_displayModeIndex;
/* Global Variable define of  */
extern int32_t sysml_state_reportAccumMs;
/* Global Variable define of  */
extern bool sysml_state_prevButton1;
/* Global Variable define of  */
extern bool sysml_state_prevButton2;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, double io_4, bool io_5, double io_6, double io_7, bool io_8, bool io_9, int32_t io_13, bool* io_10, bool* io_11, int32_t* io_12, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, double* serial_0_value_3, double* serial_0_value_4, double* serial_0_value_5, bool* serial_1_trigger, bool* serial_1_value_0, bool* serial_1_value_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_displayRoomIndex = 0;
/* Global Variable define of  */
int32_t sysml_state_displayModeIndex = 0;
/* Global Variable define of  */
int32_t sysml_state_reportAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_prevButton1 = false;
/* Global Variable define of  */
bool sysml_state_prevButton2 = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 143 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision35\\cases\\advanced_005\\arduino\\sketch\\sketch.ino"
static bool manual_debounce_input(bool raw, uint8_t index);
#line 152 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision35\\cases\\advanced_005\\arduino\\sketch\\sketch.ino"
void setup();
#line 173 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision35\\cases\\advanced_005\\arduino\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision35\\cases\\advanced_005\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayRoomIndex = 0;
    sysml_state_displayModeIndex = 0;
    sysml_state_reportAccumMs = 0;
    sysml_state_prevButton1 = false;
    sysml_state_prevButton2 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, double io_4, bool io_5, double io_6, double io_7, bool io_8, bool io_9, int32_t io_13, bool* io_10, bool* io_11, int32_t* io_12, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, double* serial_0_value_3, double* serial_0_value_4, double* serial_0_value_5, bool* serial_1_trigger, bool* serial_1_value_0, bool* serial_1_value_1) {
    static bool previousRoom=false,previousMetric=false,manual=false;
    static bool previousLivingAlarm=false,previousBedroomAlarm=false;
    static uint8_t room=0,metric=0;
    static uint32_t changed=0,lastReport=0;
    uint32_t now=millis();
    if (manual && (uint32_t)(now-changed)>=8000) manual=false;
    if (!manual) {
        unsigned page=now/2000UL%6;
        room=page/3;
        metric=page%3;
    }
    if (io_8 && !previousRoom) {room=1-room;manual=true;changed=now;}
    if (io_9 && !previousMetric) {metric=(metric+1)%3;manual=true;changed=now;}
    previousRoom=io_8;
    previousMetric=io_9;
    bool living=io_2 && io_0>32 && io_6>700;
    bool bedroom=io_5 && io_3>32 && io_7>700;
    *io_10=living;
    *io_11=bedroom;
    double data[][3]={{io_0,io_1,io_6},{io_3,io_4,io_7}};
    *io_12=metric==2 || (room ? io_5 : io_2) ? (int32_t)data[room][metric] : -1000;
    *serial_0_trigger=(uint32_t)(now-lastReport)>=5000;
    if (*serial_0_trigger) lastReport=now;
    *serial_0_value_0=io_2 ? io_0 : NAN;
    *serial_0_value_1=io_2 ? io_1 : NAN;
    *serial_0_value_2=io_6;
    *serial_0_value_3=io_5 ? io_3 : NAN;
    *serial_0_value_4=io_5 ? io_4 : NAN;
    *serial_0_value_5=io_7;
    *serial_1_trigger=(living && !previousLivingAlarm) || (bedroom && !previousBedroomAlarm);
    *serial_1_value_0=living && !previousLivingAlarm;
    *serial_1_value_1=bedroom && !previousBedroomAlarm;
    previousLivingAlarm=living;
    previousBedroomAlarm=bedroom;
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
static double hw_value_io_0 = 0;
static double hw_value_io_1 = 0;
static bool hw_value_io_2 = false;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static bool hw_value_io_5 = false;
static double hw_value_io_6 = 0;
static uint32_t hw_sampled_io_6 = 0;
static bool hw_attempted_io_6 = false;
static double hw_value_io_7 = 0;
static uint32_t hw_sampled_io_7 = 0;
static bool hw_attempted_io_7 = false;
static bool hw_value_io_8 = false;
static uint32_t hw_sampled_io_8 = 0;
static bool hw_attempted_io_8 = false;
static bool hw_value_io_9 = false;
static uint32_t hw_sampled_io_9 = 0;
static bool hw_attempted_io_9 = false;
static bool hw_value_io_10 = false;
static bool hw_value_io_11 = false;
static int32_t hw_value_io_12 = 0;
TM1637Display hw_display_io_12(4, 5);
static int32_t hw_display_previous_io_12 = INT32_MIN;
static int32_t hw_value_io_13 = 0;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static double hw_value_serial_0_value_2 = 0;
static double hw_value_serial_0_value_3 = 0;
static double hw_value_serial_0_value_4 = 0;
static double hw_value_serial_0_value_5 = 0;
static bool hw_value_serial_1_trigger = false;
static bool hw_value_serial_1_value_0 = false;
static bool hw_value_serial_1_value_1 = false;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;
DHT hw_dht_1(8, DHT11);
static bool hw_dht_1_attempted = false;
static uint32_t hw_dht_1_sampled = 0;

static bool manual_debounce_input(bool raw, uint8_t index) {
  static bool previous[8]={}, stable[8]={};
  static uint32_t changed[8]={};
  uint32_t now=millis();
  if(raw!=previous[index]) {previous[index]=raw;changed[index]=now;}
  if((uint32_t)(now-changed[index])>=25UL) stable[index]=raw;
  return stable[index];
}

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_10 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_11 ? HIGH : LOW);
  hw_display_io_12.setBrightness(5, true);
  if (hw_value_io_12 != hw_display_previous_io_12) {
    if (hw_value_io_12 < -999 || hw_value_io_12 > 9999) hw_display_io_12.clear();
    else hw_display_io_12.showNumberDec((int)hw_value_io_12, false, 4, 0);
    hw_display_previous_io_12 = hw_value_io_12;
  }
  hw_dht_0.begin();
  hw_dht_1.begin();
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_6 || (uint32_t)(hw_now - hw_sampled_io_6) >= 10UL) {
    hw_attempted_io_6 = true; hw_sampled_io_6 = hw_now;
    hw_value_io_6 = (analogRead(A0) * 1 + 0);
  }
  if (!hw_attempted_io_7 || (uint32_t)(hw_now - hw_sampled_io_7) >= 10UL) {
    hw_attempted_io_7 = true; hw_sampled_io_7 = hw_now;
    hw_value_io_7 = (analogRead(A1) * 1 + 0);
  }
  if (!hw_attempted_io_8 || (uint32_t)(hw_now - hw_sampled_io_8) >= 10UL) {
    hw_attempted_io_8 = true; hw_sampled_io_8 = hw_now;
    hw_value_io_8 = manual_debounce_input((digitalRead(2) == LOW), 0);
  }
  if (!hw_attempted_io_9 || (uint32_t)(hw_now - hw_sampled_io_9) >= 10UL) {
    hw_attempted_io_9 = true; hw_sampled_io_9 = hw_now;
    hw_value_io_9 = manual_debounce_input((digitalRead(3) == LOW), 1);
  }
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
  uint32_t hw_dht_1_now = (uint32_t)millis();
  if (!hw_dht_1_attempted || (uint32_t)(hw_dht_1_now - hw_dht_1_sampled) >= 2000UL) {
    hw_dht_1_attempted = true; hw_dht_1_sampled = hw_dht_1_now;
    bool hw_dht_1_valid = hw_dht_1.read();
    double hw_dht_1_io_3 = hw_dht_1_valid ? hw_dht_1.readTemperature() : NAN;
    double hw_dht_1_io_4 = hw_dht_1_valid ? hw_dht_1.readHumidity() : NAN;
    hw_dht_1_valid = hw_dht_1_valid && isfinite(hw_dht_1_io_3) && isfinite(hw_dht_1_io_4);
    if (hw_dht_1_valid) {
      hw_value_io_3 = hw_dht_1_io_3;
      hw_value_io_4 = hw_dht_1_io_4;
    }
    hw_value_io_5 = hw_dht_1_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_13 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_5, hw_value_io_6, hw_value_io_7, hw_value_io_8, hw_value_io_9, hw_value_io_13, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2, &hw_value_serial_0_value_3, &hw_value_serial_0_value_4, &hw_value_serial_0_value_5, &hw_value_serial_1_trigger, &hw_value_serial_1_value_0, &hw_value_serial_1_value_1);
  digitalWrite(12, hw_value_io_10 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_11 ? HIGH : LOW);
  if (hw_value_io_12 != hw_display_previous_io_12) {
    if (hw_value_io_12 < -999 || hw_value_io_12 > 9999) hw_display_io_12.clear();
    else hw_display_io_12.showNumberDec((int)hw_value_io_12, false, 4, 0);
    hw_display_previous_io_12 = hw_value_io_12;
  }
  if (hw_value_serial_0_trigger) { Serial.print("living_room_temperature_c"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("living_room_humidity_percent"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("living_room_light_adc"); Serial.println(hw_value_serial_0_value_2); }
  if (hw_value_serial_0_trigger) { Serial.print("bedroom_temperature_c"); Serial.println(hw_value_serial_0_value_3); }
  if (hw_value_serial_0_trigger) { Serial.print("bedroom_humidity_percent"); Serial.println(hw_value_serial_0_value_4); }
  if (hw_value_serial_0_trigger) { Serial.print("bedroom_light_adc"); Serial.println(hw_value_serial_0_value_5); }
  if (hw_value_serial_1_trigger) { Serial.print("living_room_alarm"); Serial.println(hw_value_serial_1_value_0); }
  if (hw_value_serial_1_trigger) { Serial.print("bedroom_alarm"); Serial.println(hw_value_serial_1_value_1); }
}

