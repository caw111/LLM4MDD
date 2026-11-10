"""Use spare CPU cores on the unscheduled tail; identical frozen firmware and plans."""
import concurrent.futures,json,os,queue,time
import run_picsimlab_comparison_groups as run
sim=run.sim

def atomic_save(path,value):
 path.parent.mkdir(parents=True,exist_ok=True);temporary=path.with_name(path.name+f'.{os.getpid()}.tmp');temporary.write_text(json.dumps(value,ensure_ascii=False,indent=2),encoding='utf8');temporary.replace(path)
def main():
 os.nice(15);sim.save=atomic_save;sim.OUT=run.OUT;sim.VERSION=run.VERSION;sim.APP=sim.BASE/'engines'/run.VERSION/'picsimlab_NOGUI';sim.RUN=run.BUILD/'fastlane_workers'
 profiles=run.load(run.OUT/'test_profiles.json');cases=run.load(run.OUT/'manifest.json')['cases'];cases=sorted(cases,key=lambda c:(c['dataset']!='EmbedBench',profiles[c['original_case']]['entries'][0]['plan'].get('horizon_ms',3000)),reverse=True)
 candidates=[]
 for case in cases:
  if case['original_case'] in {'embedbench_16','embedbench_30','embedbench_47','embedbench_5'}:continue
  folder=run.OUT/'cases'/case['id']
  if (folder/'case_execution.json').exists():continue
  if not run.load(folder/'compilation.json')['compile_ok']:continue
  candidates.append(dict(case,_variant='fastlane'))
 slots=queue.Queue()
 for i in range(6):slots.put(1000+i)
 started=time.time();rows=[]
 with concurrent.futures.ThreadPoolExecutor(max_workers=6) as pool:
  futures=[pool.submit(run.simulate_one,c,profiles,slots) for c in candidates]
  for f in concurrent.futures.as_completed(futures):
   row=f.result();rows.append(dict(case=row['case'],status=row['status']));run.save(run.OUT/'fastlane_progress.json',dict(planned=len(candidates),completed=len(rows),elapsed_seconds=time.time()-started));print(row['case'],row['status'],flush=True)
 run.save(run.OUT/'fastlane_summary.json',rows);print('FASTLANE_COMPLETE',flush=True)
if __name__=='__main__':main()
