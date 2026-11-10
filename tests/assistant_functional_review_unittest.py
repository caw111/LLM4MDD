"""Evidence and denominator regressions for the offline assistant review ledger."""
import contextlib
import importlib.util
import io
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch


SCRIPT = Path(__file__).resolve().parents[1] / 'experiment/code/assistant_functional_review.py'
spec = importlib.util.spec_from_file_location('assistant_review', SCRIPT)
review = importlib.util.module_from_spec(spec)
spec.loader.exec_module(review)


class OfflineReviewTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)
        root = Path(self.folder.name)
        self.addCleanup(patch.stopall)
        patch.object(review, 'ROOT', root).start()
        patch.object(review, 'OUT', root / 'out').start()
        self.code = root / 'sketch.ino'
        self.code.write_text('void step(int* out) {\n  *out = 42;\n}\n')
        self.config = dict(jobs=[dict(key='a', artifact='sketch.ino', code_sha256=review.sha(self.code),
                                     board='uno', requirement='Output 42')], rows=[
            dict(group='g', case='x', compile_ok=True, review_key='a'),
            dict(group='g', case='y', compile_ok=False),
            dict(group='h', case='x', compile_ok=True, review_key='a')])
        self.notes = root / 'notes.json'

    def note(self, quote='*out = 42;', verdict='judged_pass'):
        review.save(self.notes, [dict(case='x', candidate=1, verdict=verdict,
                                    reason='Reads the complete step; writes the required constant.', quotes=[quote])])

    def test_pointer_assignment_is_visible_in_reading_packet(self):
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            review.packet(self.config, 'x')
        self.assertIn('2:   *out = 42;', output.getvalue())

    def test_unsupported_quote_and_changed_original_are_rejected(self):
        self.note('invented();')
        with self.assertRaises(AssertionError):
            review.record(self.config, self.notes)
        self.assertFalse((review.OUT / 'decisions/a.json').exists())
        self.note()
        self.code.write_text('void step(int* out) {}')
        with self.assertRaises(AssertionError):
            review.record(self.config, self.notes)

    def test_shared_code_and_compilation_failure_denominator(self):
        self.note(verdict='uncertain')
        review.record(self.config, self.notes)
        with contextlib.redirect_stdout(io.StringIO()):
            review.report(self.config)
        summary = review.read(review.OUT / 'summary.json')
        self.assertTrue(summary['complete'])
        self.assertEqual(summary['reviewed_unique'], 1)
        self.assertEqual(summary['groups']['g']['compile_rate'], .5)
        self.assertEqual(summary['groups']['g']['code_judgment_pass_rate'], 0)
        self.assertEqual(summary['groups']['g']['counts']['uncertain'], 1)
        self.assertEqual(summary['groups']['h']['counts']['uncertain'], 1)
        self.assertEqual(summary['new_external_api_calls'], 0)


if __name__ == '__main__':
    unittest.main()
