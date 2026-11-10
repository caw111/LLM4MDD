import copy
import sys
import tempfile
import unittest
from unittest.mock import patch
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
import behavior_full_host as host


class CommandCheckTests(unittest.TestCase):
    def test_logically_simultaneous_phase_outputs_do_not_require_atomic_pin_order(self):
        check=dict(kind='event_order',id='completion',requirement_quote='when target reached',
            first=dict(source='servo',pin=6,expected=0),second=dict(source='gpio',pin=4,expected=1),max_gap_ms=250)
        trace=[dict(type='servo',pin=6,operation='write',active=True,angle=0,at_ms=1000,sequence=2),
            dict(type='gpio',pin=4,value=1,pwm=False,at_ms=1000,sequence=1),self.execution()]
        plan=dict(horizon_ms=6000,checks=[check])
        self.assertEqual(host.evaluate(plan,trace,'arduino:avr:uno')['status'],'failed')
        plan['checks']=[dict(check,allow_same_ms=True)]
        self.assertEqual(host.evaluate(plan,trace,'arduino:avr:uno')['status'],'passed')
        trace[1]['at_ms']=3000
        self.assertEqual(host.evaluate(plan,trace,'arduino:avr:uno')['status'],'failed')

    def test_real_execution_checks_exact_message_before_first_activation(self):
        with tempfile.TemporaryDirectory() as td,patch.object(host,'OUT',Path(td)),patch.object(host,'FIXTURE',Path(td)/'fixture'):
            host.fixture()
            plan=dict(horizon_ms=1000,checks=[dict(kind='event_order',id='order',requirement_quote='message then LED',
                first=dict(source='serial',regex='^Ready$',baud=9600),second=dict(source='gpio',pin=13,expected=1)),
                dict(kind='serial',id='exact',requirement_quote='Ready',regex='^Ready$',min_count=1)])
            host.case_files('order',plan,'arduino:avr:uno')
            for name,early,payload,status in [('good','','Ready','passed'),('early','digitalWrite(13,HIGH);','Ready','failed'),('suffix','','Ready0','failed')]:
                source=Path(td)/(name+'.cpp')
                source.write_text('#include <Arduino.h>\nvoid setup(){Serial.begin(9600);pinMode(13,OUTPUT);'+early+
                    'Serial.println("'+payload+'");digitalWrite(13,HIGH);}\nvoid loop(){delay(1);}')
                result=host.execute(source,'arduino:avr:uno','order',plan,Path(td)/name,True)
                self.assertEqual(result['status'],status,result)

    def test_event_window_covers_actual_phase_and_rejects_missing_outputs_or_phase(self):
        check=dict(kind='event_window',id='phase',requirement_quote='movement indicator',
            start=dict(source='servo',pin=5,expected=30,occurrence=1,tolerance=1),
            end=dict(source='servo',pin=5,expected=180,tolerance=1),start_delay_ms=250,end_margin_ms=1,
            conditions=[dict(source='gpio',pin=2,expected=1)])
        trace=[dict(type='reset_state',at_ms=0),
            dict(type='servo',pin=5,angle=30,operation='write',active=True,at_ms=1000,sequence=1),
            dict(type='gpio',pin=2,value=1,pwm=False,at_ms=1000,sequence=2),
            dict(type='servo',pin=5,angle=150,operation='write',active=True,at_ms=2000,sequence=3),
            dict(type='gpio',pin=2,value=0,pwm=False,at_ms=3000,sequence=4),
            dict(type='servo',pin=5,angle=180,operation='write',active=True,at_ms=3000,sequence=5),self.execution()]
        plan=dict(horizon_ms=6000,checks=[check])
        import design_behavior_tests as design
        design.validate(dict(plan,supported=True,coverage='full',reference_cpp='void setup(){}void loop(){}'),'movement indicator')
        bad_plan=copy.deepcopy(dict(plan,supported=True,coverage='full',reference_cpp='void setup(){}void loop(){}'))
        bad_plan['checks'][0]['start']['occurrence']=0
        with self.assertRaises(AssertionError):design.validate(bad_plan,'movement indicator')
        self.assertEqual(host.evaluate(plan,trace,'arduino:avr:uno')['status'],'passed')
        latency=dict(kind='event_order',id='latency',requirement_quote='two seconds',
            first=check['start'],second=check['end'],min_gap_ms=1900,max_gap_ms=2100)
        self.assertEqual(host.evaluate(dict(plan,checks=[latency]),trace,'arduino:avr:uno')['status'],'passed')
        self.assertEqual(host.evaluate(dict(plan,checks=[dict(latency,max_gap_ms=1500)]),trace,'arduino:avr:uno')['status'],'failed')
        bad=copy.deepcopy(trace);bad[2]['value']=0
        self.assertEqual(host.evaluate(plan,bad,'arduino:avr:uno')['status'],'failed')
        self.assertEqual(host.evaluate(plan,trace[:-2]+[self.execution()],'arduino:avr:uno')['status'],'failed')
        bad=copy.deepcopy(trace);bad[4]['at_ms']=2000
        self.assertEqual(host.evaluate(plan,bad,'arduino:avr:uno')['status'],'failed')

    def test_combined_initial_state_and_continuous_guard_catch_transient_errors(self):
        condition=lambda source,p,value:dict(source=source,pin=p,expected=value,tolerance=1 if source=='servo' else 0)
        witness=dict(kind='state_witness',id='initial',requirement_quote='initialize',from_ms=0,to_ms=500,
            conditions=[condition('gpio',2,0),condition('servo',5,0),condition('servo',6,0)])
        invariant=dict(kind='state_invariant',id='hold',requirement_quote='hold',from_ms=0,to_ms=6000,
            when=[condition('gpio',2,1)],conditions=[condition('servo',5,180),condition('servo',6,0)])
        def event(kind,p,value,t,seq):
            return dict(type=kind,pin=p,value=value,angle=value,at_ms=t,sequence=seq,active=True,operation='write',pwm=False)
        trace=[dict(type='reset_state',gpio_latch=0,at_ms=0),event('servo',5,0,0,1),event('servo',6,0,0,2),
            event('servo',5,30,0,3),event('servo',5,180,1000,4),event('gpio',2,1,1000,5),self.execution()]
        plan=dict(horizon_ms=6000,checks=[witness,invariant])
        self.assertEqual(host.evaluate(plan,trace,'arduino:avr:uno')['status'],'passed')
        bad=copy.deepcopy(trace);bad[2]['sequence']=4;bad[3]['sequence']=2
        self.assertEqual(host.evaluate(plan,bad,'arduino:avr:uno')['status'],'failed')
        bad=trace[:-1]+[event('servo',6,30,2000,6),event('servo',6,0,2001,7),self.execution()]
        self.assertEqual(host.evaluate(plan,bad,'arduino:avr:uno')['status'],'failed')
        absent=copy.deepcopy(trace);absent[5]['value']=0
        self.assertEqual(host.evaluate(plan,absent,'arduino:avr:uno')['status'],'failed')
        delayed=copy.deepcopy(trace);delayed[4]['angle']=0
        plan['checks']=[dict(invariant,settle_ms=250)]
        self.assertEqual(host.evaluate(plan,delayed,'arduino:avr:uno')['status'],'failed')

    def test_real_host_execution_distinguishes_attach_and_write_commands(self):
        with tempfile.TemporaryDirectory() as td,patch.object(host,'OUT',Path(td)),patch.object(host,'FIXTURE',Path(td)/'fixture'):
            host.fixture()
            plan=dict(horizon_ms=4000,input_steps=[],serial_inputs=[],checks=[dict(kind='command_sequence',id='seq',
                requirement_quote='initialize then move',source='servo',pin=5,values=[0,30,60],tolerance=1,
                min_intervals_ms=[0,1900],max_intervals_ms=[500,2100]),
                dict(kind='state_witness',id='init',requirement_quote='initialize',to_ms=500,
                     conditions=[dict(source='servo',pin=5,expected=0,tolerance=1),dict(source='gpio',pin=2,expected=0)])])
            host.case_files('fixture',plan,'arduino:avr:uno')
            source=Path(td)/'sketch.cpp'
            source.write_text('#include <Arduino.h>\n#include <Servo.h>\nServo s;bool done=false;void setup(){s.attach(5);s.write(0);s.write(30);}\nvoid loop(){if(!done&&millis()>=2000){s.write(60);done=true;}delay(1);}')
            result=host.execute(source,'arduino:avr:uno','fixture',plan,Path(td)/'execution',True)
            self.assertEqual(result['status'],'passed',result)
            import json
            events=[json.loads(line) for line in (Path(td)/'execution/trace.jsonl').read_text().splitlines()]
            self.assertEqual([e['operation'] for e in events if e['type']=='servo'][:3],['attach','write','write'])

    def execution(self):return dict(type='execution',status='completed',setup_returned=True,setup_at_ms=100,virtual_ms=6000)

    def test_initial_command_and_steps_are_checked_without_requiring_setup_state_retention(self):
        check=dict(kind='command_sequence',id='sweep',requirement_quote='initialize at zero then step',
            source='servo',pin=5,values=[0,30,60],tolerance=1,min_intervals_ms=[0,1900],max_intervals_ms=[500,2100])
        plan=dict(horizon_ms=6000,checks=[check])
        trace=[dict(type='servo',pin=5,active=True,operation=op,angle=value,at_ms=at) for op,value,at in
            [('attach',92.7,0),('write',0,0),('write',29.97,0),('write',29.97,1000),('write',59.94,2000)]]+[self.execution()]
        self.assertEqual(host.evaluate(plan,trace,'arduino:avr:uno')['status'],'passed')
        wrong=copy.deepcopy(trace);wrong[1]['angle']=180
        self.assertEqual(host.evaluate(plan,wrong,'arduino:avr:uno')['status'],'failed')
        wrong=copy.deepcopy(trace);wrong[-2]['at_ms']=1000
        self.assertEqual(host.evaluate(plan,wrong,'arduino:avr:uno')['status'],'failed')
        missing=copy.deepcopy(trace)
        for e in missing:
            e.pop('operation',None)
        self.assertEqual(host.evaluate(plan,missing,'arduino:avr:uno')['status'],'unverified_window')

    def test_bargraph_count_does_not_impose_segment_order(self):
        plan=dict(horizon_ms=6000,checks=[dict(kind='gpio_count',id='count',requirement_quote='two segments',
            pins=[4,5,6,7],at_ms=1000,expected=2)])
        for levels in [[1,1,0,0],[0,0,1,1],[1,0,1,0]]:
            trace=[dict(type='gpio',pin=p,value=v,pwm=False,at_ms=0) for p,v in zip([4,5,6,7],levels)]+[self.execution()]
            self.assertEqual(host.evaluate(plan,trace,'arduino:avr:uno')['status'],'passed')
        trace=[dict(type='gpio',pin=p,value=1,pwm=False,at_ms=0) for p in [4,5,6,7]]+[self.execution()]
        self.assertEqual(host.evaluate(plan,trace,'arduino:avr:uno')['status'],'failed')


if __name__=='__main__':unittest.main()
