"""Run the explicit review jobs in two low-priority native workers."""
import concurrent.futures,json,os,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];OUT=ROOT/'experiment/output/picsimlab_verification_20261107';REVIEW=OUT/'binary_review_34_20261108'
def run(job):
 command=['python3',str(ROOT/'experiment/code/run_picsimlab_additional.py'),'--case',job['id'],'--plans',job['plans'],'--version',job['version'],'--label',job['label']]
 if job.get('resolve'):command.append('--resolve-configuration')
 with (REVIEW/(job['id']+'.stdout.log')).open('ab') as a,(REVIEW/(job['id']+'.stderr.log')).open('ab') as b:p=subprocess.run(command,stdout=a,stderr=b,timeout=1800)
 print(job['id'],p.returncode,flush=True);return dict(case=job['id'],exit_code=p.returncode)
def main():
 os.nice(15);jobs=json.loads((REVIEW/'replay_jobs.json').read_text());rows=[]
 with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:
  for row in pool.map(run,jobs):rows.append(row)
 (REVIEW/'replay_status.json').write_text(json.dumps(rows,indent=2))
if __name__=='__main__':main()
