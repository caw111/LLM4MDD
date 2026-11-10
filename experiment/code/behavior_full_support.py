"""Host API doubles for broad behavioral capture. No claim of AVR electrical simulation."""
ARDUINO_STRING = r'''
class __FlashStringHelper {};
#undef F
#define F(x) reinterpret_cast<const __FlashStringHelper *>(x)
class String {
 std::string v;
 template<class T> void numeric(T x,int base){std::ostringstream s;if(base==16)s<<std::hex;if(base==8)s<<std::oct;s<<x;v=s.str();}
public:
 String(){} String(const char*s):v(s?s:""){} String(const std::string&s):v(s){} String(char c):v(1,c){}
 String(const __FlashStringHelper*s):v(reinterpret_cast<const char*>(s)){}
 String(int x,int b=10){numeric(x,b);} String(unsigned int x,int b=10){numeric(x,b);}
 String(long x,int b=10){numeric(x,b);} String(unsigned long x,int b=10){numeric(x,b);}
 String(float x,int d=2){std::ostringstream s;s<<std::fixed<<std::setprecision(d)<<x;v=s.str();}
 String(double x,int d=2){std::ostringstream s;s<<std::fixed<<std::setprecision(d)<<x;v=s.str();}
 const char*c_str()const{return v.c_str();} unsigned int length()const{return v.size();} bool reserve(unsigned int n){v.reserve(n);return true;}
 char charAt(unsigned int i)const{return i<v.size()?v[i]:0;} char operator[](unsigned int i)const{return charAt(i);} char&operator[](unsigned int i){return v[i];}
 void setCharAt(unsigned int i,char c){if(i<v.size())v[i]=c;}
 int indexOf(char c,unsigned int from=0)const{auto p=v.find(c,from);return p==std::string::npos?-1:int(p);}
 int indexOf(const String&s,unsigned int from=0)const{auto p=v.find(s.v,from);return p==std::string::npos?-1:int(p);}
 int lastIndexOf(char c)const{auto p=v.rfind(c);return p==std::string::npos?-1:int(p);}
 String substring(unsigned int a)const{return a<v.size()?String(v.substr(a)):String();}
 String substring(unsigned int a,unsigned int b)const{if(a>b)std::swap(a,b);return a<v.size()?String(v.substr(a,b-a)):String();}
 void trim(){auto a=v.find_first_not_of(" \r\n\t");v=a==std::string::npos?"":v.substr(a,v.find_last_not_of(" \r\n\t")-a+1);}
 void toUpperCase(){for(auto&c:v)c=char(toupper((unsigned char)c));} void toLowerCase(){for(auto&c:v)c=char(tolower((unsigned char)c));}
 bool equals(const String&s)const{return v==s.v;} bool equalsIgnoreCase(const String&s)const{String a=*this,b=s;a.toLowerCase();b.toLowerCase();return a==b;}
 bool startsWith(const String&s,unsigned int offset=0)const{return v.compare(offset,s.v.size(),s.v)==0;}
 bool endsWith(const String&s)const{return v.size()>=s.v.size()&&v.compare(v.size()-s.v.size(),s.v.size(),s.v)==0;}
 long toInt()const{return strtol(v.c_str(),nullptr,10);} float toFloat()const{return strtof(v.c_str(),nullptr);} double toDouble()const{return strtod(v.c_str(),nullptr);}
 void remove(unsigned int a){if(a<v.size())v.erase(a);} void remove(unsigned int a,unsigned int n){if(a<v.size())v.erase(a,n);}
 void replace(const String&a,const String&b){if(!a.length())return;size_t p=0;while((p=v.find(a.v,p))!=std::string::npos){v.replace(p,a.length(),b.v);p+=b.length();}}
 void toCharArray(char*buf,unsigned int n,unsigned int index=0)const{if(!n)return;auto s=v.substr(index);size_t k=std::min(size_t(n-1),s.size());memcpy(buf,s.data(),k);buf[k]=0;}
 void getBytes(unsigned char*b,unsigned int n,unsigned int index=0)const{toCharArray(reinterpret_cast<char*>(b),n,index);}
 bool concat(const String&s){v+=s.v;return true;} String&operator+=(const String&s){v+=s.v;return *this;}
 friend String operator+(const String&a,const String&b){return String(a.v+b.v);} friend bool operator==(const String&a,const String&b){return a.v==b.v;}
 friend bool operator!=(const String&a,const String&b){return !(a==b);} friend bool operator<(const String&a,const String&b){return a.v<b.v;}
 friend std::ostream&operator<<(std::ostream&o,const String&s){return o<<s.v;}
 explicit operator bool()const{return !v.empty();}
};
'''

