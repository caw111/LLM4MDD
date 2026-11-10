"""One GPT-6.1 direct-generation response per task, with unchanged target checks."""
import argparse
import copy
import json
import os
import time
from collections import Counter
from concurrent.futures import FIRST_COMPLETED, ThreadPoolExecutor, wait
from datetime import datetime, timedelta, timezone
from pathlib import Path

import rerun_workflow_gpt61 as shared
from app.services.configuration_policy import resolve_configuration

runner = shared.workflow
ROOT = runner.ROOT
OUT = ROOT/'experiment/output/direct_gpt61_full_20261021'
BASE = shared.BASE
save = runner.save


class DirectCalls(shared.GPTCalls):
    def __call__(self, prompt):
        # Replaying the only saved response is allowed; generating a second is not.
        if self.cursor >= len(self.entries) and any('response' in e for e in self.entries):
            raise ValueError('Single-response direct-generation budget exhausted')
        return super().__call__(prompt)


def prepare(case):
    case = copy.deepcopy(case)
    if case['id'] in runner.INTERPRETATIONS:
        case['requirement'] += '\n'+runner.INTERPRETATIONS[case['id']]
    resolution = resolve_configuration(case['requirement'], {}, board=case['fqbn'])
    case['requirement'] = resolution['effective_requirement']
    return case, resolution, runner.DIRECT_PROMPT+case['requirement']


def evaluate(original):
    folder = OUT/original['id']
    result_path = folder/'result.json'
    if result_path.exists():
        previous = json.loads(result_path.read_text(encoding='utf-8'))
        if not previous.get('infrastructure_error'):
            return previous
        save(folder/('previous_infrastructure_'+str(time.time_ns())+'.json'), previous)
    case, resolution, prompt = prepare(original)
    save(folder/'configuration_resolution.json', resolution)
    calls = DirectCalls(folder/'calls')
    row = dict(case=case['id'], dataset=case['dataset'], fqbn=case['fqbn'], arm='direct_gpt61', repeat=1,
               compile_ok=False, compile_status='not_generated', functional_status='not_generated_or_compile_failed')
    stage = 'direct_generation'
    try:
        _, raw = calls(prompt)
        runner.check_completion(raw)
        code = runner.extract(raw)
        stage = 'arduino_compile'
        result, _ = runner.compile_candidate(case, folder/'direct/attempt_0', code)
        row.update(result)
        if not row['compile_ok']:
            row['failure_stage'] = stage
    except (ValueError, TypeError, KeyError) as exc:
        row.update(failure_stage=stage, error_type=type(exc).__name__, error=str(exc)[:2000])
    except Exception as exc:
        row.update(failure_stage=stage, infrastructure_error=type(exc).__name__, functional_status='unverified')
        shared.STOP.set()
    assert sum('response' in e for e in calls.entries) <= 1
    row.update(runner.metrics(calls.entries), finished_utc=shared.utc(),
               actual_models=sorted({e['actual_model'] for e in calls.entries if 'actual_model' in e}))
    save(result_path, row)
    return row


