"""Audit paired contract-first development records without changing raw results."""
import argparse
import hashlib
import json
from pathlib import Path

from experiment_metrics import summarize, paired

ROOT = Path(__file__).resolve().parents[2]
ARMS = ('direct', 'direct_feedback', 'workflow')


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def audit(root):
    execution = read(root/'execution.json')
    rows = [r for p in root.glob('*/repeat_*/results.json') for r in read(p)]
    expected = {(case['id'], repeat, arm) for case in execution['cases']
                for repeat in range(1, execution['repetitions']+1) for arm in ARMS}
    reports = [read(p) for pattern in ('*/repeat_*/model_validation.json', '*/repeat_*/sysml_generation_failures.json')
               for p in root.glob(pattern)]
    result = summarize(rows, reports, expected_keys=expected)
    planned_workflow = {(case, repeat) for case, repeat, arm in expected if arm == 'workflow'}
    accepted_contracts = accepted_models = 0
    for case, repeat in planned_workflow:
        folder = root/case/f'repeat_{repeat}'
        contract_report = folder/'contract_planning.json'
        model_report = folder/'model_validation.json'
        if contract_report.is_file() and read(contract_report).get('contract_sha256'):
            accepted_contracts += 1
        if model_report.is_file():
            attempts = read(model_report).get('attempts', [])
            accepted_models += bool(attempts and attempts[-1].get('ok'))
    def all_task_gate(passes):
        return dict(passes=passes, denominator=len(planned_workflow), rate=passes/len(planned_workflow) if planned_workflow else None)
    result['workflow_gates_all_tasks'] = dict(interface_contract=all_task_gate(accepted_contracts),
        accepted_sysml_and_ir=all_task_gate(accepted_models),
        note='Uses every planned workflow task, including failures before SysML. The model section above is conditional on recorded model attempts.')
    result['max_method_calls_excluding_transport_retries'] = execution['call_limits']
    result['contrasts'] = [dict(left='workflow', right=arm,
                               compile=paired(rows,'workflow',arm,'compile'),
                               functional=paired(rows,'workflow',arm,'functional')) for arm in ('direct','direct_feedback')]
    entries = [e for p in root.glob('*/repeat_*/*_calls/calls.json') for e in read(p)]
    result['physical_usage'] = dict(calls=len(entries),
        tokens=sum((e.get('usage') or {}).get('total_tokens',0) for e in entries),
        reported_cost_usd=sum((e.get('usage') or {}).get('cost',0) or 0 for e in entries),
        calls_with_reported_cost=sum((e.get('usage') or {}).get('cost') is not None for e in entries),
        llm_seconds=sum(e.get('seconds',0) for e in entries))
    result['artifact_audit'] = []
    for row in rows:
        if not row.get('artifact'):
            continue
        path=(ROOT/row['artifact']).resolve();path.relative_to(ROOT)
        exists=path.is_file()
        result['artifact_audit'].append(dict(case=row['case'],repeat=row['repeat'],arm=row['arm'],exists=exists,
            bytes_sha256=hashlib.sha256(path.read_bytes()).hexdigest() if exists else None,
            recorded_bytes_match=(exists and hashlib.sha256(path.read_bytes()).hexdigest()==row['artifact_sha256']) if 'artifact_source_sha256_lf' in row else None,
            normalized_source_matches=exists and hashlib.sha256(path.read_text(encoding='utf-8').encode('utf-8')).hexdigest()==row.get('artifact_source_sha256_lf',row['artifact_sha256'])))
    result['snapshot_audit'] = {name: (root/'method_snapshot'/name).is_file() and
        hashlib.sha256((root/'method_snapshot'/name).read_bytes()).hexdigest()==digest for name,digest in execution['hashes'].items()}
    result['source_hash_convention'] = ('New records store artifact_sha256 over file bytes and artifact_source_sha256_lf over LF-normalized source. '
        'Legacy records without artifact_source_sha256_lf stored LF-normalized source in artifact_sha256. Both conventions are audited without changing raw records.')
    result['interpretation'] = ('Diagnostic hardware-command development tasks, not the full task distribution. '
        'Bootstrap intervals based on a small number of task clusters, including degenerate [0,0], do not establish population non-inferiority. '
        'Host traces validate observable logic; they do not validate physical electronics. Public timing interpretations were supplied equally to all arms.')
    result['complete'] = not result['missing_records'] and all(a['normalized_source_matches'] and a['recorded_bytes_match'] is not False for a in result['artifact_audit']) and all(result['snapshot_audit'].values())
    result['functional_verification_complete'] = all(a['functional']['complete'] for a in result['arms'].values())
    result['pipeline_scope'] = execution.get('pipeline_scope', 'Compiler pipeline including interface planning; optional UI module-diagram planning excluded from calls/cost.')
    return result


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--out',required=True);args=parser.parse_args()
    folder=(ROOT/args.out).resolve();report=audit(folder)
    report['summarizer_sha256']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    (folder/'audited_summary.json').write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf-8')
    print(json.dumps({arm:{'compile':value['compile'],'functional':value['functional']['all_tasks']} for arm,value in report['arms'].items()},ensure_ascii=False))
