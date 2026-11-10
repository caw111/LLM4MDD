// Hand-reviewed public-output checks. No generated model/IR is used as an oracle.
#pragma once
struct MixedState {
 uint64_t at=0;bool tv=false,dv=false;double temperature=0,distance=0;
 bool hot()const{return tv&&temperature>32;}
 bool near()const{return dv&&distance<15;}
 bool active()const{return hot()||near();}
};
inline MixedState mixed_at(uint64_t t) {
 MixedState s;s.at=t;
 for(const auto& f:oracle::dht_frames)if(f.complete<=t){s.tv=f.valid;s.temperature=f.valid?f.temperature:0;}
 for(const auto& a:oracle::acquisitions)if(a.at/1000<=t){s.dv=a.width>=116&&a.width<=23200;s.distance=s.dv?a.width/58.0:0;}
 return s;
}
inline int check_mixed_environment() {
 auto fail=[](const std::string& msg){std::cerr<<msg;return 6;};
 if(oracle::modes[8]!=OUTPUT||oracle::modes[9]!=INPUT||oracle::modes[7]!=INPUT_PULLUP)
  return fail("mixed sensor wiring or initialization");
 if(oracle::dht_frames.size()<8||oracle::dht_frames.front().start>200)return fail("missing continuous DHT sampling");
 for(size_t i=0;i<oracle::dht_frames.size();++i) {
  const auto& f=oracle::dht_frames[i];
  if(f.pin!=7||f.type!=11||!f.initialized)return fail("DHT11 must initialize on D7");
  if(i){auto dt=f.start-oracle::dht_frames[i-1].start;if(dt<2000||dt>2200)return fail("DHT acquisition interval outside 2000..2200ms");}
 }
 if(oracle::acquisitions.size()<90||oracle::acquisitions.front().at>200000)return fail("missing mixed distance acquisitions");
 for(size_t i=1;i<oracle::acquisitions.size();++i){auto dt=oracle::acquisitions[i].at-oracle::acquisitions[i-1].at;
  if(dt<60000||dt>180000)return fail("mixed distance acquisition interval outside 60..180ms: "+std::to_string(dt));}
 std::vector<uint64_t> times{0};
 for(const auto& f:oracle::dht_frames)times.push_back(f.complete);
 for(const auto& a:oracle::acquisitions)times.push_back(a.at/1000);
 std::sort(times.begin(),times.end());
 std::vector<MixedState> changes;
 for(auto t:times){auto s=mixed_at(t);if(changes.empty()||s.tv!=changes.back().tv||s.dv!=changes.back().dv||s.temperature!=changes.back().temperature||s.distance!=changes.back().distance)changes.push_back(s);}
 auto cause_changed=[&](uint64_t from,uint64_t to){
  auto last=mixed_at(from);
  for(auto s:changes)if(s.at>from&&s.at<=to){if(s.hot()!=last.hot()||s.near()!=last.near())return true;last=s;}
  return false;
 };
 // Serial reports can share a line or use one line per cause. Unrelated status
 // lines are ignored; these public reason tokens must never report false causes.
 std::vector<uint64_t> reports[2];
 for(const auto& line:oracle::serial_lines){
  bool flags[2]={line.text.find("ALARM_TEMP")!=std::string::npos,line.text.find("ALARM_DISTANCE")!=std::string::npos};
  if(!flags[0]&&!flags[1])continue;
  if(line.baud!=115200)return fail("alarm serial baud must be 115200");
  auto current=mixed_at(line.at),before=mixed_at(line.at>200?line.at-200:0);
  bool now[2]={current.hot(),current.near()},old[2]={before.hot(),before.near()};
  for(int k=0;k<2;++k)if(flags[k]){
   if(!now[k]&&!old[k])return fail("serial reported inactive cause: "+line.text);
   const auto token=k?"ALARM_DISTANCE":"ALARM_TEMP";
   auto tail=line.text.substr(line.text.find(token)+std::string(token).size());
   if(tail==" 0"||tail==": 0"||tail=="=0"||tail==" false")return fail("serial alarm cause is false: "+line.text);
   if(!reports[k].empty()&&line.at-reports[k].back()<800 && !cause_changed(reports[k].back()>200?reports[k].back()-200:0,line.at))
    return fail("alarm reports faster than public 1000ms period");
   reports[k].push_back(line.at);
  }
 }
 const uint8_t digits[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
 for(const auto& frame:oracle::displays)if(frame.clk==4&&frame.dio==5&&frame.at<2000&&frame.on)
  for(auto segment:frame.segments)if(segment)return fail("initial invalid temperature must display blank");
 MixedState previous;
 for(const auto& change:changes){
  if((change.hot()!=previous.hot()||change.near()!=previous.near())&&change.active()&&change.at+200<=18000){
   for(int k=0;k<2;++k)if(k?change.near():change.hot()){
    bool reported=false;for(auto at:reports[k])if(at>=change.at&&at<=change.at+200)reported=true;
    if(!reported)return fail("missing serial reason on alarm/cause change at "+std::to_string(change.at));
   }
  }
  previous=change;
 }
 bool prior_active=false;uint64_t active_since=0,first_sound=UINT64_MAX;
 for(uint64_t t=20;t<=18000;t+=20){
  auto s=mixed_at(t);bool active=s.active();
  if(active&&!prior_active){active_since=t;first_sound=UINT64_MAX;}
  prior_active=active;
  uint64_t changed=0;for(const auto& c:changes)if(c.at<=t)changed=c.at;
  const int frequency=frequency_at(6,t);
  if(frequency!=0&&abs(frequency-1000)>5)return fail("mixed alarm carrier must be 1000Hz");
  if(active&&frequency&&first_sound==UINT64_MAX)first_sound=t;
  if(active&&t>active_since+200&&first_sound==UINT64_MAX)return fail("missing alarm during active cause at "+std::to_string(t));
  if(t>=changed+200){
   if(!active&&frequency)return fail("alarm did not stop after causes cleared at "+std::to_string(t));
   if(active&&first_sound!=UINT64_MAX){
    auto phase=(t-first_sound)%1000;
    if(phase>=100&&phase<400&&!frequency)return fail("alarm missing ON interval at "+std::to_string(t));
    if(phase>=600&&phase<900&&frequency)return fail("alarm missing OFF interval at "+std::to_string(t));
   }
   for(int k=0;k<2;++k)if(k?s.near():s.hot()){
    uint64_t latest=UINT64_MAX;for(auto when:reports[k])if(when<=t)latest=when;
    if(latest==UINT64_MAX||t-latest>1200)return fail("missing current serial alarm reason at "+std::to_string(t));
   }
  }
  // Frame transport and a control step can straddle page/sensor transitions.
  // All arms receive the same 150ms page and 200ms sample-response allowances.
  if(t<200||t<changed+200||t%2000<150||t%2000>1850)continue;
  bool distance_page=(t/2000)%2;
  bool valid=distance_page?s.dv:s.tv;double expected=distance_page?s.distance:s.temperature;
  const oracle::DisplayEvent* frame=nullptr;
  for(const auto& f:oracle::displays)if(f.clk==4&&f.dio==5&&f.at<=t)frame=&f;
  if(!frame){
   if(oracle::tm_bus_activity){std::cerr<<"manual-tm1637-protocol-unverified";return 3;}
   return fail("missing mixed display");
  }
  bool seen=false;int value=0;
  if(frame->on)for(auto segment:frame->segments){
   if(!seen&&segment==0)continue;
   int n=-1;for(int i=0;i<10;++i)if(segment==digits[i])n=i;
   if(n<0)return fail("mixed display nondecimal segments at "+std::to_string(t));
   value=value*10+n;seen=true;
  }
  if((!valid&&seen)||(valid&&(!seen||abs(value-expected)>1.01)))
   return fail("mixed display page/value at "+std::to_string(t)+" expected "+(valid?std::to_string(expected):"blank")+", got "+(seen?std::to_string(value):"blank"));
 }
 std::cout<<"passed\n";return 0;
}
