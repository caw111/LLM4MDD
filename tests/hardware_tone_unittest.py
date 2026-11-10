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
from app.services.behavior_contract import check_contract
from app.services.sysml_compiler import compile_sysml
from app.services.interface_planner import check_explicit_requirements
from app.services.sysml import sysml_bundle_prompt
from app.services.sysml_assembly import assemble_sysml


def inventory():
    return json.loads(Path('tools/examples/periodic_tone.contract.json').read_text(encoding='utf-8'))


class ToneTests(unittest.TestCase):
    def test_pause_example_preserves_period_through_sound_and_pause(self):
        contract=inventory()
        contract['interfaces'].append(dict(name='pressed',direction='input',type='bool',unit='1',meaning='Button active',
            sample_period_ms=10,initial=False,device='button',hardware='D2',binding=dict(kind='digital_input',pin=2,pullup=True,active_high=False)))
        prompt=sysml_bundle_prompt('Pause/resume periodic sound on a press edge',contract=contract)
        self.assertIn('advance the period counter throughout BOTH',prompt)
        self.assertNotIn('reset-on-start timing',prompt)
        block=Path('tools/prompts/contract_controller.prompt.md').read_text(encoding='utf-8').split('<!-- example:paused_pulse -->')[1]
        controller=json.JSONDecoder().raw_decode(block[block.index('{"states"'):])[0]
        ir=compile_sysml(assemble_sysml(controller,contract))
        with tempfile.TemporaryDirectory() as folder:
            base=Path(folder);header,source=lower(ir,base/'lowered')
            _,sketch=generate_adapter(lambda _:self.fail('No LLM'),header,source,'',{},contract=contract)
            result,log=compile_code(base/'check',sketch,CLI,contract['board'])
            self.assertTrue(result['compile_ok'],log)
            scored=behavior(base/'check',29)
            self.assertEqual(scored['functional_status'],'passed',(base/'check/host_oracle/trace.log').read_text())

    def test_periodic_sound_prompt_explains_initial_silence(self):
        prompt=sysml_bundle_prompt('Emit periodic short beeps',contract=inventory())
        self.assertIn('FIRST scheduled pulse',prompt)
        self.assertNotIn('output lamp toggling every',prompt)
        self.assertIn('Adapt the names, period, duration and pitch',prompt)

    def test_explicit_passive_buzzer_cannot_be_replaced_with_digital_output(self):
        contract=inventory()
        check_explicit_requirements(contract,'Use a passive buzzer on D6',{})
        output=contract['interfaces'][1]
        output.update(type='bool',unit='1',initial=False)
        output['binding']={'kind':'digital_output','pin':6}
        validate_hardware_contract(contract)
        with self.assertRaisesRegex(ValueError,'passive buzzer'):
            check_explicit_requirements(contract,'Use a passive buzzer on D6',{})

    def test_contract_rejects_wrong_types_durations_and_timer_conflicts(self):
        validate_hardware_contract(inventory())
        for field,value in [('type','bool'),('unit','1'),('initial',1)]:
            contract=inventory();contract['interfaces'][1][field]=value
            with self.subTest(field=field),self.assertRaises(ValueError):validate_hardware_contract(contract)
        contract=inventory();contract['interfaces'][1]['binding']['duration_ms']=100
        with self.assertRaisesRegex(ValueError,'unknown fields'):validate_hardware_contract(contract)
        contract=inventory();contract['interfaces'][1]['binding']['invalid_policy']='clamp'
        with self.assertRaisesRegex(ValueError,'silence'):validate_hardware_contract(contract)
        contract=inventory();second=copy.deepcopy(contract['interfaces'][1]);second.update(name='second',hardware='D7');second['binding']['pin']=7
        contract['interfaces'].append(second)
        with self.assertRaisesRegex(ValueError,'one simultaneous'):validate_hardware_contract(contract)
        for board,pins,allowed in [('arduino:avr:uno',[3,11],9),('arduino:avr:mega',[9,10],44)]:
            for pin in [*pins,allowed]:
                contract=inventory();contract['board']=board
                output=copy.deepcopy(contract['interfaces'][1]);output.update(name='brightness',type='f64',unit='pwm_count',initial=0.0)
                output['binding']={'kind':'pwm_output','pin':pin};contract['interfaces'].append(output)
                with self.subTest(board=board,pin=pin):
                    if pin in pins:
                        with self.assertRaisesRegex(ValueError,'Timer2'):validate_hardware_contract(contract)
                    else:validate_hardware_contract(contract)

    def test_real_sysml_backend_board_compilation_and_independent_tone_trace(self):
        contract=inventory()
        ir=compile_sysml(Path('tools/sysml/periodic_tone.sysml').read_text(encoding='utf-8'))
        self.assertEqual(check_contract(ir,contract)['issues'],[])
        with tempfile.TemporaryDirectory() as folder:
            base=Path(folder);header,source=lower(ir,base/'lowered')
            def no_llm(_):self.fail('Hardware adaptation must not call LLM')
            for board in ('arduino:avr:uno','arduino:avr:mega'):
                contract['board']=board
                _,sketch=generate_adapter(no_llm,header,source,'requirements',{},contract=contract)
                target=base/board.split(':')[-1]
                result,log=compile_code(target,sketch,CLI,board)
                self.assertTrue(result['compile_ok'],log)
                scored=behavior(target,21,board)
                self.assertEqual(scored['functional_status'],'passed',(target/'host_oracle/trace.log').read_text() if (target/'host_oracle/trace.log').exists() else scored)

    def test_pitch_changes_stops_invalid_commands_and_no_retrigger(self):
        header='void ProgramPage1(int deltaMs, int* frequency);'
        adapter=generate_hardware_adapter(header,inventory())
        with tempfile.TemporaryDirectory() as folder:
            base=Path(folder)
            shutil.copyfile('experiment/development/oracles/gpio_arduino.h',base/'Arduino.h')
            kernel='''#include <Arduino.h>
int ProgramPage1_instance=0, command=0;
void ProgramPage1_Init(int*){}
void ProgramPage1(int,int* frequency){*frequency=command;}
'''
            trace='''
#include <cassert>
int main(){setup();assert(oracle::tones.back().frequency==0);
 for(auto wanted:{1000,1000,2000,0,30,70000,65535,-1}){
  auto count=oracle::tones.size();auto old=oracle::tones.back().frequency;
  command=wanted;oracle::now+=10;loop();
  unsigned int expected=wanted>=31 && wanted<=65535?wanted:0;
  assert(oracle::tones.back().frequency==expected);
  assert(oracle::tones.size()==count+(old!=expected));
 }
}
'''
            source=base/'test.cpp';source.write_text(kernel+adapter+trace,encoding='utf-8')
            binary=base/'test.exe'
            build=subprocess.run([shutil.which('g++'),'-std=c++17','-I',str(base),str(source),'-o',str(binary)],capture_output=True,text=True)
            self.assertEqual(build.returncode,0,build.stderr)
            run=subprocess.run([str(binary)],capture_output=True,text=True)
            self.assertEqual(run.returncode,0,run.stderr)


if __name__=='__main__':unittest.main()
