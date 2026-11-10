"""Correct zero-time first interactions in untimed source descriptions, uniformly."""
import bisect,concurrent.futures,copy,hashlib,json,os,queue
import run_picsimlab_comparison_groups as run
sim=run.sim
IDS={'embedbench_16','embedbench_30','embedbench_47','embedbench_5'}
def corrected(entry):
 original=entry['plan'];p=sim.picsimlab_native_scenarios.prepare(entry)
 if not p.get('timing_interpretation'):return copy.deepcopy(entry)
 times=sorted({c['at_ms'] for c in original['checks']});groups={t:[] for t in times};steps=[copy.deepcopy(s) for s in original.get('input_steps',[]) if s['at_ms']==0]
 for s in original.get('input_steps',[]):
  if s['at_ms']!=0:groups[times[min(len(times)-1,bisect.bisect_left(times,s['at_ms']))]].append(s)
 windows={}
 for i,t in enumerate(times):
  target=8000+i*8000;ss=groups[t];begin=500 if i==0 else 8000+(i-1)*8000+500
  if ss:
   old=min(s['at_ms'] for s in ss)
   steps.extend(dict(s,at_ms=begin+s['at_ms']-old) for s in ss);start=begin+20
  else:start=0 if i==0 else 8000+(i-1)*8000+20
  windows[t]=(start,target,i)
 for check,raw in zip(p['checks'],original['checks']):
  start,end,i=windows[raw['at_ms']];check.update(at_ms=end,native_eventual_group=i,native_eventual_from_ms=start,native_eventual_to_ms=end)
 p.update(input_steps=sorted(steps,key=lambda s:s['at_ms']),horizon_ms=8000+(len(times)-1)*8000+1000,origin='supplied_untimed_descriptions_corrected_startup_v2')
 p['timing_interpretation'].update(policy='untimed_descriptions_eventual_states_startup_v2',observation_budget_per_action_ms=8000,startup_input_margin_ms=500,reason='Initial sensor/released-button values stay at time zero. First user action follows startup; all states receive the disclosed eight-second observation budget.')
 return dict(entry,plan=p)
def main():
 os.nice(15);sim.OUT=run.OUT;sim.VERSION=run.VERSION;sim.APP=sim.BASE/'engines'/run.VERSION/'picsimlab_NOGUI';sim.RUN=run.BUILD/'corrected_startup_workers';profiles=run.load(run.OUT/'test_profiles.json');manifest=run.load(run.OUT/'manifest.json')
 fixed={cid:[corrected(e) for e in profiles[cid]['entries']] for cid in IDS};run.save(run.OUT/'untimed_startup_profiles.json',fixed)
 # First press must no longer coincide with initial released state.
 for entries in fixed.values():
  for e in entries:
   assert all(s['at_ms']>=0 for s in e['plan'].get('input_steps',[]))
   if e['plan'].get('timing_interpretation'):assert e['plan']['horizon_ms']>=9000
 slots=queue.Queue()
 for i in range(4):slots.put(800+i)
 def work(c):
  compiled=run.load(run.OUT/'cases'/c['id']/'compilation.json')
  if not compiled['compile_ok']:return
  slot=slots.get()
  try:
   c=dict(c,_variant='startup_margin');rows=[sim.run_plan(c,e,slot,i) for i,e in enumerate(fixed[c['original_case']])]
   row=dict(case=c['id'],original_case=c['original_case'],dataset=c['dataset'],source_sha256=c['source_sha256'],profile_sha256=c['profile_sha256'],applied_timing_policy='untimed_descriptions_eventual_states_startup_v2',status='scenarios_passed' if all(r['status']=='scenarios_passed' for r in rows) else 'needs_core_review',plans=rows)
   run.save(run.OUT/'cases'/c['id']/'case_execution.corrected_timing.json',row);print(c['id'],row['status'],flush=True)
  finally:slots.put(slot)
 with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:list(pool.map(work,[c for c in manifest['cases'] if c['original_case'] in IDS]))
 print('STARTUP_RETEST_COMPLETE',flush=True)
if __name__=='__main__':main()