def summarize(config):
    rows = [json.loads(p.read_text(encoding='utf-8')) for p in sorted(OUT.glob('*/result.json'))]
    expected = {c['id'] for c in config['cases']}
    assert len(rows) == len({r['case'] for r in rows}) and {r['case'] for r in rows} <= expected
    changed = [p for p,h in config['hashes'].items() if runner.digest(ROOT/p) != h]
    mismatches = [r['case'] for r in rows if r.get('artifact') and runner.digest(ROOT/r['artifact']) != r['artifact_sha256']]
    entries = [e for p in OUT.glob('*/calls/calls.json') for e in json.loads(p.read_text(encoding='utf-8'))]
    responses = [e for e in entries if 'response' in e and 'error_type' not in e]
    usage = [e.get('usage') or {} for e in responses]
    actual = dict(Counter(e['actual_model'] for e in responses))
    infra = [r['case'] for r in rows if r.get('infrastructure_error')]
    compiled = sum(r['compile_ok'] for r in rows)
    complete = {r['case'] for r in rows} == expected and not infra and not changed and not mismatches
    stats = dict(model=shared.MODEL, planned=296, completed=len(rows), compile_passes=compiled,
        compile_rate=compiled/296, complete=complete, infrastructure=infra,
        changed_sources=changed, artifact_mismatches=mismatches, actual_models=actual,
        above_267_of_296=compiled > 267 if complete else None,
        difference_from_267_percentage_points=(compiled-267)/296*100 if complete else None,
        compile_status=dict(Counter(r['compile_status'] for r in rows)),
        failure_stages=dict(Counter(r.get('failure_stage',r['compile_status']) for r in rows if not r['compile_ok'] and not r.get('infrastructure_error'))),
        functional_status=dict(Counter(r['functional_status'] for r in rows)),
        oracle_subset_status=dict(Counter(r['functional_status'] for r in rows if r['case'] in runner.ORACLE_TASKS)),
        oracle_subset_planned=len(runner.ORACLE_TASKS), no_independent_oracle=296-len(runner.ORACLE_TASKS),
        by_dataset={d:dict(planned=sum(c['dataset']==d for c in config['cases']),
            completed=sum(r['dataset']==d for r in rows), passed=sum(r['dataset']==d and r['compile_ok'] for r in rows))
            for d in sorted({c['dataset'] for c in config['cases']})},
        requests=len(entries), responses=len(responses), input_tokens=sum(u.get('prompt_tokens',0) for u in usage),
        output_tokens=sum(u.get('completion_tokens',0) for u in usage), total_tokens=sum(u.get('total_tokens',0) for u in usage),
        reported_cost_usd=sum(u.get('cost',0) or 0 for u in usage), missing_cost_responses=sum(u.get('cost') is None for u in usage),
        updated_utc=shared.utc())
    save(OUT/'summary.json', stats)
    save(OUT/'results.json', rows)
    lines = ['# GPT-6.1 Sol 全量直接生成实验', '',
        f'完成 {len(rows)}/296 题；生成及编译记录核查完成：{complete}。',
        f'目标板编译通过 **{compiled}/296（{compiled/296:.1%}）**。', '',
        '设置：openai/gpt-6.1-sol；reasoning_effort=medium；max_completion_tokens=6000（含推理tokens）；不发送temperature。',
        '296题各运行一轮，每题仅接收一次模型响应；不提供编译诊断修复，不生成SysML或接口契约，不借用工作流产物。',
        '题目、公开配置补充（configuration-defaults-v3）、提示、Uno/Mega目标板及编译检查沿用冻结消融实验第一轮。全部296个直接生成提示与该轮直接生成首稿逐字核对一致。', '',
        f'参考工作流历史累计结果为267/296（90.2%）。本次差值为{(compiled-267)/296*100:+.2f}个百分点；是否超过仅在本轮完成后判断：{stats["above_267_of_296"]}。',
        '该参考来自历史累计修复，且模型与生成预算不同；这里仅比较观察到的编译率，不作为同模型、同预算的工作流因果对比。', '',
        '## 分数据来源', '', '| 来源 | 已完成 / 计划 | 编译通过 |', '|---|---:|---:|']
    lines += [f'| {d} | {v["completed"]}/{v["planned"]} | {v["passed"]} |' for d,v in stats['by_dataset'].items()]
    lines += ['', '## 功能测试与消耗', '',
        '独立主机功能测试仅覆盖固定16题，另外280题无独立oracle，不能当作功能成功。状态：'+json.dumps(stats['oracle_subset_status'],ensure_ascii=False),
        f'请求 {stats["requests"]} 次，响应 {stats["responses"]} 次；输入 {stats["input_tokens"]:,}、输出 {stats["output_tokens"]:,}、合计 {stats["total_tokens"]:,} tokens；API报告费用 ${stats["reported_cost_usd"]:.6f}。费用缺失响应 {stats["missing_cost_responses"]}。',
        '编译通过不等于行为正确；原始响应、编译日志、代码和测试结果逐题保留。', '',
        '## 未通过任务', '', '| 任务 | 阶段 | 编译状态 |', '|---|---|---|']
    lines += [f'| {r["case"]} | {r.get("failure_stage","")} | {r["compile_status"]} |' for r in rows if not r['compile_ok']]
    (OUT/'REPORT_ZH.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
    return stats


def freeze():
    baseline = json.loads((BASE/'execution.json').read_text(encoding='utf-8'))
    cases = baseline['cases']
    assert len(cases) == len({c['id'] for c in cases}) == 296
    anchor = ROOT/'experiment/output/paper_ablation_v2_20261017/execution.json'
    previous = json.loads(anchor.read_text(encoding='utf-8'))
    hashes = dict(previous['hashes'])
    assert not [p for p,h in hashes.items() if runner.digest(ROOT/p) != h], 'Frozen method changed'
    for p,h in previous['runtime']['binaries_sha256'].items():
        if h is not None:
            assert runner.digest(ROOT/p) == h, 'Frozen tool binary changed: '+p
            hashes[p] = h
    for path in (Path(__file__), Path(shared.__file__), BASE/'execution.json'):
        hashes[str(path.relative_to(ROOT))] = runner.digest(path)
    # Verify identical inputs against actual frozen first-call ledgers, not just IDs.
    for case in cases:
        ledger = anchor.parent/'jobs'/case['id']/'repeat_1/direct_calls/calls.json'
        entries = json.loads(ledger.read_text(encoding='utf-8'))
        first = next(e for e in entries if 'response' in e and 'error_type' not in e)
        assert first['prompt'] == prepare(case)[2], 'Frozen direct prompt differs: '+case['id']
    config = dict(cases=cases, hashes=hashes, model=shared.MODEL, reasoning_effort='medium',
        max_completion_tokens=6000, temperature=None, repetitions=1, max_responses_per_task=1,
        workers=8, reference_workflow_passes=267, frozen_prompt_matches=296,
        runtime=shared.runtime_identity(), scope='Full 296-task, one-response direct generation; no compiler feedback')
    OUT.mkdir(parents=True,exist_ok=True)
    path = OUT/'execution.json'
    if path.exists():
        assert config == json.loads(path.read_text(encoding='utf-8')), 'Frozen configuration changed'
    else:
        save(path, config)
    return config


def self_check():
    from tempfile import TemporaryDirectory
    with TemporaryDirectory() as directory:
        folder = Path(directory)
        save(folder/'calls.json',[dict(prompt='only', requested_model=shared.MODEL, actual_model=shared.MODEL,
            response='void setup(){} void loop(){}', finish_reason='stop',seconds=0)])
        calls = DirectCalls(folder)
        assert calls('only')[1] == 'void setup(){} void loop(){}'
        try:
            calls('second')
            raise AssertionError('Second generation was allowed')
        except ValueError as exc:
            assert 'Single-response' in str(exc)
    print('self-check passed',flush=True)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--self-check',action='store_true')
    parser.add_argument('--report-only',action='store_true')
    args = parser.parse_args()
    if args.self_check:
        self_check()
        return
    os.chdir(ROOT)
    os.environ['PATH'] = str(runner.CLI.parent)+os.pathsep+os.environ['PATH']
    os.environ['WINDOWS_ARDUINO_CLI_PATH'] = str(runner.CLI)
    config = freeze()
    if args.report_only:
        print(json.dumps(summarize(config),ensure_ascii=True),flush=True)
        return
    retry_path = OUT/'service_retry_status.json'
    if retry_path.exists():
        retry = json.loads(retry_path.read_text(encoding='utf-8'))
        assert datetime.now(timezone.utc) >= datetime.fromisoformat(retry['next_retry_not_before_utc']), 'Service retry cooldown active'
    lock = OUT/'running.lock'
    descriptor = os.open(lock,os.O_CREAT|os.O_EXCL|os.O_WRONLY)
    os.close(descriptor)
    try:
        save(OUT/'run_status.json',dict(status='running',pid=os.getpid(),started_utc=shared.utc()))
        cases = iter(config['cases'])
        with ThreadPoolExecutor(max_workers=8) as pool:
            pending = {pool.submit(evaluate,c) for c in [next(cases) for _ in range(8)]}
            while pending:
                done,pending = wait(pending,return_when=FIRST_COMPLETED)
                for future in done:
                    future.result()
                    stats = summarize(config)
                    print(json.dumps({k:stats[k] for k in ('completed','compile_passes','infrastructure')},ensure_ascii=True),flush=True)
                    if not shared.STOP.is_set() and (case:=next(cases,None)) is not None:
                        pending.add(pool.submit(evaluate,case))
        stats = summarize(config)
        if shared.STOP.is_set():
            now = datetime.now(timezone.utc)
            save(retry_path,dict(last_failure_utc=now.isoformat(),next_retry_not_before_utc=(now+timedelta(hours=6)).isoformat(),
                error_scope='Infrastructure failure; inspect sanitized call error_type and status_code before retry'))
        save(OUT/'run_status.json',dict(status='complete' if stats['complete'] else 'incomplete',finished_utc=shared.utc()))
        print(json.dumps(stats,ensure_ascii=True),flush=True)
    finally:
        lock.unlink(missing_ok=True)


if __name__ == '__main__':
    main()
