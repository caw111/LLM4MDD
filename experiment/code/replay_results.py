"""Verify the published evidence or replay its recorded core behavior plans."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[2]
BUNDLE = ROOT / 'experiment/results'


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def verify():
    checksums = read(BUNDLE / 'checksums.json')
    for name, expected in checksums.items():
        path = BUNDLE / name
        assert path.resolve().is_relative_to(BUNDLE.resolve()), name
        assert hashlib.sha256(path.read_bytes()).hexdigest() == expected, name
    cases = read(BUNDLE / 'final_cases.json')
    groups = read(BUNDLE / 'groups.json')
    assert len(cases) == len({case['case'] for case in cases}) == 296
    assert len(groups) == 13 * 296
    for row in groups:
        for key, digest_key in (('source', 'source_sha256'), ('cpp', 'preprocessed_sha256')):
            if row[key] and row.get(digest_key):
                assert Path(row[key]).stem == row[digest_key], (row['group'], row['case'], key)
    assert sum(case['confirmed_core_pass'] for case in cases) == 284
    summary = read(BUNDLE / 'summary/repair_summary.json')
    assert summary['overall']['confirmed_core_passes'] == 284
    assert summary['overall']['direct_archive_recoveries'] == 16
    for case in cases:
        if case['confirmed_core_pass']:
            assert case['source'] and (BUNDLE / case['source']).is_file(), case['case']
            if not any(e['status'] == 'passed' for e in case['evaluations']):
                review = read(BUNDLE / 'cases' / case['case'] / 'requirement_review.json')
                assert review['verdict'] == 'core_pass' and review.get('reason'), case['case']
                assert (BUNDLE / 'cases' / case['case'] / 'baseline/trace.jsonl.gz').is_file(), case['case']
    print(f'Verified {len(checksums)} file hashes, 13 comparison arms, 296 final tasks and 284 recorded core verdicts.')
    return cases


def replay(case, output, compile_target):
    import behavior_full_host as host
    import run_behavior_pilot10 as pilot
    import run_workflow267_host_smoke as base
    compiler = os.environ.get('CXX') or shutil.which('g++')
    if not compiler:
        raise RuntimeError('Set CXX to a C++17 g++ compiler, or put g++ on PATH.')
    base.GXX = Path(compiler)
    pilot.OUT = BUNDLE / 'host_templates'
    host.OUT = output
    host.FIXTURE = output / 'fixture'
    host.fixture()
    if compile_target:
        cli = shutil.which('arduino-cli')
        if not cli:
            raise RuntimeError('Put arduino-cli on PATH to repeat target-board compilation.')
        sketch = output / case['case'] / 'arduino/sketch'
        sketch.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(BUNDLE / case['source'], sketch / 'sketch.ino')
        subprocess.run([cli, 'compile', '--fqbn', case['board'], str(sketch)], check=True)
    plans = [e for e in case['evaluations'] if e['plan'] and
             (e['status'] == 'passed' or e.get('engine') in ('trace_review', 'reviewed_assertions'))]
    if not plans:
        raise RuntimeError(f"No executable recorded passing plan for {case['case']}.")
    results = []
    for entry in plans:
        plan = read(BUNDLE / entry['plan'])
        if entry.get('engine') == 'pilot10':
            pilot.OUT = output / 'pilot'
            pilot.BASE_FIXTURE = BUNDLE / 'smoke_templates/fixture'
            fixture = pilot.make_fixture({case['case']: plan})
            source = output / case['case'] / 'pilot-source/sketch/sketch.ino'
            source.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(BUNDLE / case['source'], source)
            cpp = source.parent.parent / 'build/sketch/sketch.ino.cpp'
            cpp.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(BUNDLE / entry['cpp'], cpp)
            row = dict(source=str(source), source_sha256=pilot.sha(source), board=case['board'])
            result = pilot.execute(row, plan, fixture, case['case'])
            results.append(result['status'])
            print(case['case'], entry['stage'], result['status'])
            continue
        host.case_files(case['case'], plan, case['board'])
        result = host.execute(BUNDLE / entry['cpp'], case['board'], case['case'], plan,
                              output / case['case'] / entry['stage'])
        reproduced = result['status'] == 'passed'
        print(case['case'], entry['stage'], result['status'])
        if entry.get('engine') == 'trace_review' and result['status'] == 'unverified_no_checks':
            assert result.get('execution', {}).get('status') == 'completed'
            reproduced = True
            print('Execution reproduced; this case still requires the saved requirement review (no automated assertions).')
        if entry.get('engine') == 'reviewed_assertions' and result['status'] == 'failed':
            recorded = read(BUNDLE / entry['evidence'])
            failures = lambda data: {c['id'] for c in data['checks'] if not c['passed']}
            reproduced = failures(result) == failures(recorded)
            print('Recorded assertion failures reproduced; the saved review explains the core-function verdict.')
        results.append(reproduced)
    return all(status is True or status == 'passed' for status in results)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case', action='append', help='Replay a named case; repeat for multiple cases.')
    parser.add_argument('--compile', action='store_true', help='Also compile the selected sketch for its Arduino board.')
    parser.add_argument('--output', type=Path, default=ROOT / 'experiment/replay-output')
    args = parser.parse_args()
    cases = verify()
    selected = {case['case']: case for case in cases}
    requested = args.case or []
    unknown = set(requested) - selected.keys()
    if unknown:
        parser.error('Unknown case IDs: ' + ', '.join(sorted(unknown)))
    succeeded = True
    for name in requested:
        succeeded = replay(selected[name], args.output.resolve(), args.compile) and succeeded
    if not succeeded:
        raise SystemExit(1)


if __name__ == '__main__':
    main()
