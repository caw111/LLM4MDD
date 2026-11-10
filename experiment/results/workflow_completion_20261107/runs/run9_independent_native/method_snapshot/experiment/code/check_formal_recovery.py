"""Offline regression: backend rejection must not erase independent arms."""
import tempfile
from pathlib import Path
from unittest.mock import patch
import run_formal_experiment as r

class Calls:
    def __init__(self,folder,model):self.folder=folder;self.entries=[];self.cursor=0
    def __call__(self,prompt):return 'offline','void setup() {} void loop() {}'

def bundle(calls,prompt,max_repairs,validator):
    validator('offline')
    return 'offline',{'system_sysml2':'offline'},{'attempts':[{'ok':True}]}

with tempfile.TemporaryDirectory() as directory:
    case={'id':'offline','dataset':'internal','stratum':'test','requirement':'offline','fqbn':'unused'}
    with patch.object(r,'ReplayCalls',Calls), patch.object(r,'finish_code',return_value={'compile_ok':True,'compile_status':'passed'}), patch.object(r,'generate_validated_bundle',bundle), patch.object(r,'validate_sysml',return_value={'ok':True,'issues':[]}), patch.object(r,'compile_sysml',return_value=r.IR_EXAMPLE), patch.object(r,'lower',side_effect=r.BackendFailure('ILParser Error: test')) as lower, patch.object(r,'generate_ir',return_value=r.IR_EXAMPLE) as ir:
        rows=r.evaluate(case,1,Path(directory),'offline')
    arms={x['arm']:x for x in rows}
    assert len(arms)==6 and not any(x.get('infrastructure_error') for x in rows)
    assert arms['direct']['compile_ok'] and arms['direct_feedback']['compile_ok']
    assert arms['workflow']['failure_stage']=='backend'
    assert arms['llm_kernel']['failure_stage']=='backend_dependency'
    assert arms['direct_ir']['failure_stage']=='backend' and lower.call_count==2 and ir.call_count==1
print('PASS: input-specific backend failures isolated; independent arms retained.')

from formal_resume import archive_infrastructure_simulations
import json
with tempfile.TemporaryDirectory() as directory:
    base=Path(directory)
    for name,status in [('a','infrastructure_auth_or_quota'),('b','passed'),('c','simulation_failed')]:
        folder=base/name/'repeat_1/simulation/hash';folder.mkdir(parents=True)
        (folder/'result.json').write_text(json.dumps({'status':status}));(folder/'simulation.log').write_text('offline test')
    assert archive_infrastructure_simulations(base)==1
    assert not (base/'a/repeat_1/simulation/hash/result.json').exists()
    assert len(list(base.glob('a/repeat_1/simulation/hash/infrastructure_history/*.result.json')))==1
    assert all((base/name/'repeat_1/simulation/hash/result.json').exists() for name in ('b','c'))
print('PASS: simulation infrastructure history retained; behavioral outcomes preserved.')

import formal_resume as resume
with patch.object(resume,'raw_simulation',return_value={'status':'infrastructure_auth_or_quota'}) as simulate:
    for attempt in range(2):
        try:resume.guarded_simulation('offline')
        except RuntimeError:pass
        else:raise AssertionError('Quota must stop execution')
    assert simulate.call_count==1
resume.simulation_quota_stop.clear()
print('PASS: quota rejection prevents further queued service calls.')
