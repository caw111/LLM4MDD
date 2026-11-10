"""Independent multi-input and dynamic timing checks, including mutation controls."""
import copy
import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
from run_contract_development import behavior, lower_verified, adapt_verified, INTERPRETATIONS
from run_controlled_experiment import compile_code
from run_formal_experiment import CLI
from app.services.interface_planner import check_explicit_requirements, requires_clock, has_only_response_deadline, add_public_behavior_checks
from app.services.interface_semantics import freeze_model_semantics
from app.services.sysml_assembly import assemble_sysml
from app.services.sysml_compiler import validate_executable_sysml
from app.services.behavior_contract import check_contract


EMITTER='''#include <Arduino.h>
const byte segments[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
void display(int n){digitalWrite(A1,LOW);shiftOut(A0,A2,MSBFIRST,segments[n%10]);shiftOut(A0,A2,MSBFIRST,segments[n/10]);digitalWrite(A1,HIGH);}
void bus(){pinMode(A0,OUTPUT);pinMode(A1,OUTPUT);pinMode(A2,OUTPUT);display(0);}
'''
PARITY=EMITTER+'''
void setup(){bus();analogWrite(2,0);analogWrite(3,0);}
void loop(){int n=map(analogRead(A5),0,1023,0,99);display(n);
analogWrite(2,n%2==0?map(analogRead(A3),0,1023,0,255):0);
analogWrite(3,n%2==1?map(analogRead(A4),0,1023,0,255):0);}
'''
REPEAT=EMITTER+'''
int count=0,previous=0;unsigned long last=0;
void setup(){bus();pinMode(2,INPUT_PULLUP);pinMode(3,INPUT_PULLUP);pinMode(4,INPUT_PULLUP);}
void loop(){bool up=digitalRead(2)==LOW,down=digitalRead(3)==LOW,reset=digitalRead(4)==LOW;
int direction=up&&!down?1:down&&!up?-1:0;unsigned long now=millis();
unsigned long interval=100+map(analogRead(A3),0,1023,0,900);
if(reset){count=0;previous=0;last=now;}else if(direction==0){previous=0;last=now;}
else{if(direction!=previous || now-last>=interval){count+=direction;if(count>99)count=0;if(count<0)count=99;last=now;}previous=direction;}
display(count);}
'''


