import copy
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
from app.services.interface_planner import check_explicit_requirements, plan_interface_contract, ContractPlanningError
from app.services.hardware_adapter import validate_hardware_contract


def inventory(pin=12, period=10):
    return {'version':1,'control_period_ms':period,'interfaces':[
        dict(name='lamp',direction='output',type='bool',unit='1',meaning='LED command',
             sample_period_ms=period,initial=False,device='LED',hardware='requested LED',
             binding={'kind':'digital_output','pin':pin})],
        'requirements':[dict(id='R1',text='blink',assertion_ids=['blink'])]}


def bar_graph_circuit():
    return dict(parts=[dict(id='arduino uno1', type='wokwi-arduino-uno'),
                       dict(id='bar graph led1', type='wokwi-led-bar-graph')],
                connections=[[f'bar graph led1:A{i}', f'arduino uno1:{i+1}', 'green', []]
                             for i in range(1, 11)] + [['arduino uno1:GND.1', 'bar graph led1:C1']])


def bar_graph_inventory():
    data = inventory(2)
    data['interfaces'] = [dict(copy.deepcopy(data['interfaces'][0]), name=f'segment{i}',
                               binding=dict(kind='digital_output', pin=i+1), initial=i <= 6)
                          for i in range(1, 11)]
    return data


