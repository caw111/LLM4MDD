// Host model of the documented TM1637Display 1.2.0 API. Records visible segments;
// it does not claim electrical verification. Both raw-segment and numeric calls
// are supported, so the oracle does not require the production adapter's API.
#pragma once
#include <Arduino.h>
#include <array>
#include <string>
#define SEG_A 0x01
#define SEG_B 0x02
#define SEG_C 0x04
#define SEG_D 0x08
#define SEG_E 0x10
#define SEG_F 0x20
#define SEG_G 0x40
#define DEFAULT_BIT_DELAY 100
namespace oracle {
struct DisplayEvent {uint64_t at;int clk,dio;std::array<uint8_t,4> segments;bool on;};
inline std::vector<DisplayEvent> displays;
}
class TM1637Display {
 int clk,dio;unsigned int bit_delay;bool enabled=false;
 std::array<uint8_t,4> visible{};
public:
 TM1637Display(uint8_t c,uint8_t d,unsigned int delay=DEFAULT_BIT_DELAY):clk(c),dio(d),bit_delay(delay){}
 void setBrightness(uint8_t brightness,bool on=true){enabled=on;(void)brightness;}
 uint8_t encodeDigit(uint8_t digit){
  const uint8_t segments[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
  return segments[digit&15];
 }
 void setSegments(const uint8_t* data,uint8_t length=4,uint8_t pos=0){
  if(pos+length>4)throw std::runtime_error("display-range-unverified");
  for(int i=0;i<length;++i)visible[pos+i]=data[i];
  // 1.2.0 sends length+3 bytes, each with 24 data + 4 ACK delays,
  // plus three start/stop pairs (4 delays each). Preserve bus blocking time.
  for(unsigned int i=0;i<(length+3)*28+12;++i)delayMicroseconds(bit_delay);
  oracle::displays.push_back({oracle::now,clk,dio,visible,enabled});
 }
 void clear(){uint8_t empty[4]={};setSegments(empty);}
 void showNumberDec(int number,bool zeros=false,uint8_t length=4,uint8_t pos=0){showNumberDecEx(number,0,zeros,length,pos);}
 void showNumberDecEx(int number,uint8_t dots=0,bool zeros=false,uint8_t length=4,uint8_t pos=0){
  // Confirmed against the unmodified 1.2.0 library: this safe zero-length
  // case sends only bus commands, retaining every visible segment. Other
  // zero-length formats can access digits[-1]; keep them unverified.
  if(length==0 && zeros && number>=0 && dots==0 && pos<=4){
   uint8_t empty[4]={};setSegments(empty,0,pos);return;
  }
  // Real-library wire probe confirms bounded negative padding: -1 -> 00-1.
  // This is a visible number, never an instruction to blank the display.
  if(length<1||length>4||pos+length>4)throw std::runtime_error("display-format-unverified");
  std::string text=std::to_string(number);
  if(text.size()>length)throw std::runtime_error("display-overflow-unverified");
  text=std::string(length-text.size(),zeros?'0':' ')+text;
  uint8_t data[4]={};
  for(int i=0;i<length;++i){char c=text[i];data[i]=c==' '?0:c=='-'?SEG_G:encodeDigit(c-'0');if(dots&(0x80>>i))data[i]|=0x80;}
  setSegments(data,length,pos);
 }
};
