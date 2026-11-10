import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
from run_wokwi_case import resolve_project


class RelocatedDatasetTests(unittest.TestCase):
    def test_relocated_data_requires_equal_public_text_and_wiring(self):
        with tempfile.TemporaryDirectory() as folder:
            root=Path(folder);project=root/'experiment/datasets/PCEval-Dataset/projects/level1/example'
            project.mkdir(parents=True)
            source=project/'description.md';diagram=project/'diagram.json'
            source.write_text('\ufeffRead input and print it.\n\n',encoding='utf-8')
            circuit={'parts':[],'connections':[['sensor:SDA','uno:2','green']]}
            diagram.write_text(json.dumps(circuit,indent=2),encoding='utf-8')
            case=dict(source='tests/PCEval-Dataset/projects/level1/example/description.md',
                requirement='Read input and print it.\nGiven circuit (must preserve wiring):\n'+json.dumps(circuit),
                source_sha256='historical byte hash',circuit_sha256='historical circuit hash')
            found,identity=resolve_project(case,root)
            self.assertEqual(found,project)
            self.assertNotEqual(identity['source_sha256'],identity['recorded_source_sha256'])
            source.write_text('Replace input with constant.',encoding='utf-8')
            with self.assertRaisesRegex(ValueError,'description differs'):resolve_project(case,root)
            source.write_text('Read input and print it.',encoding='utf-8')
            circuit['connections'][0][1]='uno:3';diagram.write_text(json.dumps(circuit),encoding='utf-8')
            with self.assertRaisesRegex(ValueError,'wiring differs'):resolve_project(case,root)


if __name__=='__main__':unittest.main()
