// Independent hardware-output assertions; never supplied to model prompts.
#include <Arduino.h>
#include <TM1637Display.h>
#include <Servo.h>
#include <DHT.h>
#include <algorithm>
void setup(); void loop();
int check_mixed_environment();
int value_at(int pin, uint64_t time, bool pwm=false) {
 // Validation happens after capture. Index each physical pin once so sketches
 // that continuously refresh a serial display do not time out on unrelated PWM.
 static bool indexed=false;static std::vector<const oracle::Event*> pins[PIN_COUNT];
 if(!indexed){for(const auto& event:oracle::events)if(event.pin>=0&&event.pin<PIN_COUNT)pins[event.pin].push_back(&event);indexed=true;}
 const auto& events=pins[pin];
 auto end=std::upper_bound(events.begin(),events.end(),time,[](uint64_t at,const oracle::Event* e){return at<e->at;});
 if(end==events.begin())return LOW;
 const auto& event=**(--end);
 return pwm && !event.pwm ? (event.level?255:0) : event.level;
}
int frequency_at(int pin,uint64_t time) {
 int value=0;
 for(auto event:oracle::tones) if(event.pin==pin && event.at<=time)
   value=time<event.until?event.frequency:0;
 if(value) return value;
 // Also observe software-generated carrier edges. The oracle must not require
 // use of tone() to accept an otherwise correct passive-buzzer implementation.
 uint64_t center=time*1000,first=0,last=0;int rises=0,level=LOW;
 for(auto event:oracle::events) if(event.pin==pin && !event.pwm) {
   if(event.level==HIGH && level==LOW && event.micro_at+2000>=center && event.micro_at<=center+2000) {
     if(rises==0) first=event.micro_at;
     last=event.micro_at;++rises;
   }
   level=event.level;
 }
 return rises>=2 && last>first ? (int)((1000000ULL*(rises-1)+(last-first)/2)/(last-first)) : 0;
}
#include "mixed_environment_trace.h"
int main(int argc,char** argv) {
 if(argc!=2) return 2;
 oracle::task=atoi(argv[1]);
 const uint64_t horizon=oracle::task==301?18000:oracle::task==29?15000:6000;
 // A fixed 1ms loop overhead aliases a correct nonblocking 1000Hz carrier
 // into 500Hz. Use sub-millisecond scheduling for every arm and task. This is
 // a host observation clock, not a claim about AVR instruction timing.
 try { setup(); oracle::require_interrupts(); while(oracle::now<=horizon) { loop(); oracle::require_interrupts(); delayMicroseconds(oracle::ultrasonic_task()?1:100); } }
 catch(const std::runtime_error& e) { if(std::string(e.what())!="trace-end") { std::cerr<<e.what(); return 3; } }
 if(oracle::task==301)return check_mixed_environment();
 if(oracle::task==7||oracle::task==22) {
   if(oracle::modes[8]!=OUTPUT||oracle::modes[9]!=INPUT){std::cerr<<"ultrasonic pin mode";return 6;}
   if(oracle::task==22 && oracle::modes[13]!=OUTPUT){std::cerr<<"LED pin mode";return 6;}
   if(oracle::acquisitions.size()<30){std::cerr<<"missing continuous acquisitions";return 6;}
   for(size_t i=1;i<oracle::acquisitions.size();++i) {
     auto interval=oracle::acquisitions[i].at-oracle::acquisitions[i-1].at;
     if(interval<60000||interval>160000){std::cerr<<"sampling interval out of 60..160ms: "<<interval;return 6;}
   }
   const uint8_t digits[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
   for(const auto& e:oracle::displays)if(e.clk==4&&e.dio==5&&e.at<500&&e.on)
     for(auto segment:e.segments)if(segment){std::cerr<<"startup invalid distance displayed as a number";return 6;}
   for(uint64_t t=250;t<=6000;t+=10) {
     bool boundary=false;
     for(uint64_t edge:{500,1400,2100,2800,3400,3900,4400,4800,5100,5400,5700})if(t>=edge&&t<edge+250)boundary=true;
     if(boundary)continue;
     int expected=t<500?-1:t<1400?30:t<2100?9:t<2800?10:t<3400?11:t<3900?2:t<4400?400:t<4800?-1:t<5100?20:t<5700?-1:8;
     const oracle::DisplayEvent* last=nullptr;
     for(const auto& e:oracle::displays)if(e.clk==4&&e.dio==5&&e.at<=t)last=&e;
     if(!last||(expected>=0&&!last->on)){std::cerr<<"missing/off distance display at "<<t;return 6;}
     int actual=0;bool seen=false;
     if(last->on)for(auto segment:last->segments) {
       if(!seen&&segment==0)continue;
       int digit=-1;for(int n=0;n<10;++n)if(segment==digits[n])digit=n;
       if(digit<0){std::cerr<<"nondecimal distance display at "<<t;return 6;}
       actual=10*actual+digit;seen=true;
     }
     if((expected<0&&seen)||(expected>=0&&(!seen||abs(actual-expected)>1))){std::cerr<<"distance at "<<t<<" expected "<<expected<<", got "<<(seen?actual:-1);return 6;}
     const int led=expected>=2&&expected<10?HIGH:LOW;
     if(oracle::task==22&&value_at(13,t)!=led){std::cerr<<"distance LED at "<<t<<" expected "<<led;return 6;}
   }
   std::cout<<"passed\n";return 0;
 }
 if(oracle::task==116 || oracle::task==179 || oracle::task==204) {
   // Independent pin-level reconstruction of the supplied two-register chain:
   // A0->sr1 DS, A2 shared clock, A1 shared latch, sr1 Q7S->sr2 DS.
   // Both displays are common anode; sr1 is tens, sr2 is units.
   for(int pin:{A0,A1,A2}) if(oracle::modes[pin]!=OUTPUT) {std::cerr<<"shift pin mode";return 6;}
   if(oracle::task==116 && oracle::modes[2]!=INPUT_PULLUP) {std::cerr<<"button pullup";return 6;}
   if(oracle::task==179)for(int pin:{2,3,4})if(oracle::modes[pin]!=INPUT_PULLUP){std::cerr<<"button pullup";return 6;}
   if(oracle::task==204)for(int pin:{2,3})if(oracle::modes[pin]!=OUTPUT){std::cerr<<"LED mode";return 6;}
   struct Frame {uint64_t at;uint16_t bits;};std::vector<Frame> frames;
   int data=0,clock=0,latch=0;uint16_t shift=0;
   for(const auto& event:oracle::events) {
     if(event.pwm && (event.pin==A0||event.pin==A1||event.pin==A2)) {std::cerr<<"invalid bus PWM";return 6;}
     if(event.pin==A0) data=event.level!=0;
     if(event.pin==A2) {if(event.level && !clock) shift=(uint16_t)((shift<<1)|data);clock=event.level!=0;}
     if(event.pin==A1) {if(event.level && !latch) frames.push_back({event.at,shift});latch=event.level!=0;}
   }
   // Segment sets are independent of the production encoder and contract.
   const char* symbols[10]={"ABCDEF","BC","ABDEG","ABCDG","BCFG","ACDFG","ACDEFG","ABC","ABCDEFG","ABCDFG"};
   auto digit=[&](uint8_t raw) {
     // Q7/DP is unconnected in this supplied circuit; its level is unobservable.
     uint8_t lit=(uint8_t)~raw & 0x7f;
     for(int value=0;value<10;++value) {uint8_t expected=0;for(const char* s=symbols[value];*s;++s)expected|=1<<(*s-'A');if(lit==expected)return value;}
     return -1;
   };
   // Manually reviewed step timeline for the three-button variable-rate task.
   // 511 ADC gives 549ms, 1023 gives 1000ms, 0 gives 100ms. Changing ADC
   // during a hold changes the current interval measured since the last step.
   const uint64_t step_times[]={0,100,300,500,1049,1598,1800,2100,3100,3500,3600,3700,3800,4000,4549,5549,5800};
   const int step_values[]={0,99,0,1,2,3,0,99,98,99,0,1,2,3,4,5,0};
   for(uint64_t t=20;t<=6000;t+=(oracle::task==179?1:10)) {
     bool boundary=false;
     if(oracle::task==116)for(uint64_t edge:{1250,3300,4500,5100,5800}) if(t>=edge && t<edge+20)boundary=true;
     if(oracle::task==179)for(auto edge:step_times)if(t>=edge&&t<edge+20)boundary=true;
     // The public repeat-time tolerance is +/-20ms. Sensor/button response
     // remains causal: only autonomous repeat edges may precede their target.
     if(oracle::task==179)for(uint64_t edge:{1049,1598,3100,3600,3700,3800,4549,5549})
       if(t<edge&&t+20>=edge)boundary=true;
     if(oracle::task==204)for(uint64_t edge:{500,1000,1500,2000,2500,3000,3500,4000,4500,5000,5500})if(t>=edge&&t<edge+50)boundary=true;
     if(boundary)continue;
     auto end=std::upper_bound(frames.begin(),frames.end(),t,[](uint64_t at,const Frame& f){return at<f.at;});
     const Frame* latest=end==frames.begin()?nullptr:&*(end-1);
     int tens=latest?digit(latest->bits & 255):-1,units=latest?digit(latest->bits>>8):-1;
     int expected=t<1250?0:t<3300?37:t<4500?74:t<5100?0:t<5800?37:74;
     if(oracle::task==179){expected=0;for(unsigned i=0;i<sizeof(step_times)/sizeof(step_times[0]);++i)if(t>=step_times[i])expected=step_values[i];}
     if(oracle::task==204) {
       expected=t<1000?0:t<1500?99:t<2000?49:t<3000?50:t<4000?9:t<4500?10:t<5000?66:t<5500?67:0;
       int first=t<500?0:t<1500?255:t<2500?63:t<3500?224:t<4500?0:t<5500?127:0;
       int second=t<500?0:t<1500?127:t<2500?224:t<3500?30:t<4500?255:t<5500?63:0;
       if(expected%2)first=0;else second=0;
       for(int pin:{2,3}) {
         int actual=value_at(pin,t,true), wanted=pin==2?first:second;
         if(abs(actual-wanted)>1){std::cerr<<"incorrect parity PWM on D"<<pin<<" at "<<t<<"; expected "<<wanted<<", got "<<actual;return 6;}
       }
     }
     if(tens<0||units<0||10*tens+units!=expected) {std::cerr<<"incorrect shift display at "<<t<<" ms; expected "<<expected<<", got digits "<<tens<<","<<units;return 6;}
   }
   std::cout<<"passed\n";return 0;
 }
 if(oracle::task==125) {
   // Two independent potentiometers; verify all three physical outputs.
   // Degree tolerance covers integer command / microsecond quantization.
   for(uint64_t t=50;t<=6000;t+=10) {
     bool boundary=false;
     for(uint64_t edge:{1000,1500,2000,2300,3000,3400,4000,4700,5000,5500})
       if(t>=edge && t<edge+50) boundary=true;
     if(boundary) continue;
     int first=t<1000?0:t<2000?1023:t<3000?256:t<4000?768:t<5000?511:42;
     int second=t<1500?512:t<2300?1023:t<3400?0:t<4700?128:t<5500?900:1000;
     for(int pin:{2,3,4}) {
       const oracle::ServoEvent* latest=nullptr;
       for(const auto& e:oracle::servos) if(e.pin==pin && e.at<=t) latest=&e;
       double expected=(pin==2?first:second)*180.0/1023.0;
       double angle=latest?(latest->pulse-544)*180.0/1856.0:-999;
       if(!latest || !latest->active || angle<expected-1.1 || angle>expected+1.1) {
         std::cerr<<"incorrect servo on D"<<pin<<" at "<<t<<"; expected "<<expected<<", got "<<angle;return 6;
       }
     }
   }
   std::cout<<"passed\n";return 0;
 }
 if(oracle::task==19) {
   const uint8_t digits[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
   for(uint64_t t=50;t<=6000;t+=10){
     bool boundary=false;
     for(uint64_t edge:{1500,1750,2000,3400,3600})if(t>=edge&&t<edge+50)boundary=true;
     if(boundary)continue;
     const oracle::DisplayEvent* latest=nullptr;
     for(const auto& e:oracle::displays)if(e.clk==4&&e.dio==5&&e.at<=t)latest=&e;
     if(!latest||!latest->on){std::cerr<<"display missing/off at "<<t;return 6;}
     int value=0;bool seen=false;
     for(auto segment:latest->segments){
       if(!seen&&segment==0)continue;
       int digit=-1;for(int i=0;i<10;++i)if(segment==digits[i])digit=i;
       if(digit<0){std::cerr<<"nondecimal display at "<<t;return 6;}
       value=value*10+digit;seen=true;
     }
     int adc=t<1500?0:t<1750?1023:t<2000?511:t<3400?900:t<3600?123:0;
     int expected=adc*9999/1023;
     if(!seen||value!=expected){std::cerr<<"incorrect display at "<<t<<"; expected "<<expected<<", got "<<value;return 6;}
   }
   std::cout<<"passed\n";return 0;
 }
 if(oracle::task==21 || oracle::task==29) {
   if(oracle::task==29 && oracle::modes[2]!=INPUT_PULLUP) { std::cerr<<"missing pullup"; return 7; }
   // Public pitch/duration/phase interpretation; input timeline is independent.
   // Task 29 pauses at 1250 and resumes at 3300; pauses within its first beep
   // at 5100 and resumes at 5800. Verify remaining pulse and later periods.
   for(uint64_t t=0;t<=horizon;t+=5) {
     bool boundary=false;
     if(oracle::task==21) {
       for(uint64_t start=1000;start<=6000;start+=1000)
         for(uint64_t edge:{start,start+100}) if(t+20>=edge && t<=edge+20) boundary=true;
     } else {
       for(uint64_t edge:{5050,5100,5800,5850,8750,8850,11750,11850,14750,14850}) if(t+20>=edge && t<=edge+20) boundary=true;
     }
     if(boundary) continue;
     bool active=oracle::task==21 ? t>=1000 && t%1000<100 :
         (t>=5050 && t<5100)||(t>=5800 && t<5850)||(t>=8750 && t<8850)||(t>=11750 && t<11850)||(t>=14750 && t<14850);
     int expected=active?1000:0;
     if(frequency_at(6,t)!=expected) {
       std::cerr<<"incorrect tone at "<<t<<" ms; expected "<<expected<<", got "<<frequency_at(6,t);return 6;
     }
   }
   std::cout<<"passed\n";return 0;
 }
 const int pin=oracle::task==2?12:oracle::task==4?9:oracle::task==8?5:13;
 if(oracle::modes[pin]!=OUTPUT) { std::cerr<<"output mode"; return 4; }
 bool exercised=false;
 for(auto e:oracle::events) if(e.pin==pin) exercised=true;
 if(!exercised) { std::cerr<<"no output writes"; return 5; }
 for(uint64_t t=20;t<=6000;t+=10) {
   int expected;
   int tolerance=0;
   bool pwm=oracle::task==4 || oracle::task==8;
   if(oracle::task==1) expected=true;
   else if(oracle::task==2) {
     // Public requirement: initially off, 500 ms half-period, <=10 ms tolerance.
     if(t%500<=10 || t%500>=490) continue;
     expected=(t/500)%2;
   } else if(oracle::task==8) {
     uint64_t phase=t%2000;
     expected=(phase<=1000?phase:2000-phase)*255/1000;
     tolerance=5;
   } else if(oracle::task==28) {
     // Independent expected trace: start at first press, stop at second, restart at third.
     bool boundary=false;
     for(uint64_t edge:{1250,1750,2250,2750,3250,3300,4500,5000,5500,6000})
       if(t+20>=edge && t<=edge+20) boundary=true;
     if(boundary) continue;
     expected=t<1250?0:t<3300?((t-1250)/500)%2:t<4500?0:((t-4500)/500)%2;
   } else if(oracle::task==4) {
     bool boundary=false;
     for(uint64_t edge:{1250,1500,1750,2000,2250,3300,3400,3600,3700})
       if(t>=edge && t<=edge+20) boundary=true;
     if(boundary) continue;
     int adc=t<1500?0:t<1750?1023:t<2000?511:t<3400?900:t<3600?123:0;
     bool pressed=(t>=1250 && t<2250)||(t>=3300 && t<3700);
     expected=pressed?adc*255/1023:0;
     tolerance=1;
   } else if(oracle::task==3) {
     // Public input response bound 20 ms, independent switching sequence.
     if((t>=1250 && t<1270)||(t>=2250 && t<2270)||(t>=3300 && t<3320)||(t>=3700 && t<3720)) continue;
     expected=(t>=1250 && t<2250)||(t>=3300 && t<3700);
   } else return 8;
   if(abs(value_at(pin,t,pwm)-expected)>tolerance) { std::cerr<<"incorrect output at "<<t<<" ms; expected "<<expected<<", got "<<value_at(pin,t,pwm); return 6; }
 }
 if((oracle::task==3 || oracle::task==4 || oracle::task==28) && oracle::modes[2]!=INPUT_PULLUP) { std::cerr<<"missing pullup"; return 7; }
 std::cout<<"passed\n";
}
