"""Resume the frozen configured study with bounded Windows storage retries."""
import hashlib
import json
import sys
import time
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'experiment/output/configured_audit_v1_20261015'


def canonical_json_lines(prompt):
    lines = []
    for line in prompt.splitlines(keepends=True):
        if line.startswith('{'):
            try:
                value = json.loads(line)
            except ValueError:
                pass
            else:
                line = json.dumps(value, sort_keys=True, ensure_ascii=False) + ('\n' if line.endswith('\n') else '')
        lines.append(line)
    return ''.join(lines)


def retry_permission(operation, *args, attempts=30, delay=.1, **kwargs):
    for attempt in range(attempts):
        try:
            return operation(*args, **kwargs)
        except PermissionError:
            if attempt + 1 == attempts:
                raise
            time.sleep(delay)


def self_check():
    assert canonical_json_lines('{"a":1,"b":true}\ntext') == canonical_json_lines('{"b":true,"a":1}\ntext')
    assert canonical_json_lines('{"a":1}') != canonical_json_lines('{"a":true}')
    assert canonical_json_lines('{"a":[1,2]}') != canonical_json_lines('{"a":[2,1]}')
    assert canonical_json_lines('text') != canonical_json_lines('changed')
    calls = []
    def transient():
        calls.append(1)
        if len(calls) < 3:
            raise PermissionError('fixture')
        return 'unchanged contents'
    assert retry_permission(transient, delay=0) == 'unchanged contents'
    assert len(calls) == 3
    def permanent():
        raise PermissionError('fixture')
    try:
        retry_permission(permanent, attempts=2, delay=0)
    except PermissionError:
        pass
    else:
        raise AssertionError('Permanent storage failure was suppressed')
    def invalid():
        raise ValueError('fixture')
    try:
        retry_permission(invalid, delay=0)
    except ValueError:
        pass
    else:
        raise AssertionError('Non-storage error was suppressed')


if __name__ == '__main__':
    self_check()
    if '--self-check' in sys.argv:
        print('Storage retry regression checks passed.')
        raise SystemExit(0)
    config = json.loads((OUT/'execution.json').read_text(encoding='utf-8'))
    assert all(hashlib.sha256((ROOT/p).read_bytes()).hexdigest() == h for p,h in config['hashes'].items()), 'Frozen method changed'
    amendment = dict(utc=datetime.now(timezone.utc).isoformat(), kind='infrastructure-only',
        policy='Retry transient PermissionError for reads/writes under this output directory only; retain all responses, failures and method parameters.',
        entry_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
    history = OUT/'storage_recovery';history.mkdir(exist_ok=True)
    (history/(datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')+'.json')).write_text(json.dumps(amendment,indent=2),encoding='utf-8')
    original_read, original_write = Path.read_text, Path.write_text
    def read(path, *args, **kwargs):
        if path.resolve().is_relative_to(OUT):
            return retry_permission(original_read, path, *args, **kwargs)
        return original_read(path, *args, **kwargs)
    def write(path, *args, **kwargs):
        if path.resolve().is_relative_to(OUT):
            return retry_permission(original_write, path, *args, **kwargs)
        return original_write(path, *args, **kwargs)
    Path.read_text, Path.write_text = read, write
    import run_current_comparison as runner
    original_call = runner.ResumableCalls.__call__
    def replay_json_order(self, prompt):
        cursor = self.cursor
        while cursor < len(self.entries) and 'error_type' in self.entries[cursor]:
            cursor += 1
        if cursor < len(self.entries):
            old = self.entries[cursor]['prompt']
            if old != prompt and canonical_json_lines(old) == canonical_json_lines(prompt):
                runner.save(self.folder / f'replay_json_order_{cursor}.json', {
                    'utc': datetime.now(timezone.utc).isoformat(),
                    'reason': 'Only JSON object key order differs; replay the original exact request and response.',
                    'original_sha256': hashlib.sha256(old.encode()).hexdigest(),
                    'current_sha256': hashlib.sha256(prompt.encode()).hexdigest()})
                prompt = type(prompt)(old)
        try:
            return original_call(self, prompt)
        except RuntimeError as exc:
            import traceback
            runner.save(self.folder / 'replay_runtime_error.json', {
                'utc': datetime.now(timezone.utc).isoformat(),
                'message': str(exc) if str(exc).startswith('Cached ') else type(exc).__name__,
                'stack': traceback.format_tb(exc.__traceback__)})
            if cursor < len(self.entries):
                runner.save(self.folder / 'replay_prompt_comparison.json', {
                    'historical': self.entries[cursor]['prompt'], 'current': prompt})
            raise
    runner.ResumableCalls.__call__ = replay_json_order
    sys.argv = [str(Path(runner.__file__)), '--out', str(OUT.relative_to(ROOT)), '--model', config['model'],
                '--repetitions', str(config['repetitions']), '--workers', str(config['concurrency']['job_workers'])]
    runner.cli()
