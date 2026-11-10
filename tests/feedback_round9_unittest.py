import unittest
from unittest.mock import patch
from app.services.source_behavior import check_source_behavior
from app.services.interface_planner import prepare_interface_contract, add_public_behavior_checks


class SensorCadenceTests(unittest.TestCase):
    def test_explicit_read_and_report_period_reaches_measurement_and_validity(self):
        source='每 5 秒读取 DHT11（D7）温度并串口输出'
        proposal=dict(interface_requests=[dict(kind=kind,pin='D7',sensor='DHT11',initial=initial,
            meaning=kind,sample_period_ms=2000) for kind,initial in [('dht_temperature',0),('dht_valid',False)]])
        contract,actions=prepare_interface_contract(proposal,source,{})
        self.assertEqual([p['sample_period_ms'] for p in contract['interfaces'] if p['binding']['kind'].startswith('dht_')],[5000,5000])
        self.assertTrue(any('source_sensor_period' in action for action in actions))
        self.assertEqual(proposal['interface_requests'][0]['sample_period_ms'],2000)

    def test_exact_fixed_message_boundary_is_assembled_without_numeric_placeholder(self):
        source='每 2 秒向串口监视器输出一次 Hello Arduino。'
        proposal=dict(interface_requests=[dict(kind='elapsed_ms',initial=0,meaning='Delta')],
            serial_channels=[dict(baud=9600,trigger=dict(initial=False,meaning='Gate'),values=[])])
        contract,actions=prepare_interface_contract(proposal,source,{})
        outputs=[p for p in contract['interfaces'] if p['direction']=='output']
        self.assertEqual(len(outputs),1)
        self.assertEqual(outputs[0]['binding'],dict(kind='serial_text_output',baud=9600,text='Hello Arduino'))
        self.assertEqual(proposal['serial_channels'][0]['values'],[])
        self.assertTrue(any('source_fixed_message' in action for action in actions))
        self.assertEqual(add_public_behavior_checks(contract,source)['source_behavior_checks'][0]['period_ms'],2000)

    def test_valid_sensor_cadence_is_exercised_and_silent_controller_still_fails(self):
        checks=[dict(kind='periodic_report',output='report',delta='dt',period_ms=100,
                     evidence='periodic sensor report')]
        contract=dict(control_period_ms=10,interfaces=[dict(name='valid',direction='input',
            initial=False,binding=dict(kind='dht_valid'))])
        observed=[]
        class Controller:
            def __init__(self,ir):self.elapsed=0;self.silent=ir['silent']
            def step(self,inputs):
                observed.append(inputs['valid'])
                self.elapsed+=inputs['dt']
                due=self.elapsed>=100
                if due:self.elapsed-=100
                return {'report':due and inputs['valid'] and not self.silent}
        with patch('app.services.source_behavior.validate_source_checks',return_value=checks),patch(
                'app.services.ir_execution.IRExecution',Controller):
            good,issues=check_source_behavior({'silent':False},contract)
            bad,errors=check_source_behavior({'silent':True},contract)
        self.assertEqual(good['status'],'passed',good)
        self.assertEqual(bad['status'],'failed',bad)
        self.assertTrue(observed and all(observed))
        self.assertIs(contract['interfaces'][0]['initial'],False)


if __name__=='__main__':unittest.main()