ENVIRONMENT = r'''
#include "case_input.h"
namespace pilot {
struct Read{uint64_t at;int pin;double value;int kind;};inline std::vector<Read> reads;
inline double previous[5][PIN_COUNT]={};inline bool seen[5][PIN_COUNT]={};
inline double sensor(int pin,int kind,double fallback){double v=fallback;for(const auto&e:inputs)if(e.pin==pin&&e.kind==kind&&e.at<=oracle::now)v=e.value;
 if(pin>=0&&pin<PIN_COUNT&&(!seen[kind][pin]||previous[kind][pin]!=v)){reads.push_back({oracle::now,pin,v,kind});previous[kind][pin]=v;seen[kind][pin]=true;}return v;}
inline uint64_t echo_begin[PIN_COUNT]={},echo_end[PIN_COUNT]={},trigger_begin[PIN_COUNT]={};
inline int levels[PIN_COUNT]={},last_pwm[PIN_COUNT]={};inline bool have_output[PIN_COUNT]={};
inline int input(int pin,bool analog){
 if(analog)for(const auto&m:multiplexers)if(m.signal==pin){
  unsigned channel=0;for(unsigned i=0;i<4;++i)if(levels[m.select[i]])channel|=1U<<i;
  int value=0;
  for(const auto&sample:m.samples)if(sample.channel==channel&&sample.at<=oracle::now)value=sample.value;
  return value;
 }
 return int(sensor(pin,analog?0:1,analog?0:1));
}
struct Frame{uint64_t at;std::string name,text;};inline std::vector<Frame> frames;
inline char digit(int bits){const int codes[]={63,6,91,79,102,109,125,7,127,111};if(!bits)return ' ';if(bits==64)return '-';for(int i=0;i<10;++i)if(bits==codes[i])return char('0'+i);if(bits==0x77)return 'A';if(bits==0x73)return 'P';return '?';}
inline void frame(const std::string&name,char text){for(auto it=frames.rbegin();it!=frames.rend();++it)if(it->name==name){if(it->text==std::string(1,text))return;break;}frames.push_back({oracle::now,name,std::string(1,text)});}
inline void gpio(int pin,int value,bool pwm){
 if(pin<0||pin>=PIN_COUNT)throw std::runtime_error("pin-range");int old=levels[pin];levels[pin]=value;
 for(const auto&u:ultrasonic)if(pin==u.trigger){if(value&&old==0)trigger_begin[pin]=oracle::now*1000+oracle::sub_ms;if(!value&&old){auto t=oracle::now*1000+oracle::sub_ms;
   if(oracle::modes[pin]==OUTPUT&&t-trigger_begin[pin]>=10){double cm=sensor(u.echo,4,100);echo_begin[u.echo]=t+200;echo_end[u.echo]=std::isfinite(cm)&&cm>0?echo_begin[u.echo]+uint64_t(cm*58):0;}}}
 bool bus_pin=false;
 for(auto&b:buses){if(pin==b.data||pin==b.clock||pin==b.latch)bus_pin=true;
  if(pin==b.clock&&value&&!old)b.bits=(b.bits<<1)|(levels[b.data]?1:0);
  if(pin==b.latch&&value&&!old)for(const auto&d:b.displays){int bits=0;for(int i=0;i<7;++i){bool bit=(b.bits>>(d.reg*8+d.bits[i]))&1;if(bit!=d.anode)bits|=1<<i;}frame(d.name,digit(bits));}}
 for(const auto&d:direct_displays){bool changed=false;int bits=0;for(int i=0;i<7;++i){if(d.pins[i]==pin)changed=true;bool bit=levels[d.pins[i]]!=0;if(bit!=d.anode)bits|=1<<i;}if(changed)frame(d.name,digit(bits));}
 if(!bus_pin&&(!have_output[pin]||old!=value||last_pwm[pin]!=pwm)){oracle::events.push_back({oracle::now,pin,value,pwm,oracle::now*1000+oracle::sub_ms});have_output[pin]=true;last_pwm[pin]=pwm;}
 if(oracle::events.size()+frames.size()>2000000)throw std::runtime_error("capture-budget-unverified");
}
inline void mode(int pin,int value){
 int old=oracle::modes[pin];oracle::modes[pin]=value;
 if(value==INPUT_PULLUP)levels[pin]=HIGH;
 else if(value==INPUT)levels[pin]=LOW;
 else if(value==OUTPUT&&old!=OUTPUT)gpio(pin,levels[pin],false);
}
inline unsigned long pulse(uint8_t pin,uint8_t state,unsigned long timeout){
 oracle::require_interrupts();if(pin>=PIN_COUNT)throw std::runtime_error("pin-range");auto t=oracle::now*1000+oracle::sub_ms;
 if(state==HIGH&&echo_end[pin]>echo_begin[pin]&&echo_begin[pin]>t&&echo_end[pin]-t<=timeout){auto w=echo_end[pin]-echo_begin[pin];oracle::advance_to(echo_end[pin]);return w;}
 oracle::advance_to(t+timeout);return 0;
}
inline int digital(int pin){for(auto u:ultrasonic)if(pin==u.echo){delayMicroseconds(1);auto t=oracle::now*1000+oracle::sub_ms;return echo_end[pin]>echo_begin[pin]&&t>=echo_begin[pin]&&t<echo_end[pin]?HIGH:LOW;}return input(pin,false);}
inline size_t serial_event=0,serial_offset=0;
inline int available(){oracle::advance_clock();oracle::progress();size_t n=0;for(size_t i=serial_event;i<serial_inputs.size();++i)if(serial_inputs[i].at<=oracle::now)n+=serial_inputs[i].text.size()-(i==serial_event?serial_offset:0);return int(n);}
inline int peek(){if(!available())return -1;return (unsigned char)serial_inputs[serial_event].text[serial_offset];}
inline int read(){int c=peek();if(c<0)return c;if(++serial_offset>=serial_inputs[serial_event].text.size()){++serial_event;serial_offset=0;}return c;}
}
'''

