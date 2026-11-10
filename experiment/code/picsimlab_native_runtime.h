// PICSimLab batch fixture: electrical input scheduling and passive AVR observations.
// This file never changes candidate flash, instructions, registers or application state.
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <unistd.h>
#include <zlib.h>
#include "../lib/spareparts.h"
#include "../devices/lcd_ili9341.h"

namespace mddsim {
struct Input { uint64_t at; std::string kind; int pin; double value; };
struct TM1637 {
    int clk=0,dio=0,old_clk=1,old_dio=1,bits=0,byte=0;
    bool active=false,ack=false,enabled=false,fixed=false;
    unsigned char ram[6]={};
    std::vector<int> bytes;
};
struct Imu {uint64_t at=0;double value[6]={0,0,9.81,0,0,0};int raw_y=0,has_raw_y=0;};
struct AnalogMux {int signal=0,select[4]={};double volts[16]={};};
struct Player {int led=0,button=0,wrong=0,active=0,old=0,phase=0;uint64_t arm=0,delay=0,hold=0,start=0,next=0;};
struct Runtime {
    bsim_simavr* board=nullptr;
    avr_t* avr=nullptr;
    picpin* pins=nullptr;
    int count=0;
    FILE* trace=nullptr;
    uint64_t horizon=0,next=0,loop_pc=~0u;
    bool setup=false,applying=false;
    int physical[100]={},logical[100]={},last_value[100]={},last_dir[100]={};
    bool monitor[100]={};
    std::vector<Input> inputs;
    std::vector<TM1637> displays;
    std::vector<AnalogMux> muxes;
    std::vector<Player> players;
    bool bus_busy=false,bus_initialized=false;
    double temperature[100]={},humidity[100]={},distance[100]={};
    bool temperature_invalid[100]={},humidity_invalid[100]={},distance_invalid[100]={};
    std::vector<Imu> imu_samples;
    size_t imu_index=0;
    Imu imu;
    lcd_ili9341_t* lcd=nullptr;
    unsigned char* lcd_pins=nullptr;
    gzFile frames=nullptr;
    uint64_t next_frame=0;
    uint64_t frame_period=0;
    uLong previous_frame_crc=0;
    int previous_mac=-1,previous_on=-1;
    size_t input_index=0;
} rt;

inline uint64_t cycle(){return rt.avr?rt.avr->cycle:0;}

inline void player_step(){
    for(auto& player:rt.players){
        int value=rt.pins[player.led-1].value;
        if(!player.phase && cycle()>=player.arm && value && !player.old){
            player.start=cycle();player.phase=player.wrong?1:3;
            player.next=cycle()+(player.wrong?rt.avr->frequency*3/10:player.delay);
        }
        player.old=value;
        if(!player.phase||cycle()<player.next)continue;
        int button=player.phase<=2?player.wrong:player.button;
        bool press=player.phase==1||player.phase==3;
        rt.board->MSetPin(button,press?player.active:!player.active);
        fprintf(rt.trace,"I %llu %s %d %d\n",(unsigned long long)cycle(),press?"adaptive_press":"adaptive_release",button,press?player.active:!player.active);
        if(player.phase==1){player.phase=2;player.next=cycle()+player.hold;}
        else if(player.phase==2){player.phase=3;player.next=player.start+player.delay;}
        else if(player.phase==3){player.phase=4;player.next=cycle()+player.hold;}
        else player.phase=0;
    }
}

inline void mux_update(){
    if(rt.applying)return;
    rt.applying=true;
    for(auto& mux:rt.muxes){
        int channel=0;
        for(int i=0;i<4;i++)channel|=(rt.pins[mux.select[i]-1].value?1:0)<<i;
        rt.board->MSetAPin(mux.signal,float(mux.volts[channel]));
    }
    rt.applying=false;
}

inline void lcd_frame(){
    if(!rt.lcd||!rt.lcd_pins)return;
    if(!rt.frames){const char* name=getenv("PICSIMLAB_LCD_FRAMES");if(!name)return;rt.frames=gzopen(name,"wb1");}
    std::vector<uint32_t> pixels(240*320);
    for(int y=0;y<320;y++)for(int x=0;x<240;x++)pixels[y*240+x]=rt.lcd->ram[x][y]&0xffffff;
    uLong crc=crc32(0,(const Bytef*)pixels.data(),pixels.size()*sizeof(uint32_t));
    if(crc==rt.previous_frame_crc&&rt.previous_mac==rt.lcd->mac&&rt.previous_on==rt.lcd->on)return;
    gzprintf(rt.frames,"L %llu %u %u %d %d %d\n",(unsigned long long)cycle(),unsigned(rt.lcd->mac),unsigned(rt.lcd->on),
             rt.logical[rt.lcd_pins[4]-1],rt.logical[rt.lcd_pins[3]-1],rt.logical[rt.lcd_pins[2]-1]);
    gzwrite(rt.frames,pixels.data(),pixels.size()*sizeof(uint32_t));
    rt.previous_frame_crc=crc;rt.previous_mac=rt.lcd->mac;rt.previous_on=rt.lcd->on;
}

inline void tm_frame(TM1637& tm){
    fprintf(rt.trace,"F %llu %d %d %d",(unsigned long long)cycle(),rt.logical[tm.clk-1],rt.logical[tm.dio-1],int(tm.enabled));
    for(int b:tm.ram)fprintf(rt.trace," %d",b);
    fprintf(rt.trace,"\n");
}

inline void tm_stop(TM1637& tm){
    if(tm.bytes.empty())return;
    int cmd=tm.bytes[0];
    if((cmd&0xf0)==0x40)tm.fixed=cmd&4;
    else if((cmd&0xf0)==0x80)tm.enabled=cmd&8;
    else if((cmd&0xc0)==0xc0){
        int addr=cmd&7;
        for(size_t i=1;i<tm.bytes.size();i++){
            if(addr<6)tm.ram[addr]=tm.bytes[i];
            if(!tm.fixed)addr++;
        }
    }
    tm_frame(tm);
}

inline void tm_update(){
    if(rt.bus_busy)return;
    rt.bus_busy=true;
    for(auto& tm:rt.displays){
        auto& cp=rt.pins[tm.clk-1];auto& dp=rt.pins[tm.dio-1];
        if(cp.dir)rt.board->MSetPin(tm.clk,1);
        if(dp.dir)rt.board->MSetPin(tm.dio,tm.ack?0:1);
        int c=cp.value,d=dp.value;
        if(c&&tm.old_clk&&d!=tm.old_dio){
            if(!d){tm.active=true;tm.bits=0;tm.byte=0;tm.bytes.clear();tm.ack=false;}
            else if(tm.active){tm_stop(tm);tm.active=false;tm.ack=false;}
        }
        if(tm.active&&c&&!tm.old_clk){
            if(tm.bits<8){tm.byte|=(d&1)<<tm.bits;tm.bits++;}
            else if(tm.bits==8){tm.bytes.push_back(tm.byte);tm.bits=9;}
        }
        if(tm.active&&!c&&tm.old_clk){
            if(tm.bits==8){tm.ack=true;if(dp.dir)rt.board->MSetPin(tm.dio,0);}
            else if(tm.bits==9){tm.ack=false;tm.bits=0;tm.byte=0;if(dp.dir)rt.board->MSetPin(tm.dio,1);}
            d=dp.value;
        }
        tm.old_clk=c;tm.old_dio=d;
    }
    rt.bus_busy=false;
}

inline unsigned baud(unsigned uart){
    unsigned base=uart==0?0xc0:uart==1?0xc8:uart==2?0xd0:0x130;
    unsigned div=(rt.avr->data[base+5]<<8)|rt.avr->data[base+4];
    unsigned b=rt.avr->frequency/((rt.avr->data[base]&2?8:16)*(div+1));
    const unsigned common[]={300,1200,2400,4800,9600,19200,38400,57600,115200,230400};
    for(unsigned target:common)if(std::abs(int(b)-int(target))<target*.03)return target;
    return b;
}

inline void uart_out(struct avr_irq_t*,uint32_t value,void* param){
    if(rt.trace)fprintf(rt.trace,"S %llu %u %u %u\n",(unsigned long long)cycle(),
                         unsigned(uintptr_t(param)),baud(unsigned(uintptr_t(param))),unsigned(value&255));
}

inline void apply(const Input& in){
    if(in.kind=="digital")rt.board->MSetPin(in.pin,unsigned(in.value));
    else if(in.kind=="analog"){
        // Stimulus is millivolts at the MCU ADC pin, not a mocked analogRead result.
        rt.board->MSetAPin(in.pin,float(in.value));
        for(auto& mux:rt.muxes)if(mux.signal==in.pin)for(double& v:mux.volts)v=in.value;
    } else if(in.kind=="mux"){
        int device=in.pin/16,channel=in.pin%16;
        if(device>=0&&device<int(rt.muxes.size()))rt.muxes[device].volts[channel]=in.value;
        mux_update();
    } else if(in.kind=="uart"){
        auto irq=avr_io_getirq(rt.avr,AVR_IOCTL_UART_GETIRQ('0'+in.pin),UART_IRQ_INPUT);
        if(irq)avr_raise_irq(irq,unsigned(in.value));
    } else if(in.kind=="temperature"){rt.temperature[in.pin]=in.value;rt.temperature_invalid[in.pin]=false;}
    else if(in.kind=="humidity"){rt.humidity[in.pin]=in.value;rt.humidity_invalid[in.pin]=false;}
    else if(in.kind=="distance"){rt.distance[in.pin]=in.value;rt.distance_invalid[in.pin]=false;}
    else if(in.kind=="temperature_invalid")rt.temperature_invalid[in.pin]=true;
    else if(in.kind=="humidity_invalid")rt.humidity_invalid[in.pin]=true;
    else if(in.kind=="distance_invalid")rt.distance_invalid[in.pin]=true;
    if(rt.trace)fprintf(rt.trace,"I %llu %s %d %.9g\n",(unsigned long long)cycle(),in.kind.c_str(),in.pin,in.value);
}

inline void finish(){
    lcd_frame();if(rt.frames){gzclose(rt.frames);rt.frames=nullptr;}
    for(int p=0;p<rt.count;p++)if(rt.monitor[p]){
        unsigned port_value=0,native_ddr=0;
        if(rt.pins[p].port){
            avr_ioport_state_t state={};
            avr_ioctl(rt.avr,AVR_IOCTL_IOPORT_GETSTATE(*rt.pins[p].port),&state);
            port_value=state.port;
            native_ddr=state.ddr;
        }
        int mode=(native_ddr&(1<<rt.pins[p].pord))?1:((port_value&(1<<rt.pins[p].pord))?2:0);
        fprintf(rt.trace,"M %llu %d %d\n",(unsigned long long)cycle(),rt.logical[p],mode);
    }
    fprintf(rt.trace,"E %llu %d\n",(unsigned long long)cycle(),int(rt.setup));
    fflush(rt.trace);fclose(rt.trace);rt.trace=nullptr;
    _exit(0);
}
}

