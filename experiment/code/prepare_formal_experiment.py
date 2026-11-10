"""Freeze all dataset identities and a development/test split before further tuning."""
import json,random
from pathlib import Path
from run_controlled_experiment import cases,save,ROOT

all_cases=cases(42,full=True)
assert len(all_cases)==296 and len({c['id'] for c in all_cases})==296
seen=json.loads((ROOT/'experiment/output/controlled_pilot_v2_20260926/manifest.json').read_text(encoding='utf-8'))
seen_ids={c['id'] for c in seen['cases']}
budget={('internal','simple'):6,('internal','medium'):6,('internal','complex'):6,('internal','advanced'):6,
        ('PCEval','level1'):3,('PCEval','level2'):2,('PCEval','level3'):3,('PCEval','level4'):2,('EmbedBench','Mega'):26}
rng=random.Random(20260926)
for key,n in budget.items():
    group=[c for c in all_cases if (c['dataset'],c['stratum'])==key]
    used=[c for c in group if c['id'] in seen_ids]
    fresh=[c for c in group if c['id'] not in seen_ids]
    development={c['id'] for c in used+rng.sample(fresh,n-len(used))}
    for case in group:
        case['split']='development' if case['id'] in development else 'test'
        case['previously_inspected']=case['id'] in seen_ids
        case['oracle_status']={'internal':'needs_independent_behavior_tests','PCEval':'scenario_yaml_requires_reference_validation','EmbedBench':'descriptive_tests_require_conversion_and_reference_validation'}[case['dataset']]
assert sum(c['split']=='development' for c in all_cases)==60
assert sum(c['split']=='test' for c in all_cases)==236
assert all(c['split']=='development' for c in all_cases if c['id'] in seen_ids)
out=ROOT/'experiment/formal'
payload={'split_seed':20260926,'model':'gpt-4o-mini','repetitions':5,'cases':all_cases,
         'status':'design_frozen_not_executed','test_tasks':236,'development_tasks':60,
         'note':'Test prompts are enumerated for indexing only; no model output is generated or used for tuning. Oracle and method readiness gates must pass before the formal run.'}
path=out/'manifest.json'
if path.exists():
    assert json.loads(path.read_text(encoding='utf-8'))==payload, 'Do not overwrite a different frozen split'
else:save(path,payload)
print('Frozen: 296 tasks, 60 development, 236 test, 5 planned repeats; zero formal results claimed.')