SERIAL_METHODS = r'''
 unsigned long timeout_ms=1000;
 void setTimeout(unsigned long ms){timeout_ms=ms;}
 int available(){return pilot::available();} int read(){return pilot::read();} int peek(){return pilot::peek();}
 int availableForWrite(){return 64;}
 String readStringUntil(char end){std::string text;auto start=millis();while(millis()-start<timeout_ms){int c=read();if(c==end)break;if(c>=0){text+=char(c);start=millis();}else delayMicroseconds(1000);}return String(text);}
 String readString(){return readStringUntil('\0');}
 long parseInt(){auto start=millis();std::string s;while(millis()-start<timeout_ms){int c=peek();if(c<0){delayMicroseconds(1000);continue;}if((c>='0'&&c<='9')||(c=='-'&&s.empty())){s+=char(read());start=millis();}else{if(!s.empty())break;read();}}return strtol(s.c_str(),nullptr,10);}
 float parseFloat(){auto start=millis();std::string s;while(millis()-start<timeout_ms){int c=peek();if(c<0){delayMicroseconds(1000);continue;}if(isdigit(c)||c=='.'||(c=='-'&&s.empty())){s+=char(read());start=millis();}else{if(!s.empty())break;read();}}return strtof(s.c_str(),nullptr);}
 size_t readBytesUntil(char end,char*b,size_t n){auto s=readStringUntil(end);size_t k=std::min(n,size_t(s.length()));memcpy(b,s.c_str(),k);return k;}
 size_t readBytes(char*b,size_t n){size_t k=0;auto start=millis();while(k<n&&millis()-start<timeout_ms){int c=read();if(c>=0){b[k++]=char(c);start=millis();}else delayMicroseconds(1000);}return k;}
 size_t print(const __FlashStringHelper*s){return print(reinterpret_cast<const char*>(s));}
 size_t print(const String&s){return print(s.c_str());}
 size_t write(const char*s,size_t n){size_t r=0;for(size_t i=0;i<n;++i)r+=write(uint8_t(s[i]));return r;}
 size_t write(const uint8_t*s,size_t n){return write(reinterpret_cast<const char*>(s),n);}
'''

DHT = r'''
#pragma once
#include <Arduino.h>
enum {DHT11=11,DHT22=22,DHT21=21};
class DHT {int pin,type;bool begun=false,valid=false;uint32_t last=0;float t=NAN,h=NAN;
public:DHT(uint8_t p,uint8_t typ,uint8_t c=6):pin(p),type(typ){} void begin(uint8_t pull=55){begun=true;last=uint32_t(millis())-2000;pinMode(pin,INPUT_PULLUP);}
 bool read(bool force=false){auto now=uint32_t(millis());if(!force&&uint32_t(now-last)<2000)return valid;last=now;delay(type==11?26:7);t=pilot::sensor(pin,2,25);h=pilot::sensor(pin,3,50);valid=begun&&std::isfinite(t)&&std::isfinite(h);return valid;}
 float readTemperature(bool f=false,bool force=false){return read(force)?(f?t*1.8f+32:t):NAN;}float readHumidity(bool force=false){return read(force)?h:NAN;}
 float computeHeatIndex(float t,float h,bool f=true){return t;} };
'''

