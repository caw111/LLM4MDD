"""Refresh cached basic reviews while the authorized native batches execute."""
import json,os,subprocess,sys,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];OUT=ROOT/'experiment/output/picsimlab_comparison_20261108'
def load(p):return json.loads(p.read_text())
def main():
 os.nice(15);manifest=load(OUT/'manifest.json');iterations=0
 while True:
  with (OUT/'review_monitor.stdout.log').open('a') as log:
   run=subprocess.run([sys.executable,str(ROOT/'experiment/code/review_picsimlab_comparison.py')],stdout=log,stderr=log)
  compiled=0;executed=0;timed=0;positive=0;expected_timed=0
  for c in manifest['cases']:
   folder=OUT/'cases'/c['id'];p=folder/'compilation.json'
   if p.exists():
    compiled+=1
    if load(p)['compile_ok']:
     if c['original_case'] in {'embedbench_16','embedbench_30','embedbench_47','embedbench_5'}:expected_timed+=1
     positive+=1
     if (folder/'case_execution.json').exists():executed+=1
   if (folder/'case_execution.corrected_timing.json').exists():timed+=1
  primary=load(OUT/'progress.json').get('batch_execution_complete',False);fast=(OUT/'fastlane_summary.json').exists()
  complete=compiled==len(manifest['cases']) and executed==positive and timed==expected_timed and primary and fast
  status=dict(iteration=iterations,compile_completed=compiled,compile_passed_unique=positive,simulated_unique=executed,startup_retests=timed,startup_retests_expected=expected_timed,primary_complete=primary,fastlane_complete=fast,execution_complete=complete,review_exit_code=run.returncode,updated_at=time.time())
  temporary=OUT/'combined_progress.tmp';temporary.write_text(json.dumps(status,indent=2));temporary.replace(OUT/'combined_progress.json');print(json.dumps(status),flush=True)
  if complete and run.returncode==0:break
  iterations+=1;time.sleep(90)
if __name__=='__main__':main()
