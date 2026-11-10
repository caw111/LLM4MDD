// Independent hardware-output assertions; never supplied to model prompts.
#include <Arduino.h>
#include <TM1637Display.h>
#include <Servo.h>
void setup(); void loop();
int value_at(int pin, uint64_t time, bool pwm=false) {
 int value=LOW;
 for(auto event:oracle::events) if(event.pin==pin && event.at<=time) value=pwm && !event.pwm ? (event.level?255:0) : event.level;
 return value;
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
int main(int argc,char** argv) {
 if(argc!=2) return 2;
 oracle::task=atoi(argv[1]);
 const uint64_t horizon=oracle::task==29?15000:6000;
 try { setup(); while(oracle::now<=horizon) { loop(); ++oracle::now; } }
 catch(const std::runtime_error& e) { if(std::string(e.what())!="trace-end") { std::cerr<<e.what(); return 3; } }
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
