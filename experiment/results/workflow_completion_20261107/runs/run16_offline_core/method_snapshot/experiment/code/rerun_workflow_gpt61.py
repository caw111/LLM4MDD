"""Rerun the 29 failures of the reported 267/296 workflow with GPT-6.1."""
import argparse
import copy
import json
import os
import threading
import time
from collections import Counter
from concurrent.futures import ThreadPoolExecutor, as_completed
from datetime import datetime, timezone
from pathlib import Path

import run_current_comparison as workflow
from app.services.configuration_policy import resolve_configuration
from app.services.llm_json import CompletionText, completion_text, request_options
from method_snapshot import runtime_identity

ROOT = workflow.ROOT
BASE = ROOT / 'experiment/output/configured_audit_v1_20261015'
PREVIOUS = ROOT / 'experiment/output/repaired_workflow_continuation_20261016/cumulative_workflow_results.json'
OUT = ROOT / 'experiment/output/workflow_gpt61_failures_20261020'
MODEL = 'openai/gpt-6.1-sol'
STOP = threading.Event()
save = workflow.save


def utc():
    return datetime.now(timezone.utc).isoformat()


class GPTCalls:
    def __init__(self, folder, client=None):
        self.folder, self.client, self.cursor = folder, client, 0
        path = folder / 'calls.json'
        self.entries = json.loads(path.read_text(encoding='utf-8')) if path.exists() else []

    def __call__(self, prompt):
        options = request_options(prompt)
        while self.cursor < len(self.entries):
            entry = self.entries[self.cursor]
            self.cursor += 1
            if 'error_type' in entry:
                continue
            assert entry['prompt'] == prompt, 'Cached prompt changed'
            assert entry['requested_model'] == MODEL
            assert entry.get('response_format') == options.get('response_format')
            return entry['actual_model'], CompletionText(entry['response'],
                finish_reason=entry['finish_reason'], refusal=entry.get('refusal'),
                response_format=options.get('response_format'))
        if sum('response' in e for e in self.entries) >= 6:
            raise ValueError('Six-response workflow budget exhausted')
        for attempt in range(3):
            if STOP.is_set():
                raise RuntimeError('Service requests stopped after infrastructure failure')
            started = time.monotonic()
            entry = dict(number=len(self.entries)+1, prompt=str(prompt), requested_model=MODEL,
                         max_completion_tokens=6000, reasoning_effort='medium', **options)
            self.entries.append(entry)
            try:
                client = self.client or workflow.api.client
                response = client.with_options(timeout=180, max_retries=0).chat.completions.create(
                    model=MODEL, max_completion_tokens=6000, reasoning_effort='medium',
                    messages=[dict(role='system', content='You are an AI assistant specialized in embedded systems and code generation.'),
                              dict(role='user', content=str(prompt))], **options)
                choice = response.choices[0]
                entry.update(response=choice.message.content or '', actual_model=response.model,
                    response_id=response.id, finish_reason=choice.finish_reason,
                    refusal=getattr(choice.message, 'refusal', None),
                    usage=response.usage.model_dump() if response.usage else None)
                if 'gpt-6.1-sol' not in response.model:
                    STOP.set()
                    raise RuntimeError('Unexpected actual model; no substitution allowed')
                return response.model, completion_text(response, prompt)
            except Exception as exc:
                entry.update(error_type=type(exc).__name__, status_code=getattr(exc, 'status_code', None))
                transient = type(exc).__name__ in ('APIConnectionError', 'APITimeoutError', 'InternalServerError')
                if not transient or attempt == 2:
                    STOP.set()
                    raise RuntimeError('LLM infrastructure failure: '+type(exc).__name__) from None
            finally:
                entry['seconds'] = round(time.monotonic()-started, 3)
                self.cursor = len(self.entries)
                save(self.folder/'calls.json', self.entries)
            time.sleep(2*(attempt+1))


