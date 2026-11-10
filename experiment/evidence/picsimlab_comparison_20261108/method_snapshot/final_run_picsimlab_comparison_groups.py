"""Frozen multi-arm PICSimLab execution. No candidate edits or model calls."""
import argparse,concurrent.futures,copy,hashlib,json,os,queue,shutil,subprocess,sys,time
from pathlib import Path
import run_picsimlab_all as sim
ROOT=Path(__file__).resolve().parents[2]
OLD=ROOT/'experiment/output/picsimlab_verification_20261107'
OUT=ROOT/'experiment/output/picsimlab_comparison_20261108'
ARD=Path.home()/'.cache/llm4mdd-arduino'
BUILD=Path.home()/'.cache/llm4mdd-picsimlab-comparison'
GROUPS=['historical_direct','historical_direct_feedback','direct_gpt61','ablation_direct','ablation_direct_feedback','ablation_workflow','ablation_no_contract_feedback','ablation_no_model_feedback','ablation_blind_resampling','ablation_llm_backend']
VERSION='native-electrical-v14'
def digest(data):return hashlib.sha256(data).hexdigest()
def load(p):return sim.load(p)
def save(p,v):sim.save(p,v)
def prepare():
 OUT.mkdir(parents=True,exist_ok=True)
 if (OUT/'manifest.json').exists():return load(OUT/'manifest.json')
 sys.path.insert(0,str(ROOT))
 from app.services.configuration_policy import resolve_configuration
 previous={r['case']:r for r in load(OLD/'final_all296_cases_after_34_review.json')}
 original={r['id']:r for r in load(OLD/'manifest.json')['cases']}
 overrides={
 'advanced_005':['additional_results/advanced_005_settled_core_native-electrical-v14.json','additional_results/advanced_005_binary_review_native-electrical-v14.json'],
 'advanced_028':['additional_results/advanced_028_queue_core_native-electrical-v14.json'],
 'complex_002':['additional_results/complex_002_binary_review_native-electrical-v14.json','additional_results/complex_002_fan_core_native-electrical-v14.json'],
 'medium_017':['additional_results/medium_017_level_switch_native-electrical-v11.json'],
 'pceval_distance_sensor':['additional_results/pceval_distance_sensor_focused_core_native-electrical-v14.json'],
 'simple_029':['additional_results/simple_029_focused_core_native-electrical-v14.json'],
 'embedbench_109':['additional_results/embedbench_109_physical_frame_native-electrical-v11.json'],
 'embedbench_0':['additional_results/embedbench_0_closed_loop_native-electrical-v12.json']}
 # Use the same corrected input plans for every arm, irrespective of its output.
 profiles={}
 for cid,case in original.items():
  entries=[]
  for path in overrides.get(cid,[]):
   if not (OLD/path).exists():continue
   for row in load(OLD/path)['plans']:
    folder=OLD/row['result_directory'];entries.append(dict(name=row['plan_name'],plan=load(folder/'original_test_plan.json')))
  if not entries:
   folder=OLD/previous[cid]['core_evidence'];entries=[dict(name=previous[cid]['core_plan'],plan=load(folder/'original_test_plan.json'))]
  resolution=case.get('configuration_resolution') or resolve_configuration(case['requirement'],board=case['fqbn'])
  names={}
  for device in resolution.get('devices',[]):
   if not device['kind'].startswith('tm1637'):continue
   a,b=device['original'],device['resolved']
   if all(k in a and a[k] is not None for k in ('clk_pin','dio_pin')):
    name=lambda p:'tm:'+':'.join(str(sim.assertions.pin(p[k],case['fqbn'])) for k in ('clk_pin','dio_pin'))
    names[name(a)]=name(b)
  def remap(v):
   if isinstance(v,dict):return {k:remap(x) for k,x in v.items()}
   if isinstance(v,list):return [remap(x) for x in v]
   return names.get(v,v) if isinstance(v,str) else v
  entries=remap(entries)
  for entry in entries:
   p=entry['plan']
   if 'LCD' in cid or cid=='pceval_calendar_display':p['lcd_capture_interval_ms']=10
   if cid=='pceval_serial_LCD_display':p['lcd_reset_pullup']=True
   for check in p.get('checks',[]):
    if check['kind']=='tone' and check.get('expected',0)>0:check['tolerance']=max(check.get('tolerance',0),check['expected']*.01)
  profiles[cid]=dict(entries=entries,configuration_resolution=resolution,requirement=case['requirement'],fqbn=case['fqbn'],dataset=case['dataset'])
  profiles[cid]['sha256']=digest(json.dumps(profiles[cid],sort_keys=True,ensure_ascii=False).encode())
 save(OUT/'test_profiles.json',profiles)
 records=[r for r in load(ROOT/'experiment/results/groups.json') if r['group'] in GROUPS]
 assert len(records)==296*len(GROUPS)
 unique={};mapped=[]
 for r in records:
  row=dict(r);cid=r['case']
  if not r.get('source'):
   row.update(execution_key=None,unavailable_reason='No generated sketch in frozen group archive');mapped.append(row);continue
  source=ROOT/'experiment/results'/r['source'];data=source.read_bytes();sha=digest(data);assert sha==r['source_sha256']
  key=cid+'__'+sha[:16]
  if key not in unique:
   p=profiles[cid];case=dict(id=key,original_case=cid,dataset=r['dataset'],fqbn=r['board'],source_sha256=sha,requirement=p['requirement'],configuration_resolution=p['configuration_resolution'],profile_sha256=p['sha256'],aliases=[])
   assert case['fqbn']==p['fqbn']
   folder=OUT/'cases'/key;folder.mkdir(parents=True,exist_ok=True);(folder/'source.ino').write_bytes(data)
   unique[key]=case
  unique[key]['aliases'].append(dict(group=r['group'],case=cid,historical_compile_ok=r['compile_ok']))
  row.update(execution_key=key,profile_sha256=profiles[cid]['sha256']);mapped.append(row)
 shutil.copyfile(OLD/'timing_remote.json',OUT/'timing_remote.json')
 save(OUT/'group_records.json',mapped)
 manifest=dict(groups=GROUPS,total_records=len(mapped),unique_sources=len(unique),cases=list(unique.values()),version=VERSION,source_modified=False,policy='Frozen same-case same-source execution reuse; identical common input profiles across arms; basic functional review separate from strict assertions.')
 save(OUT/'manifest.json',manifest)
 snapshot=OUT/'method_snapshot';snapshot.mkdir(exist_ok=True)
 for name in ['run_picsimlab_comparison_groups.py','run_picsimlab_all.py','picsimlab_assertions.py','picsimlab_circuit.py','picsimlab_native_scenarios.py','picsimlab_core_checks.py','picsimlab_raster.py']:
  shutil.copyfile(ROOT/'experiment/code'/name,snapshot/name)
 return manifest

