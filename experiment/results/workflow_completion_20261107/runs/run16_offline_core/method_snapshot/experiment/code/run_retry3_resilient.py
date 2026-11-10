"""Transport-only supervision: bounded network recovery, strict refusal cooldown."""
import sys
import time
from datetime import datetime, timedelta

import run_retry3_proxy as transport


NETWORK = {'APIConnectionError', 'APITimeoutError'}


def network_delay(kind):
    return 300 if kind in NETWORK else 21600


def main():
    parallel=transport.parallel;base=parallel.base
    if '--self-check' in sys.argv:
        assert network_delay('APIConnectionError')==300
        assert network_delay('APITimeoutError')==300
        assert network_delay('PermissionDeniedError')==21600
        assert network_delay('RateLimitError')==21600
        assert network_delay('UnexpectedActualModel')==21600
        print('Network retries bounded; quota, permission and other errors retain six-hour stop.')
        return
    base.freeze()
    assert base.sha(base.ROOT/'experiment/code/run_retry3_proxy.py')==base.read(base.OUT/'transport_recovery.json')['source_sha256']
    assert base.sha(base.ROOT/'experiment/code/run_parallel_retry3.py')==base.read(base.OUT/'parallel_scheduling_amendment.json')['source_sha256']
    amendment=dict(scope='Network recovery only; no model/prompt/budget/test changes.',
        network_errors=sorted(NETWORK),network_wait_seconds=300,max_automatic_resume_sessions=3,
        service_refusal_wait_seconds=21600,workers=12,
        source_sha256=base.sha(base.ROOT/'experiment/code/run_retry3_resilient.py'),
        execution_sha256=base.sha(base.OUT/'execution.json'))
    path=base.OUT/'network_retry_policy_amendment.json'
    if path.exists():assert base.read(path)==amendment
    else:base.save(path,amendment)
    status_path=base.OUT/'service_retry_status.json'
    if status_path.exists():
        old=base.read(status_path)
        # The previous generic block function incorrectly applied a refusal wait
        # to network failures. Keep its history; never shorten refusal deadlines.
        if old.get('error_type') in NETWORK:
            correct=datetime.fromisoformat(old['failed_utc'])+timedelta(seconds=300)
            if datetime.fromisoformat(old['next_retry_not_before_utc'])>correct:
                base.save(base.OUT/('network_backoff_history_'+str(time.time_ns())+'.json'),old)
                base.save(status_path,dict(old,next_retry_not_before_utc=correct.isoformat(),
                                          scheduling_amendment='network_retry_policy_amendment.json'))
    def block(kind):
        now=base.utc()
        if status_path.exists():
            base.save(base.OUT/('service_failure_history_'+str(time.time_ns())+'.json'),base.read(status_path))
        base.save(status_path,dict(error_type=kind,failed_utc=now.isoformat(),
            next_retry_not_before_utc=(now+timedelta(seconds=network_delay(kind))).isoformat()))
    client=transport.configure()
    try:
        import run_paper_ablation as ab
        import rerun_workflow_gpt61 as sol
        for session in range(1,4):
            gate=base.retry_gate()
            remaining=(gate-base.utc()).total_seconds()
            if remaining>0:
                kind=base.read(status_path).get('error_type') if status_path.exists() else None
                base.save(base.OUT/'run_status.json',dict(status='waiting_transport_retry' if kind in NETWORK else 'waiting_service_retry',
                    next_retry_not_before_utc=gate.isoformat(),session=session,updated_utc=base.utc().isoformat()))
                if kind not in NETWORK or remaining>301:return
                # This wait happens in a finite background supervisor, not a tool.
                time.sleep(max(0,remaining))
            ab.SERVICE_BLOCK.clear();sol.STOP.clear();base.block_service=block
            sys.argv=[str(base.ROOT/'experiment/code/run_parallel_retry3.py'),'--workers','12']
            parallel.main()
            current=base.read(base.OUT/'run_status.json')
            if current.get('status')=='finished':return
            if not status_path.exists() or base.read(status_path).get('error_type') not in NETWORK:return
        base.save(base.OUT/'network_resume_limit.json',dict(
            reason='Three finite network-recovery sessions exhausted; preserve checkpoints for later resume.',
            utc=base.utc().isoformat()))
    finally:client.close()


if __name__=='__main__':main()
