import copy
import unittest

import sysml_assembly_unittest as assembly_tests
from sysml_assembly_unittest import contract
from app.services.sysml_assembly import assemble_sysml


def toggle(initial=True, edge='rising'):
    return dict(kind='edge_toggle', name='run', input='pressed', edge=edge,
                initial=initial, previous_input=False)


def pulse(enabled='component_run_value', period=3000, width=100, first='after_period', pause='preserve'):
    return dict(kind='periodic_pulse', name='beep', delta='delta', enabled=enabled,
                period_ms=period, pulse_ms=width, first_pulse=first, pause=pause)


def controller(components):
    return dict(states=[], updates=[], components=components, outputs={'sounding':'component_beep_value'})


def inventory(button=True):
    result=contract(([('pressed','bool')] if button else [])+[('delta','i32')],[('sounding','bool')])
    next(p for p in result['interfaces'] if p['name']=='delta')['unit']='ms'
    return result


class ComponentTests(unittest.TestCase):
    execute = assembly_tests.AssemblyTests.execute

    def test_json_boolean_gate_matches_existing_literal_semantics(self):
        from app.services.sysml_components import expand_components
        inputs = inventory(False)['interfaces']
        for enabled in (True, False):
            with self.subTest(enabled=enabled):
                self.assertEqual(expand_components(controller([pulse(enabled=enabled)]), inputs),
                                 expand_components(controller([pulse(enabled=str(enabled).lower())]), inputs))
        for invalid in (0, 1, None):
            with self.subTest(invalid=invalid), self.assertRaises(ValueError):
                expand_components(controller([pulse(enabled=invalid)]), inputs)
        for invalid in (True, False):
            item = toggle(); item['input'] = invalid
            with self.subTest(edge_input=invalid), self.assertRaises(ValueError):
                expand_components(controller([item, pulse()]), inventory()['interfaces'])

    def test_json_boolean_gate_executes_through_real_backend(self):
        self.execute(controller([pulse(enabled=True, period=100, width=20, first='immediate')]), inventory(False), '''
int main(){
 ProgramPage1_Init(&ProgramPage1_instance);bool sounding=false;
 ProgramPage1(0,&sounding);assert(sounding);
 ProgramPage1(19,&sounding);assert(sounding);
 ProgramPage1(1,&sounding);assert(!sounding);
 ProgramPage1(80,&sounding);assert(sounding);
}''')

    def test_composed_pause_edge_and_remaining_pulse_against_active_time(self):
        spec=controller([toggle(),pulse()])
        # Oracle accumulates time in a wide integer. It does not implement the
        # generated phase reduction or inspect component storage/generated code.
        self.execute(spec,inventory(),'''
int main(){
 ProgramPage1_Init(&ProgramPage1_instance); bool sounding=false;
 long long active=0; bool running=true, wasPressed=false; int last=0;
 for(int now=0;now<=22000;now+=10){
  bool pressed=(now>=1250&&now<2250)||(now>=3300&&now<3700)||
               (now>=5100&&now<5300)||(now>=5800&&now<6000);
  if(running) active+=now-last;
  if(pressed&&!wasPressed) running=!running;
  ProgramPage1(pressed,now-last,&sounding);
  assert(sounding==(running&&active>=3000&&active%3000<100));
  last=now; wasPressed=pressed;
 }
}''')

    def test_delayed_pulse_handles_jitter_multiple_periods_and_i32_limit(self):
        self.execute(controller([pulse(enabled='true',period=997,width=113)]),inventory(False),'''
int main(){
 ProgramPage1_Init(&ProgramPage1_instance); bool sounding=false; long long active=0;
 int deltas[]={0,113,884,1,112,3100,9,2147483647,17,2147483647,50000003};
 for(int delta:deltas){
  active+=delta; ProgramPage1(delta,&sounding);
  assert(sounding==(active>=997&&active%997<113));
 }
 ProgramPage1_Init(&ProgramPage1_instance); ProgramPage1(0,&sounding); assert(!sounding);
}''')

    def test_immediate_pulse_reset_and_falling_edge_are_explicit(self):
        self.execute(controller([toggle(False,'falling'),pulse(period=100,width=20,first='immediate',pause='reset')]),inventory(),'''
int main(){
 ProgramPage1_Init(&ProgramPage1_instance); bool sounding=true;
 ProgramPage1(false,0,&sounding);assert(!sounding);
 ProgramPage1(true,10,&sounding);assert(!sounding);
 ProgramPage1(false,10,&sounding);assert(sounding); // release enables immediate pulse
 ProgramPage1(false,19,&sounding);assert(sounding);
 ProgramPage1(false,1,&sounding);assert(!sounding);
 ProgramPage1(true,30,&sounding);assert(!sounding);
 ProgramPage1(false,10,&sounding);assert(!sounding); // disable, reset the cycle
 ProgramPage1(true,700,&sounding);assert(!sounding);
 ProgramPage1(false,700,&sounding);assert(sounding); // no paused time accumulated
 ProgramPage1(false,20,&sounding);assert(!sounding);
 ProgramPage1(false,80,&sounding);assert(sounding);
}''')

    def test_max_period_and_full_width_use_exact_integer_arithmetic(self):
        self.execute(controller([pulse(enabled='true',period=2147483647,width=2147483647)]),inventory(False),'''
int main(){
 ProgramPage1_Init(&ProgramPage1_instance);bool sounding=true;
 ProgramPage1(0,&sounding);assert(!sounding);
 ProgramPage1(2147483646,&sounding);assert(!sounding);
 ProgramPage1(1,&sounding);assert(sounding);
 ProgramPage1(2147483647,&sounding);assert(sounding);
}''')

    def test_bad_configuration_order_types_and_manual_writes_are_rejected(self):
        base=controller([toggle(),pulse()])
        variants=[]
        for key,value in [('period_ms',True),('period_ms',0),('pulse_ms',3001),('first_pulse','maybe'),('pause','ignore'),('enabled','missing')]:
            spec=copy.deepcopy(base);spec['components'][1][key]=value;variants.append(spec)
        for key,value in [('input','delta'),('edge','both'),('initial','true')]:
            spec=copy.deepcopy(base);spec['components'][0][key]=value;variants.append(spec)
        spec=copy.deepcopy(base);spec['components'].reverse();variants.append(spec)
        spec=copy.deepcopy(base);spec['components'].append(toggle());variants.append(spec)
        spec=copy.deepcopy(base);spec['updates']=[{'assign':'component_run_value','value':'false'}];variants.append(spec)
        spec=copy.deepcopy(base);spec['outputs']['sounding']='component_beep_started';variants.append(spec)
        spec=copy.deepcopy(base);spec['states']=[{'name':'component_run_value','type':'bool','initial':'false'}];variants.append(spec)
        spec=copy.deepcopy(base);spec['components']=[];spec['updates']=[{'assign':'child.component_run_value','value':'false'}];variants.append(spec)
        for spec in variants:
            with self.subTest(spec=spec),self.assertRaises(ValueError):assemble_sysml(spec,inventory())
        wrong=inventory();next(p for p in wrong['interfaces'] if p['name']=='delta')['unit']='s'
        with self.assertRaisesRegex(ValueError,'unit ms'):assemble_sysml(base,wrong)
        wrong=inventory();next(p for p in wrong['interfaces'] if p['name']=='delta')['binding']={'kind':'analog_input','pin':'A0'}
        with self.assertRaisesRegex(ValueError,'elapsed_ms'):assemble_sysml(base,wrong)
        saved=copy.deepcopy(base);assemble_sysml(base,inventory());self.assertEqual(base,saved)


if __name__=='__main__':unittest.main()
