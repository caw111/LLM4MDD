import copy
import unittest

from source_behavior_unittest import proposal, lower
from app.services.interface_planner import prepare_interface_contract, add_public_behavior_checks, check_explicit_requirements
from app.services.source_behavior import add_source_checks, check_source_behavior


TEXT = '''Continuously read the DHT sensor's humidity value.
Print humidity to the serial monitor in the following format: "Humidity: {humidity value}"'''


def sensor_contract():
    p=proposal([dict(kind='dht_humidity',pin='D2',sensor='DHT22',initial=0,meaning='Humidity'),
                dict(kind='elapsed_ms',initial=0,meaning='Real delta')])
    p['serial_channels']=[dict(baud=9600,trigger=dict(initial=False,meaning='Measurement report'),
        values=[dict(type='f64',unit='%RH',initial=0,meaning='Humidity',label='Humidity: ')])]
    c,_=prepare_interface_contract(p,TEXT,{})
    c['requirements']=[dict(id='R1',text=TEXT,assertion_ids=['source_requirement_R1'])]
    return add_public_behavior_checks(c,TEXT)


class SourceBehaviorRound3Tests(unittest.TestCase):
    def test_sensor_changes_reject_untouched_cache_but_accept_actual_measurement(self):
        c=sensor_contract()
        for mode in ['untouched','once','fresh']:
            updates=[] if mode=='untouched' else [dict(assign='cachedHumidity',value='io_0')]
            if mode=='once':
                updates=[{'if':'not initialized','then':updates+[dict(assign='initialized',value=True)]}]
            model=dict(states=[dict(name='cachedHumidity',type='f64',initial=0.0),
                               dict(name='initialized',type='bool',initial=False)],
                updates=updates,
                outputs={'serial_0_trigger':'true','serial_0_value_0':'cachedHumidity'})
            report,issues=check_source_behavior(lower(c,model),c)
            self.assertEqual(report['status'],'passed' if mode=='fresh' else 'failed',report)
            if mode=='untouched':self.assertEqual(report['counterexample']['trace'][0]['last_reports'],[0.0]*5)
            if mode=='once':self.assertEqual(report['counterexample']['trace'][-1]['last_reports'],[c['source_behavior_checks'][0]['samples'][0]]*5)

    def test_absent_sensor_report_is_unverified_without_inventing_response_deadline(self):
        c=sensor_contract()
        model=dict(states=[],updates=[],outputs={'serial_0_trigger':'false','serial_0_value_0':'io_0'})
        report,issues=check_source_behavior(lower(c,model),c)
        self.assertEqual(report['status'],'incomplete');self.assertEqual(issues,[])

    def test_sensor_prefix_preserves_spaces_and_punctuation(self):
        c=sensor_contract();c.pop('source_behavior_checks')
        p=next(p for p in c['interfaces'] if p['binding']['kind']=='serial_value')
        for prefix in ['humidity','Humidity:','Humidity ']:
            p['binding']['label']=prefix
            with self.subTest(prefix=prefix),self.assertRaisesRegex(ValueError,'SOURCE_SENSOR_FORMAT'):
                add_source_checks(c,TEXT)

    def test_report_unit_cannot_substitute_temperature_for_humidity(self):
        c=sensor_contract();c.pop('source_behavior_checks')
        next(p for p in c['interfaces'] if p['binding']['kind']=='serial_value')['unit']='degC'
        with self.assertRaisesRegex(ValueError,'matching-unit'):
            add_source_checks(c,TEXT)

    def test_conditional_alarm_literal_is_checked_at_shared_contract_boundary(self):
        p=proposal([dict(kind='serial_text_output',baud=9600,text='wrong',initial=False,meaning='alarm')])
        requirement='若温度过高，则报警并串口输出 fire alert。'
        c,_=prepare_interface_contract(p,requirement,{})
        with self.assertRaisesRegex(ValueError,'SOURCE_MESSAGE'):
            check_explicit_requirements(c,requirement,{})
        c['interfaces'][0]['binding']['text']='fire alert'
        check_explicit_requirements(c,requirement,{})

    def test_negated_messages_and_format_names_are_not_required_literals(self):
        p=proposal([dict(kind='digital_output',pin='D13',initial=False,meaning='LED')])
        c,_=prepare_interface_contract(p,'LED',{})
        for text in ['无需串口输出 fire alert。','禁止串口输出 fire alert。','串口输出 JSON 格式数据。']:
            check_explicit_requirements(c,text,{})


if __name__=='__main__':
    unittest.main()
