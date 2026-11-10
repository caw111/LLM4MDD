"""合并原始生成审计和独立重放测量；不覆写原实验分数。"""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path

from experiment_metrics import paired, summarize
from summarize_contract_development import audit, ROOT


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def combine(original, replay):
    original=original.resolve();replay=replay.resolve()
    initial=audit(original)
    rescored=read(replay/'report.json')
    execution=read(original/'execution.json')
    if not initial['complete'] or not all(rescored.get(k) for k in
            ('complete','source_snapshot_unchanged','originals_unchanged')):
        raise ValueError('Both generation and replay audits must be complete')
    if (ROOT/rescored['original']).resolve()!=original or rescored['original_execution_sha256']!=sha(original/'execution.json'):
        raise ValueError('Replay belongs to another generation run')
    for name,digest in rescored['original_file_hashes'].items():
        if sha(ROOT/name)!=digest:raise ValueError('Original changed: '+name)
    for name,digest in rescored['oracle_source_hashes'].items():
        if sha(replay/'oracle_snapshot'/name)!=digest:raise ValueError('Oracle snapshot changed: '+name)
    expected={(c['id'],r,a) for c in execution['cases'] for r in range(1,execution['repetitions']+1)
              for a in ('direct','direct_feedback','workflow')}
    rows=rescored['rows']
    raw={(r['case'],r['repeat'],r['arm']):r for p in original.glob('*/repeat_*/results.json') for r in read(p)}
    for row in rows:
        before=raw[(row['case'],row['repeat'],row['arm'])]
        for key in ('compile_ok','artifact','artifact_sha256','llm_calls','tokens','reported_cost_usd','llm_seconds'):
            if row.get(key)!=before.get(key):raise ValueError('Replay altered generation evidence: '+key)
    measured=summarize(rows,expected_keys=expected)
    if measured['missing_records']:raise ValueError('Replay records missing')
    failures=[]
    for row in rows:
        if row.get('compile_ok') and row['functional_status']=='passed':continue
        entry={k:row.get(k) for k in ('case','repeat','arm','compile_ok','functional_status','failure_stage','error','artifact')}
        if row.get('replay_directory'):
            log=ROOT/row['replay_directory']/'host_oracle/trace.log'
            if log.is_file():entry['independent_diagnostic']=log.read_text(encoding='utf-8').strip()
        failures.append(entry)
    diagnostics=[]
    for path in original.glob('*/repeat_*/sysml_generation_failures.json'):
        diagnostics.append(dict(case=path.parent.parent.name,repeat=int(path.parent.name.split('_')[-1]),
            attempts=[dict(attempt=a['attempt'],codes=[i.get('code') for i in a['issues']],
                           messages=[i.get('message') for i in a['issues']],no_progress=a.get('no_progress',False))
                      for a in read(path)['attempts']]))
    return dict(run=str(original.relative_to(ROOT)),replay=str(replay.relative_to(ROOT)),
        original_audit_complete=initial['complete'],replay_complete=True,
        original_execution_sha256=sha(original/'execution.json'),replay_report_sha256=sha(replay/'report.json'),
        original_files_unchanged=True,oracle_snapshot_valid=True,new_llm_calls_for_replay=rescored['new_llm_calls'],
        arms=measured['arms'],model=initial['model'],workflow_gates_all_tasks=initial['workflow_gates_all_tasks'],
        physical_usage=initial['physical_usage'],wall_seconds=read(original/'summary.json')['seconds'],
        call_limits=execution['call_limits'],
        contrasts=[dict(left='workflow',right=arm,compile=paired(rows,'workflow',arm,'compile'),
                        functional=paired(rows,'workflow',arm,'functional')) for arm in ('direct','direct_feedback')],
        changed_measurements=[{k:r[k] for k in ('case','repeat','arm','original_functional_status','functional_status')}
                              for r in rows if r['functional_status']!=r['original_functional_status']],
        failures=failures,exhausted_model_diagnostics=diagnostics,
        failure_counts=dict(Counter((r.get('failure_stage') or 'functional_after_compile' if r['compile_ok'] else
                                     r.get('failure_stage') or 'target_compile') for r in failures)),
        interpretation=f'{len(execution["cases"])} development tasks, {execution["repetitions"]} paired repeats per arm. Host command traces, not physical/AVR numeric proof. '
                       'Measurement repairs are not generation improvements. Unequal call budgets and shared baseline prefixes; '
                       'allocated usage must not be summed as physical cost. No population non-inferiority claim.')


if __name__=='__main__':
    parser=argparse.ArgumentParser()
    for name in ('original','replay','out'):parser.add_argument('--'+name,required=True)
    args=parser.parse_args();target=ROOT/args.out
    if target.exists():raise ValueError('Preserve the existing analysis; use a new output path')
    result=combine(ROOT/args.original,ROOT/args.replay)
    result['analysis_script_sha256']=sha(Path(__file__))
    target.parent.mkdir(parents=True,exist_ok=True)
    target.write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
    print(json.dumps({k:result[k] for k in ('physical_usage','wall_seconds','contrasts','failure_counts')},ensure_ascii=False))