WIRE = r'''
#pragma once
#include <Arduino.h>
class TwoWire{public:void begin(){}void setClock(unsigned long){}void beginTransmission(uint8_t){}uint8_t endTransmission(bool=true){return 0;}
 size_t write(uint8_t){return 1;}size_t write(const uint8_t*,size_t n){return n;}uint8_t requestFrom(uint8_t,uint8_t n){throw std::runtime_error("raw-i2c-read-unverified");}int available(){return 0;}int read(){return -1;}};inline TwoWire Wire;
'''

RTC = r'''
#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <ctime>
enum timestampOpt{TIMESTAMP_FULL,TIMESTAMP_TIME,TIMESTAMP_DATE};
class TimeSpan{int32_t seconds_;public:TimeSpan(int32_t s=0):seconds_(s){}TimeSpan(int16_t d,int8_t h,int8_t m,int8_t s):seconds_(d*86400+h*3600+m*60+s){}int32_t totalseconds()const{return seconds_;}int16_t days()const{return seconds_/86400;}int8_t hours()const{return seconds_/3600%24;}int8_t minutes()const{return seconds_/60%60;}int8_t seconds()const{return seconds_%60;}};
class DateTime{uint32_t value;std::tm tm()const{std::time_t n=value;return *std::gmtime(&n);}public:
 DateTime(uint32_t t=0):value(t){} DateTime(uint16_t y,uint8_t m,uint8_t d,uint8_t h=0,uint8_t mi=0,uint8_t s=0){std::tm t={};t.tm_year=(y<100?y+2000:y)-1900;t.tm_mon=m-1;t.tm_mday=d;t.tm_hour=h;t.tm_min=mi;t.tm_sec=s;value=uint32_t(_mkgmtime(&t));}
 DateTime(const char*,const char*):DateTime(2026,1,1){}DateTime(const __FlashStringHelper*a,const __FlashStringHelper*b):DateTime((const char*)a,(const char*)b){}
 uint16_t year()const{return tm().tm_year+1900;}uint8_t month()const{return tm().tm_mon+1;}uint8_t day()const{return tm().tm_mday;}uint8_t hour()const{return tm().tm_hour;}uint8_t minute()const{return tm().tm_min;}uint8_t second()const{return tm().tm_sec;}uint8_t dayOfTheWeek()const{return tm().tm_wday;}bool isValid()const{return true;}uint32_t unixtime()const{return value;}
 String timestamp(timestampOpt opt=TIMESTAMP_FULL)const{char b[40];auto t=tm();strftime(b,sizeof(b),opt==TIMESTAMP_TIME?"%H:%M:%S":opt==TIMESTAMP_DATE?"%Y-%m-%d":"%Y-%m-%dT%H:%M:%S",&t);return String(b);}
 char*toString(char*b)const{auto t=timestamp();strcpy(b,t.c_str());return b;}DateTime operator+(const TimeSpan&s)const{return DateTime(value+s.totalseconds());}DateTime operator-(const TimeSpan&s)const{return DateTime(value-s.totalseconds());}TimeSpan operator-(const DateTime&d)const{return TimeSpan(value-d.value);}
 bool operator<(const DateTime&d)const{return value<d.value;}bool operator>(const DateTime&d)const{return value>d.value;}bool operator==(const DateTime&d)const{return value==d.value;}
};
class RTC_DS1307{DateTime base=DateTime(pilot::rtc_epoch);uint64_t at=0;public:bool begin(TwoWire* = &Wire){return true;}bool isrunning(){return true;}bool lostPower(){return false;}void start(){}void stop(){}void adjust(const DateTime&d){base=d;at=oracle::now;}DateTime now(){return base+TimeSpan((oracle::now-at)/1000);}void writeSqwPinMode(int){}};
class RTC_DS3231:public RTC_DS1307{public:float getTemperature(){return 25;}};
'''

