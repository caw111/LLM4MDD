#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <math.h> // Arduino AVR's public Arduino.h includes these declarations.
#include <vector>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#define F(text) text
enum { DEC=10, HEX=16, OCT=8, BIN=2 };
using byte = uint8_t;
enum { LOW=0, HIGH=1, INPUT=0, OUTPUT=1, INPUT_PULLUP=2, LED_BUILTIN=13 };
enum { DEFAULT=1, EXTERNAL=0 };
enum { LSBFIRST=0, MSBFIRST=1 };
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
// AVR-libc Park-Miller sequence, independently of the host libc RNG.
// The fixture supplies deterministic ADC input; this does not model entropy.
inline uint32_t random_state=1;
inline long random_next() {
 int64_t x=random_state<=INT32_MAX?int64_t(random_state):int64_t(random_state)-4294967296LL;
 if(x==0)x=123459876;
 x=(16807*x)%2147483647LL;
 if(x<0)x+=2147483647LL;
 random_state=uint32_t(x);return long(x);
}
inline void random_bound(long value) {
 if(value<INT32_MIN||value>INT32_MAX)throw std::runtime_error("random-range-unverified");
}
}
inline void randomSeed(unsigned long seed) { if(uint32_t(seed)!=0)oracle::random_state=uint32_t(seed); }
inline long random(long upper) {
 oracle::random_bound(upper);return upper==0?0:oracle::random_next()%upper;
}
inline long random(long lower,long upper) {
 oracle::random_bound(lower);oracle::random_bound(upper);
 if(lower>=upper)return lower;
 if(int64_t(upper)-lower>INT32_MAX)throw std::runtime_error("random-range-unverified");
 return random(upper-lower)+lower;
}
namespace oracle {
struct Event { uint64_t at; int pin; int level; bool pwm=false; uint64_t micro_at=0; };
inline uint64_t now=0;
inline unsigned int sub_ms=0;
inline int task=0; inline int scenario=0;
inline uint8_t analog_reference=DEFAULT;
inline int modes[PIN_COUNT]={};
inline std::vector<Event> events;
inline bool tm_bus_activity=false;
struct ToneEvent { uint64_t at; int pin; unsigned int frequency; uint64_t until; };
inline std::vector<ToneEvent> tones;
// Limited GPIO critical sections, not an AVR interrupt/timer simulator.
// Preserve explicit microsecond delays; reject timing reads, long modeled
// sections and sections left open across setup/loop boundaries as unverified.
inline bool interrupts_enabled=true;
inline uint64_t critical_start_us=0;
inline unsigned critical_gpio_ops=0;
inline void require_interrupts() {
 if(!interrupts_enabled)throw std::runtime_error("interrupt-dependent-operation-unverified");
}
inline void advance_clock(){ if(++sub_ms>=1000){++now;sub_ms=0;} }
inline void progress() {
 if(!interrupts_enabled && now*1000+sub_ms-critical_start_us>100)
   throw std::runtime_error("long-interrupt-section-unverified");
 if(now > 10000) throw std::runtime_error("trace-end");
}
inline void gpio_operation() {
 if(!interrupts_enabled && ++critical_gpio_ops>128)
   throw std::runtime_error("large-interrupt-section-unverified");
}
}
inline void noInterrupts() {
 if(oracle::interrupts_enabled){oracle::critical_start_us=oracle::now*1000+oracle::sub_ms;oracle::critical_gpio_ops=0;}
 oracle::interrupts_enabled=false;
}
inline void interrupts() { oracle::progress(); oracle::interrupts_enabled=true; }
inline unsigned long millis() { oracle::require_interrupts(); oracle::advance_clock(); oracle::progress(); return (uint32_t)oracle::now; }
inline unsigned long micros() { oracle::require_interrupts(); oracle::advance_clock(); oracle::progress(); return (uint32_t)(oracle::now*1000+oracle::sub_ms); }
inline void delay(unsigned long ms) { oracle::require_interrupts(); oracle::now+=ms; oracle::progress(); }
inline void delayMicroseconds(unsigned int us) {
 uint64_t time=oracle::now*1000+oracle::sub_ms+us;
 oracle::now=time/1000;oracle::sub_ms=time%1000;oracle::progress();
}
namespace oracle {
// Independent HC-SR04 electrical fixture. Each valid TRIG falling edge creates
// one ECHO pulse from the environment at acquisition start; it is not a model output.
inline bool ultrasonic_task(){return task==7||task==22||task==301;}
struct Acquisition {uint64_t at,width;};
inline std::vector<Acquisition> acquisitions;
inline int trigger_level=LOW;
inline uint64_t trigger_rise=0,echo_begin=UINT64_MAX,echo_end=UINT64_MAX;
inline uint64_t pulse_width_at(uint64_t t) {
 if(task==301) {
  if(t<1000)return 464;
  if(t<3500)return 1740;
  if(t<5000)return 580;
  if(t<7000)return 870; // exactly 15cm
  if(t<9000)return 841; // 14.5cm: compare before display rounding
  if(t<11000)return 0;
  if(t<13000)return 1740;
  if(t<15000)return UINT64_MAX;
  return 464;
 }
 if(t<500)return 0;
 if(t<1400)return 1740; // 30cm
 if(t<2100)return 551;  // 9.5cm: display quantization must not change threshold behavior
 if(t<2800)return 580;  // 10cm: strictly-less-than threshold must be OFF
 if(t<3400)return 638;  // 11cm
 if(t<3900)return 116;  // 2cm
 if(t<4400)return 23200; // 400cm
 if(t<4800)return 0;    // absent echo
 if(t<5100)return 1160; // 20cm
 if(t<5400)return 58;   // out-of-range short echo (1cm)
 if(t<5700)return UINT64_MAX; // echo stuck high
 return 464; // 8cm, recovery
}
inline void trigger_write(int pin,int level) {
 if(!ultrasonic_task()||pin!=8)return;
 const uint64_t t=now*1000+sub_ms;
 if(level==HIGH && trigger_level==LOW)trigger_rise=t;
 if(level==LOW && trigger_level==HIGH) {
   echo_begin=echo_end=UINT64_MAX;
   if(modes[8]==OUTPUT && t-trigger_rise>=10) {
     const auto width=pulse_width_at(t/1000);acquisitions.push_back({t,width});
     if(width) {echo_begin=t+200;echo_end=width==UINT64_MAX?UINT64_MAX:echo_begin+width;}
   }
 }
 trigger_level=level;
}
inline void advance_to(uint64_t time){now=time/1000;sub_ms=time%1000;progress();}
}
inline unsigned long pulseIn(uint8_t pin,uint8_t state,unsigned long timeout=1000000UL) {
 oracle::require_interrupts();
 if(pin>=PIN_COUNT)throw std::runtime_error("pin-range");
 unsigned long width=oracle::scenario==0?0:oracle::scenario==1?1740:580;
 delayMicroseconds(width && width<=timeout?width:1000);
 return width<=timeout?width:0;
}
inline void pinMode(int pin,int mode) { oracle::gpio_operation(); if(pin<0||pin>=PIN_COUNT) throw std::runtime_error("pin-range"); if(oracle::task==301&&(pin==4||pin==5))oracle::tm_bus_activity=true; oracle::modes[pin]=mode; }
inline void digitalWrite(int pin,int level) { oracle::gpio_operation(); if(oracle::task==301&&(pin==4||pin==5))oracle::tm_bus_activity=true; if(pin<0||pin>=PIN_COUNT)throw std::runtime_error("pin-range"); oracle::trigger_write(pin,level); oracle::events.push_back({oracle::now,pin,level,false,oracle::now*1000+oracle::sub_ms}); }
inline void shiftOut(uint8_t data,uint8_t clock,uint8_t order,uint8_t value) {
 for(uint8_t i=0;i<8;++i) {
  digitalWrite(data,(value & (1 << (order==LSBFIRST?i:7-i))) ? HIGH : LOW);
  digitalWrite(clock,HIGH);digitalWrite(clock,LOW);
 }
}
inline void analogWrite(int pin,int level) {
 if(level<0 || level>255) throw std::runtime_error("pwm-range");
 pinMode(pin,OUTPUT); // Arduino AVR 1.8.6 wiring_analog.c does this itself.
 oracle::events.push_back({oracle::now,pin,level,true});
}
inline void tone(int pin,unsigned int frequency,unsigned long duration=0) {
 oracle::require_interrupts();
 if(pin<0||pin>=PIN_COUNT) throw std::runtime_error("pin-range");
 oracle::modes[pin]=OUTPUT; // AVR tone() configures the output pin itself.
 oracle::tones.push_back({oracle::now,pin,frequency,duration?oracle::now+duration:UINT64_MAX});
}
inline void noTone(int pin) { oracle::tones.push_back({oracle::now,pin,0,0}); }
inline void analogReference(uint8_t mode) { oracle::analog_reference=mode; }
inline int analogRead(int pin) {
 oracle::require_interrupts(); oracle::advance_clock(); oracle::progress();
 if(pin>=0 && pin<ADC_COUNT)pin+=A0;
 if(pin<A0 || pin>=A0+ADC_COUNT)throw std::runtime_error("pin-range");
 return oracle::scenario==0?0:oracle::scenario==1?1023:int((oracle::now/1000)%3)*511;
}
inline int digitalRead(int pin) {
 oracle::advance_clock(); oracle::progress();
 if(pin<0 || pin>=PIN_COUNT)throw std::runtime_error("pin-range");
 if(oracle::modes[pin]==OUTPUT){for(auto i=oracle::events.rbegin();i!=oracle::events.rend();++i)if(i->pin==pin)return i->level?HIGH:LOW;return LOW;}
 return oracle::scenario==0?LOW:oracle::scenario==1?HIGH:((oracle::now/500)%2?HIGH:LOW);
}
namespace oracle {
struct SerialEvent {uint64_t at;unsigned long baud;std::string text;};
inline std::vector<SerialEvent> serial_lines;
}
struct SerialStub {
 unsigned long baud=0;std::string pending;
 int available(){return 0;} int read(){return -1;} int peek(){return -1;}

