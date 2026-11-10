"""Resume frozen calls when only the order of identical diagnostics changes."""
import json,threading,time,hashlib,gzip,copy
from pathlib import Path
import run_formal_experiment as runner

original=runner.ReplayCalls.__call__
MARKER='\nDiagnostics (line/column refer to system_sysml2):\n'

def same_diagnostics(a,b):
    if MARKER not in a or MARKER not in b:return False
    prefix_a,issues_a=a.rsplit(MARKER,1);prefix_b,issues_b=b.rsplit(MARKER,1)
    if prefix_a!=prefix_b:return False
    try:
        canon=lambda text:sorted(json.dumps(x,sort_keys=True,ensure_ascii=False) for x in json.loads(text))
        return canon(issues_a)==canon(issues_b)
    except (TypeError,ValueError):return False

def replay(self,prompt):
    cursor=self.cursor
    while cursor<len(self.entries) and 'error_type' in self.entries[cursor]:cursor+=1
    if cursor<len(self.entries):
        old=self.entries[cursor]['prompt']
        if old!=prompt:
            if same_diagnostics(old,prompt):prompt=old
            else:
                # Reproduce feedback already sent before an interruption. A fresh compiler
                # result must not be paired with a response generated for another diagnostic.
                for marker in ('\nCompiler diagnostics:\n','\nRepair compiler diagnostics without dropping requirements:\n'):
                    if marker in old and marker in prompt and old.rsplit(marker,1)[0]==prompt.rsplit(marker,1)[0]:
                        runner.save(self.folder/f'replay_diagnostics_{cursor}.json',{'cached':old.rsplit(marker,1)[1],'current':prompt.rsplit(marker,1)[1],'policy':'Replay the exact original feedback/response pair for the identical candidate.'})
                        prompt=old
                        break
    for attempt in range(3):
        try:return original(self,prompt)
        except RuntimeError as exc:
            if attempt==2 or not any(name in str(exc) for name in ('APIConnectionError','APITimeoutError','InternalServerError','RateLimitError')):raise
            time.sleep(5*(attempt+1))


# Infrastructure-only memoization of the pinned reference validator, keyed by exact source.
reference_validate=runner.validate_sysml
cache_root=runner.ROOT/'experiment/output/formal_20260928/reference_validation_cache'
cache_lock=threading.Lock()
namespace=runner.digest(runner.ROOT/'tools/sysml/SysMLBridge.java')+runner.digest(runner.ROOT/'app/services/sysml.py')+'2026-08/0.62.0'
def cache_path(source):return cache_root/(hashlib.sha256((namespace+'\0'+source).encode()).hexdigest()+'.json.gz')
def store_validation(source,result):
    path=cache_path(source)
    with cache_lock:
        if not path.exists():
            cache_root.mkdir(parents=True,exist_ok=True)
            temporary=path.with_suffix('.tmp')
            with gzip.open(temporary,'wt',encoding='utf-8') as handle:json.dump(result,handle,ensure_ascii=False)
            temporary.replace(path)
def cached_validate(source):
    path=cache_path(source)
    with cache_lock:
        if path.exists():
            with gzip.open(path,'rt',encoding='utf-8') as handle:return json.load(handle)
    try:result=reference_validate(source)
    except runner.SysMLToolError as exc:
        # Retry the same candidate once: transport/load problems do not spend a model repair.
        first=str(exc)
        try:result=reference_validate(source)
        except runner.SysMLToolError as again:
            if 'NullPointerException' not in first or 'NullPointerException' not in str(again):raise
            # Acceptance was not established. Report a tool crash, not a fabricated syntax error.
            result={'ok':False,'elements':[],'issues':[{'severity':'error','code':'REFERENCE_TOOL_CRASH','message':'The pinned reference validator crashed twice on this candidate (NullPointerException). Its validity could not be established. Regenerate an equivalent model using the supported scalar subset without dropping requirements.'}]}
    store_validation(source,result)
    return result

def seed_failed_validations():
    count=0
    root=runner.ROOT/'experiment/output/formal_20260928'
    for path in root.glob('*/repeat_*/model_validation.json'):
        report=json.loads(path.read_text(encoding='utf-8'))
        for attempt in report['attempts']:
            errors=[i for i in attempt.get('issues',[]) if i.get('severity')=='error']
            if attempt['ok'] or not errors or any(not i.get('code','').startswith(('org.eclipse.','validate')) for i in errors):continue
            try:source=runner.json_response(attempt['response'])['system_sysml2']
            except (ValueError,KeyError,TypeError):continue
            store_validation(source,{'ok':False,'elements':[],'issues':attempt['issues']});count+=1
    return count

runner.validate_sysml=cached_validate
runner.ReplayCalls.__call__=replay
runner.JAVA=threading.Semaphore(8)
runner.BUILD=threading.Semaphore(4)

def archive_infrastructure_simulations(root):
    """Retain failed service attempts, but do not reuse them as behavioral outcomes."""
    from datetime import datetime,timezone
    stamp=datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
    count=0
    for path in root.glob('*/repeat_*/simulation/*/result.json'):
        result=json.loads(path.read_text(encoding='utf-8'))
        if not result.get('status','').startswith(('infrastructure_','blocked_')):continue
        path.resolve().relative_to(root.resolve())
        history=path.parent/'infrastructure_history';history.mkdir(exist_ok=True)
        path.rename(history/(stamp+'.result.json'))
        log=path.parent/'simulation.log'
        if log.exists():log.rename(history/(stamp+'.simulation.log'))
        count+=1
    return count

# Stop queued simulations after a service quota rejection; preserve completed caches.
import run_wokwi_case
raw_simulation=run_wokwi_case.run
simulation_quota_stop=threading.Event()
def guarded_simulation(*args,**kwargs):
    if simulation_quota_stop.is_set():raise RuntimeError('Wokwi quota/authentication blocked; batch stopped')
    result=raw_simulation(*args,**kwargs)
    if result.get('status')=='infrastructure_auth_or_quota':
        simulation_quota_stop.set()
        raise RuntimeError('Wokwi quota/authentication blocked; outcome cached, batch stopped')
    return result
run_wokwi_case.run=guarded_simulation

if __name__=='__main__':
    assert same_diagnostics('p'+MARKER+'[{"message":"a"},{"message":"b"}]','p'+MARKER+'[{"message":"b"},{"message":"a"}]')
    assert not same_diagnostics('p'+MARKER+'[{"message":"a"}]','p'+MARKER+'[{"message":"changed"}]')
    print('Reused reference-failure validations:',seed_failed_validations(),flush=True)
    import sys
    if '--generation-only' not in sys.argv:
        destination=sys.argv[sys.argv.index('--out')+1] if '--out' in sys.argv else 'experiment/output/formal_20260928'
        print('Archived infrastructure-only simulation attempts:',archive_infrastructure_simulations(runner.ROOT/destination),flush=True)
    runner.cli()