def evaluate(case):
    folder = OUT / case['id']
    path = folder / 'result.json'
    if path.exists():
        old = json.loads(path.read_text(encoding='utf-8'))
        if not old.get('infrastructure_error'):
            return old
        save(folder / ('previous_infrastructure_'+str(time.time_ns())+'.json'), old)
    case = copy.deepcopy(case)
    case['requirement'] += ('\n'+workflow.INTERPRETATIONS[case['id']]) if case['id'] in workflow.INTERPRETATIONS else ''
    resolution = resolve_configuration(case['requirement'], {}, board=case['fqbn'])
    case['requirement'] = resolution['effective_requirement']
    save(folder/'configuration_resolution.json', resolution)
    calls = GPTCalls(folder/'calls')
    row = dict(case=case['id'], dataset=case['dataset'], arm='workflow_gpt61', repeat=1,
        compile_ok=False, compile_status='not_generated', functional_status='not_generated_or_compile_failed')
    stage = 'interface_contract'
    try:
        contract, planning = workflow.plan_interface_contract(calls, case['requirement'], {},
            target_board=case['fqbn'], max_repairs=2, configuration_resolution=resolution)
        save(folder/'contract.json', contract)
        save(folder/'contract_planning.json', planning)
        row.update(contract_ok=True, contract_sha256=planning['contract_sha256'])
        stage = 'sysml_generation'
        accepted = {}

        def validator(source):
            checked = workflow.validate_candidate(source)
            if checked['ok']:
                accepted.update(source=source, ir=checked['ir'])
            return checked

        _, bundle, validation = workflow.generate_validated_bundle(calls,
            workflow.sysml_bundle_prompt(case['requirement'], contract=contract),
            max_repairs=2, validator=validator, contract=contract)
        save(folder/'model.json', bundle)
        save(folder/'model_validation.json', validation)
        row.update(model_ok=True, model_attempts=len(validation['attempts']))
        source = bundle['system_sysml2']
        assert source == accepted['source'], 'Accepted source mismatch'
        stage = 'ir_to_code'
        with workflow.TRANSLATOR, workflow.JAVA:
            generated = workflow.lower_verified(source, contract, accepted['ir'], folder/'lowered')
        row.update(backend_ok=True, provenance_status=generated.provenance_status)
        stage = 'hardware_adapter'
        code = workflow.adapt_verified(generated, contract, folder/'lowered')
        receipt = json.loads((folder/'lowered/arduino_provenance.json').read_text(encoding='utf-8'))
        stage = 'arduino_compile'
        result, _ = workflow.compile_candidate(case, folder/'workflow/attempt_0', code, receipt)
        row.update(result)
        if not row['compile_ok']:
            row['failure_stage'] = stage
    except (workflow.ContractPlanningError, workflow.SysMLValidationError) as exc:
        report = dict(ok=False, attempts=exc.attempts)
        if isinstance(exc, workflow.ContractPlanningError):
            report['blocking_category'] = exc.blocking_category
            row['blocking_category'] = exc.blocking_category
        save(folder/('contract_planning.json' if stage == 'interface_contract' else 'model_validation.json'), report)
        row.update(failure_stage=(exc.attempts[-1].get('failure_stage') if exc.attempts else None) or stage,
                   error=str(exc), model_ok=False if stage != 'interface_contract' else None)
    except workflow.BackendFailure as exc:
        row.update(failure_stage=stage, backend_ok=False, error=str(exc))
    except workflow.api.HTTPException as exc:
        if exc.status_code in (400, 422):
            row.update(failure_stage=stage, error=str(exc.detail)[:3000])
        else:
            row.update(failure_stage=stage, infrastructure_error=type(exc).__name__, functional_status='unverified')
            STOP.set()
    except (ValueError, TypeError, KeyError) as exc:
        row.update(failure_stage=stage, error=str(exc)[:3000])
    except Exception as exc:
        row.update(failure_stage=stage, infrastructure_error=type(exc).__name__, functional_status='unverified')
        STOP.set()
    row.update(workflow.metrics(calls.entries))
    row.update(actual_models=sorted({e['actual_model'] for e in calls.entries if 'actual_model' in e}), finished_utc=utc())
    save(path, row)
    return row