inline void mddsim_pin_hook(picpin* p,bool direction){
    auto& r=mddsim::rt;
    if(!r.trace||!r.pins)return;
    int index=int(p-r.pins);
    if(index<0||index>=r.count)return;
    if(r.monitor[index]&&(r.last_value[index]!=p->value||r.last_dir[index]!=p->dir)){
        fprintf(r.trace,"G %llu %d %u %u\n",(unsigned long long)mddsim::cycle(),r.logical[index],unsigned(p->value),unsigned(p->dir));
        r.last_value[index]=p->value;r.last_dir[index]=p->dir;
    }
    for(auto& tm:r.displays)if(index==tm.clk-1||index==tm.dio-1){mddsim::tm_update();break;}
    for(auto& mux:r.muxes)for(int select:mux.select)if(index==select-1){mddsim::mux_update();break;}
    // Sample the native LCD on every bus edge, including edges inside one CPU
    // instruction, so the AVR SPI clock needs no artificial 4x slowdown.
    if(r.lcd&&r.lcd_pins){
        bool connected=true,changed=false;
        for(int i=0;i<5;i++){if(!r.lcd_pins[i])connected=false;if(index==r.lcd_pins[i]-1)changed=true;}
        if(connected&&changed){
            const unsigned char* values[5]={&r.pins[r.lcd_pins[0]-1].value,&r.pins[r.lcd_pins[1]-1].value,
                &r.pins[r.lcd_pins[4]-1].value,&r.pins[r.lcd_pins[3]-1].value,&r.pins[r.lcd_pins[2]-1].value};
            lcd_ili9341_SPI_io(r.lcd,values);
        }
    }
}

