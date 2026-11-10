"""Calibrated passive-observer re-evaluation; preserves every original result."""
import gzip
import json
import os
from pathlib import Path
import run_picsimlab_all as sim


def main():
    os.nice(15)
    rows=[]
    for case in sim.load(sim.OUT/'manifest.json')['cases']:
        root=sim.OUT/'cases'/case['id']/'simulation/native-electrical-v11'
        saved=sim.load(root/'case_result.json')
        assessments=[]
        for original in saved['plans']:
            folder=sim.OUT/original['result_directory'];plan=sim.load(folder/'test_plan.json')
            corrected=json.loads(json.dumps(plan));changed=False
            for c in corrected['checks']:
                if c['kind']=='tone' and c.get('expected',0)>0:
                    c['original_tolerance']=c.get('tolerance',0)
                    c['tolerance']=max(c.get('tolerance',0),c['expected']*.01)
                    c['measurement_allowance']='1% physical frequency allowance: integer timer divisor and interrupt jitter, not exact host API argument equality'
                    changed=True
            group=folder/'led_group_reassessment.json'
            if not changed and not group.exists():
                assessments.append(original);continue
            with gzip.open(folder/'observations.jsonl.gz','rt') as file:trace=[json.loads(line) for line in file]
            result=sim.evaluate(corrected,trace,case['fqbn'])
            if group.exists():
                corrected_group=sim.load(group)
                by_id={c['id']:c for c in corrected_group['checks'] if c['test']['kind'] in ('gpio_count','display_gpio_count','distinct_gpio_patterns')}
                result['checks']=[by_id.get(c['id'],c) for c in result['checks']]
                failed=[c for c in result['checks'] if not c['passed'] and not c.get('unverified')]
                unverified=[c for c in result['checks'] if c.get('unverified')]
                result['status']='unverified_no_deterministic_checks' if not result['checks'] else 'failed' if failed else 'partial_unverified' if unverified else 'scenarios_passed'
                result['failed_assertions']=len(failed);result['unverified_checks']=len(unverified)
            row=dict(original,**result,raw_status=original['status'],observer_revision='physical-observer-v2')
            sim.save(folder/'physical_observer_reassessment.json',row);assessments.append(row)
        failed=any(p['status']=='failed' for p in assessments)
        informative=[p for p in assessments if p.get('checks') or p['status']!='unverified_no_deterministic_checks']
        status='failed' if failed else 'scenarios_passed' if informative and all(p['status']=='scenarios_passed' for p in informative) else 'partial_unverified'
        row=dict(case=case['id'],dataset=case['dataset'],raw_status=saved['status'],status=status,plans=assessments,empty_diagnostic_plans_ignored=sum(not p.get('checks') for p in assessments))
        sim.save(root/'physical_observer_case_assessment.json',row);rows.append(row)
        if row['raw_status']!=status:print(case['id'],row['raw_status'],'->',status,flush=True)
    summary=dict(planned=296,completed=len(rows),counts={s:sum(r['status']==s for r in rows) for s in {r['status'] for r in rows}},observer_revision='physical-observer-v2')
    sim.save(sim.OUT/'physical_observer_summary.json',summary);print(json.dumps(summary),flush=True)


if __name__=='__main__':main()