 void begin(unsigned long b){baud=b;}
 explicit operator bool()const{return baud!=0;}
 size_t write(uint8_t c){
  // Bound UART transport with one serialized ten-bit character. This does not
  // simulate AVR ring buffers, interrupts, or instruction-level timing.
  if(baud)delayMicroseconds((10000000UL+baud-1)/baud);
  if(c=='\n'){oracle::serial_lines.push_back({oracle::now,baud,pending});pending.clear();}
  else if(c!='\r')pending.push_back(char(c));
  if(pending.size()>4096||oracle::serial_lines.size()>200000)throw std::runtime_error("serial-volume-unverified");
  return 1;
 }
 size_t print(const char* s){size_t n=0;while(*s)n+=write(uint8_t(*s++));return n;}
 size_t print(char c){return write(uint8_t(c));}
 size_t print(double value,int digits=2){
  if(digits<0||digits>9)throw std::runtime_error("serial-precision-unverified");
  std::ostringstream text;text<<std::fixed<<std::setprecision(digits)<<value;return print(text.str().c_str());
 }
 size_t print(float value,int digits=2){return print(double(value),digits);}
 template<class T> size_t print(T value,int base=10){
  if(base!=10)throw std::runtime_error("serial-base-unverified");
  std::ostringstream text;text<<+value;return print(text.str().c_str());
 }
 size_t println(){return write('\r')+write('\n');}
 template<class T> size_t println(T value){auto n=print(value);return n+println();}
 template<class T> size_t println(T value,int format){auto n=print(value,format);return n+println();}
 void flush(){}
};
inline SerialStub Serial;