class ShiftBehaviorTests(unittest.TestCase):
    def check_cases(self,task,cases):
        with tempfile.TemporaryDirectory() as folder:
            for index,(code,expected) in enumerate(cases):
                with self.subTest(index=index,task=task):
                    attempt=Path(folder)/str(index);cpp=attempt/'build/sketch/sketch.ino.cpp';cpp.parent.mkdir(parents=True)
                    cpp.write_text(code,encoding='utf-8');result=behavior(attempt,task,'arduino:avr:mega')
                    log=attempt/'host_oracle/trace.log'
                    self.assertEqual(result['functional_status'],expected,log.read_text() if log.exists() else result)

    def test_independent_parity_adc_outputs_and_continuous_bus_refresh(self):
        self.check_cases(204,[(PARITY,'passed'),
            (PARITY.replace('analogRead(A5)','512'),'failed'),
            (PARITY.replace('analogRead(A3)','analogRead(A4)'),'failed'),
            (PARITY.replace('analogRead(A4)','analogRead(A3)'),'failed'),
            (PARITY.replace('n%2==0','n%2==1'),'failed'),
            (PARITY.replace('0,99','0,98'),'failed'),
            (PARITY.replace('void loop(){','void loop(){delay(200);'),'failed')])

    def test_independent_variable_hold_wrap_reset_and_direction(self):
        self.check_cases(179,[(REPEAT,'passed'),
            (REPEAT.replace('analogRead(A3)','511'),'failed'),
            (REPEAT.replace('now-last>=interval','false'),'failed'),
            (REPEAT.replace('count=99;','count=0;'),'failed'),
            (REPEAT.replace('count>99)count=0','count>99)count=99'),'failed'),
            (REPEAT.replace('if(reset)','if(false)'),'failed'),
            (REPEAT.replace('up&&!down?1:down&&!up?-1:0','up?1:down?-1:0'),'failed')])

    def test_repeat_tolerance_is_symmetric_but_input_responses_are_causal(self):
        # Hand-reviewed trace fixtures exercise the oracle boundary itself,
        # independently of any generated model or production timer code.
        times=[0,100,300,500,1049,1598,1800,2100,3100,3500,3600,3700,3800,4000,4549,5549,5800]
        values=[0,99,0,1,2,3,0,99,98,99,0,1,2,3,4,5,0]
        def fixture(repeat_offset=0,press_offset=0):
            shifted=times.copy();shifted[4]+=repeat_offset;shifted[1]+=press_offset
            return EMITTER+'''void setup(){bus();pinMode(2,INPUT_PULLUP);pinMode(3,INPUT_PULLUP);pinMode(4,INPUT_PULLUP);}
void loop(){const unsigned long times[]={'''+','.join(map(str,shifted))+'''};
const int values[]={'''+','.join(map(str,values))+'''};
int value=0;for(int i=0;i<17;++i)if(millis()>=times[i])value=values[i];display(value);}
'''
        self.check_cases(179,[(fixture(-20),'passed'),(fixture(20),'passed'),
                             (fixture(-21),'failed'),(fixture(21),'failed'),
                             (fixture(press_offset=-1),'failed')])

    def test_two_reference_models_pass_the_production_chain_and_independent_oracles(self):
        for name,task,case_id in [('shift595_parity',204,'embedbench_104'),('shift595_repeat',179,'embedbench_79')]:
            with self.subTest(name=name),tempfile.TemporaryDirectory() as folder:
                c=freeze_model_semantics(json.loads(Path(f'tools/examples/{name}.contract.json').read_text(encoding='utf-8')))
                c=add_public_behavior_checks(c,INTERPRETATIONS[case_id])
                self.assertEqual(len(c['numeric_checks']),1)
                controller=json.loads(Path(f'tools/examples/{name}.controller.json').read_text(encoding='utf-8'))
                checked=validate_executable_sysml(assemble_sysml(controller,c))
                self.assertTrue(checked['ok'],checked)
                self.assertEqual(check_contract(checked['ir'],c)['issues'],[])
                base=Path(folder)
                generated=lower_verified(assemble_sysml(controller,c),c,checked['ir'],base/'lowered')
                code=adapt_verified(generated,c,base/'lowered')
                result,log=compile_code(base/'compiled',code,CLI,'arduino:avr:mega')
                self.assertTrue(result['compile_ok'],log)
                self.assertEqual(behavior(base/'compiled',task,'arduino:avr:mega')['functional_status'],'passed')

    def test_auto_repeat_requires_real_elapsed_input(self):
        text=INTERPRETATIONS['embedbench_79']
        self.assertTrue(requires_clock(text))
        for negative in ('长按和松开不重复加', '长按不重复', '不要长按重复', 'No auto-repeat',
                         INTERPRETATIONS['embedbench_16']):
            with self.subTest(negative=negative):
                self.assertFalse(requires_clock(negative))
        for positive in ('长按时重复步进', 'Enable auto-repeat', '长按不重复；每500ms翻转输出'):
            with self.subTest(positive=positive):
                self.assertTrue(requires_clock(positive))
        c=json.loads(Path('tools/examples/shift595_repeat.contract.json').read_text(encoding='utf-8'))
        check_explicit_requirements(c,text,{})
        bad=copy.deepcopy(c);bad['interfaces']=[p for p in bad['interfaces'] if p['name']!='dt']
        bad['required_dependencies']['displayNumber'].remove('dt')
        with self.assertRaisesRegex(ValueError,'elapsed_ms'):check_explicit_requirements(bad,text,{})
        self.assertTrue(has_only_response_deadline(INTERPRETATIONS['embedbench_104']))
        self.assertFalse(has_only_response_deadline(INTERPRETATIONS['embedbench_104']+' 每2秒翻转输出'))


if __name__=='__main__':unittest.main()