class InterfacePlanningTests(unittest.TestCase):
    def test_pin_inventory_does_not_invent_device_ownership(self):
        from app.services.interface_planner import confirmed_source_wiring
        cases = [('Button D2 controls an LED; LED wiring unspecified.', {'digital_input': {'pin': 'D2'}}),
                 ('Fan PWM D5 and a display with unspecified wiring.', {'pwm_output': {'pin': 'D5'}}),
                 ('A0 is listed; the analog sensor wiring is unspecified.', {})]
        for requirement, expected in cases:
            with self.subTest(requirement=requirement):
                self.assertEqual(confirmed_source_wiring(requirement, {}), expected)

    def test_confirmed_device_roles_survive_without_inventory_guessing(self):
        from app.services.interface_planner import confirmed_source_wiring
        circuit = dict(parts=[dict(id='uno', type='wokwi-arduino-uno'),
                              dict(id='pot', type='wokwi-potentiometer'),
                              dict(id='servo', type='wokwi-servo')],
                       connections=[['pot:SIG', 'uno:A0'], ['uno:9', 'servo:PWM']])
        facts = confirmed_source_wiring('Given circuit: ' + json.dumps(circuit), {})
        self.assertEqual(facts, {'analog_input': {'pin': 'A0'}, 'servo_angle': {'pin': 'D9'}})
        self.assertEqual(confirmed_source_wiring('TM1637 CLK=D4,DIO=D5', {}),
                         {'tm1637_number': {'clk_pin': 'D4', 'dio_pin': 'D5'}})

    def test_supplied_and_planned_api_contracts_share_board_endpoint_validation(self):
        from fastapi.testclient import TestClient
        from app.main import app
        requirement = 'Arduino Uno bar graph threshold 6. Given circuit: ' + json.dumps(bar_graph_circuit())
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client:
            for supplied in (True, False):
                path = Path(folder) / ('supplied.json' if supplied else 'planned.json')
                payload = dict(requirement=requirement, out_path=str(path), max_repair_attempts=0)
                if supplied:
                    payload['interface_contract'] = bar_graph_inventory()
                with patch('app.main._call_llm', return_value=('fixture', json.dumps(bar_graph_inventory()))) as call:
                    response = client.post('/generate/interface-contract', json=payload)
                self.assertEqual(response.status_code, 200, response.text)
                self.assertEqual(call.call_count, 0 if supplied else 1)
                self.assertTrue(path.exists())
            bad = bar_graph_inventory(); bad['interfaces'].pop()
            path = Path(folder) / 'missing.json'
            response = client.post('/generate/interface-contract', json=dict(
                requirement=requirement, interface_contract=bad, out_path=str(path)))
            self.assertEqual(response.status_code, 422)
            self.assertIn('D11', response.text)
            self.assertFalse(path.exists())

    def test_component_anodes_are_not_board_analog_pins(self):
        data = bar_graph_inventory()
        requirement = 'Arduino Uno bar graph threshold 6. Given circuit: ' + json.dumps(bar_graph_circuit())
        validate_hardware_contract(data)
        prompts = []
        def call(prompt):
            prompts.append(prompt)
            return 'fixture', json.dumps(data)
        result, _ = plan_interface_contract(call, requirement, {}, max_repairs=0)
        self.assertEqual(len(result['interfaces']), 10)
        authoritative = prompts[0].split('Authoritative pin names')[1]
        self.assertNotIn('"A10"', authoritative)
        self.assertIn('"D11"', authoritative)

    def test_numeric_board_connections_still_require_every_signal(self):
        data = bar_graph_inventory()
        data['interfaces'].pop()
        for requirement, pins in [('Given circuit: ' + json.dumps(bar_graph_circuit()), {}),
                                   ('Bar graph', bar_graph_circuit())]:
            with self.subTest(source=requirement[:20]), self.assertRaisesRegex(ValueError, 'D11'):
                check_explicit_requirements(data, requirement, pins)

    def test_circuit_does_not_hide_prose_pins_or_allow_wrong_bindings(self):
        requirement = 'Arduino Uno D12 LED. Given circuit: ' + json.dumps(bar_graph_circuit())
        with self.assertRaisesRegex(ValueError, 'D12'):
            check_explicit_requirements(bar_graph_inventory(), requirement, {})
        data = bar_graph_inventory()
        data['interfaces'][-1]['binding']['pin'] = 13
        with self.assertRaisesRegex(ValueError, 'D11'):
            check_explicit_requirements(data, json.dumps(bar_graph_circuit()), {})
        circuit = bar_graph_circuit()
        circuit['connections'][-1] = ['arduino uno1:A10', 'bar graph led1:A1']
        with self.assertRaisesRegex(ValueError, 'Unsupported pin.*A10'):
            check_explicit_requirements(bar_graph_inventory(), json.dumps(circuit), {})

    def test_qualified_device_terminals_in_prose_and_mega_analog_alias(self):
        circuit = dict(parts=[dict(id='mega', type='wokwi-arduino-mega'),
                              dict(id='display', type='wokwi-led')],
                       connections=[['display:A10', 'mega:A15'], ['mega:GND.1', 'display:C']])
        data = inventory(69); data['board'] = 'arduino:avr:mega'
        check_explicit_requirements(data, 'display:A10 connects to mega:A15. ' + json.dumps(circuit), {})
        data['interfaces'][0]['binding']['pin'] = 68
        with self.assertRaisesRegex(ValueError, 'A15'):
            check_explicit_requirements(data, json.dumps(circuit), {})

    def test_dependency_repair_names_invalid_references_and_valid_ports(self):
        good = inventory()
        good['interfaces'].append(dict(name='button', direction='input', type='bool', unit='1',
            meaning='pressed', sample_period_ms=10, initial=False, device='button', hardware='D2 input',
            binding=dict(kind='digital_input', pin=2)))
        wrong = copy.deepcopy(good)
        wrong['required_dependencies'] = {'lamp': ['button', 'stateCounter'], 'ghost': ['lamp']}
        replies = iter([wrong, wrong, good]); prompts = []
        def call(prompt):
            prompts.append(prompt)
            return 'fixture', json.dumps(next(replies))
        _, report = plan_interface_contract(call, 'D2 button controls D12 LED', {}, max_repairs=2)
        self.assertIn('required_dependencies.lamp[1]', prompts[1])
        self.assertIn('stateCounter', prompts[1])
        self.assertIn('required_dependencies.ghost', prompts[1])
        self.assertIn('declared inputs: ["button"]', prompts[1])
        self.assertIn('declared outputs: ["lamp"]', prompts[1])
        self.assertIn('NO PROGRESS', prompts[2])
        self.assertTrue(report['attempts'][1]['no_progress'])

    def test_missing_fields_and_serial_trigger_feedback_locate_the_fault(self):
        bad = inventory(); del bad['interfaces'][0]['initial']
        prompts = []
        def call(prompt):
            prompts.append(prompt)
            return 'fixture', json.dumps(bad)
        with self.assertRaises(ContractPlanningError):
            plan_interface_contract(call, 'D12 LED', {}, max_repairs=1)
        self.assertIn('interfaces[0].initial', prompts[1])
        serial = dict(name='reading', direction='output', type='i32', unit='1', meaning='serial reading',
                      sample_period_ms=10, initial=0, device='serial', hardware='UART',
                      binding=dict(kind='serial_value', trigger='absent', label='', baud=115200))
        bad = inventory(); bad['interfaces'].append(serial)
        prompts.clear()
        with self.assertRaises(ContractPlanningError):
            plan_interface_contract(call, 'D12 LED and serial output', {}, max_repairs=1)
        self.assertIn('reading.binding.trigger', prompts[1])
        self.assertIn('absent', prompts[1])
        self.assertIn('Boolean outputs: ["lamp"]', prompts[1])
        for trigger in ([], {}, 7):
            bad['interfaces'][-1]['binding']['trigger'] = trigger
            with self.subTest(trigger=trigger), self.assertRaisesRegex(ValueError, 'reading.binding.trigger'):
                validate_hardware_contract(bad)

    def test_binding_kind_feedback_and_dependency_shapes_remain_strict(self):
        wrong = inventory(); wrong['interfaces'][0]['binding']['kind'] = 'digital_input'
        replies = iter([wrong, inventory()]); prompts = []
        def call(prompt):
            prompts.append(prompt)
            return 'fixture', json.dumps(next(replies))
        result, _ = plan_interface_contract(call, 'D12 LED', {}, max_repairs=1)
        self.assertIn('lamp.binding.kind', prompts[1])
        self.assertIn('allowed output kinds', prompts[1])
        self.assertIn('digital_output', prompts[1])
        self.assertEqual(result['interfaces'][0]['binding']['kind'], 'digital_output')
        for dependency in ([], {'lamp': 'button'}, {'lamp': [42]}, {'lamp': [{}]}, {'lamp': ['lamp']}):
            bad = inventory(); bad['required_dependencies'] = dependency
            with self.subTest(dependency=dependency), self.assertRaisesRegex(ValueError, 'required_dependencies'):
                validate_hardware_contract(bad)
        bad = inventory(); del bad['interfaces'][0]['binding']['pin']
        with self.assertRaisesRegex(ValueError, r'lamp.binding.pin=None'):
            validate_hardware_contract(bad)

    def test_repair_budget_does_not_accept_a_real_pin_collision(self):
        bad = inventory(5)
        bad['interfaces'].append(dict(name='display', direction='output', type='i32', unit='1',
            meaning='display number', sample_period_ms=10, initial=0, device='TM1637', hardware='CLK D4 DIO D5',
            binding=dict(kind='tm1637_number', clk_pin=4, dio_pin=5, brightness=7,
                         leading_zero=False, invalid_policy='blank')))
        with self.assertRaises(ContractPlanningError) as caught:
            plan_interface_contract(lambda _: ('fixture', json.dumps(bad)), 'D5 LED TM1637 CLK=D4 DIO=D5', {}, max_repairs=1, resolve_defaults=False)
        self.assertEqual(len(caught.exception.attempts), 2)
        self.assertTrue(all('Conflicting hardware bindings' in a['error'] for a in caught.exception.attempts))
        resolved, report = plan_interface_contract(lambda _: ('fixture', json.dumps(bad)),
            'D5 LED TM1637 CLK=D4 DIO=D5', {}, max_repairs=0)
        self.assertNotEqual(resolved['interfaces'][1]['binding']['dio_pin'], 5)
        self.assertTrue(report['configuration_resolution']['changes'])
        validate_hardware_contract(resolved)

    def test_digest_only_normalizes_equivalent_json_numbers(self):
        from app.services.interface_planner import contract_digest
        self.assertEqual(contract_digest({'nested':[0.0,1.0,1e20]}),contract_digest({'nested':[0,1,100000000000000000000]}))
        for changed in ([0,True,1e20],[0,1.5,1e20],[0,1,1e21]):
            self.assertNotEqual(contract_digest({'nested':[0,1,1e20]}),contract_digest({'nested':changed}))
        self.assertNotEqual(contract_digest({'value':9007199254740993}),contract_digest({'value':9007199254740992}))
        for invalid in (float('nan'),float('inf')):
            with self.assertRaises(ValueError):contract_digest({'value':invalid})

    def test_example_pin_cannot_replace_explicit_requirement_pin(self):
        with self.assertRaisesRegex(ValueError,'D12'):
            check_explicit_requirements(inventory(13),'D12 引脚 LED',{})
        check_explicit_requirements(inventory(12),'D12 引脚 LED',{})

    def test_analog_alias_and_device_names(self):
        c=inventory(14)
        c['interfaces'].append(dict(name='temperature',direction='input',type='f64',unit='degC',
            meaning='temperature',sample_period_ms=2000,initial=0,device='DHT22',hardware='D2',
            binding=dict(kind='dht_temperature',pin=2,sensor='DHT22',invalid_policy='hold_last_valid')))
        check_explicit_requirements(c,'A0 输入；DHT22 器件',{})
        with self.assertRaisesRegex(ValueError,'Explicit DHT22'):
            check_explicit_requirements(inventory(14),'A0 输入；DHT22 器件',{})
        with self.assertRaisesRegex(ValueError,'D4'):
            check_explicit_requirements(inventory(12),'D12 LED',{'display':'CLK=D4,DIO=D5'})

    def test_business_period_cannot_be_used_as_control_step(self):
        with self.assertRaisesRegex(ValueError,'Control period'):
            check_explicit_requirements(inventory(12,1000),'D12 LED每500ms翻转，时间容差10ms',{})
        with self.assertRaisesRegex(ValueError,'elapsed_ms'):
            check_explicit_requirements(inventory(12,10),'D12 LED每500ms翻转，时间容差10ms',{})

    def test_only_relevant_example_is_included(self):
        from app.services.sysml import sysml_bundle_prompt
        prompt=sysml_bundle_prompt('constant output',contract=inventory())
        self.assertIn('For constant ON',prompt)
        self.assertNotIn('Example of an initially-off',prompt)
        self.assertNotIn('For a button input',prompt)
        self.assertNotIn('<!--',prompt)

    def test_planner_repairs_wrong_wiring_before_sysml_generation(self):
        responses=iter([inventory(13),inventory(12)])
        prompts=[]
        def call(prompt):
            prompts.append(prompt)
            return 'fixture',json.dumps(next(responses))
        result,report=plan_interface_contract(call,'D12 LED',{},max_repairs=1)
        self.assertEqual(result['interfaces'][0]['binding']['pin'],12)
        self.assertEqual([r['ok'] for r in report['attempts']],[False,True])
        self.assertIn('Explicit requirement pins',prompts[1])
        self.assertIn('Authoritative pin names',prompts[0])
        self.assertIn('["D12"]',prompts[0])

    def test_elapsed_binding_does_not_accept_absolute_time(self):
        data=inventory()
        data['interfaces'].append(dict(name='elapsed',direction='input',type='i32',unit='ms',meaning='clock',
             sample_period_ms=10,initial=0,device='clock',hardware='millis',
             binding={'kind':'elapsed_ms','semantics':'absolute_uptime'}))
        with self.assertRaisesRegex(ValueError,'never absolute'):
            validate_hardware_contract(data)

    def test_continuous_waveform_cannot_replace_clock_with_step_count(self):
        with self.assertRaisesRegex(ValueError, 'elapsed_ms'):
            check_explicit_requirements(inventory(5), 'D5 PWM LED渐亮渐灭，完整周期2000ms', {})

    def test_repeated_binding_field_error_identifies_exact_level(self):
        good = inventory()
        good['interfaces'].append(dict(name='elapsed',direction='input',type='i32',unit='ms',meaning='clock',
            sample_period_ms=10,initial=0,device='clock',hardware='millis',binding={'kind':'elapsed_ms'}))
        wrong = copy.deepcopy(good)
        wrong['interfaces'][-1]['binding']['initial'] = 0
        replies = iter([wrong, wrong, good]); prompts = []
        def call(prompt):
            prompts.append(prompt)
            return 'fixture', json.dumps(next(replies))
        result, report = plan_interface_contract(call, 'D12 LED每500ms翻转', {}, max_repairs=2)
        self.assertIn('elapsed_ms input is REQUIRED', prompts[0])
        self.assertIn('elapsed.binding has unknown fields initial', prompts[1])
        self.assertIn('outside binding', prompts[1])
        self.assertIn('NO PROGRESS', prompts[2])
        self.assertTrue(report['attempts'][1]['no_progress'])
        self.assertNotIn('initial', result['interfaces'][-1]['binding'])


if __name__=='__main__':unittest.main()
