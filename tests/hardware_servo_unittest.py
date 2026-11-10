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
from app.services.sysml_compiler import compile_sysml
from app.services.behavior_contract import check_contract
from app.services.interface_planner import check_explicit_requirements


def inventory():
    return json.loads(Path('tools/examples/three_servos.contract.json').read_text(encoding='utf-8'))


class ServoTests(unittest.TestCase):
    def test_all_explicit_circuit_servo_pins_are_required(self):
        c=inventory()
        circuit={'parts':[{'id':'mega','type':'wokwi-arduino-mega'},*[{'id':f's{i}','type':'wokwi-servo'} for i in range(3)]],
                 'connections':[['mega:2','s0:PWM','orange'],['s1:PWM','mega:3','orange'],['mega:4','s2:PWM','orange']]}
        requirement='Control three servos. Given circuit: '+json.dumps(circuit)
        check_explicit_requirements(c,requirement,{})
        c['interfaces'][4]['binding']['pin']=5
        with self.assertRaisesRegex(ValueError,'servo wiring'):check_explicit_requirements(c,requirement,{})
        c['interfaces']=c['interfaces'][:2]
        with self.assertRaisesRegex(ValueError,'explicit positional servo'):check_explicit_requirements(c,requirement,{})

    def test_contract_units_bounds_pin_storage_and_logic_rejection(self):
        validate_hardware_contract(inventory())
        for field,value in [('type','bool'),('unit','rad'),('initial',181),('initial',-1)]:
            c=inventory();c['interfaces'][2][field]=value
            with self.subTest(field=field,value=value),self.assertRaises(ValueError):validate_hardware_contract(c)
        for field,value in [('pin',64),('pin','A10'),('pin',69),('invalid_policy','clamp'),('speed',20),('duration_ms',100)]:
            c=inventory();c['interfaces'][2]['binding'][field]=value
            with self.subTest(field=field,value=value),self.assertRaises(ValueError):validate_hardware_contract(c)
        c=inventory();c['interfaces'][2]['binding']['pin']='A9';validate_hardware_contract(c)
        c['interfaces'][2]['binding']['pin']=3
        with self.assertRaisesRegex(ValueError,'Conflicting'):validate_hardware_contract(c)

    def test_servo_count_allocates_timer_banks_on_each_board(self):
        for board,banks in [('arduino:avr:uno',[[9,10]]),('arduino:avr:mega',[[44,45,46],[11,12],[2,3,5],[6,7,8]])]:
            for count in [1,12,13,24,25,36,37,48,49]:
                if board.endswith('uno') and count>13:continue
                for pwm in [4,9,11,44] if board.endswith('mega') else [3,9]:
                    pins=[p for p in range(64 if board.endswith('mega') else 20) if p!=pwm]
                    c=inventory();c['board']=board;c['required_dependencies']={};c['interfaces']=[]
                    for i in range(count):
                        p=copy.deepcopy(inventory()['interfaces'][2]);p['name']=f'angle{i}';p['binding']['pin']=pins[i];c['interfaces'].append(p)
                    out=copy.deepcopy(inventory()['interfaces'][2]);out.update(name='brightness',unit='pwm_count');out['binding']={'kind':'pwm_output','pin':pwm};c['interfaces'].append(out)
                    conflict=count>12*len(banks) or pwm in set().union(*[set(b) for b in banks[:(count+11)//12]])
                    with self.subTest(board=board,count=count,pwm=pwm):
                        if conflict:
                            with self.assertRaisesRegex(ValueError,'capacity|timer conflicts'):validate_hardware_contract(c)
                        else:validate_hardware_contract(c)

    def test_real_reference_compiler_backend_and_three_servo_mapping(self):
        c=inventory();ir=compile_sysml(Path('tools/sysml/three_servos.sysml').read_text())
        self.assertEqual(check_contract(ir,c)['issues'],[])
        with tempfile.TemporaryDirectory() as folder:
            base=Path(folder);header,source=lower(ir,base/'lowered')
            for board in ('arduino:avr:uno','arduino:avr:mega'):
                c['board']=board
                _,sketch=generate_adapter(lambda _:self.fail('No LLM for deterministic adaptation'),header,source,'',{},contract=c)
                target=base/board.split(':')[-1];check,log=compile_code(target,sketch,CLI,board)
                self.assertTrue(check['compile_ok'],log)
                result=behavior(target,125,board)
                self.assertEqual(result['functional_status'],'passed',(target/'host_oracle/trace.log').read_text() if (target/'host_oracle/trace.log').exists() else result)

    def test_invalid_commands_hold_last_angle_without_microsecond_confusion(self):
        c=inventory();c['interfaces']=c['interfaces'][2:3];c['required_dependencies']={}
        c['interfaces'][0]['initial']=45.0
        adapter=generate_hardware_adapter('void ProgramPage1(double* angle1);',c)
        with tempfile.TemporaryDirectory() as folder:
            base=Path(folder)
            for name,target in [('gpio_arduino.h','Arduino.h'),('Servo.h','Servo.h')]:
                shutil.copyfile(Path('experiment/development/oracles')/name,base/target)
            source='''#include <Arduino.h>
int ProgramPage1_instance=0;double command=0;
void ProgramPage1_Init(int*){}
void ProgramPage1(double* angle){*angle=command;}
'''+adapter+'''
#include <cassert>
int main(){setup();assert(oracle::servos.back().pin==2);assert(oracle::servos.back().pulse==1008);
command=90.9;loop();assert(oracle::servos.back().pulse==1472);
auto n=oracle::servos.size();
for(double value:{90.1,-1.0,181.0,1800.0,(double)NAN,(double)INFINITY}){
 command=value;oracle::now+=10;loop();assert(oracle::servos.size()==n);}
command=180;oracle::now+=10;loop();assert(oracle::servos.back().pulse==2400);
command=0;oracle::now+=10;loop();assert(oracle::servos.back().pulse==544);}
'''
            src=base/'check.cpp';src.write_text(source,encoding='utf-8');exe=base/'check.exe'
            build=subprocess.run([shutil.which('g++'),'-std=c++17','-DARDUINO_AVR_MEGA2560','-I',str(base),str(src),'-o',str(exe)],capture_output=True,text=True)
            self.assertEqual(build.returncode,0,build.stderr)
            run=subprocess.run([str(exe)],capture_output=True,text=True)
            self.assertEqual(run.returncode,0,run.stderr)

    def test_independent_oracle_accepts_both_apis_and_rejects_behavior_faults(self):
        sketch='''#include <Servo.h>
Servo a,b,c;
void setup(){a.attach(2);b.attach(3);c.attach(4);}
void loop(){int x=map(analogRead(A3),0,1023,0,180);int y=map(analogRead(A4),0,1023,0,180);
a.write(x);b.write(y);c.write(y);}
'''
        micro=sketch.replace('a.write(x);b.write(y);c.write(y);','a.writeMicroseconds(map(x,0,180,544,2400));b.writeMicroseconds(map(y,0,180,544,2400));c.writeMicroseconds(map(y,0,180,544,2400));')
        math_sketch=sketch.replace('map(analogRead(A3),0,1023,0,180)','(int)roundf(fabsf(analogRead(A3)*180.0f/1023.0f))').replace('map(analogRead(A4),0,1023,0,180)','(int)round(fabs(analogRead(A4)*180.0/1023.0))')
        cases=[(sketch,'passed'),(micro,'passed'),(math_sketch,'passed'),(sketch.replace('analogRead(A3)','analogRead(3)').replace('analogRead(A4)','analogRead(4)'),'passed'),
            (sketch.replace('analogRead(A3)','512'),'failed'),(sketch.replace('analogRead(A3)','analogRead(A4)'),'failed'),
            (sketch.replace('c.attach(4)','c.attach(5)'),'failed'),(sketch.replace('c.write(y);',''),'failed'),
            (sketch.replace('1023,0,180','1023,0,90'),'failed'),(sketch.replace('void loop(){','void loop(){delay(200);'),'failed'),
            (sketch.replace('c.write(y);','c.write(y);c.detach();'),'failed')]
        with tempfile.TemporaryDirectory() as folder:
            for i,(code,expected) in enumerate(cases):
                attempt=Path(folder)/str(i);cpp=attempt/'build/sketch/sketch.ino.cpp';cpp.parent.mkdir(parents=True);cpp.write_text('#include <Arduino.h>\n'+code)
                with self.subTest(case=i):
                    result=behavior(attempt,125,'arduino:avr:mega')
                    self.assertEqual(result['functional_status'],expected,(attempt/'host_oracle/trace.log').read_text() if (attempt/'host_oracle/trace.log').exists() else result)


if __name__=='__main__':unittest.main()
