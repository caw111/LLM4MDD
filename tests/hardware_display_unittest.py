import copy
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest

sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
from run_formal_experiment import lower, CLI
from run_controlled_experiment import compile_code
from run_contract_development import behavior
from app.services.arduino_adapter import generate_adapter
from app.services.hardware_adapter import validate_hardware_contract, generate_hardware_adapter
from app.services.interface_planner import check_explicit_requirements
from app.services.sysml_compiler import compile_sysml
from app.services.behavior_contract import check_contract


def inventory():
    return json.loads(Path('tools/examples/adc_display.contract.json').read_text(encoding='utf-8'))


class DisplayTests(unittest.TestCase):
    def test_two_pin_roles_and_conflicts_are_checked(self):
        c=inventory();validate_hardware_contract(c)
        check_explicit_requirements(c,'A0 -> TM1637 CLK=D4, DIO=D5',{})
        swapped=copy.deepcopy(c);swapped['interfaces'][1]['binding'].update(clk_pin=5,dio_pin=4)
        with self.assertRaisesRegex(ValueError,'CLK wiring'):check_explicit_requirements(swapped,'TM1637 CLK=D4, DIO=D5',{})
        for field,value in [('dio_pin',4),('clk_pin','A0')]:
            wrong=copy.deepcopy(c);wrong['interfaces'][1]['binding'][field]=value
            with self.assertRaisesRegex(ValueError,'Conflicting'):validate_hardware_contract(wrong)
        for field,value in [('brightness',8),('brightness',True),('leading_zero','false'),('invalid_policy','clamp')]:
            wrong=copy.deepcopy(c);wrong['interfaces'][1]['binding'][field]=value
            with self.assertRaises(ValueError):validate_hardware_contract(wrong)
        wrong=copy.deepcopy(c);wrong['interfaces'][1]['type']='f64'
        with self.assertRaisesRegex(ValueError,'requires i32'):validate_hardware_contract(wrong)

    def test_actual_pipeline_compiles_on_both_boards_and_displays_sampled_mapping(self):
        c=inventory();ir=compile_sysml(Path('tools/sysml/adc_display.sysml').read_text())
        self.assertEqual(check_contract(ir,c)['issues'],[])
        with tempfile.TemporaryDirectory() as folder:
            base=Path(folder);header,source=lower(ir,base/'lowered')
            def no_llm(_):self.fail('Deterministic hardware adapter must not call LLM')
            for board in ('arduino:avr:uno','arduino:avr:mega'):
                c['board']=board
                _,sketch=generate_adapter(no_llm,header,source,'',{},contract=c)
                target=base/board.split(':')[-1]
                result,log=compile_code(target,sketch,CLI,board)
                self.assertTrue(result['compile_ok'],log)
                score=behavior(target,19,board)
                self.assertEqual(score['functional_status'],'passed', (target/'host_oracle/trace.log').read_text() if (target/'host_oracle/trace.log').exists() else score)

    def test_driver_formats_signed_range_blanks_overflow_and_avoids_repeated_bus_writes(self):
        c=inventory();adapter=generate_hardware_adapter('void ProgramPage1(int adc, int* displayValue);',c)
        with tempfile.TemporaryDirectory() as folder:
            base=Path(folder)
            for filename,target in [('gpio_arduino.h','Arduino.h'),('TM1637Display.h','TM1637Display.h')]:
                shutil.copyfile(Path('experiment/development/oracles')/filename,base/target)
            kernel='''#include <Arduino.h>
int ProgramPage1_instance=0,command=0;
void ProgramPage1_Init(int*){}
void ProgramPage1(int,int* output){*output=command;}
'''
            harness='''
#include <cassert>
int main(){setup();assert(oracle::displays.back().segments[3]==0x3f);
 command=-999;oracle::now+=10;loop();auto e=oracle::displays.back();
 assert(e.clk==4&&e.dio==5&&e.on);assert((e.segments==std::array<uint8_t,4>{0x40,0x6f,0x6f,0x6f}));
 auto count=oracle::displays.size();oracle::now+=10;loop();assert(oracle::displays.size()==count);
 command=9999;oracle::now+=10;loop();assert((oracle::displays.back().segments==std::array<uint8_t,4>{0x6f,0x6f,0x6f,0x6f}));
 for(int v:{10000,-1000}){command=v;oracle::now+=10;loop();assert((oracle::displays.back().segments==std::array<uint8_t,4>{}));}
 command=42;oracle::now+=10;loop();assert((oracle::displays.back().segments==std::array<uint8_t,4>{0,0,0x66,0x5b}));}
'''
            src=base/'test.cpp';src.write_text(kernel+adapter+harness,encoding='utf-8');exe=base/'test.exe'
            build=subprocess.run([shutil.which('g++'),'-std=c++17','-I',str(base),str(src),'-o',str(exe)],capture_output=True,text=True)
            self.assertEqual(build.returncode,0,build.stderr)
            run=subprocess.run([str(exe)],capture_output=True,text=True)
            self.assertEqual(run.returncode,0,run.stderr)


if __name__=='__main__':unittest.main()
