import hashlib
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
import compare_development_subset as comparison


class SubsetComparisonTests(unittest.TestCase):
    def test_excludes_unselected_tasks_and_refuses_different_oracles_or_requirements(self):
        with tempfile.TemporaryDirectory() as tmp,patch.object(comparison,'ROOT',Path(tmp)):
            root=Path(tmp)
            for label,ids in [('before',['selected','unselected']),('after',['selected'])]:
                folder=root/label;folder.mkdir()
                source='experiment/development/oracles/test.cpp'
                snapshot=folder/'method_snapshot'/source;snapshot.parent.mkdir(parents=True);snapshot.write_bytes(b'fixed oracle')
                execution=dict(cases=[dict(id=case,split='development',requirement=case) for case in ids],repetitions=1,
                    public_interpretations={case:'same public specification' for case in ids},
                    call_limits=dict(direct=1,direct_feedback=6,workflow=6),hashes={source:hashlib.sha256(snapshot.read_bytes()).hexdigest()})
                (folder/'execution.json').write_text(json.dumps(execution))
                rows=[]
                for case in ids:
                    records=[dict(case=case,repeat=1,arm=arm,compile_ok=(case=='selected'),
                        compile_status='passed' if case=='selected' else 'failed',
                        functional_status='passed' if case=='selected' else 'not_generated_or_compile_failed') for arm in comparison.ARMS]
                    target=folder/case/'repeat_1';target.mkdir(parents=True)
                    (target/'results.json').write_text(json.dumps(records));rows+=records
                (folder/'results.json').write_text(json.dumps(rows))
            report=comparison.compare(root/'before',root/'after')
            self.assertEqual(report['runs']['before']['arms']['workflow']['functional']['all_tasks']['denominator'],1)
            self.assertEqual(report['runs']['before']['arms']['workflow']['functional']['all_tasks']['rate'],1)
            path=root/'after/execution.json';original=json.loads(path.read_text())
            wrong=json.loads(path.read_text());wrong['public_interpretations']['selected']='changed'
            path.write_text(json.dumps(wrong))
            with self.assertRaisesRegex(ValueError,'interpretation changed'):comparison.compare(root/'before',root/'after')
            wrong=original;wrong['hashes'][source]='0'*64;path.write_text(json.dumps(wrong))
            with self.assertRaisesRegex(ValueError,'oracle sources'):comparison.compare(root/'before',root/'after')


if __name__=='__main__':unittest.main()
