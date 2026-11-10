// API-level frame fixture for Adafruit DHT 1.4.7. No physical pulse emulation.
#pragma once
#include <Arduino.h>
enum {DHT11=11,DHT22=22};
namespace oracle {
struct DHTFrame {uint64_t start,complete;int pin,type;bool valid;float temperature,humidity;bool initialized;};
inline std::vector<DHTFrame> dht_frames;
}
class DHT {
 int pin,type;bool begun=false,result=false;uint32_t last=0;float temperature=NAN,humidity=NAN;
public:
 DHT(uint8_t p,uint8_t t,uint8_t count=6):pin(p),type(t){(void)count;}
 void begin(uint8_t pull=55){
  if(pull!=55)throw std::runtime_error("dht-pull-time-unverified");
  begun=true;last=uint32_t(millis())-2000;pinMode(pin,INPUT_PULLUP);
 }
 bool read(bool force=false){
  if(oracle::task!=301)throw std::runtime_error("dht-scenario-unverified");
  uint32_t current=millis();if(!force&&uint32_t(current-last)<2000)return result;
  last=current;auto start=oracle::now;
  // Fixed fixture transport durations include a failed attempt. Captured start
  // intervals enforce the library/sensor rate independently of sketch timers.
  delay(type==DHT11?26:7);
  result=begun&&pin==7&&type==DHT11&&start>=2000&&!(start>=6000&&start<10000);
  temperature=start<10000?35:start<14000?32:28;humidity=start<10000?65:45;
  oracle::dht_frames.push_back({start,oracle::now,pin,type,result,temperature,humidity,begun});
  return result;
 }
 float readTemperature(bool fahrenheit=false,bool force=false){
  if(!read(force))return NAN;return fahrenheit?temperature*1.8f+32:temperature;
 }
 float readHumidity(bool force=false){return read(force)?humidity:NAN;}
};
