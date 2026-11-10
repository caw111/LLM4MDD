"""Targeted development probe of a staged feedback/prompt candidate, no Arduino backend."""
import hashlib,importlib.util,json,os,sys
from pathlib import Path
ROOT=Path(r'C:\Users\27872\Desktop\work\GeneratedCodeOfMing\LLM4MDD');sys.path[:0]=[str(ROOT),str(ROOT/'experiment/code')];os.chdir(ROOT)
import formal_resume as recovery
from app.services.sysml_compiler import compile_sysml,ir_to_il
from formal_ir import validate_ir
stage=Path(r'C:\Users\27872\Documents\ChatGPT\LLM4MDD论文\tmp\sysml_repair_patterns')
out=ROOT/'experiment/output/feedback_candidate_probe_20261009'
recovery.cache_root=out/'reference_validation_cache'
spec=importlib.util.spec_from_file_location('staged_sysml',stage/'sysml_v3.candidate.py')
candidate=importlib.util.module_from_spec(spec);spec.loader.exec_module(candidate)
prompt=(stage/'system_sysml2_v3.candidate.prompt.md').read_text(encoding='utf-8')
ids=['simple_002','complex_001','advanced_005']
manifest=json.loads((ROOT/'experiment/formal/manifest.json').read_text(encoding='utf-8'))
cases=[next(c for c in manifest['cases'] if c['id']==name) for name in ids]
assert all(c['split']=='development' for c in cases)
execution={'cases':ids,'model':'gpt-4o-mini','repairs':2,'scope':'targeted development model gate only; not Arduino compile or functional comparison','candidate_hashes':{name:hashlib.sha256((stage/name).read_bytes()).hexdigest() for name in ('sysml_v3.candidate.py','system_sysml2_v3.candidate.prompt.md')}}
if (out/'execution.json').exists():assert json.loads((out/'execution.json').read_text(encoding='utf-8'))==execution
else:recovery.runner.save(out/'execution.json',execution)
rows=[]
for case in cases:
 folder=out/case['id'];result_path=folder/'result.json'
 if result_path.exists():
  old=json.loads(result_path.read_text(encoding='utf-8'))
  if not old.get('infrastructure_error'):rows.append(old);continue
 calls=recovery.runner.ReplayCalls(folder/'calls','gpt-4o-mini');accepted={}
 def validator(source):
  result=recovery.cached_validate(source)
  if result['ok']:
   try:
    accepted['ir']=validate_ir(compile_sysml(source,validated=result));ir_to_il(accepted['ir'])
   except Exception as e:
    result['ok']=False;result['issues'].append({'severity':'error','code':'LOWERING_OR_IR','message':str(e)[:2500]})
  return result
 try:
  _,bundle,report=candidate.generate_validated_bundle(calls,prompt.replace('<<USER_REQUIREMENTS>>',case['requirement']),max_repairs=2,validator=validator)
  recovery.runner.save(folder/'model.json',bundle);recovery.runner.save(folder/'ir.json',accepted['ir'])
  row={'case':case['id'],'model_ok':True,'initial_ok':report['attempts'][0]['ok']}
 except candidate.SysMLValidationError as e:
  report={'attempts':e.attempts};row={'case':case['id'],'model_ok':False,'initial_ok':False}
 except RuntimeError as e:
  row={'case':case['id'],'model_ok':False,'infrastructure_error':type(e).__name__};report={}
 recovery.runner.save(folder/'validation.json',report);recovery.runner.save(result_path,row)
 rows.append(row);print(row,flush=True)
 if row.get('infrastructure_error'):break
recovery.runner.save(out/'summary.json',{'complete':len(rows)==3 and not any(r.get('infrastructure_error') for r in rows),'rows':rows})
