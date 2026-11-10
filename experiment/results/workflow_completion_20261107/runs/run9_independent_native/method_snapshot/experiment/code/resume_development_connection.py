"""Diagnose transient service connections while preserving frozen development calls."""
import json,uuid
from datetime import datetime,timezone
import resume_development_repair as recovery
out=recovery.out
original_create=recovery.recovery.runner.main.client.chat.completions.create

def traced_create(*args,**kwargs):
    try:return original_create(*args,**kwargs)
    except Exception as exc:
        chain=[]; current=exc; seen=set()
        while current is not None and id(current) not in seen:
            seen.add(id(current))
            item={'type':type(current).__name__}
            for field in ('errno','winerror','status_code'):
                value=getattr(current,field,None)
                if isinstance(value,int):item[field]=value
            chain.append(item);current=current.__cause__ or current.__context__
        recovery.recovery.runner.save(out/'connection_diagnostics'/(uuid.uuid4().hex+'.json'),
            {'utc':datetime.now(timezone.utc).isoformat(),'exception_chain':chain})
        raise

recovery.recovery.runner.main.client.chat.completions.create=traced_create
original_pool=recovery.development.futures.ThreadPoolExecutor

def single_worker(*args,**kwargs):
    kwargs['max_workers']=1
    return original_pool(*args,**kwargs)

recovery.development.futures.ThreadPoolExecutor=single_worker
if __name__=='__main__':
    recovery.recovery.runner.save(out/'connection_recovery.json',{
        'utc':datetime.now(timezone.utc).isoformat(),'workers':1,
        'policy':'Infrastructure only: lower concurrency, trace exception types/numeric codes without messages or credentials; preserve prompts, models and method budgets.',
        'wrapper_sha256':recovery.recovery.runner.digest(recovery.recovery.runner.ROOT/'experiment/code/resume_development_connection.py')})
    recovery.development.main()
