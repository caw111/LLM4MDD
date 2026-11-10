"""Retry missing third-party dependencies without changing any source bytes."""
import concurrent.futures,json,os,queue,re,shutil
import run_picsimlab_comparison_groups as run
sim=run.sim

def main():
 os.nice(15);manifest=run.load(run.OUT/'manifest.json');profiles=run.load(run.OUT/'test_profiles.json');sim.OUT=run.OUT;sim.VERSION=run.VERSION;sim.APP=sim.BASE/'engines'/run.VERSION/'picsimlab_NOGUI';sim.RUN=run.BUILD/'dependency_retry_workers'
 cs,ss=queue.Queue(),queue.Queue()
 for i in range(3):cs.put(600+i);ss.put(600+i)
 cases=[]
 for c in manifest['cases']:
  folder=run.OUT/'cases'/c['id'];path=folder/'compilation.json'
  if not path.exists():continue
  row=run.load(path)
  if row.get('compile_status')!='dependency_unverified':continue
  shutil.copyfile(path,folder/'compilation.before_dependency_retry.json');shutil.copyfile(folder/'compile.log',folder/'compile.before_dependency_retry.log')
  if 'fatal error: strings.h:' in row.get('diagnostic_tail',''):
   row.update(compile_status='compile_failed',failure_reason='POSIX strings.h is unavailable on the specified AVR target; no replacement header supplied');run.save(path,row);continue
  path.unlink();cases.append(c)
 def work(c):
  compiled=run.compile_one(c,cs)
  result=run.simulate_one(c,profiles,ss) if compiled['compile_ok'] else None
  return dict(case=c['id'],compiled=compiled['compile_ok'],execution=result)
 with concurrent.futures.ThreadPoolExecutor(max_workers=3) as pool:
  rows=[]
  for row in pool.map(work,cases):rows.append(row);print(row['case'],row['compiled'],flush=True)
 run.save(run.OUT/'dependency_retry_summary.json',rows)
if __name__=='__main__':main()
