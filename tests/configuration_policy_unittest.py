import copy
import json
import unittest

from app.services.configuration_policy import resolve_configuration, apply_configuration
from app.services.interface_planner import prepare_interface_contract, check_explicit_requirements, plan_interface_contract
from app.services.hardware_adapter import validate_hardware_contract
from app.services.interface_registry import normalize_interface_proposal


class ConfigurationPolicyTests(unittest.TestCase):
    def test_explicit_english_bench_pins_are_not_replaced_with_defaults(self):
        policy=resolve_configuration('PWM LED on D9, potentiometer ADC on A0. Buttons D2/D3.')
        roles={d['kind']:d['resolved'] for d in policy['devices'] if d['kind'] in ('pwm_output','analog_input')}
        self.assertEqual(roles,dict(pwm_output=dict(pin='D9'),analog_input=dict(pin='A0')))
        self.assertFalse(any(d['kind']=='digital_output' for d in policy['devices']))
        policy=resolve_configuration('DHT11 DATA=D7; HC-SR04 TRIG=D8,ECHO=D9; TM1637 CLK=D4,DIO=D5.')
        self.assertEqual(next(d['resolved'] for d in policy['devices'] if d['kind']=='dht'),dict(pin='D7'))
        self.assertFalse(any(c.get('reason')=='unspecified wiring' for c in policy['changes']))

    def test_grouped_buttons_and_fan_are_reserved_before_display_assignment(self):
        source = '按钮 D2、D3、D4 选择，风扇 D5，TM1637（CLK=D4，DIO=D5），蜂鸣器 D6。'
        policy = resolve_configuration(source)
        buttons = [d['resolved']['pin'] for d in policy['devices'] if d['kind'] == 'digital_input']
        self.assertEqual(buttons, ['D2', 'D3', 'D4'])
        display = next(d['resolved'] for d in policy['devices'] if d['kind'] == 'tm1637_number')
        self.assertNotIn(display['clk_pin'], buttons)
        self.assertNotEqual(display['dio_pin'], 'D5')
        self.assertIn('风扇 D5', policy['effective_requirement'])
        self.assertIn('CLK='+display['clk_pin'], policy['effective_requirement'])

    def test_reverse_dht_wiring_does_not_create_a_second_sensor(self):
        policy = resolve_configuration('读取连接在 D7 引脚的数据温湿度传感器 DHT11 的温度值。')
        self.assertEqual(policy['devices'][0]['resolved'], {'pin': 'D7'})
        self.assertEqual(policy['changes'], [])

    def test_pwm_fan_is_not_a_second_digital_actuator(self):
        policy = resolve_configuration('LM35 温度传感器（A1）、风扇 PWM 控制引脚 D5、TM1637（CLK=D4，DIO=D5）。')
        self.assertFalse(any(d['kind'] == 'digital_output' for d in policy['devices']))
        self.assertEqual(next(d for d in policy['devices'] if d['kind'] == 'analog_input')['resolved']['pin'], 'A1')

    def test_sonar_connected_word_and_multiple_room_sensors_preserve_pins(self):
        policy = resolve_configuration('HC-SR04 超声波模块，其中 Trig 连接 D8，Echo 连接 D9。')
        self.assertEqual(policy['changes'], [])
        self.assertEqual(policy['devices'][0]['resolved'], {'trigger_pin':'D8','echo_pin':'D9'})
        policy = resolve_configuration('两个 DHT11，客厅传感器 D7、卧室传感器 D8，按钮 D2/D3。')
        self.assertEqual(policy['changes'], [])
        self.assertEqual({d['resolved']['pin'] for d in policy['devices'] if d['kind']=='dht'}, {'D7','D8'})

    def test_display_connected_word_and_alternative_pump_name(self):
        p = resolve_configuration('TM1637 模块，CLK 连接 D2，DIO 连接 D3。')
        self.assertEqual(p['changes'], [])
        self.assertEqual(p['devices'][0]['resolved'], {'clk_pin':'D2','dio_pin':'D3'})
        p = resolve_configuration('水泵或增氧泵 D5、加热器 D10、TM1637（CLK=D4，DIO=D5）。')
        pins = {d['resolved']['pin'] for d in p['devices'] if d['kind']=='digital_output'}
        self.assertEqual(pins, {'D5','D10'})
        self.assertNotEqual(next(d for d in p['devices'] if d['kind']=='tm1637_number')['resolved']['dio_pin'],'D5')

    def test_missing_display_wiring_is_deterministic_and_respects_sensor(self):
        source = '使用 TM1637 + DHT11（D7）：显示温度，每 2 秒刷新'
        policy = resolve_configuration(source)
        self.assertEqual(policy, resolve_configuration(source))
        devices = {d['kind']: d['resolved'] for d in policy['devices']}
        self.assertEqual(devices['dht']['pin'], 'D7')
        self.assertEqual(devices['tm1637_number'], {'clk_pin': 'D4', 'dio_pin': 'D5'})
        self.assertEqual(policy['original_requirement'], source)
        self.assertEqual(len(policy['changes']), 2)
        again = resolve_configuration(policy['effective_requirement'])
        self.assertEqual(again['effective_requirement'], policy['effective_requirement'])
        self.assertEqual(again['changes'], [])

    def test_pwm_display_conflict_has_one_reproducible_solution(self):
        source = '风扇 PWM D5，TM1637（CLK=D4，DIO=D5），按钮 D2。'
        policy = resolve_configuration(source)
        self.assertIn('DIO=D3', policy['effective_requirement'])
        self.assertIn('PWM D5', policy['effective_requirement'])
        proposal = {'interface_requests': [
            dict(kind='pwm_output', pin=5, initial=False, meaning='Fan PWM'),
            dict(kind='tm1637_number', clk_pin=4, dio_pin=5, initial=0, meaning='Display')],
            'control_period_ms':10}
        original = copy.deepcopy(proposal)
        contract, actions = prepare_interface_contract(proposal, policy['effective_requirement'], {}, resolution=policy)
        self.assertEqual(contract['interfaces'][0]['initial'], 0)
        self.assertEqual(contract['interfaces'][1]['binding']['dio_pin'], 'D3')
        contract['requirements'] = [dict(id='R1', text='Fan and display', assertion_ids=['trace'])]
        validate_hardware_contract(contract)
        self.assertTrue(actions)
        self.assertEqual(proposal, original)

    def test_echo_moves_but_pwm_keeps_compatible_pin(self):
        policy = resolve_configuration('HC-SR04（Trig=D8，Echo=D9），PWM LED D9。')
        d = {x['kind']: x['resolved'] for x in policy['devices']}
        self.assertEqual(d['pwm_output']['pin'], 'D9')
        self.assertNotEqual(d['ultrasonic']['echo_pin'], 'D9')
        self.assertIn('Echo='+d['ultrasonic']['echo_pin'], policy['effective_requirement'])

    def test_serial_defaults_preserve_tokens_and_explicit_baud(self):
        policy = resolve_configuration('串口输入 BUZZ：蜂鸣器（D6）鸣叫 500ms')
        self.assertEqual(policy['serial']['baud'], 9600)
        self.assertEqual(policy['serial']['commands'], {'BUZZ':1})
        self.assertEqual(resolve_configuration('Serial input ON OFF; baud=115200')['serial']['baud'], 115200)
        proposal = dict(interface_requests=[dict(kind='serial_command_input', initial=0, meaning='Received command')])
        fixed, _ = apply_configuration(proposal, policy)
        self.assertEqual(fixed['interface_requests'][0]['commands'], {'BUZZ':1})
        self.assertEqual(fixed['interface_requests'][0]['baud'], 9600)

    def test_units_and_initial_values_are_typed_without_changing_calibration(self):
        proposal = dict(interface_requests=[dict(kind='dht_valid', pin=7, sensor='DHT11', unit='degC', initial=0, meaning='Valid'),
                                           dict(kind='pwm_output', pin=5, initial=False, meaning='Fan')])
        result, actions = normalize_interface_proposal(proposal, repair_catalog_fields=True, allow_default_initial=True)
        self.assertEqual(result['interface_requests'][0]['unit'], '1')
        self.assertIs(result['interface_requests'][0]['initial'], False)
        self.assertIs(type(result['interface_requests'][1]['initial']), int)
        self.assertTrue(actions)
        with self.assertRaisesRegex(ValueError, 'calibration'):
            prepare_interface_contract(dict(interface_requests=[dict(kind='analog_input', pin='A0', unit='volts', raw_unit='volts', meaning='ADC', initial=0)]), 'Read A0', {})

    def test_missing_startup_defaults_never_override_explicit_startup(self):
        p = dict(interface_requests=[dict(kind='digital_output', pin=13, meaning='LED')])
        contract, _ = prepare_interface_contract(p, 'LED D13', {})
        self.assertIs(contract['interfaces'][0]['initial'], False)
        with self.assertRaises(ValueError):
            prepare_interface_contract(p, 'LED D13 initially on', {})

    def test_planner_receives_shared_resolved_requirement_and_records_it(self):
        source = 'TM1637 shows 1234'
        policy = resolve_configuration(source)
        prompts = []
        proposal = dict(interface_requests=[dict(kind='tm1637_number', initial=0, meaning='Display')])
        def call(prompt):
            prompts.append(prompt)
            return 'fixture', json.dumps(proposal)
        contract, report = plan_interface_contract(call, source, {}, max_repairs=0,
                                                  configuration_resolution=policy)
        self.assertIn(policy['effective_requirement'], prompts[0])
        self.assertEqual(report['configuration_resolution'], policy)
        self.assertEqual(contract['configuration_resolution']['original_requirement'], source)
        check_explicit_requirements(contract, policy['effective_requirement'], {})
        from app.services.sysml import sysml_bundle_prompt
        prompt = sysml_bundle_prompt('Prior model context must remain.\nCurrent: '+source,
                                     contract=contract, complete_constraints=False)
        self.assertIn('Prior model context must remain.', prompt)
        self.assertIn(policy['effective_requirement'], prompt)

    def test_circuit_conflict_rewrites_diagram_and_preserves_source(self):
        circuit = dict(parts=[dict(id='uno', type='wokwi-arduino-uno'), dict(id='led', type='wokwi-led'),
                              dict(id='sonar', type='wokwi-hc-sr04')],
                       connections=[['sonar:ECHO','uno:9'], ['led:A','uno:9'], ['sonar:TRIG','uno:8']])
        original = copy.deepcopy(circuit)
        source = 'PWM LED D9. Circuit: ' + json.dumps(circuit)
        policy = resolve_configuration(source)
        actual = policy['resolved_circuits'][0]['connections']
        self.assertEqual(actual[1], ['led:A','uno:9'])
        self.assertNotEqual(actual[0][1], 'uno:9')
        self.assertIn(json.dumps(policy['resolved_circuits'][0], ensure_ascii=False), policy['effective_requirement'])
        self.assertEqual(circuit, original)
        self.assertTrue(policy['changes'])

    def test_shared_i2c_bus_is_not_a_pin_conflict(self):
        circuit = dict(parts=[dict(id='uno', type='wokwi-arduino-uno'), dict(id='x', type='sensor'), dict(id='y', type='sensor')],
                       connections=[['x:SDA','uno:A4'], ['y:SDA','uno:A4'], ['x:SCL','uno:A5'], ['y:SCL','uno:A5']])
        policy = resolve_configuration(json.dumps(circuit))
        self.assertEqual(policy['changes'], [])
        self.assertEqual(policy['resolved_circuits'][0], circuit)

    def test_parallel_passive_loads_are_not_rewired(self):
        circuit = dict(parts=[dict(id='uno', type='wokwi-arduino-uno'),
                              dict(id='a', type='wokwi-led-bar-graph'), dict(id='b', type='wokwi-led-bar-graph')],
                       connections=[['a:A1','uno:3'], ['b:A1','uno:3']])
        policy = resolve_configuration(json.dumps(circuit))
        self.assertEqual(policy['changes'], [])

    def test_existing_pin_roles_and_lists_are_preserved(self):
        a = resolve_configuration('读取连接在 A0 引脚的电位器，根据数值控制 D9 PWM LED 的亮度。')
        self.assertEqual(a['changes'], [])
        b = resolve_configuration('两个 LED 分别连接在 D12 和 D13 引脚。')
        self.assertEqual(b['changes'], [])
        for requirement, expected in [('让连接在 D12 引脚的 LED 每秒闪烁一次。', 'D12'),
                                      ('点亮一个连接在 D13 引脚的 LED，并保持常亮。', 'D13')]:
            policy = resolve_configuration(requirement)
            self.assertEqual(policy['changes'], [])
            self.assertEqual(policy['devices'][0]['resolved']['pin'], expected)
        policy = resolve_configuration('D2 按钮、A0 电位器控制 D9 LED 亮度。LED uses PWM output.')
        self.assertEqual(policy['changes'], [])
        self.assertEqual([d['kind'] for d in policy['devices']].count('pwm_output'), 1)

    def test_explicit_named_pin_dictionary_is_authoritative(self):
        policy = resolve_configuration('Button controls an LED', {'button':'D2','LED':'D12'})
        self.assertEqual(policy['changes'], [])


if __name__ == '__main__':
    unittest.main()
