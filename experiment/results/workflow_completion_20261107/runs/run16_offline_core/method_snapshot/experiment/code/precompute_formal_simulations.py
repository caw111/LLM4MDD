"""One finite batch of hidden tests; never changes generation results or prompts."""
import json
from concurrent.futures import ThreadPoolExecutor,as_completed
from run_wokwi_case import run,ROOT

root=ROOT/'experiment/output/formal_20260928'
eligible={x['case'] for x in json.loads((ROOT/'experiment/formal/oracle_audit/classified_summary.json').read_text(encoding='utf-8'))['cases'] if x['category']=='passed'}
jobs={}
for p in root.glob('*/repeat_*/results.json'):
    rows=json.loads(p.read_text(encoding='utf-8'))
    if len(rows)!=6 or any(x.get('infrastructure_error') for x in rows):continue
    for row in rows:
        if row['case'] in eligible and row['compile_ok']:
            key=(row['case'],row['repeat'],row['artifact_sha256'])
            folder=root/key[0]/f'repeat_{key[1]}'/'simulation'/key[2][:16]
            if not (folder/'result.json').exists():jobs[key]=(row,folder)

def one(item):
    row,folder=item
    result=run(row['case'],ROOT/row['artifact'],folder)
    return row['case'],row['repeat'],result['status']

if __name__=='__main__':
    print('New hidden simulation artifacts:',len(jobs),flush=True)
    with ThreadPoolExecutor(max_workers=2) as pool:
        for future in as_completed([pool.submit(one,item) for item in jobs.values()]):print(*future.result(),flush=True)
