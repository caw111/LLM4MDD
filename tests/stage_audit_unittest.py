import hashlib
import json
from pathlib import Path
import tempfile
import unittest

from experiment.code.audit_contract_stages import audit


class StageAuditTests(unittest.TestCase):
    def test_known_upstream_failure_differs_from_missing_evidence(self):
        with tempfile.TemporaryDirectory() as folder:
            root=Path(folder)
            def save(path,data):
                p=root/path;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(json.dumps(data),encoding='utf-8')
            save('execution.json',dict(cases=[dict(id=n) for n in ('planning','missing','accepted')],repetitions=1))
            save('planning/repeat_1/results.json',[dict(arm='workflow',compile_ok=False,failure_stage='interface_contract')])
            passed=dict(ok=True,stages=dict(sysml_generation=True,sysml_to_ir=True,ir_to_il=True),coverage=dict(status='structural_pass'))
            save('accepted/repeat_1/model_validation.json',dict(attempts=[passed]))
            hashes={p:hashlib.sha256(p.read_bytes()).hexdigest() for p in root.rglob('*.json')}
            report=audit(root)
            self.assertEqual(report['upstream_rejections'],{'interface_contract':1})
            self.assertEqual(report['final_model_rejections'],{})
            for gate in report['gates_all_planned_tasks']['final_candidate'].values():
                self.assertEqual((gate['passed'],gate['failed'],gate['not_reached'],gate['unrecorded']),(1,0,1,1))
                self.assertEqual(gate['denominator'],3)
            self.assertTrue(all(hashlib.sha256(p.read_bytes()).hexdigest()==h for p,h in hashes.items()))
            self.assertTrue(any('results.json' in p for p in report['evidence_sha256']))
            save('planning/repeat_1/model_validation.json',dict(attempts=[passed]))
            with self.assertRaisesRegex(ValueError,'contradicts model evidence'):audit(root)


if __name__=='__main__':unittest.main()
