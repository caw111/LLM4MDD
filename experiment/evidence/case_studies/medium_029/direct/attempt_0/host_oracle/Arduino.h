#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <math.h> // Arduino AVR's public Arduino.h includes these declarations.
#include <vector>
#include <stdexcept>
#include <iostream>
using byte = uint8_t;
enum { LOW=0, HIGH=1, INPUT=0, OUTPUT=1, INPUT_PULLUP=2, LED_BUILTIN=13 };
enum { DEFAULT=1, EXTERNAL=0 };
extern "C" void yield();
#if defined(ARDUINO_AVR_MEGA2560)
enum { INTERNAL1V1=2, INTERNAL2V56=3 };
enum { A0=54, A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15, PIN_COUNT=70, ADC_COUNT=16 };
#else
enum { INTERNAL=3 };
enum { A0=14, A1,A2,A3,A4,A5, PIN_COUNT=20, ADC_COUNT=6 };
#endif
#define constrain(x,a,b) ((x)<(a)?(a):((x)>(b)?(b):(x)))
inline long map(long x,long a,long b,long c,long d) { return (x-a)*(d-c)/(b-a)+c; }
namespace oracle {
struct Event { uint64_t at; int pin; int level; bool pwm=false; uint64_t micro_at=0; };
inline uint64_t now=0;
inline unsigned int sub_ms=0;
inline int task=0;
inline uint8_t analog_reference=DEFAULT;
inline int modes[PIN_COUNT]={};
inline std::vector<Event> events;
struct ToneEvent { uint64_t at; int pin; unsigned int frequency; uint64_t until; };
inline std::vector<ToneEvent> tones;
inline void progress() { if(now > (task==29?15500:6500)) throw std::runtime_error("trace-end"); }
}
inline unsigned long millis() { oracle::progress(); return (uint32_t)oracle::now; }
inline unsigned long micros() { oracle::progress(); return (uint32_t)(oracle::now*1000+oracle::sub_ms); }
inline void delay(unsigned long ms) { oracle::now+=ms; oracle::progress(); }
inline void delayMicroseconds(unsigned int us) {
 uint64_t time=oracle::now*1000+oracle::sub_ms+us;
 oracle::now=time/1000;oracle::sub_ms=time%1000;oracle::progress();
}
inline void pinMode(int pin,int mode) { if(pin<0||pin>=PIN_COUNT) throw std::runtime_error("pin-range"); oracle::modes[pin]=mode; }
inline void digitalWrite(int pin,int level) { oracle::events.push_back({oracle::now,pin,level,false,oracle::now*1000+oracle::sub_ms}); }
inline void analogWrite(int pin,int level) {
 if(level<0 || level>255) throw std::runtime_error("pwm-range");
 oracle::events.push_back({oracle::now,pin,level,true});
}
inline void tone(int pin,unsigned int frequency,unsigned long duration=0) {
 if(pin<0||pin>=PIN_COUNT) throw std::runtime_error("pin-range");
 oracle::modes[pin]=OUTPUT; // AVR tone() configures the output pin itself.
 oracle::tones.push_back({oracle::now,pin,frequency,duration?oracle::now+duration:UINT64_MAX});
}
inline void noTone(int pin) { oracle::tones.push_back({oracle::now,pin,0,0}); }
inline void analogReference(uint8_t mode) { oracle::analog_reference=mode; }
inline int analogRead(int pin) {
 if(oracle::analog_reference!=DEFAULT) throw std::runtime_error("analog-reference-unverified");
 if(pin>=0 && pin<ADC_COUNT) pin+=A0; // AVR accepts ADC channel numbers too.
 if(oracle::task==125) {
   if(pin==A3) return oracle::now<1000?0:oracle::now<2000?1023:oracle::now<3000?256:oracle::now<4000?768:oracle::now<5000?511:42;
   if(pin==A4) return oracle::now<1500?512:oracle::now<2300?1023:oracle::now<3400?0:oracle::now<4700?128:oracle::now<5500?900:1000;
   throw std::runtime_error("unexpected-analog-input");
 }
 if(pin!=A0) throw std::runtime_error("unexpected-analog-input");
 return oracle::now<1500?0:oracle::now<1750?1023:oracle::now<2000?511:oracle::now<3400?900:oracle::now<3600?123:0;
}
inline int digitalRead(int pin) {
 if(pin>=0 && pin<PIN_COUNT && oracle::modes[pin]==OUTPUT) {
   int level=LOW;
   for(auto event:oracle::events) if(event.pin==pin) {
     if(event.pwm) throw std::runtime_error("pwm-pin-read-unverified");
     level=event.level;
   }
   return level;
 }
 if(pin!=2) throw std::runtime_error("unexpected-input");
 if(oracle::task==29 && ((oracle::now>=5100 && oracle::now<5300) || (oracle::now>=5800 && oracle::now<6000))) return LOW;
 if(oracle::task==28 && oracle::now>=4500 && oracle::now<4700) return LOW;
 return (oracle::now>=1250 && oracle::now<2250) || (oracle::now>=3300 && oracle::now<3700) ? LOW : HIGH;
}
struct SerialStub { void begin(unsigned long){} template<class T> void print(T){} template<class T> void println(T){} };
inline SerialStub Serial;
