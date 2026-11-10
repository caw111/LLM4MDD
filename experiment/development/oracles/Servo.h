#pragma once
// Observable commanded pulses only: no physical shaft, load, or ISR simulation.
// API subset follows Arduino Servo 1.3.0 AVR; unsupported calibration stays unverified.
#include <Arduino.h>
#define MIN_PULSE_WIDTH 544
#define MAX_PULSE_WIDTH 2400
#define DEFAULT_PULSE_WIDTH 1500
#define INVALID_SERVO 255
namespace oracle {
struct ServoEvent { uint64_t at; int pin; int pulse; bool active; };
inline std::vector<ServoEvent> servos;
inline int servo_count=0;
}
class Servo {
 int index=oracle::servo_count++, pin_=-1, pulse_=1500, low_=544, high_=2400;
 bool active_=false;
 void record() { if(active_) oracle::servos.push_back({oracle::now,pin_,pulse_,true}); }
public:
 uint8_t attach(int pin) { return attach(pin,544,2400); }
 uint8_t attach(int pin,int low,int high) {
   if(pin<0 || pin>=PIN_COUNT || pin>63) throw std::runtime_error("servo-pin-unverified");
   int lo=(544-low)/4, hi=(2400-high)/4;
   if(lo < -128 || lo>127 || hi < -128 || hi>127 || low>=high)
     throw std::runtime_error("servo-calibration-unverified");
   if(index >= (PIN_COUNT==70?48:12)) return INVALID_SERVO;
   low_=544-lo*4;high_=2400-hi*4;
   if(active_) detach();
   pin_=pin;active_=true;pinMode(pin,OUTPUT);record();return index;
 }
 void detach() { if(active_) oracle::servos.push_back({oracle::now,pin_,pulse_,false});active_=false; }
 bool attached() { return active_; }
 void write(int value) {
   if(value<544) value=map(constrain(value,0,180),0,180,low_,high_);
   writeMicroseconds(value);
 }
 void writeMicroseconds(int value) { pulse_=constrain(value,low_,high_);record(); }
 int readMicroseconds() { return pulse_; }
 int read() { return map(pulse_+1,low_,high_,0,180); }
};