inline void mddsim_init(bsim_simavr* board,avr_t* avr,picpin* pins,int count){
    const char* config=getenv("PICSIMLAB_FIXTURE");
    if(!config)return;
    auto& r=mddsim::rt;
    r.board=board;r.avr=avr;r.pins=pins;r.count=count;
    const char* frame_ms=getenv("PICSIMLAB_LCD_FRAME_MS");
    r.frame_period=avr->frequency*(frame_ms?std::max(1,atoi(frame_ms)):100)/1000;
    for(int i=0;i<100;i++){r.temperature[i]=25;r.humidity[i]=50;r.distance[i]=100;}
    const char* path=getenv("PICSIMLAB_TRACE");
    if(!path){fprintf(stderr,"Missing PICSIMLAB_TRACE\n");_exit(70);}
    r.trace=fopen(path,"w");if(!r.trace)_exit(71);
    fprintf(r.trace,"V 1 %u\n",avr->frequency);
    std::ifstream file(config);std::string line;
    while(std::getline(file,line)){
        std::istringstream row(line);std::string type;row>>type;
        if(type=="H"){unsigned long long ms;row>>ms>>r.loop_pc;r.horizon=ms*avr->frequency/1000;}
        else if(type=="P"||type=="B"){
            int logical,physical;row>>logical>>physical;
            if(physical<1||physical>count)_exit(72);
            r.physical[logical]=physical;r.logical[physical-1]=logical;
            if(type=="P")r.monitor[physical-1]=true;
            r.last_value[physical-1]=-1;r.last_dir[physical-1]=-1;
        }else if(type=="I"){
            mddsim::Input in;unsigned long long us;row>>us>>in.kind>>in.pin>>in.value;
            in.at=us*avr->frequency/1000000;r.inputs.push_back(in);
        }else if(type=="T"){
            mddsim::TM1637 tm;row>>tm.clk>>tm.dio;r.displays.push_back(tm);
        }else if(type=="Q"){
            mddsim::AnalogMux mux;row>>mux.signal;
            for(int& select:mux.select)row>>select;
            r.muxes.push_back(mux);
        }else if(type=="C"){
            mddsim::Player player;unsigned long long arm,delay,hold;
            row>>player.led>>player.button>>player.wrong>>player.active>>arm>>delay>>hold;
            player.arm=arm*avr->frequency/1000000;player.delay=delay*avr->frequency/1000000;player.hold=hold*avr->frequency/1000000;r.players.push_back(player);
        }else if(type=="J"){
            mddsim::Imu sample;unsigned long long us;row>>us;
            for(double& value:sample.value)row>>value;
            row>>sample.raw_y>>sample.has_raw_y;sample.at=us*avr->frequency/1000000;r.imu_samples.push_back(sample);
        }
    }
    std::stable_sort(r.inputs.begin(),r.inputs.end(),[](const mddsim::Input&a,const mddsim::Input&b){return a.at<b.at;});
    std::stable_sort(r.imu_samples.begin(),r.imu_samples.end(),[](const mddsim::Imu&a,const mddsim::Imu&b){return a.at<b.at;});
    // Initial physical inputs must exist before native parts cache their first
    // measurement; otherwise a startup DHT read sees the fixture's default 25 C.
    for(const auto& input:r.inputs){
        if(input.at)break;
        if(input.kind.find("temperature")==0||input.kind.find("humidity")==0||input.kind.find("distance")==0)mddsim::apply(input);
    }
    for(const auto& sample:r.imu_samples){if(sample.at)break;r.imu=sample;}
    // Electrical GPIO/ADC inputs are still applied at the first CPU step,
    // after board/part initialization has finished setting initial pin states.
    for(int uart=0;uart<(count>28?4:1);uart++){
        auto irq=avr_io_getirq(avr,AVR_IOCTL_UART_GETIRQ('0'+uart),UART_IRQ_OUTPUT);
        if(irq)avr_irq_register_notify(irq,mddsim::uart_out,(void*)uintptr_t(uart));
    }
    for(int p=0;p<count;p++)mddsim_pin_hook(&pins[p],false);
    fflush(r.trace);
}

