"""Compare only matching tasks in two complete frozen development runs."""
import argparse
import hashlib
import json
from pathlib import Path
from experiment_metrics import summarize, paired
from summarize_contract_development import audit

ROOT = Path(__file__).resolve().parents[2]
ARMS = ('direct', 'direct_feedback', 'workflow')


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def compare(before, after):
    runs = [read(p/'execution.json') for p in (before, after)]
    selected = {c['id'] for c in runs[1]['cases']}
    if runs[0]['repetitions'] != runs[1]['repetitions']:
        raise ValueError('Use matching repeat counts')
    old = {c['id']:c for c in runs[0]['cases']}
    for case in runs[1]['cases']:
        if case['split'] != 'development' or old.get(case['id']) != case:
            raise ValueError('Development task identity changed')
        if runs[0]['public_interpretations'][case['id']] != runs[1]['public_interpretations'][case['id']]:
            raise ValueError('Public requirement interpretation changed')
    oracle_hashes = [{name.replace('\\','/'):value for name,value in run['hashes'].items()
                      if '/oracles/' in name.replace('\\','/')} for run in runs]
    if not oracle_hashes[0] or oracle_hashes[0] != oracle_hashes[1]:
        raise ValueError('Independent oracle sources must be identical; use a separate audited replay if they changed')
    expected = {(case,repeat,arm) for case in selected for repeat in range(1,runs[1]['repetitions']+1) for arm in ARMS}
    output = dict(selected_tasks=sorted(selected), repetitions=runs[1]['repetitions'],runs={}, evidence_sha256={})
    for label,folder,execution in zip(('before','after'),(before,after),runs):
        check = audit(folder)
        if not check['complete'] or not check['functional_verification_complete']:
            raise ValueError('Both complete runs must pass artifact and functional audits')
        rows = [r for r in read(folder/'results.json') if r['case'] in selected]
        reports = [read(p) for case in selected for name in ('model_validation.json','sysml_generation_failures.json')
                   for p in (folder/case).glob('repeat_*/'+name)]
        summary = summarize(rows,reports,expected_keys=expected)
        if summary['missing_records']:
            raise ValueError('Missing subset task records')
        summary['contrasts'] = [dict(left='workflow',right=arm,compile=paired(rows,'workflow',arm,'compile'),
            functional=paired(rows,'workflow',arm,'functional')) for arm in ('direct','direct_feedback')]
        summary['max_method_calls_excluding_transport_retries'] = execution['call_limits']
        summary['run'] = str(folder.relative_to(ROOT))
        entries = [e for case in selected for p in (folder/case).glob('repeat_*/*_calls/calls.json') for e in read(p)]
        summary['physical_usage'] = dict(calls=len(entries),tokens=sum((e.get('usage') or {}).get('total_tokens',0) for e in entries),
            reported_cost_usd=sum((e.get('usage') or {}).get('cost',0) or 0 for e in entries),
            llm_seconds=sum(e.get('seconds',0) for e in entries))
        output['runs'][label] = summary
        for path in (folder/'results.json',folder/'execution.json'):
            output['evidence_sha256'][str(path.relative_to(ROOT))] = hashlib.sha256(path.read_bytes()).hexdigest()
    output['interpretation'] = ('Diagnostic selection of previously failing development tasks. Comparisons use only identical tasks, '
        'public interpretations, repeats and independent oracle sources. Within-run paired intervals use the selected tasks as clusters. '
        'Separate LLM samples and multiple simultaneous changes prevent causal attribution of before/after differences. '
        'This subset cannot establish overall non-inferiority or replace held-out formal evaluation.')
    return output


if __name__ == '__main__':
    parser=argparse.ArgumentParser()
    for name in ('before','after','out'):parser.add_argument('--'+name,required=True)
    args=parser.parse_args();result=compare(ROOT/args.before,ROOT/args.after)
    result['script_sha256']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    with (ROOT/args.out).open('x',encoding='utf-8') as stream:json.dump(result,stream,ensure_ascii=False,indent=2)
    print(json.dumps({label:{arm:dict(compile=x['compile'],functional=x['functional']['all_tasks'])
        for arm,x in run['arms'].items()} for label,run in result['runs'].items()},ensure_ascii=False))
