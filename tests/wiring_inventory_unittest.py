import json
import unittest
import tempfile
from pathlib import Path
from unittest.mock import patch

from app.services.wiring_inventory import circuit_inventory
from app.services.interface_planner import plan_interface_contract, check_explicit_requirements, prepare_interface_contract
from app.services.interface_registry import normalize_interface_proposal


def circuit():
    return dict(parts=[dict(id='mega', type='wokwi-arduino-mega'),
                       dict(id='bargraph1', type='wokwi-led-bar-graph'),
                       dict(id='pot1', type='wokwi-potentiometer')],
                connections=[[f'bargraph1:A{i}', f'mega:{i+1}'] for i in range(1, 11)] +
                            [[f'bargraph1:C{i}', 'mega:GND.1'] for i in range(1, 11)] + [['pot1:SIG', 'mega:A0']])


class WiringInventoryTests(unittest.TestCase):
    def test_component_segment_labels_are_not_mcu_analog_pins(self):
        requirement = 'The bar graph displays segments (PIN A1-A10). Potentiometer on mega:A0. Circuit: ' + json.dumps(circuit())
        proposal = dict(board='arduino:avr:mega', interface_requests=[
            dict(kind='led_bar_graph', device='bargraph1', pins=['A'+str(i) for i in range(1,11)], initial=False, meaning='Segments'),
            dict(kind='analog_input', device='pot1', pin='A0', initial=0, meaning='ADC')])
        prompts = []
        def call(prompt):
            prompts.append(prompt)
            return 'fixture', json.dumps(proposal)
        contract, report = plan_interface_contract(call, requirement, {}, target_board='arduino:avr:mega', max_repairs=0)
        self.assertEqual([p['binding']['pin'] for p in contract['interfaces'][:10]], ['D'+str(i) for i in range(2,12)])
        check_explicit_requirements(contract, requirement, {})
        self.assertIn('Program-extracted per-component electrical facts', prompts[0])
        self.assertTrue(any('verified circuit fact' in a for a in report['attempts'][0]['construction']))
        contract['interfaces'].pop(0)
        with self.assertRaisesRegex(ValueError, 'D2'):
            check_explicit_requirements(contract, requirement, {})

    def test_colon_and_led_only_segment_wording_keep_real_analog_inputs(self):
        from app.services.interface_planner import _declared_pins, _wiring_sources
        for wording in ('LED lights from pin A1 to A10.', 'LED bar graph(pin:A1 to pin:A10).',
                        'All LEDs in even positions lit (A0 A2 ....).',
                        'The graph (bargraph1). (pins:A1-A10)',
                        'LED bar graph from A1 to A8.\nAfter reaching A8, restart from A1.',
                        'Bar graph brightness follows its analog value (0-1023). (from pin:A1 to A10)'):
            text, circuits = _wiring_sources(wording + ' Potentiometer on A0. ' + json.dumps(circuit()), {})
            pins = _declared_pins(text, circuits)
            self.assertEqual(pins, {'A0'} | {'D'+str(i) for i in range(2,12)})

    def test_numeric_neutral_option_can_be_removed_but_inversion_cannot(self):
        request = dict(interface_requests=[dict(kind='analog_input', pin='A0', initial=0, meaning='ADC', active_high=True)])
        normalized, _ = normalize_interface_proposal(request, repair_catalog_fields=True)
        self.assertNotIn('active_high', normalized['interface_requests'][0])
        request['interface_requests'][0]['active_high'] = False
        with self.assertRaises(ValueError):
            normalize_interface_proposal(request, repair_catalog_fields=True)

    def test_repeated_device_classes_remain_distinct(self):
        c = dict(parts=[dict(id='mega', type='wokwi-arduino-mega'),
                       dict(id='s1', type='wokwi-servo'), dict(id='s2', type='wokwi-servo')],
                 connections=[['s1:PWM','mega:5'], ['s2:PWM','mega:6']])
        p = dict(interface_requests=[dict(kind='servo_angle', device='s1', pin=44, initial=0, meaning='First'),
                                    dict(kind='servo_angle', device='s2', pin=45, initial=0, meaning='Second')])
        contract, _ = prepare_interface_contract(p, json.dumps(c), {}, target_board='arduino:avr:mega')
        self.assertEqual([r['binding']['pin'] for r in contract['interfaces']], ['D5','D6'])
        self.assertEqual(p['interface_requests'][0]['pin'], 44)

    def test_partial_bar_graph_uses_only_verified_contiguous_segments(self):
        c = circuit()
        c['connections'] = [wire for wire in c['connections'] if not any(
            endpoint in ('bargraph1:A9','bargraph1:C9','bargraph1:A10','bargraph1:C10') for endpoint in wire)]
        facts = circuit_inventory([c], 'arduino:avr:mega')
        bar = next(d for d in facts if d['device']=='bargraph1')
        self.assertEqual(bar['catalog_operations'][0]['pins'], ['D'+str(i) for i in range(2,10)])
        c['connections'] = [wire for wire in c['connections'] if 'bargraph1:A4' not in wire]
        bar = next(d for d in circuit_inventory([c], 'arduino:avr:mega') if d['device']=='bargraph1')
        self.assertEqual(bar['catalog_operations'], [])

    def test_native_sensor_requires_device_protocol_rather_than_fake_gpio(self):
        c = dict(parts=[dict(id='uno', type='wokwi-arduino-uno'),dict(id='imu',type='wokwi-mpu6050')],
                 connections=[['imu:SDA','uno:A4'],['imu:SCL','uno:A5']])
        facts = circuit_inventory([c], 'arduino:avr:uno')
        self.assertIn('mpu_acceleration', {op['kind'] for op in facts[0]['catalog_operations']})
        self.assertEqual(facts[0]['signals']['SDA'], ['A4'])
        from app.services.wiring_inventory import check_circuit_inventory
        with self.assertRaisesRegex(ValueError, 'native device operation'):
            check_circuit_inventory(dict(board='arduino:avr:uno',interfaces=[]),facts)
        with self.assertRaises(ValueError):
            check_circuit_inventory(dict(board='arduino:avr:uno',interfaces=[dict(binding=dict(kind='analog_input',pin='A4'))]),facts)
        check_circuit_inventory(dict(board='arduino:avr:uno',interfaces=[dict(binding=dict(
            kind='mpu_acceleration',sda_pin='A4',scl_pin='A5',axis='y',address=104))]),facts)

    def test_grounded_button_restores_normalized_pressed_level(self):
        c = dict(parts=[dict(id='uno',type='wokwi-arduino-uno'), dict(id='btn',type='wokwi-pushbutton')],
                 connections=[['btn:1.l','uno:2'],['btn:2.l','uno:GND.1']])
        proposal = dict(interface_requests=[dict(kind='digital_input',pin=2,initial=False,meaning='Button')])
        contract, actions = prepare_interface_contract(proposal, json.dumps(c), {})
        self.assertIs(contract['interfaces'][0]['binding']['active_high'], False)
        self.assertIs(contract['interfaces'][0]['binding']['pullup'], True)
        self.assertTrue(actions)

    def test_shift_register_terminal_spellings_are_normalized_exactly(self):
        p = dict(interface_requests=[dict(kind='shift595_number', initial=0, meaning='Digit',
                  registers=[dict(register='sr1',segment_bits=['sr1:Q0','Q1','Q2','Q3','Q4','Q5','Q6'])])])
        normalized, _ = normalize_interface_proposal(p)
        self.assertEqual(normalized['interface_requests'][0]['registers'][0]['segment_bits'], list(range(7)))
        p['interface_requests'][0]['registers'][0]['segment_bits'][0] = 'other:Q0'
        normalized, _ = normalize_interface_proposal(p)
        self.assertEqual(normalized['interface_requests'][0]['registers'][0]['segment_bits'][0], 'other:Q0')

    def test_common_anode_rgb_has_known_inverse_electrical_mapping(self):
        c = dict(parts=[dict(id='mega',type='wokwi-arduino-mega'),dict(id='rgb',type='wokwi-rgb-led')],
                 connections=[['rgb:COM','mega:5V'],['rgb:R','mega:2'],['rgb:G','mega:3'],['rgb:B','mega:4']])
        p = dict(interface_requests=[dict(kind='pwm_output',device='rgb',pin=2,initial=0,meaning='Red brightness')])
        contract, _ = prepare_interface_contract(p, json.dumps(c), {},target_board='arduino:avr:mega')
        binding = contract['interfaces'][0]['binding']
        self.assertEqual((binding['scale'],binding['offset']),(-1,255))
        p['interface_requests'][0].update(unit='%',scale=2.55,offset=0)
        with self.assertRaisesRegex(ValueError,'calibration'):
            prepare_interface_contract(p,json.dumps(c),{},target_board='arduino:avr:mega')
        self.assertEqual(p['interface_requests'][0]['scale'],2.55)

    def test_restored_bar_graph_through_official_compiler_backend_and_boards(self):
        from fastapi.testclient import TestClient
        from app.main import app
        from app.services.sysml_assembly import assemble_sysml
        for board in ('arduino:avr:uno', 'arduino:avr:mega'):
            with self.subTest(board=board), tempfile.TemporaryDirectory() as folder, TestClient(app) as client:
                base = Path(folder)
                c = circuit(); c['parts'][0]['type'] = 'wokwi-arduino-uno' if board.endswith('uno') else 'wokwi-arduino-mega'
                proposal = dict(interface_requests=[dict(kind='led_bar_graph',device='bargraph1',
                    pins=['A'+str(i) for i in range(1,11)], initial=False, meaning='Ten GPIO segments'),
                    dict(kind='analog_input',device='pot1',pin='A0',initial=0,meaning='ADC')])
                with patch('app.main._call_llm', side_effect=AssertionError('No model requests in regression')):
                    prepared = client.post('/generate/interface-contract',json=dict(board=board,
                        requirement='Drive bar graph segments; given circuit: '+json.dumps(c),
                        interface_contract=proposal,out_path=str(base/'contract.json')))
                self.assertEqual(prepared.status_code,200,prepared.text)
                contract = prepared.json()['interface_contract']
                outputs = {p['name']:p['initial'] for p in contract['interfaces'] if p['direction']=='output'}
                source = assemble_sysml(dict(states=[],updates=[],outputs=outputs),contract)
                compiled = client.post('/compile/sysml2',json=dict(system_sysml2=source,
                    interface_contract=contract,out_dir=str(base/'ir')))
                self.assertEqual(compiled.status_code,200,compiled.text)
                generated = client.post('/convert/il-to-c',json=dict(interface_contract=contract,
                    provenance=compiled.json()['contents']['provenance'],il_file_path=compiled.json()['files']['il'],out_c_dir=str(base/'c')))
                self.assertEqual(generated.status_code,200,generated.text)
                contents = generated.json()['contents']
                adapted = client.post('/convert/c-to-arduino',json=dict(provenance=generated.json()['provenance'],
                    c_code=contents['ProgramPage1.c'],interface_contract=contract,
                    compiled_kernel=dict(header=contents['define.h'],source=contents['ProgramPage1.c']),out_path=str(base/'bargraph.ino')))
                self.assertEqual(adapted.status_code,200,adapted.text)
                built = client.post('/arduino/compile',json=dict(sketch_path=adapted.json()['saved_path'],fqbn=board))
                self.assertEqual(built.status_code,200,built.text)
                self.assertTrue(built.json()['success'],built.text)


if __name__ == '__main__':
    unittest.main()
