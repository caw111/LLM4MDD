"""Verify/replay the portable targeted-completion evidence without model calls."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil

ROOT=Path(__file__).resolve().parents[2]
BUNDLE=ROOT/'experiment/results/workflow_completion_20261107'


def read(path):return json.loads(path.read_text(encoding='utf8'))


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case',action='append')
    parser.add_argument('--all',action='store_true')
    parser.add_argument('--output',type=Path,default=ROOT/'experiment/replay-output/workflow_completion_20261107')
    args=parser.parse_args()
    checksums=read(BUNDLE/'checksums.json')
    for name,expected in checksums.items():
        path=BUNDLE/name;path.resolve().relative_to(BUNDLE)
        assert hashlib.sha256(path.read_bytes()).hexdigest()==expected,name
    rows=read(BUNDLE/'cases.json');summary=read(BUNDLE/'summary.json')
    assert len(rows)==summary['targeted']==28
    assert sum(r['compile_ok'] for r in rows)==summary['target_compile']
    assert sum(r['core_status']=='core_scenarios_passed' for r in rows)==summary['target_core']
    for row in read(BUNDLE/'all296_compilation_sources.json'):
        source=ROOT/row['source'];source.resolve().relative_to(ROOT)
        assert hashlib.sha256(source.read_bytes()).hexdigest()==row['sha256'],row['case']
    print('Verified',len(checksums),'completion evidence hashes and296 compilation-source mappings.',flush=True)
    selected={r['case']:r for r in rows}
    requested=[r['case'] for r in rows if r['core_status']=='core_scenarios_passed'] if args.all else args.case or []
    if not requested:return
    import behavior_full_host as host
    import run_behavior_pilot10 as pilot
    import run_workflow267_host_smoke as base
    compiler=shutil.which('g++')
    if not compiler:raise RuntimeError('Put a C++17 g++ compiler on PATH.')
    base.GXX=Path(compiler);pilot.OUT=ROOT/'experiment/results/host_templates'
    results=[]
    for name in requested:
        if name not in selected:parser.error('Unknown case: '+name)
        row=selected[name];folder=BUNDLE/'cases'/name
        plans=read(folder/'test_plans.json') if (folder/'test_plans.json').exists() else []
        if not plans:raise RuntimeError('No executable new core plan for '+name)
        host.OUT=args.output/name;host.FIXTURE=host.OUT/'fixture';host.fixture()
        board=read(folder/'input.json')['fqbn'];cpp=folder/f"attempt_{row['selected_attempt']}"/'compiled_candidate.cpp'
        statuses=[]
        for entry in plans:
            host.case_files(name,entry['plan'],board)
            result=host.execute(cpp,board,name,entry['plan'],host.OUT/entry['name'])
            statuses.append(result['status']);print(name,entry['name'],result['status'],flush=True)
        results.append(dict(case=name,status='passed' if all(s=='passed' for s in statuses) else 'unverified_or_failed',plans=statuses))
    args.output.mkdir(parents=True,exist_ok=True)
    (args.output/'replay_summary.json').write_text(json.dumps(results,indent=2),encoding='utf8')
    if not all(r['status']=='passed' for r in results):raise SystemExit(1)


if __name__=='__main__':main()