def compile_one(case,slots):
 slot=slots.get();folder=OUT/'cases'/case['id'];target=folder/'compilation.json';start=time.monotonic()
 try:
  if target.exists():return load(target)
  source=(folder/'source.ino').read_bytes();assert digest(source)==case['source_sha256']
  work=BUILD/'compiler'/str(slot)/('mega' if 'mega' in case['fqbn'] else 'uno');sketch=work/'sketch';sketch.mkdir(parents=True,exist_ok=True)
  (sketch/'sketch.ino').write_bytes(source)
  cmd=[str(ARD/'arduino-cli'),'--config-file',str(ARD/'native_config.yaml'),'compile','--fqbn',case['fqbn'],'--jobs','1','--build-path',str(work/'build'),str(sketch)]
  result=subprocess.run(cmd,capture_output=True,timeout=240);(folder/'compile.log').write_bytes(result.stdout+result.stderr)
  row=dict(case=case['id'],original_case=case['original_case'],dataset=case['dataset'],compile_ok=result.returncode==0,exit_code=result.returncode,source_sha256=case['source_sha256'],command=cmd)
  if row['compile_ok']:
   firmware=folder/'firmware';firmware.mkdir(exist_ok=True)
   for name in ('sketch.ino.hex','sketch.ino.elf'):shutil.copyfile(work/'build'/name,firmware/name)
   nm=ARD/'data/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-nm'
   symbols=subprocess.run([str(nm),'-n',str(firmware/'sketch.ino.elf')],capture_output=True,check=True).stdout;(folder/'symbols.txt').write_bytes(symbols)
   row['hex']=dict(sha256=digest((firmware/'sketch.ino.hex').read_bytes()))
  else:
   log=(result.stdout+result.stderr).decode(errors='replace')
   row['compile_status']='dependency_unverified' if 'No such file or directory' in log and '.h' in log else 'compile_failed'
   row['diagnostic_tail']=log[-3000:]
  row['seconds']=round(time.monotonic()-start,3);save(target,row);return row
 except Exception as exc:
  row=dict(case=case['id'],original_case=case['original_case'],compile_ok=False,compile_status='infrastructure_unverified',error=str(exc));save(target,row);return row
 finally:slots.put(slot)

