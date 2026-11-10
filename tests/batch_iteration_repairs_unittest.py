import json
import unittest
from app.services.configuration_policy import apply_configuration, resolve_configuration
from app.services.interface_planner import plan_interface_contract, prepare_interface_contract
from app.services.source_behavior import derive_checks


class IterationRepairTests(unittest.TestCase):
    def test_digital_input_facts_distinguish_initializer_from_first_sample(self):
        from app.services.interface_semantics import adapter_boundary_facts
        proposal={'interface_requests':[dict(kind='digital_input',pin='D2',initial=True,
            meaning='Button released',active_high=False,pullup=True)]}
        contract,_=prepare_interface_contract(proposal,'Button D2 controls a counter',{})
        fact=next(f for f in adapter_boundary_facts(contract) if f['kind']=='digital_acquisition')
        self.assertTrue(fact['initial_model_value'])
        self.assertEqual(fact['model_semantics']['true_level'],'LOW')
        self.assertIn('BEFORE the first controller tick',fact['lifecycle'])
        self.assertIn('not injected as a synthetic button event',fact['lifecycle'])

    def test_temporal_language_supplies_clock_even_when_increment_precedes_period(self):
        from app.services.interface_planner import requires_clock
        for source in ['increment its angle by 30 degrees every 2 seconds',
                       'decrement every 2 seconds until zero','refresh each second','hold every 0.5-second interval']:
            self.assertTrue(requires_clock(source),source)
        self.assertFalse(requires_clock('on each button press update within 250ms'))
        source='increment its angle by 30 degrees every 2 seconds'
        proposal={'interface_requests':[dict(kind='servo_angle',pin='D5',initial=0,meaning='Angle')]}
        contract,_=prepare_interface_contract(proposal,source,{})
        self.assertTrue(any(p['binding']['kind']=='elapsed_ms' for p in contract['interfaces']))

    def test_pwm_facts_expose_scale_before_clamp_including_native_count_units(self):
        from app.services.interface_semantics import adapter_boundary_facts
        contract=dict(board='arduino:avr:mega',interfaces=[dict(name='duty',unit='pwm_count',binding=dict(kind='pwm_output',scale=255,offset=0))])
        facts=adapter_boundary_facts(contract)[0]
        self.assertEqual(facts['unclamped_model_range'],[0,1])
        self.assertIn('model_output * scale',facts['formula'])
        self.assertEqual(min(255,127*facts['scale']+facts['offset']),255)
        contract['interfaces'][0]['binding'].update(scale=-1,offset=255)
        self.assertEqual(adapter_boundary_facts(contract)[0]['unclamped_model_range'],[0,255])

    def test_external_button_bias_does_not_force_internal_pullup(self):
        from app.services.wiring_inventory import circuit_inventory
        for rail,opposite,active in [('5V','GND.1',False),('GND.1','5V',True)]:
            c=dict(parts=[dict(id='uno',type='wokwi-arduino-uno'),dict(id='btn',type='wokwi-pushbutton'),
                dict(id='r',type='wokwi-resistor',attrs={'value':'10000'})],
                connections=[['btn:1.l','uno:4'],['btn:1.r','r:1'],['r:2','uno:'+rail],['btn:2.r','uno:'+opposite]])
            item=next(x for x in circuit_inventory([c],'arduino:avr:uno') if x['device']=='btn')
            self.assertEqual(item['catalog_operations'],[dict(kind='digital_input',pin='D4',active_high=active,pullup=False)])
            # Electrical nets remain separate across the resistor.
            self.assertNotIn('1.l',item['rails'])

    def test_all_equivalent_resistor_literals_are_applied(self):
        circuit=dict(parts=[dict(id='uno',type='wokwi-arduino-uno'),
            dict(id='r1',type='wokwi-resistor',attrs={'value':'220'}),dict(id='r2',type='wokwi-resistor',attrs={'value':'10000'})],connections=[])
        source='set the property name "value" to "220 ohm". set the property name "value" to "10000 ohm". '+json.dumps(circuit)
        result=resolve_configuration(source)
        self.assertEqual([p['attrs']['value'] for p in result['resolved_circuits'][0]['parts'][1:]],['220 ohm','10000 ohm'])

    def test_explicit_resistor_literal_is_applied_without_rewiring_or_resizing(self):
        circuit=dict(parts=[dict(id='uno',type='wokwi-arduino-uno'),dict(id='r',type='wokwi-resistor',attrs={'value':'220'}),
            dict(id='other',type='wokwi-resistor',attrs={'value':'1000'})],connections=[['uno:13','r:1']])
        source='To use 220Ω resistor, set the property name "value" to "220 ohm". Circuit: '+json.dumps(circuit)
        result=resolve_configuration(source)
        actual=result['resolved_circuits'][0]
        self.assertEqual(actual['parts'][1]['attrs']['value'],'220 ohm')
        self.assertEqual(actual['parts'][2]['attrs']['value'],'1000')
        self.assertEqual(actual['connections'],circuit['connections'])
        self.assertEqual(circuit['parts'][1]['attrs']['value'],'220')
        self.assertIn('220 ohm',result['effective_requirement'])

    def test_numeric_serial_domain_is_not_a_256_token_command_dictionary(self):
        source='串口输入 0~255：控制 D9 PWM LED 亮度并回显'
        proposal={'interface_requests':[dict(kind='pwm_output',pin='D9',initial=0,meaning='Brightness'),
            dict(kind='serial_command_input',initial=0,baud=9600,meaning='Numeric input',commands={str(i):i for i in range(256)})]}
        contract,actions=prepare_interface_contract(proposal,source,{})
        receiver=next(p for p in contract['interfaces'] if p['binding']['kind']=='serial_integer_input')
        self.assertEqual((receiver['binding']['min'],receiver['binding']['max']),(0,255))
        self.assertEqual(proposal['interface_requests'][1]['kind'],'serial_command_input')
        self.assertTrue(any('source_numeric_serial' in a for a in actions))

    def test_partial_test_coverage_is_repaired_not_silently_renamed_full(self):
        import tempfile,sys
        from pathlib import Path
        from unittest.mock import patch
        sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
        import design_behavior_tests as designer
        plan=dict(supported=True,coverage='partial')
        full=dict(supported=True,coverage='full')
        review=dict(approved=True,coverage='partial',coverage_gaps=['Add setup deadline'])
        complete=dict(approved=True,coverage='full',coverage_gaps=[])
        with tempfile.TemporaryDirectory() as td,patch.object(designer,'OUT',Path(td)),patch.object(designer,'validate'),patch.object(
                designer,'call',side_effect=[plan,review,full,complete]) as call:
            result=designer.one(dict(id='fixture',effective_requirement='setup within 500ms'))
            self.assertEqual(result['coverage'],'full')
            self.assertIn('Coverage gaps',call.call_args_list[2].args[-1])
        with tempfile.TemporaryDirectory() as td,patch.object(designer,'OUT',Path(td)),patch.object(designer,'validate'),patch.object(
                designer,'call',side_effect=[plan,review,ValueError('bad revision'),ValueError('bad revision')]):
            result=designer.one(dict(id='fixture',effective_requirement='setup within 500ms'))
            self.assertEqual(result['coverage'],'partial')

    def test_contract_validation_failure_routes_to_separate_repair_service(self):
        from unittest.mock import Mock
        good={'interface_requests':[dict(kind='digital_output',pin='D13',initial=False,meaning='LED')]}
        initial=Mock(return_value=('initial',json.dumps(dict(good,serial_channels=['wrong']))))
        repair=Mock(return_value=('repair',json.dumps(good)))
        _,report=plan_interface_contract(initial,'LED D13 on',{},max_repairs=1,repair_call=repair)
        self.assertEqual(initial.call_count,1)
        self.assertEqual(repair.call_count,1)
        self.assertEqual([a['model'] for a in report['attempts']],['initial','repair'])

    def test_compact_role_pin_pairs_do_not_create_phantom_conflicts(self):
        for source in ['D2 button D13 LED','button D2 LED D13','D2按钮 D13指示灯']:
            result=resolve_configuration(source)
            self.assertFalse(any(c.get('original')=='D13' and c.get('resolved')!='D13' for c in result['changes']),result)
            inputs=[d['resolved']['pin'] for d in result['devices'] if d['kind']=='digital_input']
            self.assertEqual(inputs,['D2'])

    def test_avr_adc_range_and_calibration_are_explicit_review_evidence(self):
        from app.services.interface_semantics import adapter_boundary_facts
        from app.services.sysml_semantic_review import build_review_context
        contract=dict(board='arduino:avr:uno',interfaces=[dict(name='pot',binding=dict(kind='analog_input',pin='A0'))])
        facts=adapter_boundary_facts(contract)
        self.assertEqual(facts[0]['model_range'],[0,1023])
        context=build_review_context(original_requirement='Map potentiometer to display',contract=contract,system_sysml2='package P {}')
        self.assertEqual(context['adapter_boundary_facts'],facts)
        contract['interfaces'][0]['binding'].update(scale=-2,offset=10)
        self.assertEqual(adapter_boundary_facts(contract)[0]['model_range'],[-2036,10])
        contract['board']='unknown'
        self.assertEqual(adapter_boundary_facts(contract),[])

    def test_malformed_channels_are_repairable_validation_errors(self):
        resolution=resolve_configuration('Report button state over serial.')
        for value in ['bad', ['bad']]:
            with self.subTest(value=value),self.assertRaisesRegex(ValueError,'serial_channels'):
                apply_configuration({'interface_requests':[],'serial_channels':value},resolution)

    def test_exact_conditional_messages_are_real_text_gates(self):
        source='Continuously check the button value, and print "Button pressed" if the value is HIGH, or print "Button not pressed" if the value is LOW.'
        proposal={'interface_requests':[dict(kind='digital_input',pin='D4',initial=False,meaning='Button')],
                  'serial_channels':[dict(baud=9600,trigger=dict(initial=False,meaning='Report'),values=[])]}
        contract,actions=prepare_interface_contract(proposal,source,{})
        messages=[p['binding']['text'] for p in contract['interfaces'] if p['binding']['kind']=='serial_text_output']
        self.assertEqual(messages,['Button pressed','Button not pressed'])
        self.assertNotIn('serial_text_output',[r['kind'] for r in proposal['interface_requests']])
        self.assertEqual(derive_checks(contract,source)[0]['kind'],'button_text_state')
        self.assertTrue(any('source_binary_messages' in action for action in actions))

    def test_bad_channel_shape_enters_planner_retry_instead_of_attribute_error(self):
        proposal={'interface_requests':[dict(kind='digital_output',pin='D13',initial=False,meaning='LED')]}
        bad=dict(proposal,serial_channels=['wrong'])
        answers=iter([bad,proposal]);prompts=[]
        def call(prompt):
            prompts.append(str(prompt));return 'fixture',json.dumps(next(answers))
        _,report=plan_interface_contract(call,'LED D13 on',{},max_repairs=1)
        self.assertEqual([r['ok'] for r in report['attempts']],[False,True])
        self.assertIn('serial_channels[0] must be an object',prompts[1])


if __name__=='__main__':unittest.main()
