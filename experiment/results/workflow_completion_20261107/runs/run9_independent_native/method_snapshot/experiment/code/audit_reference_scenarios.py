"""Audit all original PCEval oracles on reference programs before scoring models."""
from concurrent.futures import ThreadPoolExecutor,as_completed
from pathlib import Path
import json
from run_wokwi_case import run
from run_controlled_experiment import ROOT,save

manifest=json.loads((ROOT/'experiment/formal/manifest.json').read_text(encoding='utf-8'))
cases=[c for c in manifest['cases'] if c['dataset']=='PCEval']
root=ROOT/'experiment/formal/oracle_audit'
results=[]

def one(case):
    path=root/case['id']/'result.json'
    if path.exists():
        old=json.loads(path.read_text(encoding='utf-8'))
        if old.get('status')=='passed':return old
    try:return run(case['id'],None,root/case['id'])
    except Exception as exc:
        row={'case':case['id'],'status':'infrastructure_error','error_type':type(exc).__name__}
        save(path,row);return row

# Small batches avoid exhausting a provider quota after the first authorization error.
with ThreadPoolExecutor(max_workers=2) as pool:
    for i in range(0,len(cases),2):
        batch=[pool.submit(one,c) for c in cases[i:i+2]]
        for future in as_completed(batch):
            result=future.result();results.append(result)
            save(root/'summary.json',results)
            print(result['case'],result['status'],flush=True)
        if any(r['status'] in ('infrastructure_auth_or_quota','blocked_missing_token') for r in results[-2:]):
            raise SystemExit('Stopped on credential/quota blocker; completed results retained.')
print('Audit complete:',len(results),'reference projects',flush=True)