def simulate_one(case,profiles,slots):
 slot=slots.get()
 try:
  target=OUT/'cases'/case['id']/'case_execution.json'
  if target.exists() and load(target).get('status')!='infrastructure_unverified':return load(target)
  rows=[sim.run_plan(case,e,slot,index) for index,e in enumerate(profiles[case['original_case']]['entries'])]
  status='scenarios_passed' if rows and all(r['status']=='scenarios_passed' for r in rows) else 'needs_core_review' if any(r['status']=='failed' for r in rows) else 'unverified'
  row=dict(case=case['id'],original_case=case['original_case'],dataset=case['dataset'],source_sha256=case['source_sha256'],profile_sha256=case['profile_sha256'],status=status,plans=rows)
  save(target,row);return row
 except Exception as exc:
  row=dict(case=case['id'],original_case=case['original_case'],status='infrastructure_unverified',error=str(exc));save(OUT/'cases'/case['id']/'case_execution.json',row);return row
 finally:slots.put(slot)

def main():
 p=argparse.ArgumentParser();p.add_argument('--prepare',action='store_true');p.add_argument('--compile-workers',type=int,default=6);p.add_argument('--sim-workers',type=int,default=12);p.add_argument('--limit',type=int);p.add_argument('--case',action='append');args=p.parse_args()
 assert os.name=='posix';os.nice(15)
 manifest=prepare()
 if args.prepare:print(json.dumps(dict(groups=manifest['groups'],records=manifest['total_records'],unique=manifest['unique_sources'])),flush=True);return
 sim.OUT=OUT;sim.VERSION=VERSION;sim.APP=sim.BASE/'engines'/VERSION/'picsimlab_NOGUI';sim.RUN=BUILD/'simulator_workers'
 assert sim.APP.is_file()
 profiles=load(OUT/'test_profiles.json');cases=manifest['cases']
 if args.case:cases=[c for c in cases if c['original_case'] in args.case]
 cases=sorted(cases,key=lambda c:(c['dataset']!='EmbedBench',profiles[c['original_case']]['entries'][0]['plan'].get('horizon_ms',3000)))
 if args.limit:cases=cases[:args.limit]
 cs,ss=queue.Queue(),queue.Queue()
 for i in range(args.compile_workers):cs.put(i)
 for i in range(args.sim_workers):ss.put(i+200)
 started=time.time();pending=set();compiled=0;simulated=0;compile_fail=0;dependency=0;statuses={}
 def snapshot(done=False):
  save(OUT/'progress.json',dict(groups=len(manifest['groups']),planned_records=manifest['total_records'],unique_sources=manifest['unique_sources'],batch_unique=len(cases),compile_completed=compiled,compile_failed=compile_fail,dependency_or_infrastructure=dependency,simulation_completed=simulated,simulation_statuses=statuses,started_at=started,elapsed_seconds=round(time.time()-started,1),compile_workers=args.compile_workers,sim_workers=args.sim_workers,batch_execution_complete=done))
 with concurrent.futures.ThreadPoolExecutor(max_workers=args.compile_workers) as compiler,concurrent.futures.ThreadPoolExecutor(max_workers=args.sim_workers) as simulator:
  futures={compiler.submit(compile_one,c,cs):c for c in cases}
  for f in concurrent.futures.as_completed(futures):
   case=futures[f];row=f.result();compiled+=1
   if row['compile_ok']:pending.add(simulator.submit(simulate_one,case,profiles,ss))
   else:
    compile_fail+=1
    if row.get('compile_status')!='compile_failed':dependency+=1
   finished={f for f in pending if f.done()}
   for f in finished:
    r=f.result();simulated+=1;statuses[r['status']]=statuses.get(r['status'],0)+1;print(r['original_case'],r['case'][-16:],r['status'],flush=True)
   pending-=finished;snapshot()
  for f in concurrent.futures.as_completed(pending):
   r=f.result();simulated+=1;statuses[r['status']]=statuses.get(r['status'],0)+1;print(r['original_case'],r['case'][-16:],r['status'],flush=True);snapshot()
 snapshot(True);print('BATCH_EXECUTION_COMPLETE',flush=True)
if __name__=='__main__':main()