LCD = r'''
#pragma once
#include <Arduino.h>
#include <Wire.h>
namespace pilot{struct LCDFrame{uint64_t at;std::string text;bool initialized;int rotation,background,x,y,color,size,cs,dc,rst;};inline std::vector<LCDFrame> lcd;}
enum{ILI9341_BLACK=0,ILI9341_WHITE=65535,ILI9341_RED=63488,ILI9341_GREEN=2016,ILI9341_BLUE=31,ILI9341_YELLOW=65504,ILI9341_CYAN=2047,ILI9341_MAGENTA=63519,ILI9341_ORANGE=64800};
class Adafruit_ILI9341{
 std::string text;bool initialized=false;int rotation=0,background=-1,x=0,y=0,color=65535,size=1,cs,dc,rst;
public:
 Adafruit_ILI9341(int cs,int dc,int rst=-1):cs(cs),dc(dc),rst(rst){}
 Adafruit_ILI9341(int cs,int dc,int,int,int rst,int=-1):cs(cs),dc(dc),rst(rst){}
 void begin(){initialized=true;}void setRotation(uint8_t value){rotation=value%4;}
 void fillScreen(uint16_t value){text.clear();background=value;}
 void setCursor(int16_t a,int16_t b){text.clear();x=a;y=b;}
 void setTextColor(uint16_t value){color=value;}void setTextColor(uint16_t value,uint16_t){color=value;}
 void setTextSize(uint8_t value){size=value;}void setTextWrap(bool){}int16_t width(){return 320;}int16_t height(){return 240;}
 size_t print(const char*s){text+=s;pilot::lcd.push_back({oracle::now,text,initialized,rotation,background,x,y,color,size,cs,dc,rst});return strlen(s);}size_t print(const __FlashStringHelper*s){return print((const char*)s);}size_t print(const String&s){return print(s.c_str());}
 template<class T>size_t print(T x){std::ostringstream s;s<<x;return print(s.str().c_str());}size_t print(float x,int d){return print(String(x,d));}size_t print(double x,int d){return print(String(x,d));}
 size_t println(){text.clear();return 1;}template<class T>size_t println(T x){auto n=print(x);println();return n;}template<class T>size_t println(T x,int d){auto n=print(x,d);println();return n;}
 void drawPixel(int16_t,int16_t,uint16_t){throw std::runtime_error("lcd-pixel-graphics-unverified");}void drawRect(int16_t,int16_t,int16_t,int16_t,uint16_t){throw std::runtime_error("lcd-shape-graphics-unverified");}void fillRect(int16_t,int16_t,int16_t,int16_t,uint16_t){throw std::runtime_error("lcd-shape-graphics-unverified");}};
'''

MPU = r'''
#pragma once
#include <Arduino.h>
#include <Wire.h>
struct sensors_vec_t{float x=0,y=0,z=0;};struct sensors_event_t{sensors_vec_t acceleration,gyro;float temperature=25;};
enum{MPU6050_RANGE_2_G,MPU6050_RANGE_4_G,MPU6050_RANGE_8_G,MPU6050_RANGE_16_G,MPU6050_RANGE_250_DEG,MPU6050_RANGE_500_DEG,MPU6050_RANGE_1000_DEG,MPU6050_RANGE_2000_DEG,MPU6050_BAND_260_HZ,MPU6050_BAND_184_HZ,MPU6050_BAND_94_HZ,MPU6050_BAND_44_HZ,MPU6050_BAND_21_HZ,MPU6050_BAND_10_HZ,MPU6050_BAND_5_HZ};
enum{MPU6050_GYRO_FS_250=0};
namespace pilot {
inline const ImuSample& current_imu() {
 if(imu_samples.empty())throw std::runtime_error("imu-physical-input-unverified");
 auto current=imu_samples.begin();
 for(auto it=imu_samples.begin();it!=imu_samples.end()&&it->at<=oracle::now;++it)current=it;
 return *current;
}
}
class Adafruit_MPU6050{public:bool begin(uint8_t=0x68,TwoWire* = &Wire,int32_t=0){return true;}void setAccelerometerRange(int){}void setGyroRange(int){}void setFilterBandwidth(int){}int getAccelerometerRange(){return 0;}int getGyroRange(){return 0;}int getFilterBandwidth(){return 0;}
 bool getEvent(sensors_event_t*a,sensors_event_t*g,sensors_event_t*){auto&s=pilot::current_imu();a->acceleration={s.acceleration[0],s.acceleration[1],s.acceleration[2]};g->gyro={s.gyro[0],s.gyro[1],s.gyro[2]};return true;}
};
class MPU6050{public:MPU6050(uint8_t=0x68){}void initialize(){}bool testConnection(){return true;}void setFullScaleGyroRange(int){}int16_t getRotationY(){return pilot::current_imu().raw_y;}
 void getMotion6(int16_t*,int16_t*,int16_t*,int16_t*,int16_t*,int16_t*){throw std::runtime_error("imu-physical-input-unverified");}
};
'''
