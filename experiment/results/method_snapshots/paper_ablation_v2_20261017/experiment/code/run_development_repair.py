"""Development-only verification of the repaired workflow; never overwrite formal results."""
import argparse
import concurrent.futures as futures
import hashlib
import json
import os
from pathlib import Path
import run_formal_experiment as runner


def main():
    os.chdir(runner.ROOT)
    parser = argparse.ArgumentParser()
    parser.add_argument('--out', default='experiment/output/development_repair_v4_20261012')
    parser.add_argument('--limit', type=int, default=2, help='Small development batch; 0 selects all development cases')
    parser.add_argument('--case', action='append', default=[], help='Explicit development case ID; repeatable')
    parser.add_argument('--model', default=os.getenv('OPENAI_MODEL', 'openai/gpt-5.4-nano'))
    parser.add_argument('--repetitions', type=int, default=1)
    parser.add_argument('--workers', type=int, default=1)
    parser.add_argument('--prepare-only', action='store_true')
    args = parser.parse_args()
    out = (runner.ROOT / args.out).resolve()
    out.relative_to((runner.ROOT / 'experiment/output').resolve())
    manifest = json.loads((runner.ROOT / 'experiment/formal/manifest.json').read_text(encoding='utf-8'))
    cases = [case for case in manifest['cases'] if case['split'] == 'development']
    assert len(cases) == 60 and len({case['id'] for case in cases}) == 60
    if args.limit < 0 or args.repetitions < 1 or args.workers < 1:
        parser.error('Invalid limit, repetitions or workers')
    if args.case:
        if not set(args.case) <= {case['id'] for case in cases}:
            parser.error('--case must identify development cases; test cases are forbidden')
        cases = [case for case in cases if case['id'] in args.case]
    elif args.limit:
        cases = cases[:args.limit]
    files = ['tools/prompts/contract_controller.prompt.md', 'app/services/sysml_assembly.py', 'tools/prompts/contract_sysml2.prompt.md', 'app/services/hardware_adapter.py', 'app/services/interface_planner.py', 'app/services/ir_contract.py', 'app/services/ir_prompt.py', 'app/main.py', 'app/services/sysml.py', 'app/services/sysml_compiler.py', 'app/services/arduino_adapter.py',
             'tools/prompts/system_sysml2.prompt.md', 'experiment/code/run_formal_experiment.py',
             'experiment/code/formal_ir.py', 'experiment/code/run_development_repair.py',
             'app/services/behavior_contract.py', 'app/services/failure_analysis.py',
             'experiment/code/run_controlled_experiment.py', 'tools/sysml/SysMLBridge.java',
             'experiment/code/method_snapshot.py', 'experiment/code/experiment_metrics.py']
    from app.services.sysml import VERSION
    from experiment_metrics import CALL_BUDGETS
    from method_snapshot import runtime_identity
    execution = {'cases': [case['id'] for case in cases], 'model': args.model, 'repetitions': args.repetitions,
                 'manifest_sha256': hashlib.sha256((runner.ROOT/'experiment/formal/manifest.json').read_bytes()).hexdigest(),
                 'validator_version': VERSION, 'temperature': 0.2, 'max_tokens': 6000,
                 'repair_budgets': {'model': 2, 'adapter': 2, 'compile': 2},
                 'max_method_calls': CALL_BUDGETS,
                 'runtime': runtime_identity(),
                 'arms': list(runner.ARMS), 'scope': 'development only; compile is not functional correctness',
                 'hashes': {name: hashlib.sha256((runner.ROOT/name).read_bytes()).hexdigest() for name in files}}
    if (out/'execution.json').exists():
        assert json.loads((out/'execution.json').read_text(encoding='utf-8')) == execution, 'Method changed: use a new output directory'
    else:
        runner.save(out/'execution.json', execution)
        for name in files:
            target = out/'method_snapshot'/name
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes((runner.ROOT/name).read_bytes())
    if args.prepare_only:
        print(f'Prepared {len(cases)} development tasks x {args.repetitions} repeats x 6 arms; no model calls were made.', flush=True)
        return
    if not runner.main._llm_api_key:
        runner.save(out/'blocked.json', {'stage': 'preflight', 'reason': 'API key is not configured',
                                       'model_calls': 0, 'formal_experiment_started': False})
        raise SystemExit('Development run not started: API key is not configured; execution snapshot retained.')
    pending_cases = iter((case, repeat) for repeat in range(1, args.repetitions + 1) for case in cases)
    results = []
    blocked = False
    with futures.ThreadPoolExecutor(max_workers=args.workers) as pool:
        active = {}
        def submit():
            job = next(pending_cases, None)
            if job is not None:
                case, repeat = job
                active[pool.submit(runner.evaluate, case, repeat, out, args.model)] = case['id']
        for _ in range(args.workers): submit()
        while active:
            done, _ = futures.wait(active, return_when=futures.FIRST_COMPLETED)
            for task in done:
                case_id = active.pop(task)
                rows = task.result()
                results.extend(rows)
                blocked = blocked or any(row.get('infrastructure_error') for row in rows)
                runner.save(out/'results.json', results)
                print(case_id, 'completed', len(results), 'records', flush=True)
                if not blocked: submit()
    summary = {'complete': len(results) == len(cases)*args.repetitions*6 and not blocked, 'records': len(results),
               'infrastructure_blocked': blocked, 'functional_testing': 'not performed',
               'groups': {arm: {'n': sum(r['arm'] == arm for r in results),
                                'infrastructure_failures': sum(r['arm'] == arm and bool(r.get('infrastructure_error')) for r in results),
                                'evaluated_method_records': sum(r['arm'] == arm and not r.get('infrastructure_error') for r in results),
                                'compile_pass': sum(r['arm'] == arm and r['compile_ok'] for r in results)} for arm in runner.ARMS}}
    runner.save(out/'summary.json', summary)
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
