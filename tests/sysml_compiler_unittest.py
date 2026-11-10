import unittest
from pathlib import Path
from app.services.sysml import validate_sysml
from app.services.sysml_compiler import compile_sysml, ir_to_il, SysMLCompileError
from tools.ir_to_tblink import ir_to_tblink
from xml.etree import ElementTree as ET

class CompilerTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source = Path('tools/sysml/noise_monitor.sysml').read_text(encoding='utf-8-sig')
        cls.model = validate_sysml(cls.source)

    def test_ast_to_existing_ir_formats(self):
        self.assertTrue(self.model['ok'], self.model['issues'])
        ir = compile_sysml(self.source, validated=self.model)
        self.assertEqual([n['params']['Expression'] for n in ir['nodes']], ['(arg0 * arg0)', '(arg0 > arg1)'])
        self.assertEqual([p['name'] for p in ir['outputs']], ['level', 'alarm'])
        self.assertEqual(len(ir['edges']), 5)
        self.assertEqual(ir_to_tblink(ir).getroot().tag, 'Root')
        il = ET.fromstring(ir_to_il(ir))
        assignments = [e.attrib['Expression'] for e in il.iter('Expression')]
        self.assertIn('sysml_value_0 = (mic * mic)', assignments)
        self.assertIn('sysml_value_1 = (sysml_value_0 > threshold)', assignments)
        self.assertIn('*alarm = sysml_value_1', assignments)

    def test_all_compiled_models_satisfy_v2_contract(self):
        from experiment.code.formal_ir import validate_ir
        for filename in ('noise_monitor.sysml', 'modular_monitor.sysml', 'counter.sysml', 'ordered_updates.sysml'):
            with self.subTest(filename=filename):
                source = Path('tools/sysml', filename).read_text(encoding='utf-8-sig')
                ir = compile_sysml(source)
                self.assertEqual(ir['version'], 2)
                validate_ir(ir)
                if ir['states']:
                    with self.assertRaisesRegex(ValueError, 'stateless'):
                        ir_to_tblink(ir)
                else:
                    self.assertEqual(ir['updates'], [])
                    self.assertEqual(ir_to_tblink(ir).getroot().tag, 'Root')

    def test_il_arithmetic_normalization_preserves_lazy_boundaries_and_bool_casts(self):
        ir = {'version': 2, 'name': 'Lazy', 'page': 'ProgramPage1',
              'inputs': [{'name': 'enabled', 'type': 'bool'}, {'name': 'value', 'type': 'f64'}],
              'outputs': [{'name': 'selected', 'type': 'bool'}],
              'nodes': [{'id': 'gate', 'type': 'Expression',
                         'inports': [{'name': 'arg0', 'type': 'bool'}, {'name': 'arg1', 'type': 'f64'}],
                         'outports': [{'name': 'result', 'type': 'bool'}], 'params': {}}],
              'edges': [{'src': {'kind': 'input', 'name': name}, 'dst': {'kind': 'node', 'id': 'gate', 'port': port}}
                        for name, port in [('enabled', 'arg0'), ('value', 'arg1')]],
              'states': [], 'updates': [], 'execution': 'step-actions-then-outputs'}
        ir['edges'].append({'src': {'kind': 'node', 'id': 'gate', 'port': 'result'},
                            'dst': {'kind': 'output', 'name': 'selected'}})
        for expression in ('arg0 && ((arg1 * (arg1 - 1.0)) > 0.0)',
                           'arg0 ? ((arg1 * (arg1 - 1.0)) > 0.0) : false',
                           '(bool)arg1'):
            with self.subTest(expression=expression):
                ir['nodes'][0]['params']['Expression'] = expression
                xml = ir_to_il(ir)
                self.assertIn('sysml_value_0', xml)
                if expression != '(bool)arg1':
                    tree = ET.fromstring(xml)
                    calc = next(e for e in tree.iter('Calculate') if e.attrib['Name']=='ProgramPage1_body')
                    self.assertTrue(list(calc.iter('If')))
                    self.assertFalse(any('sysml_arithmetic_' in e.attrib.get('Expression','') for e in calc if e.tag=='Expression'))

    def test_missing_value_is_rejected(self):
        model = validate_sysml('package P { private import ScalarValues::*; part def Controller { out attribute alarm : Boolean; } }')
        self.assertTrue(model['ok'], model['issues'])
        with self.assertRaisesRegex(SysMLCompileError, 'value expression'):
            compile_sysml('', validated=model)

class ModularCompilerTests(unittest.TestCase):
    def test_instances_have_distinct_storage_and_connections(self):
        source = """package P {
        private import ScalarValues::*;
        part def Square { in attribute x : Real; out attribute y : Real = x*x; }
        part def Monitor {
          in attribute a : Real; in attribute b : Real;
          part left : Square; part right : Square;
          bind a = left.x; bind right.x = b;
          out attribute result : Real = left.y + right.y;
        }
        part system : Monitor;
        }"""
        ir = compile_sysml(source)
        nodes = {n['id'] for n in ir['nodes']}
        self.assertIn('compute_left__y', nodes)
        self.assertIn('compute_right__y', nodes)
        inputs = {(e['src'].get('name'),e['dst'].get('id')) for e in ir['edges'] if e['src']['kind']=='input'}
        self.assertEqual(inputs, {('a','compute_left__y'),('b','compute_right__y')})

    def test_condition_and_array_rejection(self):
        source = 'package P { private import ScalarValues::*; part def A { in attribute a : Real; out attribute y : Real = if a > 0.0 ? a else 0.0; } }'
        self.assertEqual(compile_sysml(source)['nodes'][0]['params']['Expression'], '((arg0 > 0.0) ? arg0 : 0.0)')
        with self.assertRaisesRegex(SysMLCompileError, 'multiplicity'):
            compile_sysml(source.replace('a : Real;', 'a : Real[2];'))

if __name__ == '__main__':
    unittest.main()
