"""Measure original and edited firmware sources against the same supplemental checks."""
from concurrent.futures import ThreadPoolExecutor
import hashlib
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'experiment/code'))
import behavior_full_host as host
from review_manual_repair_results import OUT, read, save


def check_file(path, digest):
    if hashlib.sha256(path.read_bytes()).hexdigest() != digest:
        raise ValueError(f'Artifact identity changed: {path}')


def replay(row):
    case = row['case']
    source_dir = Path(row['result_record']).parent
    folder = host.OUT / case
    output = folder / 'pair.json'
    if output.is_file():
        previous = read(output)
        if previous['edited_result'] == row['result_record'] and all(
            previous[stage] in ('passed', 'failed') for stage in ('before', 'after')
        ):
            return previous
    plan = read(source_dir / 'supplement_plan.json')
    original = read(source_dir / 'before/result.json')
    repaired = read(source_dir / 'supplement/result.json')
    assert original['board'] == repaired['board'] and plan['checks']
    host.case_files(case, plan, original['board'])
    statuses = {}
    for stage, record in [('before', original), ('after', repaired)]:
        cpp = Path(record['cpp'])
        check_file(cpp, record['cpp_sha256'])
        measured = host.execute(cpp, record['board'], case, plan, folder / stage)
        statuses[stage] = measured['status']
    pair = dict(case=case, dataset=row['dataset'], edited_result=row['result_record'],
                plan=str(source_dir / 'supplement_plan.json'), **statuses)
    save(output, pair)
    return pair


def main():
    host.OUT = OUT / 'supplement_pair_audit'
    host.FIXTURE = host.OUT / 'fixture'
    host.fixture()
    rows = [r for r in read(OUT / 'merged_results.json')
            if r['core_verdict'] == 'core_scenarios_passed' and r.get('code_changed')
            and r.get('supplement_status') == 'passed'
            and not (r.get('before_status') == 'failed' and r.get('after_status') == 'passed')]
    with ThreadPoolExecutor(max_workers=3) as pool:
        results = []
        for pair in pool.map(replay, rows):
            results.append(pair)
            print(pair['case'], pair['before'], pair['after'], flush=True)
            save(host.OUT / 'summary.json', dict(planned=len(rows), completed=len(results),
                 fail_to_pass=sum(r['before'] == 'failed' and r['after'] == 'passed' for r in results), rows=results))


if __name__ == '__main__':
    main()
