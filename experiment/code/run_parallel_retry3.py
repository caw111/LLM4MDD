"""Parallel scheduling only; reuse the frozen generation/evaluation methods."""
import argparse
from concurrent.futures import ThreadPoolExecutor, wait, FIRST_COMPLETED
import os
import threading
import time

import run_pass3_followup as base


def settled(row, host):
    return row['compile_ok'] and (not host or row['functional_status']!='failed')


def jobs(config):
    result=[]
    originals=sorted(config['original_rows'],key=lambda r:(r['case'] not in config['host_cases'],base.GROUPS.index(r['group']),r['case']))
    for original in originals:
        host=original['case'] in config['host_cases']
        if not base.failed(original,host):continue
        pending=[]
        for n in (2,3,4):
            path=base.OUT/'attempts'/original['group']/original['case']/f'attempt_{n}'/'result.json'
            if path.exists():
                value=base.read(path)
                if not value.get('infrastructure_error'):
                    if settled(value,host):
                        pending=[]
                        break
                    continue
            pending.append((original,n))
        result.extend(pending)
    keys=[(r['group'],r['case'],n) for r,n in result]
    assert len(keys)==len(set(keys))
    return result


def run(config,workers):
    # Globals in the existing method modules are shared by threads, as in the
    # original concurrent ablation runner. Each attempt has a distinct directory.
    import run_paper_ablation as ab
    import rerun_workflow_gpt61 as sol
    stop=threading.Event();state_lock=threading.Lock();settled_keys=set()
    original_block=base.block_service
    def block(kind):
        with state_lock:
            if not stop.is_set():original_block(kind)
            stop.set();ab.SERVICE_BLOCK.set();sol.STOP.set()
    base.block_service=block
    def worker(original,n):
        key=original['group'],original['case']
        with state_lock:
            if stop.is_set() or key in settled_keys:return None
        path=base.OUT/'attempts'/key[0]/key[1]/f'attempt_{n}'/'result.json'
        if path.exists():
            old=base.read(path)
            if old.get('infrastructure_error'):
                base.save(path.parent/('infrastructure_history_'+str(time.time_ns())+'.json'),old)
        row=base.evaluate(config,original,n)
        with state_lock:
            if row.get('infrastructure_error'):stop.set()
            if settled(row,key[1] in config['host_cases']):settled_keys.add(key)
        return row
    pending=iter(jobs(config));futures={};completed=0
    with ThreadPoolExecutor(max_workers=workers) as pool:
        def submit():
            while not stop.is_set():
                job=next(pending,None)
                if job is None:return False
                r,n=job
                with state_lock:
                    if (r['group'],r['case']) in settled_keys:continue
                futures[pool.submit(worker,r,n)]=(r['group'],r['case'],n)
                return True
            return False
        for _ in range(workers):submit()
        while futures:
            done,_=wait(futures,timeout=30,return_when=FIRST_COMPLETED)
            if not done:
                base.save(base.OUT/'run_status.json',dict(status='running_parallel',workers=workers,
                    active=len(futures),finished_in_this_process=completed,updated_utc=base.utc().isoformat()))
                continue
            for future in done:
                key=futures.pop(future)
                if future.cancelled():continue
                try:row=future.result()
                except Exception as exc:
                    stop.set();ab.SERVICE_BLOCK.set();sol.STOP.set()
                    base.save(base.OUT/'parallel_harness_failure.json',dict(job=key,error_type=type(exc).__name__,utc=base.utc().isoformat()))
                    continue
                if row is not None:
                    completed+=1
                    print(row['group'],row['case'],row['attempt'],row['compile_ok'],row['functional_pass'],flush=True)
            with state_lock:keys=set(settled_keys)
            for future,key in list(futures.items()):
                if stop.is_set() or key[:2] in keys:
                    if future.cancel():futures.pop(future)
            base.report(config)
            while len(futures)<workers and submit():pass
    summary=base.report(config)
    base.save(base.OUT/'run_status.json',dict(status='infrastructure_stopped' if stop.is_set() else 'finished',
        complete=summary['complete'],workers=workers,finished_in_this_process=completed,updated_utc=base.utc().isoformat()))


def self_check():
    assert settled(dict(compile_ok=True,functional_status='no_validated_oracle'),False)
    assert not settled(dict(compile_ok=True,functional_status='failed'),True)
    assert settled(dict(compile_ok=True,functional_status='passed'),True)
    assert not settled(dict(compile_ok=False,functional_status='not_generated'),False)
    base.self_check()
    config=base.freeze()
    selected=jobs(config)
    assert all(base.failed(r,r['case'] in config['host_cases']) and n in (2,3,4) for r,n in selected)
    assert all(sum(r['case']==c['id'] for r in config['original_rows'])==11 for c in config['cases'])
    print('Unique failure-only scheduling, three-attempt ceilings and success gates checked; jobs:',len(selected))


def main():
    parser=argparse.ArgumentParser();parser.add_argument('--workers',type=int,default=12);parser.add_argument('--self-check',action='store_true')
    args=parser.parse_args()
    if not 1<=args.workers<=24:parser.error('workers must be 1..24')
    if args.self_check:self_check();return
    os.chdir(base.ROOT);config=base.freeze()
    gate=base.retry_gate()
    if base.utc()<gate:
        base.save(base.OUT/'run_status.json',dict(status='waiting_service_retry',next_retry_not_before_utc=gate.isoformat()));return
    lock=base.OUT/'running.lock'
    try:handle=lock.open('x',encoding='utf-8')
    except FileExistsError:raise SystemExit('An existing run lock must be checked against actual process identity')
    with handle:handle.write(str(os.getpid()))
    try:
        path=base.OUT/'parallel_scheduling_amendment.json'
        amendment=dict(authorization='User requested concurrent tasks and up to three simultaneous fresh attempts for each failed task, 2026-10-23.',
            workers=args.workers,source_sha256=base.sha(base.ROOT/'experiment/code/run_parallel_retry3.py'),
            frozen_execution_sha256=base.sha(base.OUT/'execution.json'),
            semantics='Same frozen prompts/models/within-method budgets/test standards. Up to three extra attempts per failed group/task. Cancel queued attempts after a success; retain and charge already in-flight calls.',
            baseline_artifacts_changed=False)
        if path.exists():assert base.read(path)==amendment,'Parallel scheduler changed'
        else:base.save(path,amendment)
        os.environ['PATH']=str(base.ROOT/'.tools/arduino')+os.pathsep+os.environ['PATH']
        run(config,args.workers)
    finally:lock.unlink(missing_ok=True)


if __name__=='__main__':main()
