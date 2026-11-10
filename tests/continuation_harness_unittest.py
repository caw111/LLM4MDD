import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
import continue_repaired_workflows as harness


class ContinuationHarnessTests(unittest.TestCase):
    def fixture(self, root, count):
        baseline=root/'baseline';out=root/'continuation';baseline.mkdir();out.mkdir()
        rows=[dict(case=f'case{i}',arm='workflow',compile_ok=i<199,
                   functional_status='no_validated_oracle' if i<199 else 'not_generated_or_compile_failed') for i in range(296)]
        (baseline/'results.json').write_text(json.dumps(rows),encoding='utf-8')
        for i in range(count):
            folder=out/f'case{199+i}';folder.mkdir()
            (folder/'result.json').write_text(json.dumps(dict(case=f'case{199+i}',compile_ok=i<40,
                compile_status='passed' if i<40 else 'not_generated',functional_status='no_validated_oracle' if i<40 else 'not_generated_or_compile_failed')),
                encoding='utf-8')
        return baseline,out,dict(cases=[dict(id=f'case{i}') for i in range(199,269)],hashes={})

    def test_partial_and_complete_counts_reuse_199_without_double_counting(self):
        for count,expected,complete in ((1,200,False),(70,239,True)):
            with self.subTest(count=count),tempfile.TemporaryDirectory() as folder:
                baseline,out,config=self.fixture(Path(folder),count)
                original=(baseline/'results.json').read_bytes()
                with patch.object(harness,'BASE',baseline),patch.object(harness,'OUT',out):
                    summary=harness.summarize(config)
                self.assertEqual(summary['cumulative_compile_passes'],expected)
                self.assertEqual(summary['complete'],complete)
                self.assertEqual((baseline/'results.json').read_bytes(),original)

    def test_duplicate_case_records_are_rejected(self):
        with tempfile.TemporaryDirectory() as folder:
            baseline,out,config=self.fixture(Path(folder),1)
            (out/'duplicate').mkdir();(out/'duplicate/result.json').write_bytes((out/'case199/result.json').read_bytes())
            with patch.object(harness,'BASE',baseline),patch.object(harness,'OUT',out),self.assertRaises(AssertionError):
                harness.summarize(config)


if __name__=='__main__':unittest.main()