int mddsim_dht_state(int pin,double& temperature,double& humidity){
    auto& r=mddsim::rt;if(!r.trace)return 0;
    if(pin<1||pin>=100)return -1;
    temperature=r.temperature[pin];humidity=r.humidity[pin];
    return r.temperature_invalid[pin]||r.humidity_invalid[pin]?-1:1;
}

int mddsim_distance_state(int pin,double& distance){
    auto& r=mddsim::rt;if(!r.trace)return 0;
    if(pin<1||pin>=100)return -1;
    distance=r.distance[pin];return r.distance_invalid[pin]?-1:1;
}

int mddsim_imu_state(double* values,int& raw_y,int& has_raw_y){
    auto& r=mddsim::rt;if(!r.trace)return 0;
    for(int i=0;i<6;i++)values[i]=r.imu.value[i];raw_y=r.imu.raw_y;has_raw_y=r.imu.has_raw_y;return 1;
}

void mddsim_lcd_register(void* lcd,unsigned char* pins){
    auto& r=mddsim::rt;if(!r.trace)return;
    r.lcd=(lcd_ili9341_t*)lcd;r.lcd_pins=pins;
}

void mddsim_step(){
    auto& r=mddsim::rt;if(!r.trace)return;
    if(!r.bus_initialized){r.bus_initialized=true;mddsim::tm_update();mddsim::mux_update();for(auto& tm:r.displays)mddsim::tm_frame(tm);}
    if(!r.setup&&r.avr->pc==r.loop_pc){
        r.setup=true;fprintf(r.trace,"A %llu\n",(unsigned long long)mddsim::cycle());
    }
    while(r.input_index<r.inputs.size()&&r.inputs[r.input_index].at<=mddsim::cycle())
        mddsim::apply(r.inputs[r.input_index++]);
    while(r.imu_index<r.imu_samples.size()&&r.imu_samples[r.imu_index].at<=mddsim::cycle())r.imu=r.imu_samples[r.imu_index++];
    mddsim::player_step();
    if(mddsim::cycle()>=r.next_frame){mddsim::lcd_frame();r.next_frame=mddsim::cycle()+r.frame_period;}
    if(mddsim::cycle()>=r.horizon)mddsim::finish();
}
