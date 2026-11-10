"""Re-evaluate group LED checks from saved physical traces, preserving raw results."""
import argparse
import gzip
import json
from pathlib import Path
import shutil
import tempfile
import run_picsimlab_all as sim


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--version',default='native-electrical-v11');args=p.parse_args()
    rows=[]
    for case in sim.load(sim.OUT/'manifest.json')['cases']:
        path=sim.OUT/'cases'/case['id']/'simulation'/args.version/'case_result.json'
        if not path.exists():continue
        for row in sim.load(path)['plans']:
            folder=sim.OUT/row['result_directory']
            plan=sim.load(folder/'test_plan.json')
            if not any(c['kind'] in ('gpio_count','display_gpio_count','distinct_gpio_patterns') for c in plan['checks']):continue
            with tempfile.TemporaryDirectory(prefix='picsim-led-group-') as work:
                raw=Path(work)/'electrical_trace.tsv'
                with gzip.open(folder/'electrical_trace.tsv.gz','rb') as source,raw.open('wb') as target:shutil.copyfileobj(source,target)
                trace=sim.decode_trace(raw,plan,case['fqbn'])
                with gzip.open(folder/'observations.jsonl.gz','rt') as file:trace.extend(json.loads(line) for line in file if '"lcd"' in line and json.loads(line).get('type')=='lcd')
                result=sim.evaluate(plan,trace,case['fqbn'])
                result.update(case=case['id'],plan_name=row['plan_name'],plan_index=row['plan_index'],source_sha256=row['source_sha256'],firmware_sha256=row['firmware_sha256'],raw_result_directory=row['result_directory'],method='LED group states derived from complete measured PWM periods; no re-execution or candidate modification')
                output=folder/'led_group_reassessment.json';sim.save(output,result)
                rows.append(dict(case=case['id'],plan_index=row['plan_index'],raw_status=row['status'],status=result['status'],path=str(output.relative_to(sim.OUT))))
                print(case['id'],row['plan_name'],row['status'],'->',result['status'],flush=True)
    sim.save(sim.OUT/'led_group_reassessment_summary.json',rows)


if __name__=='__main__':main()
