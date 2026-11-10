"""Small observed-output witnesses for the untimed-source review; no simulation rerun."""
import gzip,json,os
import run_picsimlab_comparison_groups as run

def main():
 os.nice(15);rows=[]
 for case in run.load(run.OUT/'manifest.json')['cases']:
  folder=run.OUT/'cases'/case['id'];path=folder/'case_execution.corrected_timing.json'
  if not path.exists():continue
  execution=run.load(path);row=execution['plans'][0];checks=row['checks'];times=sorted({c['test']['native_eventual_to_ms'] for c in checks if 'native_eventual_to_ms' in c['test']});wanted={c['test']['kind'] for c in checks};latest={}
  with gzip.open(run.OUT/row['result_directory']/'observations.jsonl.gz','rt') as f:
   for line in f:
    if not any(f'"type": "{kind}"' in line for kind in wanted):continue
    e=json.loads(line);key=e.get('name') if e['type']=='display' else e.get('pin');t=e.get('at_ms',0)
    for end in times:
     k=(end,e['type'],key)
     if t<=end and (k not in latest or t>=latest[k]['at_ms']):latest[k]=e
  groups=[]
  for end in times:
   values=[]
   for check in checks:
    c=check['test']
    if c.get('native_eventual_to_ms')!=end:continue
    k=c.get('name') if c['kind']=='display' else run.sim.assertions.pin(c['pin'],case['fqbn']);e=latest.get((end,c['kind'],k));v=e.get('text') if e and c['kind']=='display' else e.get('value') if e else None
    values.append(dict(kind=c['kind'],signal=k,expected=c.get('expected'),actual=v))
   groups.append(dict(at_ms=end,values=values))
  rows.append(dict(key=case['id'],case=case['original_case'],result_directory=row['result_directory'],samples=groups))
 run.save(run.OUT/'untimed_observed_witnesses.json',rows);print('Witnesses saved',len(rows),flush=True)
if __name__=='__main__':main()
