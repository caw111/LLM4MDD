"""Paired known-source controls for the common comparison test profiles."""
import concurrent.futures,hashlib,json,os,queue,shutil,time
import run_picsimlab_all as sim
ROOT=sim.ROOT;OLD=ROOT/'experiment/output/picsimlab_verification_20261107';OUT=ROOT/'experiment/output/picsimlab_comparison_20261108/controls'
def main():
 os.nice(15);sim.OUT=OUT;sim.VERSION='native-electrical-v14';sim.APP=sim.BASE/'engines'/sim.VERSION/'picsimlab_NOGUI';sim.RUN=sim.BASE/'comparison_control_workers'
 OUT.mkdir(exist_ok=True);shutil.copyfile(OLD/'timing_remote.json',OUT/'timing_remote.json')
 previous=sim.load(OLD/'final_all296_cases_after_34_review.json');review=sim.load(OLD/'binary_review_34_20261108/binary_verdicts_34.json');qualified=sim.load(OLD/'basic_function_qualifications.json');profiles=sim.load(OUT.parent/'test_profiles.json');original={c['id']:c for c in sim.load(OLD/'manifest.json')['cases']}
 ids={c['case'] for c in review['decisions']}|{c['case'] for c in qualified}|{'advanced_009'};cases=[]
 for cid in sorted(ids):
  c=dict(original[cid],original_case=cid,profile_sha256=profiles[cid]['sha256'],configuration_resolution=profiles[cid]['configuration_resolution'],_firmware_path=str(OLD/'cases'/cid/'firmware/sketch.ino.hex'),_symbols_path=str(OLD/'cases'/cid/'symbols.txt'))
  folder=OUT/'cases'/cid;folder.mkdir(parents=True,exist_ok=True);shutil.copyfile(OLD/'cases'/cid/'compilation.json',folder/'compilation.json');cases.append(c)
 sim.save(OUT/'manifest.json',dict(cases=cases,expected={r['case']:r['basic_core_status'] for r in previous if r['case'] in ids}))
 slots=queue.Queue()
 for i in range(4):slots.put(400+i)
 def run(c):
  slot=slots.get()
  try:
   rows=[sim.run_plan(c,e,slot,i) for i,e in enumerate(profiles[c['id']]['entries'])];r=dict(case=c['id'],original_case=c['id'],status='scenarios_passed' if all(r['status']=='scenarios_passed' for r in rows) else 'needs_core_review',plans=rows);sim.save(OUT/'cases'/c['id']/'case_execution.json',r);print(c['id'],r['status'],flush=True);return r
  finally:slots.put(slot)
 with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:list(pool.map(run,cases))
 print('CONTROL_EXECUTION_COMPLETE',flush=True)
if __name__=='__main__':main()
