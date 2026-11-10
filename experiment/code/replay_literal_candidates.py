"""Recheck archived literal-format failures with zero generation calls."""
import argparse
import hashlib
import json
from pathlib import Path
import sys
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from app.services.sysml_assembly import assemble_sysml
from app.services.sysml_compiler import validate_executable_sysml
from app.services.behavior_contract import check_contract
from run_contract_development import lower_verified, adapt_verified, compiled
from method_snapshot import runtime_identity


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def replay(root, output):
    execution = json.loads((root/'execution.json').read_text(encoding='utf-8'))
    cases = {c['id']: c for c in execution['cases']}
    if any(c.get('split') != 'development' for c in cases.values()):
        raise ValueError('Replay is restricted to archived development tasks')
    output.mkdir(parents=True, exist_ok=False)
    sources = [*sorted((ROOT/'app/services').glob('*.py')),
               *[p for p in sorted((ROOT/'experiment/development/oracles').glob('*')) if p.is_file()]]
    sources += [ROOT/'tools/sysml/SysMLBridge.java', Path(__file__).resolve(), ROOT/'app/main.py',
                *[ROOT/'experiment/code'/name for name in ('run_contract_development.py', 'run_formal_experiment.py',
                    'run_controlled_experiment.py', 'method_snapshot.py')]]
    hashes = {str(p.relative_to(ROOT)): digest(p) for p in sources}
    for p in sources:
        target = output/'source_snapshot'/p.relative_to(ROOT)
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(p.read_bytes())
    originals = {str((root/'execution.json').relative_to(ROOT)): digest(root/'execution.json')}
    rows = []
    for path in sorted(root.glob('*/repeat_*/sysml_generation_failures.json')):
        report = json.loads(path.read_text(encoding='utf-8'))
        for attempt in report['attempts']:
            if not any(i.get('code') == 'BUNDLE_FORMAT' and i.get('message', '').startswith('Expressions must be scalar strings') for i in attempt.get('issues', [])):
                continue
            contract_path = path.parent/'contract.json'
            for p in (path, contract_path):
                originals[str(p.relative_to(ROOT))] = digest(p)
            row = dict(case=path.parent.parent.name, repeat=path.parent.name, attempt=attempt['attempt'],
                       original_report=str(path.relative_to(ROOT)), original_issues=attempt['issues'])
            try:
                response = attempt['response'].strip()
                if response.startswith('```json\n') and response.endswith('\n```'):
                    response = response[8:-4]
                bundle = json.loads(response)
                contract = json.loads(contract_path.read_text(encoding='utf-8'))
                source = assemble_sysml(bundle['controller'], contract)
                candidate = output/row['case']/row['repeat']/f"candidate_{row['attempt']}.sysml"
                candidate.parent.mkdir(parents=True, exist_ok=True)
                candidate.write_text(source, encoding='utf-8')
                result = validate_executable_sysml(source)
                row.update(stages=result['stages'], issues=result['issues'])
                if result['ok']:
                    row['coverage'] = check_contract(result['ir'], contract)
                row['model_accepted'] = bool(result['ok'] and row.get('coverage', {}).get('status') == 'structural_pass')
                if row['model_accepted']:
                    target = candidate.with_suffix('')
                    with patch('app.main._call_llm', side_effect=AssertionError('No LLM in literal replay')):
                        generated = lower_verified(source, contract, result['ir'], target/'lowered')
                        code = adapt_verified(generated, contract, target/'lowered')
                        row['independent_execution'], _ = compiled(cases[row['case']], target/'verified', code)
            except (ValueError, TypeError) as exc:
                row.update(model_accepted=False, assembly_error=str(exc))
            rows.append(row)
    result = dict(rows=rows, new_llm_calls=0, runtime=runtime_identity(), source_hashes=hashes, original_hashes=originals,
        source_unchanged=all(digest(ROOT/p) == h for p,h in hashes.items()),
        originals_unchanged=all(digest(ROOT/p) == h for p,h in originals.items()),
        interpretation='Replayed archived candidates only; no experiment scores changed. Model acceptance is not independent functional success.')
    (output/'report.json').write_text(json.dumps(result, ensure_ascii=False, indent=2), encoding='utf-8')
    print(json.dumps({k:v for k,v in result.items() if k not in ('rows','source_hashes','original_hashes')}, ensure_ascii=False))
    print(json.dumps([dict(case=r['case'],repeat=r['repeat'],attempt=r['attempt'],model_accepted=r['model_accepted'],
        stages=r.get('stages'),issues=[i.get('code') for i in r.get('issues', [])],
        coverage=r.get('coverage', {}).get('status'),assembly_error=r.get('assembly_error'),
        independent_execution=r.get('independent_execution')) for r in rows], ensure_ascii=False))


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--run', required=True)
    parser.add_argument('--out', required=True)
    args = parser.parse_args()
    replay((ROOT/args.run).resolve(), (ROOT/args.out).resolve())
