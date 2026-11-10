import unittest
from unittest.mock import patch
from app.services.interface_planner import prepare_interface_contract
from app.services.source_behavior import add_source_checks, check_source_behavior


START='1. Initialize serial communication\n2. Send a message "Ready" to the serial monitor\n3. Begin blinking the LED'


class RoundSixTests(unittest.TestCase):
    def startup_contract(self,initial=False):
        proposal=dict(interface_requests=[dict(kind='digital_output',pin='D13',initial=initial,meaning='LED')],
            serial_channels=[dict(baud=9600,trigger=dict(initial=False,meaning='Startup'),
                values=[dict(type='i32',unit='1',initial=0,meaning='Unused number',label='Ready')])])
        c,_=prepare_interface_contract(proposal,START,{})
        self.assertEqual(proposal['serial_channels'][0]['values'][0]['label'],'Ready')
        self.assertFalse(c.get('serial_channels'))
        self.assertEqual([p['binding']['text'] for p in c['interfaces'] if p['binding']['kind']=='serial_text_output'],['Ready'])
        return add_source_checks(c,START)

    def test_fixed_text_contract_and_startup_sequence_detect_early_activation(self):
        c=self.startup_contract()
        gate=next(p['name'] for p in c['interfaces'] if p['binding']['kind']=='serial_text_output')
        for early in [False,True]:
            class Engine:
                n=0
                def step(self,inputs):
                    self.n+=1
                    return {'io_0':early or self.n>1,gate:self.n==1}
            with self.subTest(early=early),patch('app.services.ir_execution.IRExecution',return_value=Engine()):
                report,_=check_source_behavior({},c)
                self.assertEqual(report['status'],'failed' if early else 'passed')

    def test_setup_activation_is_rejected_before_any_model_tick(self):
        with self.assertRaisesRegex(ValueError,'SOURCE_STARTUP'):
            self.startup_contract(initial=True)

    def test_timed_debounce_is_allowed_but_overcount_and_excess_delay_are_rejected(self):
        source='Second Press: The next two LEDs light up. Sixth Press: All LEDs turn off'
        c,_=prepare_interface_contract(dict(control_period_ms=5,interface_requests=[
            dict(kind='led_bar_graph',pins=['D'+str(p) for p in range(2,12)],initial=False,meaning='Bar'),
            dict(kind='digital_input',pin='D12',initial=False,meaning='Button'),dict(kind='elapsed_ms',initial=0,meaning='Real elapsed time')]),source,{})
        c=add_source_checks(c,source)
        outputs=[p['name'] for p in c['interfaces'] if p['direction']=='output']
        button=next(p['name'] for p in c['interfaces'] if p['binding']['kind']=='digital_input')
        clock=next(p['name'] for p in c['interfaces'] if p['binding']['kind']=='elapsed_ms')
        for delay,increment,expected in [(20,1,'passed'),(20,2,'failed'),(300,1,'failed')]:
            class Engine:
                previous=False;stable=False;elapsed=0;count=0
                def step(self,inputs):
                    raw=inputs[button]
                    self.elapsed=self.elapsed+inputs[clock] if raw==self.previous else 0
                    self.previous=raw
                    if self.elapsed>=delay and raw!=self.stable:
                        self.stable=raw
                        if raw:self.count=(self.count+increment)%6
                    return {name:i<self.count*2 for i,name in enumerate(outputs)}
            with self.subTest(delay=delay,increment=increment),patch('app.services.ir_execution.IRExecution',return_value=Engine()):
                report,_=check_source_behavior({},c)
                self.assertEqual(report['status'],expected,report)


if __name__=='__main__':unittest.main()
