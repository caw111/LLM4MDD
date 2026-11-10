"""Finite background continuation: generation, quota preflight, simulation, report."""
import json,subprocess,sys
from datetime import datetime,timezone
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
root=ROOT/'experiment/output/formal_20260928'

def stage(name,args):
    print('STAGE',name,datetime.now(timezone.utc).isoformat(),flush=True)
    code=subprocess.call([sys.executable,*args],cwd=ROOT)
    if code:raise SystemExit(code)

if __name__=='__main__':
    stage('generation',['experiment/code/formal_resume.py','--generation-only','--workers','24'])
    from formal_resume import archive_infrastructure_simulations,guarded_simulation
    pending=next((p for p in root.glob('*/repeat_*/simulation/*/result.json') if json.loads(p.read_text()).get('status')=='infrastructure_auth_or_quota'),None)
    if pending:
        folder=pending.parent;job=folder.parent.parent
        row=next(r for r in json.loads((job/'results.json').read_text(encoding='utf-8')) if r.get('artifact_sha256','').startswith(folder.name))
        archive_infrastructure_simulations(root)
        print('STAGE single Wokwi quota preflight',flush=True)
        result=guarded_simulation(row['case'],ROOT/row['artifact'],folder)
        if result['status'] not in ('passed','simulation_failed'):raise SystemExit('Unresolved preflight: '+result['status'])
    stage('simulation',['experiment/code/formal_resume.py','--simulate-only','--workers','24'])
    stage('report',['experiment/code/summarize_formal_experiment.py'])