def summarize(config):
    rows = [json.loads(p.read_text(encoding='utf-8')) for p in sorted(OUT.glob('*/result.json'))]
    expected = {c['id'] for c in config['cases']}
    assert len(rows) == len({r['case'] for r in rows}) and {r['case'] for r in rows} <= expected
    changed = [p for p,h in config['hashes'].items() if workflow.digest(ROOT/p) != h]
    bad_artifacts = [r['case'] for r in rows if r.get('artifact') and workflow.digest(ROOT/r['artifact']) != r['artifact_sha256']]
    entries = [e for p in OUT.glob('*/calls/calls.json') for e in json.loads(p.read_text(encoding='utf-8'))]
    bad_models = sorted({e['actual_model'] for e in entries if 'actual_model' in e and 'gpt-6.1-sol' not in e['actual_model']})
    infra = [r['case'] for r in rows if r.get('infrastructure_error')]
    passed = sum(r['compile_ok'] for r in rows)
    successful = [e for e in entries if 'response' in e and 'error_type' not in e]
    usage = [e.get('usage') or {} for e in successful]
    summary = dict(planned=29, completed=len(rows), compile_passes=passed, rerun_rate=passed/29,
        original_compile_passes=267, denominator=296, cumulative_compile_passes=267+passed,
        cumulative_compile_rate=(267+passed)/296, model=MODEL,
        complete={r['case'] for r in rows} == expected and not infra and not changed and not bad_artifacts and not bad_models,
        infrastructure=infra, changed_sources=changed, artifact_mismatches=bad_artifacts, unexpected_models=bad_models,
        failure_stages=dict(Counter(r.get('failure_stage', r['compile_status']) for r in rows if not r['compile_ok'] and not r.get('infrastructure_error'))),
        functional_status=dict(Counter(r['functional_status'] for r in rows)),
        by_dataset={d:dict(total=sum(r['dataset']==d for r in rows), passed=sum(r['dataset']==d and r['compile_ok'] for r in rows)) for d in sorted({c['dataset'] for c in config['cases']})},
        requests=len(entries), responses=len(successful),
        input_tokens=sum(u.get('prompt_tokens', 0) for u in usage),
        output_tokens=sum(u.get('completion_tokens', 0) for u in usage),
        total_tokens=sum(u.get('total_tokens', 0) for u in usage),
        reported_cost_usd=sum(u.get('cost', 0) or 0 for u in usage),
        missing_cost_responses=sum(u.get('cost') is None for u in usage), updated_utc=utc())
    save(OUT/'summary.json', summary)
    save(OUT/'results.json', rows)
    old = json.loads(PREVIOUS.read_text(encoding='utf-8'))
    updated = {r['case']:r for r in old}
    for row in rows:
        if row['compile_ok']:
            updated[row['case']] = row
    save(OUT/'cumulative_workflow_results.json', list(updated.values()))
    lines = ['# GPT-6.1 工作流失败题补跑', '',
        f'已完成 {len(rows)}/29 题；全部记录、源码和工件核查通过：{summary["complete"]}。', '',
        f'本次目标板编译通过 **{passed}/29（{passed/29:.1%}）**。',
        f'原累计通过 267 题，加本次新增 {passed} 题，合计 **{267+passed}/296（{(267+passed)/296:.1%}）**。', '',
        '模型为 openai/gpt-6.1-sol，reasoning_effort=medium；不发送该模型不支持的temperature；max_completion_tokens=6000（包含推理tokens）。',
        '只对原90.2%结果的29个编译未通过任务完整重跑：接口首稿及最多2次修复，模型首稿及最多2次修复，官方SysML校验、确定性代码生成、硬件适配、目标板编译和已有独立主机测试。',
        '当前方法源码与上一轮冻结消融源码一致，未改变提示或通过标准；原始成功题及直接生成组没有重跑。', '',
        '失败阶段：'+json.dumps(summary['failure_stages'], ensure_ascii=False),
        '独立功能测试状态：'+json.dumps(summary['functional_status'], ensure_ascii=False),
        f'调用请求 {summary["requests"]} 次，收到响应 {summary["responses"]} 次；输入 {summary["input_tokens"]:,}、输出 {summary["output_tokens"]:,}、合计 {summary["total_tokens"]:,} tokens；API报告费用 ${summary["reported_cost_usd"]:.6f}。费用缺失响应：{summary["missing_cost_responses"]}。', '',
        '这是针对历史失败题更换模型后的定向补跑，296题合计混合历史版本及模型，不能当作GPT-6.1在296题上的独立单轮成功率或等预算对比。编译通过不等于功能正确；无独立测试的题目不计功能成功。', '',
        '| 任务 | 编译 | 功能状态 | 失败阶段 |', '|---|---|---|---|']
    lines += [f'| {r["case"]} | {"通过" if r["compile_ok"] else "未通过"} | {r["functional_status"]} | {r.get("failure_stage", "")} |' for r in rows]
    (OUT/'REPORT_ZH.md').write_text('\n'.join(lines)+'\n', encoding='utf-8')
    return summary


