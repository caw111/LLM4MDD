"""Resume after a verified local network repair; never bypass service refusals."""
import json
import os
import socket
import ssl
import sys

import run_retry3_proxy as transport


def main():
    parallel=transport.parallel;base=parallel.base
    os.chdir(base.ROOT);config=base.freeze()
    assert base.sha(base.ROOT/'experiment/code/run_retry3_proxy.py') == base.read(base.OUT/'transport_recovery.json')['source_sha256']
    # Only a transport error can be released after a verified changed route.
    path=base.OUT/'service_retry_status.json'
    old=base.read(path)
    if old['error_type']!='APIConnectionError':
        raise SystemExit('Current service failure is not a transport-only error; honor its retry deadline')
    with socket.create_connection(('127.0.0.1',7897),timeout=8) as sock:
        sock.settimeout(8)
        sock.sendall(b'CONNECT openrouter.ai:443 HTTP/1.1\r\nHost: openrouter.ai:443\r\n\r\n')
        header=b''
        while b'\r\n\r\n' not in header:
            chunk=sock.recv(4096)
            if not chunk:break
            header+=chunk
        assert header.split(b'\r\n',1)[0].split()[1]==b'200','Local proxy failed'
        with ssl.create_default_context().wrap_socket(sock,server_hostname='openrouter.ai'):pass
    snapshot=base.OUT/'connection_error_before_transport_repair.json'
    if not snapshot.exists():base.save(snapshot,old)
    base.save(base.OUT/'network_backoff_amendment.json',dict(
        scope='Transport repair only; original method/model/prompt/budget/evaluation remain frozen.',
        reason='Automatic six-hour wait was applied to APIConnectionError, not a server permission/quota refusal. After direct TLS timeout and successful verified TLS through the existing local proxy, allow one pending task as a live probe.',
        source_sha256=base.sha(base.ROOT/'experiment/code/resume_retry3_network.py'),
        previous_status=old,utc=base.utc().isoformat()))
    base.save(path,dict(error_type='transport_reconfigured_pending_probe',
                       next_retry_not_before_utc=base.utc().isoformat()))
    # Any other independent service-refusal deadline remains binding.
    if base.utc()<base.retry_gate():
        raise SystemExit('A separate service-refusal deadline still applies')
    client=transport.configure()
    try:
        probe=next(r for r in config['original_rows'] if r['group']=='historical_direct' and r['case']=='simple_019')
        result_path=base.OUT/'attempts/historical_direct/simple_019/attempt_4/result.json'
        old_result=base.read(result_path)
        assert old_result.get('infrastructure_error')=='APIConnectionError'
        base.save(result_path.parent/'connection_failure_before_proxy.json',old_result)
        result=base.evaluate(config,probe,4)
        base.report(config)
        base.save(base.OUT/'live_network_probe.json',dict(
            task='historical_direct/simple_019/attempt_4',
            actual_models=result.get('actual_models'),
            model_response_received=not result.get('infrastructure_error'),
            compile_ok=result['compile_ok'],functional_status=result['functional_status'],
            infrastructure_error=result.get('infrastructure_error'),utc=base.utc().isoformat()))
        if result.get('infrastructure_error'):
            print('Pending-task network probe did not recover:',result['infrastructure_error'],flush=True)
            return
        print('Pending-task API response received through existing proxy; resuming 12 workers.',flush=True)
        sys.argv=[str(base.ROOT/'experiment/code/run_parallel_retry3.py'),'--workers','12']
        parallel.main()
    finally:client.close()


if __name__=='__main__':main()
