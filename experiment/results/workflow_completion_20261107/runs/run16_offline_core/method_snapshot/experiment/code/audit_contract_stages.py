"""Separate parser, lowering and public requirements evidence without rewriting runs."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path

STAGES = {'reference_validation': 'sysml_generation', 'sysml_to_ir': 'sysml_to_ir',
          'ir_to_il': 'ir_to_il'}


def candidate_evidence(attempt):
    if attempt is None:
        return {name: 'unrecorded' for name in (*STAGES, 'public_requirements')}
    stages = attempt.get('stages', {})
    errors = [i for i in attempt.get('issues', []) if i.get('severity', 'error') == 'error']
    before_parser = not stages and any(i.get('code') in ('BUNDLE_FORMAT', 'EXPRESSION_SYNTAX') for i in errors)
    evidence = {}
    for name, key in STAGES.items():
        value = stages.get(key)
        evidence[name] = ('passed' if value is True else 'failed' if value is False else
                          'not_reached' if key in stages or before_parser else 'unrecorded')
    coverage = attempt.get('coverage', {}).get('status')
    evidence['public_requirements'] = ('passed' if coverage == 'structural_pass' else
        'failed' if coverage == 'failed' else 'not_reached' if coverage == 'not_run' else 'unrecorded')
    return evidence


def rejection(attempt):
    if not attempt or attempt.get('ok'):
        return None
    evidence = candidate_evidence(attempt)
    for name, status in evidence.items():
        if status == 'failed':
            return name
    if evidence['reference_validation'] == 'not_reached':
        return 'bundle_or_controller_assembly'
    return 'unclassified'


def audit(root):
    root = Path(root).resolve()
    read = lambda p: json.loads(p.read_text(encoding='utf-8'))
    execution = read(root / 'execution.json')
    cases = []
    hashes = {}
    candidate_failures = Counter()
    for case in execution['cases']:
        for repeat in range(1, execution['repetitions'] + 1):
            folder = root / case['id'] / f'repeat_{repeat}'
            files = [p for p in (folder/'model_validation.json', folder/'sysml_generation_failures.json') if p.is_file()]
            if len(files) > 1:
                raise ValueError(f'Ambiguous model reports: {folder}')
            attempts = read(files[0]).get('attempts', []) if files else []
            upstream_failure = None
            results = folder/'results.json'
            if results.is_file():
                workflow = [r for r in read(results) if r.get('arm')=='workflow']
                if len(workflow)>1:
                    raise ValueError(f'Ambiguous workflow results: {folder}')
                if workflow and workflow[0].get('failure_stage')=='interface_contract' and workflow[0].get('compile_ok') is False:
                    if files:
                        raise ValueError(f'Planning failure contradicts model evidence: {folder}')
                    upstream_failure='interface_contract'
                    hashes[str(results.relative_to(root))]=hashlib.sha256(results.read_bytes()).hexdigest()
            if files:
                hashes[str(files[0].relative_to(root))] = hashlib.sha256(files[0].read_bytes()).hexdigest()
            for attempt in attempts:
                reason = rejection(attempt)
                if reason:
                    candidate_failures[reason] += 1
            last = attempts[-1] if attempts else None
            not_reached={name:'not_reached' for name in (*STAGES,'public_requirements')}
            cases.append(dict(case=case['id'], repeat=repeat, attempts=len(attempts),
                report=str(files[0].relative_to(root)) if files else None,
                accepted=bool(last and last.get('ok')), final_rejection=rejection(last),
                upstream_failure=upstream_failure,
                first_candidate=not_reached if upstream_failure else candidate_evidence(attempts[0] if attempts else None),
                final_candidate=not_reached if upstream_failure else candidate_evidence(last),
                final_issues=last.get('issues', []) if last else [],
                any_candidate_passed_reference=any(a.get('stages', {}).get('sysml_generation') is True for a in attempts)))
    gates = {}
    for selection in ('first_candidate', 'final_candidate'):
        gates[selection] = {}
        for stage in (*STAGES, 'public_requirements'):
            counts = Counter(c[selection][stage] for c in cases)
            gates[selection][stage] = dict(denominator=len(cases),
                **{status: counts[status] for status in ('passed', 'failed', 'not_reached', 'unrecorded')},
                observed_pass_rate=counts['passed']/len(cases) if cases else None)
    return dict(planned_workflow_tasks=len(cases), recorded_model_reports=sum(bool(c['report']) for c in cases),
        upstream_rejections=dict(Counter(c['upstream_failure'] for c in cases if c['upstream_failure'])),
        gates_all_planned_tasks=gates, accepted_models=sum(c['accepted'] for c in cases),
        final_model_rejections=dict(Counter(c['final_rejection'] for c in cases if c['final_rejection'])),
        all_candidate_rejections=dict(candidate_failures), cases=cases, evidence_sha256=hashes,
        interpretation='Stage counts concern first or final candidate, as labelled. All planned task-repeats remain in the denominator. '
        'Not reached and unrecorded are not measured failures. Reference validation includes syntax and semantic diagnostics. '
        'Public requirements passing means bounded contract checks only, not independent functional success. '
        'A previous passing candidate does not make a rejected final candidate pass. Raw run records are unchanged.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--run', required=True)
    parser.add_argument('--out', required=True)
    args = parser.parse_args()
    report = audit(args.run)
    report['auditor_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    output = Path(args.out)
    output.parent.mkdir(parents=True, exist_ok=True)
    with output.open('x', encoding='utf-8') as stream:
        json.dump(report, stream, ensure_ascii=False, indent=2)
    print(json.dumps({k: v for k, v in report.items() if k not in ('cases', 'evidence_sha256')}, ensure_ascii=False))
