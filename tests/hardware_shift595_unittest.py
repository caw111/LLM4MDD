import copy
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest

sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
from run_contract_development import behavior, lower_verified, adapt_verified, INTERPRETATIONS
from fastapi.testclient import TestClient
from app.main import app
from app.services.hardware_adapter import validate_hardware_contract, generate_hardware_adapter, physical_pin
from app.services.interface_planner import check_explicit_requirements, plan_interface_contract, contract_digest, has_only_response_deadline
from app.services.shift595 import wiring_evidence
from app.services.sysml_assembly import assemble_sysml
from app.services.sysml_compiler import validate_executable_sysml
from app.services.behavior_contract import check_contract
from app.services.interface_semantics import freeze_model_semantics


def inventory():
    return json.loads(Path('tools/examples/shift595_counter.contract.json').read_text(encoding='utf-8'))


def source_case():
    return next(c for c in json.loads(Path('experiment/formal/manifest.json').read_text(encoding='utf-8'))['cases'] if c['id']=='embedbench_16')


class Shift595Tests(unittest.TestCase):
    def test_wiring_evidence_and_repair_keep_behavior_and_detect_renamed_errors(self):
        text=source_case()['requirement']+'\n'+INTERPRETATIONS['embedbench_16']
        facts=wiring_evidence(text,lambda p:physical_pin(p,'arduino:avr:mega'))
        self.assertEqual(len(facts),1)
        self.assertEqual(facts[0]['data_pin'],'A0')
        self.assertEqual(facts[0]['registers_nearest_mcu_first'],[
            {k:v for k,v in r.items() if k!='digit'} for r in inventory()['interfaces'][1]['binding']['registers']])
        self.assertEqual(wiring_evidence(text.replace('sr1:Q0','sr1:Q6'),lambda p:physical_pin(p,'arduino:avr:mega')),[])
        self.assertTrue(has_only_response_deadline(text))
        for extra in (' 每500ms翻转', ' 每次按下持续鸣叫100ms', ' 平滑运动', ' delay before displaying'):
            self.assertFalse(has_only_response_deadline(text+extra))
        clock=inventory();clock['interfaces'].append(dict(name='dt',direction='input',type='i32',unit='ms',
            initial=0,meaning='clock',device='clock',hardware='millis',sample_period_ms=10,binding={'kind':'elapsed_ms'}))
        with self.assertRaisesRegex(ValueError,'Only a response deadline'):check_explicit_requirements(clock,text,{})
        bad=inventory();bad['interfaces'][1]['binding']={'kind':'unsupported'}
        renamed=copy.deepcopy(bad);renamed['interfaces'][1]['name']='renamed';renamed['required_dependencies']={'renamed':['pressed']}
        replies=iter([bad,renamed,inventory()]);prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',json.dumps(next(replies))
        accepted,report=plan_interface_contract(call,text,{},max_repairs=2,target_board='arduino:avr:mega')
        self.assertTrue(report['attempts'][1]['no_progress'])
        self.assertIn('NO PROGRESS',prompts[2]);self.assertIn('Verified electrical facts',prompts[0])
        self.assertIn('Only response-deadline timing',prompts[0])
        from app.services.interface_registry import register_contract
        self.assertEqual(accepted,register_contract(freeze_model_semantics(inventory())))

    def test_contract_rejects_wrong_types_ranges_mappings_and_application_logic(self):
        validate_hardware_contract(inventory())
        for field,value in [('initial',100),('initial',-1),('type','f64'),('unit',''),('unit',None)]:
            c=inventory();c['interfaces'][1][field]=value
            with self.subTest(field=field,value=value),self.assertRaises(ValueError):validate_hardware_contract(c)
        for field,value in [('registers',[]),('data_pin','A2'),('clock_pin',2),('leading_zero',1),('invalid_policy','wrap'),('increment',37)]:
            c=inventory();c['interfaces'][1]['binding'][field]=value
            with self.subTest(field=field,value=value),self.assertRaises(ValueError):validate_hardware_contract(c)
        for field,value in [('digit',True),('digit',1),('segment_bits',[0,1,2,3,4,5,5]),('segment_bits',[0,1,2,3,4,5,8]),('active_low',1),('display','sevseg2')]:
            c=inventory();c['interfaces'][1]['binding']['registers'][0][field]=value
            with self.subTest(field=field,value=value),self.assertRaises(ValueError):validate_hardware_contract(c)
        c=inventory();second=copy.deepcopy(c['interfaces'][1]);second['name']='otherDisplay'
        second['binding'].update(data_pin='A3',clock_pin='A4',latch_pin='A5');c['interfaces'].append(second)
        with self.assertRaisesRegex(ValueError,'multiple bindings'):validate_hardware_contract(c)

    def test_quantity_units_are_preserved_without_adapter_conversion(self):
        c=inventory()
        expected=generate_hardware_adapter('void ProgramPage1(bool pressed, int32_t* displayNumber);',c)
        for unit in ('cm','Hz','digit_value'):
            with self.subTest(unit=unit):
                quantity=copy.deepcopy(c);quantity['interfaces'][1]['unit']=unit
                before=copy.deepcopy(quantity)
                validate_hardware_contract(quantity)
                self.assertEqual(generate_hardware_adapter('void ProgramPage1(bool pressed, int32_t* displayNumber);',quantity),expected)
                self.assertEqual(quantity,before)

    def test_split_cascade_diagnostic_resolves_pin_aliases_without_merging_chains(self):
        c=inventory();second=copy.deepcopy(c['interfaces'][1]);second['name']='units'
        second['binding'].update(data_pin=54,clock_pin='D56',latch_pin=55)
        c['interfaces'].append(second)
        with self.assertRaisesRegex(ValueError,'share one 74HC595 cascade.*whole number'):
            validate_hardware_contract(c)
        second['binding']['data_pin']='A3'
        with self.assertRaisesRegex(ValueError,'Conflicting hardware bindings'):
            validate_hardware_contract(c)

    def test_dependency_repair_identifies_output_reference_and_preserves_inputs(self):
        c=inventory();led=copy.deepcopy(c['interfaces'][1])
        led.update(name='indicator',type='bool',initial=False,binding={'kind':'digital_output','pin':13})
        c['interfaces'].append(led);c['required_dependencies']['indicator']=['pressed']
        invalid=copy.deepcopy(c);invalid['required_dependencies']['indicator']=['displayNumber']
        with self.assertRaisesRegex(ValueError,"required_dependencies.*indicator.*not declared inputs: displayNumber.*pressed"):
            validate_hardware_contract(invalid)
        replies=iter([invalid,c]);prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',json.dumps(next(replies))
        accepted,report=plan_interface_contract(call,'Button on D2 controls the counter and indicator LED on D13.',{},
                                              max_repairs=1,target_board='arduino:avr:mega')
        self.assertEqual([a['ok'] for a in report['attempts']],[False,True])
        self.assertEqual(accepted['required_dependencies'],c['required_dependencies'])
        self.assertIn('original sensor/time inputs',prompts[1])

    def test_multi_digit_encoding_zero_suppression_and_invalid_blanking(self):
        c=inventory();c['interfaces']=c['interfaces'][1:];c['required_dependencies']={}
        binding=c['interfaces'][0]['binding'];binding['leading_zero']=False
        # Four registers with nontrivial order, bit wiring and alternating polarity.
        binding['registers']=[dict(register=f'sr{i}',display=f'display{i}',digit=digit,
            segment_bits=[6,5,4,3,2,1,0],active_low=bool(i%2)) for i,digit in enumerate((2,0,3,1))]
        validate_hardware_contract(c)
        adapter=generate_hardware_adapter('void ProgramPage1(int32_t* displayNumber);',c)
        symbols=['ABCDEF','BC','ABDEG','ABCDG','BCFG','ACDFG','ACDEFG','ABC','ABCDEFG','ABCDFG']
        checks=[]
        for value in (0,7,42,105,9999,-1,10000):
            text=str(value).rjust(4) if 0<=value<=9999 else '    '
            expected=0
            for i,register in enumerate(binding['registers']):
                ch=text[register['digit']]
                active=symbols[int(ch)] if ch!=' ' else ''
                bits=sum(1 << (6-('ABCDEFG'.index(s))) for s in active)
                if register['active_low']:bits^=255
                expected|=bits << (8*i)
            checks.append(f'command={value};oracle::now+=10;loop();assert(frame()=={expected}UL);')
        source='''#include <Arduino.h>
#include <cassert>
int ProgramPage1_instance=0;int32_t command=0;
void ProgramPage1_Init(int*){} void ProgramPage1(int32_t* value){*value=command;}
'''+adapter+'''
uint32_t frame(){uint32_t shift=0,result=0;bool data=false,clock=false,latch=false;
for(const auto& e:oracle::events){if(e.pin==A0)data=e.level;
if(e.pin==A2){if(e.level&&!clock)shift=(shift<<1)|data;clock=e.level;}
if(e.pin==A1){if(e.level&&!latch)result=shift;latch=e.level;}}return result;}
int main(){setup();
'''+''.join(checks)+'}'
        with tempfile.TemporaryDirectory() as folder:
            base=Path(folder);shutil.copyfile('experiment/development/oracles/gpio_arduino.h',base/'Arduino.h')
            cpp=base/'probe.cpp';cpp.write_text(source,encoding='utf-8');exe=base/'probe.exe'
            build=subprocess.run([shutil.which('g++'),'-std=c++17','-DARDUINO_AVR_MEGA2560','-I',str(base),str(cpp),'-o',str(exe)],capture_output=True,text=True,timeout=60)
            self.assertEqual(build.returncode,0,build.stderr)
            run=subprocess.run([str(exe)],capture_output=True,text=True,timeout=10)
            self.assertEqual(run.returncode,0,run.stderr)
    def test_actual_circuit_preserves_roles_chain_polarity_and_all_registers(self):
        requirement=source_case()['requirement'];c=inventory()
        check_explicit_requirements(c,requirement,{})
        for mutation in ('swap_bus','polarity','segments','cascade','omit_register','omit_device'):
            bad=copy.deepcopy(c);b=bad['interfaces'][1]['binding']
            if mutation=='swap_bus':b['clock_pin'],b['latch_pin']=b['latch_pin'],b['clock_pin']
            if mutation=='polarity':b['registers'][0]['active_low']=False
            if mutation=='segments':b['registers'][0]['segment_bits']=[1,0,2,3,4,5,6]
            if mutation=='cascade':b['registers'].reverse()
            if mutation=='omit_register':b['registers'].pop()
            if mutation=='omit_device':bad['interfaces'].pop()
            with self.subTest(mutation=mutation),self.assertRaises(ValueError):check_explicit_requirements(bad,requirement,{})
        bad=copy.deepcopy(c);bad['interfaces'][1]['binding']['clock_pin']='A1';bad['interfaces'][1]['binding']['latch_pin']='A2'
        responses=iter([bad,c]);prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',json.dumps(next(responses))
        accepted,report=plan_interface_contract(call,requirement,{},max_repairs=1,target_board='arduino:avr:mega')
        self.assertEqual([a['ok'] for a in report['attempts']],[False,True])
        from app.services.interface_registry import register_contract
        self.assertEqual(accepted,register_contract(freeze_model_semantics(c)))
        self.assertIn('74HC595 wiring mismatch',prompts[1])

    def test_independent_pin_trace_accepts_two_emitters_and_rejects_faults(self):
        code='''#include <Arduino.h>
const byte digits[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
int count=0;bool previous=false;
void send(){digitalWrite(A1,LOW);shiftOut(A0,A2,MSBFIRST,digits[count%10]);shiftOut(A0,A2,MSBFIRST,digits[count/10]);digitalWrite(A1,HIGH);}
void setup(){pinMode(2,INPUT_PULLUP);pinMode(A0,OUTPUT);pinMode(A1,OUTPUT);pinMode(A2,OUTPUT);send();}
void loop(){bool pressed=digitalRead(2)==LOW;if(pressed&&!previous){count+=37;if(count>99)count=0;send();}previous=pressed;}
'''
        bitbang='void emit(byte d){for(int b=7;b>=0;--b){digitalWrite(A0,(d>>b)&1);digitalWrite(A2,HIGH);digitalWrite(A2,LOW);}}\n'
        alternative=code.replace('void send()',bitbang+'void send()').replace('shiftOut(A0,A2,MSBFIRST,','emit(')
        unused_bit=code.replace('digits[count%10]','(digits[count%10]&0x7f)').replace('digits[count/10]','(digits[count/10]&0x7f)')
        cases=[(code,'passed'),(alternative,'passed'),(unused_bit,'passed'),(code.replace('count+=37','count+=36'),'failed'),
               (code.replace('pressed&&!previous','pressed'),'failed'),(code.replace('count=0;send();','count-=100;send();'),'failed'),
               (code.replace('MSBFIRST','LSBFIRST'),'failed'),(code.replace('digits[count%10]','digits[count/10]'),'failed'),
               (code.replace('digitalWrite(A1,HIGH)','digitalWrite(A1,LOW)'),'failed'),
               (code.replace('digitalRead(2)==LOW','false'),'failed')]
        with tempfile.TemporaryDirectory() as folder:
            for i,(sketch,expected) in enumerate(cases):
                with self.subTest(case=i):
                    attempt=Path(folder)/str(i);cpp=attempt/'build/sketch/sketch.ino.cpp';cpp.parent.mkdir(parents=True)
                    cpp.write_text(sketch,encoding='utf-8')
                    self.assertEqual(behavior(attempt,116,'arduino:avr:mega')['functional_status'],expected)

    def test_reference_pipeline_uses_real_sysml_ir_backend_and_arduino_compiler(self):
        controller=json.loads(Path('tools/examples/shift595_counter.controller.json').read_text(encoding='utf-8'))
        with tempfile.TemporaryDirectory() as folder:
            target=Path(folder)
            with TestClient(app) as client:
                prepared=client.post('/generate/interface-contract',json=dict(requirement=source_case()['requirement'],
                    interface_contract=inventory(),out_path=str(target/'contract.json')))
            self.assertEqual(prepared.status_code,200,prepared.text)
            browser=subprocess.run([shutil.which('node'),'-e',"let s='';process.stdin.on('data',c=>s+=c);process.stdin.on('end',()=>process.stdout.write(JSON.stringify(JSON.parse(s))));"],
                input=prepared.text,capture_output=True,text=True,encoding='utf-8',check=True)
            c=json.loads(browser.stdout)['interface_contract']
            self.assertEqual(contract_digest(c),prepared.json()['contract_sha256'])
            self.assertEqual(c['interfaces'][1]['binding']['registers'],inventory()['interfaces'][1]['binding']['registers'])
            source=assemble_sysml(controller,c);checked=validate_executable_sysml(source)
            self.assertTrue(checked['ok'],checked)
            self.assertEqual(checked['ir']['version'],2)
            self.assertEqual(check_contract(checked['ir'],c)['issues'],[])
            generated=lower_verified(source,c,checked['ir'],target/'lowered')
            sketch=adapt_verified(generated,c,target/'lowered')
            from run_controlled_experiment import compile_code
            from run_formal_experiment import CLI
            check,log=compile_code(target/'compiled',sketch,CLI,'arduino:avr:mega')
            self.assertTrue(check['compile_ok'],log)
            self.assertEqual(behavior(target/'compiled',116,'arduino:avr:mega')['functional_status'],'passed')


if __name__=='__main__':unittest.main()
