import copy
import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
from unittest.mock import patch

from app.services.expression_normalization import normalize_controller
from app.services.sysml_assembly import expression, assemble_sysml, ExpressionSyntaxError
from app.services.sysml_compiler import compile_sysml, validate_executable_sysml
from app.services.interface_registry import assemble_interface_contract, register_contract
from app.services.interface_planner import add_public_behavior_checks
from app.services.behavior_contract import check_contract
from app.services.hardware_adapter import validate_hardware_contract
from app.services.sysml import generate_validated_bundle, sysml_bundle_prompt
import sysml_assembly_unittest as assembly_tests
from sysml_assembly_unittest import contract


class CompilerCompletionTests(unittest.TestCase):
    def test_scalar_normalization_preserves_precedence_branch_types_and_lazy_execution(self):
        spec=dict(states=[],updates=[],outputs={
            'flag':'and(enabled, not(paused)) || other && !enabled',
            'value':'enabled ? (sample >= 0.0 ? RealFunctions::floor(sample) : -2) : 3',
            'safe':'!enabled ? 0.0 : sample / (sample - 1.0)'})
        assembly_tests.AssemblyTests.execute(self,spec,contract([('enabled','bool'),('paused','bool'),('other','bool'),('sample','f64')],
            [('flag','bool'),('value','i32'),('safe','f64')]),'''
#include <cfenv>
int main(){bool flag=false;int value=0;double safe=0;ProgramPage1_Init(&ProgramPage1_instance);
 std::feclearexcept(FE_ALL_EXCEPT);
 ProgramPage1(false,true,true,1.0,&flag,&value,&safe);assert(flag && value==3 && safe==0);
 assert(!std::fetestexcept(FE_DIVBYZERO | FE_INVALID));
 ProgramPage1(true,false,false,2.5,&flag,&value,&safe);assert(flag && value==2 && safe>1.66 && safe<1.67);
 ProgramPage1(true,true,true,-1.0,&flag,&value,&safe);assert(!flag && value==-2 && safe==0.5);
}''')

    def test_normalization_logs_exact_locations_without_touching_source_or_metadata(self):
        original=dict(states=[],updates=[{'if':'a && !b','then':[]}],outputs={'out':'a ? 1 : 2'},
                      definitions=[dict(name='Child',body=dict(states=[],updates=[],outputs={'x':'or(a,b)'}))])
        saved=copy.deepcopy(original)
        normalized,events=normalize_controller(original,expression)
        self.assertEqual(original,saved)
        self.assertEqual({e['path'] for e in events},{'/controller/updates/0/if','/controller/outputs/out','/controller/definitions/0/body/outputs/x'})
        self.assertEqual(normalized['outputs']['out'],{'if':'a','then':1,'else':2})
        self.assertEqual(normalize_controller(normalized,expression),(normalized,[]))

    def test_unsupported_syntax_arity_injection_and_overdeep_expressions_remain_errors(self):
        for text in ['and(a)','and(a,b,)','a ? 1 :','a && millis()','a || b; part P {}','a && (b % 2 > 0)',
                     'a && "string"','a && [1]','and('+','.join(['a']*257)+')','a && '+'('*34+'b'+')'*34]:
            with self.subTest(text=text[:80]),self.assertRaises(ExpressionSyntaxError):expression(text)

    def test_equivalent_syntax_does_not_coerce_numeric_truthiness_or_discard_unknown_operands(self):
        for text in ['and(a,42)','a && unknownInput','+a || false']:
            source=assemble_sysml(dict(states=[],updates=[],outputs={'out':text}),contract([('a','bool')],[('out','bool')]))
            with self.subTest(text=text):self.assertFalse(validate_executable_sysml(source)['ok'])

    def test_original_buzzer_candidates_reach_real_checks_and_finite_enumeration_still_fails(self):
        base=Path('experiment/output/interface_hardening_comparison_20261014/simple_021/repeat_1')
        frozen=base/'model_validation.json';original=frozen.read_bytes()
        attempts=json.loads(original)['attempts']
        data=json.loads((base/'contract.json').read_text(encoding='utf-8'))
        data=add_public_behavior_checks(data,data['requirements'][0]['text'])
        self.assertEqual(data['periodic_checks'][0]['period_ms'],1000)
        last=json.loads(attempts[-1]['response'])
        normalized,events=normalize_controller(last['controller'],expression)
        self.assertEqual(len(events),5)
        ir=compile_sysml(assemble_sysml(normalized,data))
        result=check_contract(ir,data)
        self.assertIn('CONTRACT_PERIODIC',[i['code'] for i in result['issues']])
        self.assertEqual(frozen.read_bytes(),original)

    def test_typed_periodic_fragment_uses_frozen_names_and_original_parameters(self):
        base=Path('experiment/output/interface_hardening_comparison_20261014/simple_021/repeat_1')
        data=json.loads((base/'contract.json').read_text(encoding='utf-8'))
        data=add_public_behavior_checks(data,data['requirements'][0]['text'])
        from app.services.model_repair import periodic_component_example
        fragment=periodic_component_example(data['periodic_checks'][0],data,{})['controller_fragment']
        spec=dict(states=[],updates=[],**fragment)
        mapping=dict(requirements_to_testpoints=[dict(requirement_id='R1',requirement_text=data['requirements'][0]['text'],
                        test_point_id='T1',test_method='trace',stimulus='delta',oracle='repeating pulse')])
        _,_,report=generate_validated_bundle(lambda _:('fixture',json.dumps(dict(controller=spec,req_test_map=mapping))),
            sysml_bundle_prompt(data['requirements'][0]['text'],contract=data),contract=data,validator=validate_executable_sysml,max_repairs=0)
        self.assertTrue(report['attempts'][0]['ok'])
        self.assertEqual(report['coverage']['periodic']['status'],'passed')

    def test_fixed_text_registration_preserves_exact_message_and_rejects_real_conflicts(self):
        data=self.text_contract('arduino:avr:uno')
        self.assertEqual(data['interface_registry']['serial_text_outputs'][0]['trigger_port'],'io_0')
        for key,value in [('initial',0),('text',''),('text','x\0y'),('text','界'*90),('baud',False)]:
            draft=self.text_proposal('arduino:avr:uno');draft['interface_requests'][0][key]=value
            with self.subTest(key=key),self.assertRaises(ValueError):
                c,_=assemble_interface_contract(draft,require_clock=True)
                c['requirements']=data['requirements'];validate_hardware_contract(c)
        bad=copy.deepcopy(data);bad['interfaces'][0]['binding']['text']='different'
        with self.assertRaisesRegex(ValueError,'interface_registry differs'):validate_hardware_contract(bad)

    def text_proposal(self,board):
        return dict(board=board,control_period_ms=10,interface_requests=[dict(kind='serial_text_output',initial=False,
                    meaning='Gate for fixed message every two seconds',baud=9600,text='Example "quoted" message\\path')])

    def text_contract(self,board):
        c,_=assemble_interface_contract(self.text_proposal(board),require_clock=True)
        c['requirements']=[dict(id='R1',text='Every two seconds print the exact fixed message',assertion_ids=['text'])]
        validate_hardware_contract(c)
        return register_contract(c)

    def test_fixed_text_through_production_chain_host_trace_and_uno_mega_compilers(self):
        from fastapi.testclient import TestClient
        from app.main import app
        for board in ('arduino:avr:uno','arduino:avr:mega'):
            with self.subTest(board=board),tempfile.TemporaryDirectory() as folder,TestClient(app) as client,patch('app.main._call_llm',side_effect=AssertionError('No model calls')):
                base=Path(folder);data=self.text_contract(board)
                spec=dict(states=[],updates=[],components=[dict(kind='periodic_pulse',name='report',delta='elapsedMilliseconds',
                    enabled='true',period_ms=2000,pulse_ms=10,first_pulse='after_period',pause='preserve')],outputs={'io_0':'component_report_value'})
                compiled=client.post('/compile/sysml2',json=dict(system_sysml2=assemble_sysml(spec,data),interface_contract=data,out_dir=str(base/'ir')))
                self.assertEqual(compiled.status_code,200,compiled.text)
                generated=client.post('/convert/il-to-c',json=dict(interface_contract=data,provenance=compiled.json()['contents']['provenance'],
                    il_file_path=compiled.json()['files']['il'],out_c_dir=str(base/'c')))
                self.assertEqual(generated.status_code,200,generated.text)
                content=generated.json()['contents']
                adapted=client.post('/convert/c-to-arduino',json=dict(interface_contract=data,provenance=generated.json()['provenance'],
                    c_code=content['ProgramPage1.c'],compiled_kernel=dict(header=content['define.h'],source=content['ProgramPage1.c']),out_path=str(base/'text.ino')))
                self.assertEqual(adapted.status_code,200,adapted.text)
                code=Path(adapted.json()['saved_path']).read_text(encoding='utf-8')
                self.host_text(base,code,data['interfaces'][0]['binding']['text'])
                target=client.post('/arduino/compile',json=dict(sketch_path=adapted.json()['saved_path'],fqbn=board))
                self.assertEqual(target.status_code,200,target.text);self.assertTrue(target.json()['success'],target.text)

    def host_text(self,base,code,message):
        shim='''#pragma once
#include <stdint.h>
#include <string>
#include <vector>
static uint32_t now=0;
static unsigned long millis(){return now;}
struct SerialStub{std::vector<std::string> lines;std::vector<uint32_t> times;
 void begin(int baud){if(baud!=9600)__builtin_abort();}
 void println(const char* value){lines.push_back(value);times.push_back(now);}};
static SerialStub Serial;
'''
        harness='''\n#include <cassert>
int main(){setup();for(now=0;now<=6010;now+=10)loop();assert(Serial.lines.size()==3);
 for(int i=0;i<3;++i){assert(Serial.lines[i]==MESSAGE);assert(Serial.times[i]==(uint32_t)(2000*(i+1)));}}
'''.replace('MESSAGE',json.dumps(message))
        (base/'Arduino.h').write_text(shim,encoding='utf-8');(base/'test.cpp').write_text(code+harness,encoding='utf-8')
        binary=base/'test.exe'
        build=subprocess.run([shutil.which('g++'),'-std=c++17','-I',str(base),str(base/'test.cpp'),'-o',str(binary)],capture_output=True,text=True)
        self.assertEqual(build.returncode,0,build.stderr)
        run=subprocess.run([str(binary)],capture_output=True,text=True,timeout=10);self.assertEqual(run.returncode,0,run.stderr)


if __name__=='__main__':unittest.main()
