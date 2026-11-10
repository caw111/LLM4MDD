"""Transport-only launcher using the already running local Clash proxy.

Preserves the frozen methods, request parameters, artifacts and retry gate.
No environment file or global system proxy is changed.
"""
import argparse
import sys

import run_parallel_retry3 as parallel


PROXY = 'http://127.0.0.1:7897'


def configure():
    from openai import DefaultHttpxClient
    import run_current_comparison as production
    old = production.api.client
    transport = DefaultHttpxClient(proxy=PROXY, trust_env=False)
    new = old.with_options(http_client=transport)
    assert new.api_key == old.api_key
    assert str(new.base_url) == str(old.base_url)
    assert new.timeout == old.timeout and new.max_retries == old.max_retries
    production.api.client = new
    return new


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--workers', type=int, default=12)
    parser.add_argument('--prepare-only', action='store_true')
    args = parser.parse_args()
    base = parallel.base
    base.freeze()
    assert base.sha(base.ROOT/'experiment/code/run_parallel_retry3.py') == base.read(base.OUT/'parallel_scheduling_amendment.json')['source_sha256']
    info = dict(scope='Transport recovery only; no new model call by prepare-only.',
        proxy=PROXY, source_sha256=base.sha(base.ROOT/'experiment/code/run_retry3_proxy.py'),
        execution_sha256=base.sha(base.OUT/'execution.json'),
        parallel_source_sha256=base.sha(base.ROOT/'experiment/code/run_parallel_retry3.py'),
        diagnostics='Direct openrouter.ai TLS connection timed out; existing local proxy CONNECT and verified TLS handshake succeeded. No API response or account recovery inferred.',
        service_retry_gate='Unchanged. Honor latest next_retry_not_before_utc; no early model request.',
        env_file_changed=False, prompts_or_models_or_budgets_changed=False)
    path = base.OUT/'transport_recovery.json'
    if path.exists():
        assert base.read(path) == info, 'Transport launcher changed'
    else:
        base.save(path,info)
    client=configure()
    try:
        if args.prepare_only:
            print('Transport configuration verified offline; no model request. Existing retry deadline retained.')
            return
        sys.argv=[str(base.ROOT/'experiment/code/run_parallel_retry3.py'),'--workers',str(args.workers)]
        parallel.main()
    finally:
        client.close()


if __name__=='__main__':
    main()
