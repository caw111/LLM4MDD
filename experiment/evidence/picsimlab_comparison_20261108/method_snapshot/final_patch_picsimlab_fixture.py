"""Apply the auditable batch-fixture additions to an isolated PICSimLab build."""
from pathlib import Path
import re
import shutil
import subprocess

ROOT=Path(__file__).resolve().parents[2]
BASE=Path.home()/'.cache/llm4mdd-picsimlab-build'
SRC=BASE/'picsimlab-0.9.3/src'


def main():
    shutil.copyfile(ROOT/'experiment/code/picsimlab_native_runtime.h',SRC/'sim_backend/mddsim_runtime.h')
    p=SRC/'sim_backend/bsim_simavr.h';s=p.read_text()
    if 'mddsim_pin_hook' not in s:
        s=s.replace('class bsim_simavr :','void mddsim_pin_hook(picpin*,bool);\n\nclass bsim_simavr :')
        s=s.replace('p->value = value;','p->value = value;\n        mddsim_pin_hook(p,false);')
        s=s.replace('p->dir = !(value & (1 << p->pord));','p->dir = !(value & (1 << p->pord));\n        mddsim_pin_hook(p,true);')
        p.write_text(s)
    p=SRC/'sim_backend/bsim_simavr.cc';s=p.read_text()
    if '#include "mddsim_runtime.h"' not in s:
        s=s.replace('#include "bsim_simavr.h"','#include "bsim_simavr.h"\n#include "mddsim_runtime.h"')
        s=s.replace('    MReset(0);\n\n    return ret;','    MReset(0);\n    mddsim_init(this,avr,pins,MGetPinCount());\n\n    return ret;',1)
        s=s.replace('void bsim_simavr::MStep(void) {\n    avr_run(avr);','void bsim_simavr::MStep(void) {\n    mddsim_step();\n    avr_run(avr);')
        p.write_text(s)
    p.touch()
    board=SRC/'boards/board_Arduino_Uno.cc';s=board.read_text()
    if 'mddsim_step();' not in s:
        s='void mddsim_step();\n'+s
        s=s.replace('                    avr_run(avr);','                    mddsim_step();\n                    avr_run(avr);')
        board.write_text(s)
    for name in ('input_dht11.cc','input_dht22.cc'):
        path=SRC/'parts'/name;s=path.read_text()
        if 'mddsim_dht_state' not in s:
            s='extern int mddsim_dht_state(int,double&,double&);\n'+s
            s=re.sub(r'sen_dhtxx_setTempHum\(&([^,]+), ([^;]+)\);',
                     r'double native_temperature=25,native_humidity=50;\n        int fixture_state=mddsim_dht_state(output_pins[0],native_temperature,native_humidity);\n        if(fixture_state<0)return;\n        if(fixture_state)sen_dhtxx_setTempHum(&\1,native_temperature,native_humidity);\n        else sen_dhtxx_setTempHum(&\1, \2);',s)
            marker='const picpin* ppins = SpareParts.GetPinsValues();'
            s=s.replace(marker,'double native_t,native_h;\n        if(mddsim_dht_state(output_pins[0],native_t,native_h)<0){SpareParts.SetPin(output_pins[0],1);return;}\n        '+marker)
            path.write_text(s)
    path=SRC/'parts/input_hcsr04.cc';s=path.read_text()
    if 'mddsim_distance_state' not in s:
        s='extern int mddsim_distance_state(int,double&);\n'+s
        s=s.replace('count = (200 - value) * 116.144018583e-6 * pboard->MGetInstClockFreq();',
                    'double distance_cm=2.0*(200-value);\n                int fixture_state=mddsim_distance_state(pins[1],distance_cm);\n                count=fixture_state<0?0:distance_cm*58.0720092915e-6*pboard->MGetInstClockFreq();')
        path.write_text(s)
    path=SRC/'devices/rtc_ds1307.cc';s=path.read_text()
    if 'PICSIMLAB_RTC_EPOCH' not in s:
        s=s.replace('    utime = time(NULL);','    utime = getenv("PICSIMLAB_RTC_EPOCH")?strtoll(getenv("PICSIMLAB_RTC_EPOCH"),nullptr,10):time(NULL);')
        s=s.replace('if (rtc->ucont == 10) {','if (rtc->ucont == 10 && !getenv("PICSIMLAB_FIXTURE")) {')
        path.write_text(s)
    path=SRC/'parts/input_MPU6050.cc';s=path.read_text()
    if 'mddsim_imu_state' not in s:
        s='extern int mddsim_imu_state(double*,int&,int&);\n'+s
        target='    mpu6050_set_addr(&mpu, addr);'
        replacement=target+'''\n    double physical[6];int raw_y,has_raw_y;
    if(mddsim_imu_state(physical,raw_y,has_raw_y)){
        mpu6050_set_accel(&mpu,physical[0]/9.80665,physical[1]/9.80665,physical[2]/9.80665);
        mpu6050_set_gyro(&mpu,physical[3]*57.2957795131,physical[4]*57.2957795131,physical[5]*57.2957795131);
        if(has_raw_y){mpu.regs[GYRO_YOUT_H]=(raw_y>>8)&255;mpu.regs[GYRO_YOUT_L]=raw_y&255;}
    }
'''
        assert target in s;s=s.replace(target,replacement);path.write_text(s)
    path=SRC/'parts/output_LCD_ili9341.cc';s=path.read_text()
    if 'mddsim_lcd_register' not in s:
        s='extern void mddsim_lcd_register(void*,unsigned char*);\n'+s
        target='void cpart_LCD_ili9341::PreProcess(void) {'
        assert target in s;s=s.replace(target,target+'\n    mddsim_lcd_register(&lcd,input_pins);');path.write_text(s)
    lx=BASE/'lxrad_NOGUI-main/lib/lxutils.cc';s=lx.read_text()
    marker='PICSIMLAB_USER_DATA'
    if marker not in s:
        match=re.search(r'lxString\s+lxGetUserDataDir\([^)]*\)\s*\{',s)
        assert match
        s=s[:match.end()]+'\n const char* explicit_dir=getenv("PICSIMLAB_USER_DATA");\n if(explicit_dir)return lxString(explicit_dir);\n'+s[match.end():]
        lx.write_text(s)
    s=lx.read_text()
    old='return lxString ("/home/") + getlogin () + "/";'
    if old in s:
        s=s.replace(old,'return lxString(getenv("HOME")?getenv("HOME"):"/home/chang")+"/";')
        lx.write_text(s)
    remote=SRC/'lib/rcontrol.cc';s=remote.read_text()
    old='serv.sin_port = htons(tcpport);'
    if old in s:
        s=s.replace(old,'serv.sin_port = htons(getenv("PICSIMLAB_FIXTURE")?0:tcpport);')
        remote.write_text(s)
    for name in ('avr_spi.c','avr_twi.c'):
        path=BASE/'simavr-master/simavr/sim'/name;s=path.read_text()
        target='p->bit_bang.clk_cycles = clock_divider<<2;'
        if target in s:
            s='#include <stdlib.h>\n'+s
            s=s.replace(target,'p->bit_bang.clk_cycles = getenv("PICSIMLAB_FIXTURE")?clock_divider:(clock_divider<<2);')
            path.write_text(s)
    path=BASE/'simavr-master/simavr/sim/avr_twi.c';s=path.read_text()
    if 'mdd_twi_update_clock' not in s:
        helper='''
static void mdd_twi_update_clock(avr_t* avr,avr_twi_t* p){
    unsigned prescaler=1u<<(2*avr_regbit_get(avr,p->twps));
    p->bit_bang.clk_cycles=16+2*avr->data[p->r_twbr]*prescaler;
    p->bit_bang_ack.clk_cycles=p->bit_bang.clk_cycles;
}
static void mdd_twi_write_bitrate(avr_t* avr,avr_io_addr_t addr,uint8_t value,void* param){
    avr_core_watch_write(avr,addr,value);
    mdd_twi_update_clock(avr,(avr_twi_t*)param);
}
'''
        match=re.search(r'static void\s+avr_twi_write_status',s);assert match
        s=s[:match.start()]+helper+s[match.start():]
        begin=s.index('{',s.index('avr_twi_write_status('));depth=1;end=begin+1
        while depth:
            if s[end]=='{':depth+=1
            elif s[end]=='}':depth-=1
            end+=1
        s=s[:end-1]+'\n if(getenv("PICSIMLAB_FIXTURE"))mdd_twi_update_clock(avr,p);\n'+s[end-1:]
        marker=' avr_register_io_write (avr, p->twsr.reg, avr_twi_write_status, p);'
        assert marker in s
        s=s.replace(marker,marker+'\n if(getenv("PICSIMLAB_FIXTURE"))avr_register_io_write(avr,p->r_twbr,mdd_twi_write_bitrate,p);')
        path.write_text(s)
    # Mega core left Arduino's standard phase-correct PWM modes as TODOs.
    path=BASE/'simavr-master/simavr/cores/sim_mega2560.c';s=path.read_text()
    if 'TODO: 1 PWM phase correct 8bit' in s:
        s=re.sub(r'// TODO: 1 PWM phase correct 8bit\s*//\s*2 PWM phase correct 9bit\s*//\s*3 PWM phase correct 10bit',
                 '[1] = AVR_TIMER_WGM_FCPWM8(),\n             [2] = AVR_TIMER_WGM_FCPWM9(),\n             [3] = AVR_TIMER_WGM_FCPWM10(),',s)
        s=s.replace('// TODO 1 pwm phase correct','[1] = AVR_TIMER_WGM_FCPWM8(),')
        path.write_text(s)
    # Repeated writes of the same OCR value must not re-trigger PWM output.
    path=BASE/'simavr-master/simavr/sim/avr_timer.c';s=path.read_text()
    if '#include <stdlib.h>' not in s:s='#include <stdlib.h>\n'+s
    old='case avr_timer_wgm_fc_pwm:\t// OCR is not used here\n\t\t\tavr_timer_reconfigure(timer, 0);'
    if old in s:
        s=s.replace(old,'case avr_timer_wgm_fc_pwm:\n\t\t\tif(oldv != _timer_get_comp_ocr(avr, comp)) avr_timer_reconfigure(timer, 0);')
        path.write_text(s)
    if 'mdd_phase_correct' not in s:
        # Datasheet: dual-slope counting, period=2*TOP*N; OC clears upward and
        # sets downward. The stock internally clocked FCPWM used fast-PWM timing.
        marker='\tuint8_t mode = avr_regbit_get(avr, p->comp[comp].com);'
        where=s.index(marker)
        s=s[:where]+s[where:].replace(marker,marker+'''
    int mdd_phase_correct=getenv("PICSIMLAB_FIXTURE") && p->wgm_op_mode_kind==avr_timer_wgm_fc_pwm && p->tov_cycles;
    uint64_t mdd_offset=when-p->tov_base;
    if(mdd_phase_correct && mdd_offset>p->tov_cycles/2){
        if(mode==avr_timer_com_clear)mode=avr_timer_com_set;
        else if(mode==avr_timer_com_set)mode=avr_timer_com_clear;
    }
''',1)
        marker='\treturn p->tov_cycles ? 0 :'
        assert marker in s
        s=s.replace(marker,'''
    if(mdd_phase_correct){
        uint64_t up=p->comp[comp].comp_cycles;
        if(mdd_offset<=p->tov_cycles/2 && up && up<p->tov_cycles/2)
            return p->tov_base+p->tov_cycles-up;
        return 0;
    }
'''+marker,1)
        marker='\t\tif (p->comp[compi].comp_cycles) {'
        start=s.index('static avr_cycle_count_t\navr_timer_tov(')
        position=s.index(marker,start)
        s=s[:position]+'''
        if(getenv("PICSIMLAB_FIXTURE") && p->wgm_op_mode_kind==avr_timer_wgm_fc_pwm){
            uint64_t elapsed=avr->cycle-when,up=p->comp[compi].comp_cycles,down=p->tov_cycles-up;
            unsigned com=avr_regbit_get(avr,p->comp[compi].com);
            if(com==avr_timer_com_clear||com==avr_timer_com_set){
                unsigned high=(elapsed<up || elapsed>=down);
                avr_raise_irq(&p->io.irq[TIMER_IRQ_OUT_COMP+compi],com==avr_timer_com_clear?high:!high);
            }
            if(up && up<p->tov_cycles/2){
                if(elapsed<up)avr_cycle_timer_register(avr,up-elapsed,dispatch[compi],p);
                else if(elapsed<down)avr_cycle_timer_register(avr,down-elapsed,dispatch[compi],p);
            }
            continue;
        }
'''+s[position:]
        marker='\t\t\treturn (when * (((uint32_t)p->tov_top)+1)) / p->tov_cycles;'
        assert marker in s
        s=s.replace(marker,'''
            if(getenv("PICSIMLAB_FIXTURE") && p->wgm_op_mode_kind==avr_timer_wgm_fc_pwm && p->cs_div_value && p->tov_top){
                uint64_t ticks=(when/p->cs_div_value)%(2*p->tov_top);
                return ticks<=p->tov_top?ticks:2*p->tov_top-ticks;
            }
'''+marker,1)
        marker='\t\tp->tov_cycles = prescaler * (top+1);'
        assert marker in s
        s=s.replace(marker,marker+'''
        if(getenv("PICSIMLAB_FIXTURE") && p->wgm_op_mode_kind==avr_timer_wgm_fc_pwm)p->tov_cycles=prescaler*2*top;
''',1)
        marker='\t\tp->comp[compi].comp_cycles = 0;'
        assert marker in s
        s=s.replace(marker,'''
        if(getenv("PICSIMLAB_FIXTURE") && p->wgm_op_mode_kind==avr_timer_wgm_fc_pwm && !use_ext_clock)comp_cycles=prescaler*ocr;
'''+marker,1)
        path.write_text(s)
    path.write_text(s)
    path=BASE/'simavr-master/simavr/sim/avr_ioport.c';s=path.read_text()
    if 'mdd_timer_override_mask' not in s:
        s='#include <stdlib.h>\n#include <string.h>\n#include "avr_timer.h"\n'+s
        marker='static uint8_t\navr_ioport_read('
        helper='''
static uint8_t mdd_timer_override_mask(avr_ioport_t* port){
    if(!getenv("PICSIMLAB_FIXTURE"))return 0;
    uint8_t mask=0;avr_t* avr=port->io.avr;
    for(avr_io_t* io=avr->io_port;io;io=io->next){
        if(!io->kind||strcmp(io->kind,"timer"))continue;
        avr_timer_t* timer=(avr_timer_t*)io;
        for(int i=0;i<AVR_TIMER_COMP_COUNT;i++){
            avr_timer_comp_t* comp=&timer->comp[i];
            if(comp->com_pin.reg==port->r_port && avr_regbit_get(avr,comp->com))mask|=1<<comp->com_pin.bit;
        }
    }
    return mask;
}
'''
        assert marker in s;s=s.replace(marker,helper+'\n'+marker,1)
        s=s.replace('uint8_t v = (avr->data[p->r_pin] & ~ddr) | (avr->data[p->r_port] & ddr);','uint8_t overridden=mdd_timer_override_mask(p);\n\tuint8_t v = (avr->data[p->r_pin] & (~ddr|overridden)) | (avr->data[p->r_port] & ddr & ~overridden);',1)
        s=s.replace('for (int i = 0; i < 8; i++) {','uint8_t driven_by_timer=mdd_timer_override_mask(p);\n\tfor (int i = 0; i < 8; i++) {',1)
        s=s.replace('if (ddr & (1 << i))','if ((ddr & (1 << i)) && !(driven_by_timer & (1 << i)))',1)
        s=s.replace('uint8_t pin = (avr->data[p->r_pin] & ~ddr) | (avr->data[p->r_port] & ddr);','uint8_t overridden=mdd_timer_override_mask(p);\n\tuint8_t pin = (avr->data[p->r_pin] & (~ddr|overridden)) | (avr->data[p->r_port] & ddr & ~overridden);',1)
        path.write_text(s)
    subprocess.run(['make','static','-j8'],cwd=BASE/'lxrad_NOGUI-main',check=True)
    subprocess.run(['python3',str(ROOT/'experiment/code/build_picsimlab_avr.py')],check=True)


if __name__=='__main__':main()