def self_check():
    from tempfile import TemporaryDirectory
    with TemporaryDirectory() as directory:
        folder = Path(directory)
        save(folder/'calls.json', [dict(prompt='same', requested_model=MODEL, actual_model=MODEL,
             response='{}', finish_reason='stop', seconds=0)])
        calls = GPTCalls(folder)
        assert calls('same')[1] == '{}'
        try:
            GPTCalls(folder)('different')
            raise AssertionError('Mismatched cached prompt was accepted')
        except AssertionError as exc:
            assert str(exc) == 'Cached prompt changed'
        save(folder/'calls.json', [dict(response='{}', seconds=0)]*6)
        calls = GPTCalls(folder)
        calls.cursor = 6
        try:
            calls('seventh')
            raise AssertionError('Budget gate did not stop request')
        except ValueError as exc:
            assert 'budget' in str(exc)
    print('self-check passed', flush=True)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--self-check', action='store_true')
    parser.add_argument('--report-only', action='store_true')
    args = parser.parse_args()
    if args.self_check:
        self_check()
        return
    os.chdir(ROOT)
    os.environ['PATH'] = str(workflow.CLI.parent)+os.pathsep+os.environ['PATH']
    os.environ['WINDOWS_ARDUINO_CLI_PATH'] = str(workflow.CLI)
    baseline = json.loads((BASE/'execution.json').read_text(encoding='utf-8'))
    previous = json.loads(PREVIOUS.read_text(encoding='utf-8'))
    failures = {r['case'] for r in previous if not r['compile_ok']}
    assert len(previous) == len({r['case'] for r in previous}) == 296 and len(failures) == 29
    cases = [c for c in baseline['cases'] if c['id'] in failures]
    assert len(cases) == 29
    anchor = ROOT/'experiment/output/paper_ablation_v2_20261017/execution.json'
    frozen = json.loads(anchor.read_text(encoding='utf-8'))
    assert not [p for p,h in frozen['hashes'].items() if workflow.digest(ROOT/p) != h], 'Existing method changed from frozen ablation'
    for name, expected in frozen['runtime']['binaries_sha256'].items():
        if expected is not None:
            assert workflow.digest(ROOT/name) == expected, 'Frozen tool binary changed: '+name
    hashes = {**frozen['hashes'], str(Path(__file__).relative_to(ROOT)):workflow.digest(Path(__file__)),
              str(PREVIOUS.relative_to(ROOT)):workflow.digest(PREVIOUS),
              str((BASE/'execution.json').relative_to(ROOT)):workflow.digest(BASE/'execution.json')}
    hashes.update({name:expected for name,expected in frozen['runtime']['binaries_sha256'].items() if expected is not None})
    config = dict(cases=cases, model=MODEL, reasoning_effort='medium', temperature=None,
        max_completion_tokens=6000, max_contract_repairs=2, max_model_repairs=2, workers=4,
        hashes=hashes, runtime=runtime_identity(), scope='29 failures of historical cumulative 267/296; full workflow rerun; no direct calls')
    OUT.mkdir(parents=True, exist_ok=True)
    if (OUT/'execution.json').exists():
        assert config == json.loads((OUT/'execution.json').read_text(encoding='utf-8')), 'Rerun configuration changed'
    else:
        save(OUT/'execution.json', config)
    if args.report_only:
        print(json.dumps(summarize(config), ensure_ascii=True), flush=True)
        return
    lock = OUT/'running.lock'
    descriptor = os.open(lock, os.O_CREAT | os.O_EXCL | os.O_WRONLY)
    os.close(descriptor)
    try:
        save(OUT/'run_status.json', dict(status='running', pid=os.getpid(), started_utc=utc()))
        with ThreadPoolExecutor(max_workers=4) as pool:
            pending = {pool.submit(evaluate, case) for case in cases}
            for future in as_completed(pending):
                future.result()
                state = summarize(config)
                print(json.dumps({k:state[k] for k in ('completed','compile_passes','infrastructure')}, ensure_ascii=True), flush=True)
        state = summarize(config)
        save(OUT/'run_status.json', dict(status='complete' if state['complete'] else 'incomplete', finished_utc=utc()))
        print(json.dumps(state, ensure_ascii=True), flush=True)
    finally:
        lock.unlink(missing_ok=True)


if __name__ == '__main__':
    main()
