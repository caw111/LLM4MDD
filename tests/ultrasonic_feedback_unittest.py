"""Independent API evidence and targeted repairs, without paid model calls."""
import copy
import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

from tests import sysml_assembly_unittest as assembly
from tests.ultrasonic_unittest import DIRECT, inventory, behavior
from app.services.interface_planner import plan_interface_contract, ContractPlanningError
from app.services.sysml import generate_validated_bundle, SysMLValidationError
from app.services.sysml_compiler import validate_executable_sysml


class UltrasonicFeedbackTests(unittest.TestCase):
    def test_rng_vectors_and_real_library_negative_padding(self):
        code='''#include <Arduino.h>
#include <TM1637Display.h>
#include <cassert>
int main(){
 const long expected[]={16807,282475249,1622650073,984943658,1144108930,470211272};
 for(auto n:expected)assert(random(2147483647L)==n);
 randomSeed(1);assert(random(10)==7);randomSeed(0);assert(random(10)==9);
 randomSeed(1);assert(random(0)==0);assert(random(4,4)==4);assert(random(7,4)==7);
 assert(random(60,161)==101); // 16807 % 101 + 60
 randomSeed(1);assert(random(-10,10)==-3);
 bool rejected=false;try{random(-2147483647L,2147483647L);}catch(const std::runtime_error&){rejected=true;}assert(rejected);
 TM1637Display display(4,5);display.setBrightness(7);
 display.showNumberDecEx(-1,0,true);
 assert((oracle::displays.back().segments==std::array<uint8_t,4>{0x3f,0x3f,0x40,0x06}));
 assert(oracle::now*1000+oracle::sub_ms==20800);
 display.showNumberDec(-12,true);
 assert((oracle::displays.back().segments==std::array<uint8_t,4>{0x3f,0x40,0x06,0x5b}));
 display.showNumberDec(-123,true);
 assert((oracle::displays.back().segments==std::array<uint8_t,4>{0x40,0x06,0x5b,0x4f}));
 rejected=false;try{display.showNumberDec(-1234,true);}catch(const std::runtime_error&){rejected=true;}assert(rejected);
}
'''
        with tempfile.TemporaryDirectory() as folder:
            base=Path(folder)
            for src,dest in [('gpio_arduino.h','Arduino.h'),('TM1637Display.h','TM1637Display.h')]:
                shutil.copyfile(Path('experiment/development/oracles')/src,base/dest)
            (base/'test.cpp').write_text(code,encoding='utf-8')
            built=subprocess.run([shutil.which('g++'),'-std=c++17','-I',str(base),str(base/'test.cpp'),'-o',str(base/'test.exe')],capture_output=True,text=True)
            self.assertEqual(built.returncode,0,built.stderr)
            ran=subprocess.run([str(base/'test.exe')],capture_output=True,text=True,timeout=10)
            self.assertEqual(ran.returncode,0,ran.stderr)

    def test_random_sampling_and_negative_blank_mutant(self):
        randomized=DIRECT.replace('bool started=false;', 'bool started=false;unsigned long interval=100;')
        randomized=randomized.replace('(now-last)<100','(now-last)<interval').replace('started=true;last=now;','started=true;last=now;interval=random(90,111);')
        cases=[(randomized.replace('void setup(){',f'void setup(){{randomSeed({seed});'),'passed') for seed in (1,42,12345)]
        cases += [(randomized.replace('random(90,111)','random(300,401)'),'failed'),
                  (randomized.replace('else display.clear();','else display.showNumberDecEx(-1,0,true);'),'failed')]
        with tempfile.TemporaryDirectory() as folder:
            for i,(code,expected) in enumerate(cases):
                with self.subTest(i=i):
                    attempt=Path(folder)/str(i);source=attempt/'build/sketch/sketch.ino.cpp'
                    source.parent.mkdir(parents=True);source.write_text(code,encoding='utf-8')
                    result=behavior(attempt,22)
                    self.assertEqual(result['functional_status'],expected,result)

    def test_planner_repeated_metadata_error_and_sampling_repair(self):
        good=inventory(False);bad=copy.deepcopy(good)
        bad['interfaces'][0]['hardware']=copy.deepcopy(bad['interfaces'][0]['binding'])
        slow=copy.deepcopy(good)
        for p in slow['interfaces'][:2]:p['sample_period_ms']=10
        for candidates,fragment in [([bad,bad,good],'distanceCm.hardware: expected a non-empty string, got dict'),
                                    ([slow,slow,good],'distanceCm.sample_period_ms: got 10')]:
            replies=iter(candidates);prompts=[]
            def call(prompt):
                prompts.append(prompt);return 'fixture',json.dumps(next(replies))
            result,report=plan_interface_contract(call,'Display HC-SR04 distance using TM1637',{},max_repairs=2)
            self.assertEqual([a['ok'] for a in report['attempts']],[False,False,True])
            self.assertIn(fragment,prompts[1]);self.assertIn('NO PROGRESS',prompts[2])
            self.assertEqual(result['interfaces'][0]['binding'],good['interfaces'][0]['binding'])
            self.assertEqual(result['requirements'],good['requirements'])
        with self.assertRaises(ContractPlanningError) as caught:
            plan_interface_contract(lambda _:('fixture',json.dumps(bad)),'Display HC-SR04 distance using TM1637',{},max_repairs=1)
        self.assertEqual(len(caught.exception.attempts),2)

    def test_nested_numeric_guard_repair_is_validated_and_executed(self):
        c=assembly.contract([('distanceCm','f64'),('distanceValid','bool')],[('value','i32')])
        good={'states':[],'updates':[],'outputs':{'value':{'if':'distanceValid','then':{'floor':'distanceCm'},'else':10000}}}
        bad=copy.deepcopy(good)
        bad['outputs']['value']['then']={'if':'distanceCm','then':{'floor':'distanceCm'},'else':0}
        replies=iter([bad,bad,good]);prompts=[]
        def call(prompt):
            prompts.append(prompt);return 'fixture',assembly.candidate(next(replies))
        result=generate_validated_bundle(call,'Display integer cm when valid, otherwise blank (10000).',
            contract=c,max_repairs=2,validator=validate_executable_sysml)
        self.assertIn('has type f64; expected bool',prompts[1])
        self.assertIn('changing branch values cannot fix this guard',prompts[2])
        self.assertIn('distanceCm',prompts[2])
        assembly.AssemblyTests.execute(self,good,c,'''int main(){int value;ProgramPage1_Init(&ProgramPage1_instance);
ProgramPage1(9.5,true,&value);assert(value==9);
ProgramPage1(9.5,false,&value);assert(value==10000);
ProgramPage1(30,true,&value);assert(value==30);}''')
        with self.assertRaises(SysMLValidationError):
            generate_validated_bundle(lambda _:('fixture',assembly.candidate(bad)), 'same requirement',
                contract=c,max_repairs=0,validator=validate_executable_sysml)


if __name__=='__main__':unittest.main()
