import json
import sys
import tempfile
import unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
from run_semantic_repair_validation import report


class TenCaseGateTests(unittest.TestCase):
    def batch(self,passed,partial=0):
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)
            for i in range(10):
                p=out/'cases'/str(i)/'result.json';p.parent.mkdir(parents=True)
                p.write_text(json.dumps(dict(case=str(i),dataset='internal',compile_ok=True,rounds=[],
                    functional_status='passed_public_scenarios' if i<passed else 'partial_scenarios_passed' if i<passed+partial else 'failed',
                    simulide_status='not_run')))
            return report(out,[dict(id=str(i),dataset='internal') for i in range(10)])['acceptance_gate']

    def test_nine_of_ten_does_not_exceed_ninety_percent(self):
        self.assertEqual(self.batch(9)['state'],'repair_required')
        self.assertEqual(self.batch(10)['state'],'passed')

    def test_partial_coverage_is_not_full_runtime_acceptance(self):
        gate=self.batch(0,10)
        self.assertEqual(gate['compiled'],10)
        self.assertEqual(gate['runtime_verified'],0)
        self.assertEqual(gate['partial_scenarios_passed'],10)
        self.assertEqual(gate['state'],'repair_required')


if __name__=='__main__':unittest.main()
