"""Apply the frozen workflow267 host smoke fixture to all frozen comparison arms."""
import collections
import concurrent.futures
import copy
import hashlib
import json
from pathlib import Path
import shutil
import time

import run_workflow267_host_smoke as smoke

ROOT = smoke.ROOT
OUT = ROOT / 'experiment/output/all_groups_host_smoke_20261023'
PRIOR = ROOT / 'experiment/output/workflow267_host_smoke_20261023'
EXPECTED = {'historical_direct': 244, 'historical_direct_feedback': 291,
            'historical_workflow_initial': 199, 'workflow_267': 267, 'direct_gpt61': 291,
            'workflow_280': 280, 'ablation_direct': 250, 'ablation_direct_feedback': 292,
            'ablation_workflow': 257, 'ablation_no_contract_feedback': 214,
            'ablation_no_model_feedback': 200, 'ablation_blind_resampling': 241,
            'ablation_llm_backend': 208}
LABELS = {'historical_direct': '旧直接生成', 'workflow_267': '历史累计工作流267',
          'historical_direct_feedback': '旧直接生成＋编译反馈',
          'historical_workflow_initial': '旧工作流初始轮次（累计修复前）',
          'direct_gpt61': '后续GPT-6.1直接生成', 'workflow_280': '后续累计工作流280',
          'ablation_direct': '单轮直接生成', 'ablation_direct_feedback': '直接生成＋编译反馈',
          'ablation_workflow': '单轮完整工作流',
          'ablation_no_contract_feedback': '移除契约反馈',
          'ablation_no_model_feedback': '移除模型反馈',
          'ablation_blind_resampling': '盲重采样', 'ablation_llm_backend': 'LLM代码后端'}


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def identity(code_sha, board, cpp_sha):
    return hashlib.sha256(json.dumps([code_sha, board, cpp_sha]).encode()).hexdigest()


def category(result):
    if result['runtime_status'] == 'host_smoke_pass':
        return 'host_smoke_pass'
    if result.get('reason') != 'inspect_scenario_evidence':
        return result.get('reason', 'unverified')
    scenarios = result.get('scenarios', [])
    if scenarios and all(s.get('setup_returned') and s.get('loop_returns', 0) >= 2
                         and s.get('virtual_ms', 0) >= 10000
                         and s.get('status') in ('completed', 'trace-end') for s in scenarios):
        return 'loop_alive_but_output_not_confirmed_in_all_scenarios'
    return 'runtime_not_confirmed'


def summarize(rows, results, started, final=False):
    mapped = []
    groups = {}
    for group in EXPECTED:
        subset = [r for r in rows if r['group'] == group]
        counts = collections.Counter()
        for row in subset:
            item = copy.deepcopy(row)
            if not row['compile_ok']:
                item['runtime_category'] = 'historical_compile_failed_not_run'
            elif row['execution_key'] not in results:
                item['runtime_category'] = 'pending'
            else:
                result = results[row['execution_key']]
                item['runtime_category'] = category(result)
                item['evidence'] = result['evidence']
            counts[item['runtime_category']] += 1
            mapped.append(item)
        passed = counts['host_smoke_pass']
        compiled = sum(r['compile_ok'] for r in subset)
        groups[group] = dict(label=LABELS[group], total=len(subset), compiled=compiled,
                             completed=compiled-counts['pending'], host_smoke_pass=passed,
                             compile_rate=compiled/len(subset),
                             pass_rate_among_compiled=passed/compiled,
                             compile_and_smoke_rate=passed/len(subset), counts=dict(counts))
    summary = dict(final=final, elapsed_seconds=round(time.time()-started, 1),
                   unique_executions_completed=len(results),
                   groups=groups, scope='Frozen host C++ API-level smoke fixture; NOT SimulIDE/AVR/hardware or requirement correctness')
    (OUT/'summary.json').write_text(json.dumps(summary, ensure_ascii=False, indent=2), encoding='utf-8')
    (OUT/'mapped_results.json').write_text(json.dumps(mapped, ensure_ascii=False, indent=2), encoding='utf-8')
    (OUT/'execution_results.json').write_text(json.dumps(results, ensure_ascii=False, indent=2), encoding='utf-8')
    if final:
        report = ['# 全部对比与消融组：主机运行冒烟测试', '',
                  '每组296题，仅执行历史编译通过的代码。测试接口和判定条件与workflow267测试完全相同；重复执行仅在源代码SHA、开发板和Arduino预处理文件SHA三者一致时复用。', '',
                  '| 实验组 | 编译通过/296 | 三场景冒烟通过 | 编译通过样本中比例 | 全部296题中比例 | 未确认 |',
                  '|---|---:|---:|---:|---:|---:|']
        for group, v in groups.items():
            report.append(f"| {v['label']} ({group}) | {v['compiled']} ({v['compile_rate']:.1%}) | {v['host_smoke_pass']} | {v['pass_rate_among_compiled']:.1%} | {v['compile_and_smoke_rate']:.1%} | {v['compiled']-v['host_smoke_pass']} |")
        report += ['', '## 解释与范围', '',
                   '测试实际执行Windows主机进程，执行生成代码的setup与loop。三个场景分别为数字输入LOW/ADC0、数字输入HIGH/ADC1023、数字翻转/ADC阶梯；各观察10秒虚拟时间。三个场景均要求setup返回、loop至少返回两次、完成观察窗口并至少记录一次输出。输出可以来自setup，不要求与需求一致或响应输入。串口没有输入注入，所有传感器及执行器是API替身。', '',
                   '未确认分类见summary.json的counts：host_adapter_build_unsupported表示主机接口/库不支持，不是原Arduino编译失败；loop_alive_but_output_not_confirmed_in_all_scenarios表示循环运行但未在所有场景观察到输出；runtime_not_confirmed包括范围检查异常、观察窗口内setup未返回、超时或崩溃等，须进一步复核。不能把这些类别一概作为实板运行失败。', '',
                   '这些比例是主机接口模拟的冒烟结果，不是硬件可用率、SimulIDE通过率、功能正确率。主机整数宽度、资源和时序不等同AVR；不检查接线、真实外设通信、中断与长期行为。各实验组对Arduino API/第三方库的使用不同，当前接口覆盖不足会影响可比较性，需结合各组未支持数量解读，不能仅依此推断工作流质量。', '',
                   '历史累计workflow267/280包含后续修复结果，单轮消融完整工作流为另一组257/296；两种实验协议分别列出，不能把累计267与单轮消融直接当作受控消融比较。', '',
                   '## 证据', '',
                   'frozen_manifest.json保存分组及源路径/哈希；mapped_results.json保存每题分类及证据；execution_results.json保存去重执行日志；fixture_hashes.json保存固定接口哈希；cases/保留构建和进程输出。缓存复用记录引用上一轮的证据路径，不重新生成结论。原生成代码和原实验结果均未修改。', '',
                   '## 各组未确认分类', '']
        for group, v in groups.items():
            report.append(f"- {LABELS[group]}：" + json.dumps({k:n for k,n in v['counts'].items() if k not in ('host_smoke_pass', 'historical_compile_failed_not_run')}, ensure_ascii=False))
        (OUT/'REPORT_ZH.md').write_text('\n'.join(report)+'\n', encoding='utf-8')
    return summary


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    smoke.OUT = OUT
    expected_hashes = json.loads((PRIOR/'fixture_hashes.json').read_text())
    fixture = OUT/'fixture'
    fixture.mkdir(exist_ok=True)
    for name, digest in expected_hashes.items():
        assert sha(PRIOR/'fixture'/name) == digest
        shutil.copyfile(PRIOR/'fixture'/name, fixture/name)
        assert sha(fixture/name) == digest
    (OUT/'fixture_hashes.json').write_text(json.dumps(expected_hashes, indent=2))
    smoke.self_check(fixture)
    frozen_path = ROOT/'experiment/output/code_review_all_20261023/execution.json'
    frozen = json.loads(frozen_path.read_text())
    jobs = {j['key']:j for j in frozen['jobs']}
    dataset = {r['case']:r['dataset'] for r in json.loads((PRIOR/'frozen_manifest.json').read_text())}
    case_boards = {r['case']:jobs[r['review_key']]['board'] for r in frozen['rows'] if r.get('review_key')}
    historical_path = ROOT/'experiment/output/configured_audit_v1_20261015/results.json'
    for index, old in enumerate(json.loads(historical_path.read_text())):
        if old['arm'] not in ('direct_feedback', 'workflow'):
            continue
        key = 'historical_extra_'+str(index)
        group = 'historical_direct_feedback' if old['arm']=='direct_feedback' else 'historical_workflow_initial'
        row = dict(group=group, case=old['case'], compile_ok=old['compile_ok'])
        if old.get('artifact'):
            jobs[key] = dict(artifact=old['artifact'], code_sha256=old['artifact_sha256'], board=case_boards[old['case']])
            row['review_key'] = key
        frozen['rows'].append(row)
    rows, executions = [], {}
    for original in frozen['rows']:
        row = dict(original, dataset=dataset[original['case']])
        if 'review_key' not in row:
            assert not row['compile_ok']
            rows.append(row)
            continue
        job = jobs[row['review_key']]
        source = ROOT/job['artifact']
        row.update(source=str(source), source_sha256=job['code_sha256'], board=job['board'])
        if row['compile_ok']:
            assert sha(source) == job['code_sha256'], source
            cpp_sha = sha(source.parent.parent/'build/sketch/sketch.ino.cpp')
            key = identity(job['code_sha256'], job['board'], cpp_sha)
            row.update(execution_key=key, preprocessed_sha256=cpp_sha)
            executions.setdefault(key, dict(case=key, dataset=row['dataset'], source=str(source),
                                            sha256=job['code_sha256'], board=job['board']))
        rows.append(row)
    assert set(r['group'] for r in rows) == set(EXPECTED)
    for group, expected in EXPECTED.items():
        subset = [r for r in rows if r['group']==group]
        assert len(subset)==296 and sum(r['compile_ok'] for r in subset)==expected
    (OUT/'frozen_manifest.json').write_text(json.dumps(rows, ensure_ascii=False, indent=2), encoding='utf-8')
    (OUT/'provenance.json').write_text(json.dumps(dict(frozen_input=str(frozen_path), frozen_input_sha256=sha(frozen_path),
                                                     additional_historical_input=str(historical_path), additional_historical_sha256=sha(historical_path),
                                                     runner_sha256=sha(__file__), baseline_runner_sha256=sha(smoke.__file__)), indent=2))
    results = {}
    prior_boards = {r['case']:r['board'] for r in json.loads((PRIOR/'frozen_manifest.json').read_text())}
    for result in json.loads((PRIOR/'results.json').read_text()):
        key = identity(result['source_sha256'], prior_boards[result['case']], result['preprocessed_sha256'])
        if key in executions and sha(result['source'])==result['source_sha256']:
            results[key] = dict(result, evidence=str(PRIOR/'cases'/result['case']), cached=True)
    if (OUT/'execution_results.json').exists():
        for key, result in json.loads((OUT/'execution_results.json').read_text()).items():
            if key in executions:
                results.setdefault(key, result)
    started = time.time()
    print(json.dumps(dict(groups=len(EXPECTED), unique=len(executions), reused=len(results), remaining=len(executions)-len(results)), ensure_ascii=False), flush=True)
    summarize(rows, results, started)
    with concurrent.futures.ThreadPoolExecutor(max_workers=12) as pool:
        futures = {pool.submit(smoke.check, row, fixture):key for key,row in executions.items() if key not in results}
        for index, future in enumerate(concurrent.futures.as_completed(futures), 1):
            key = futures[future]
            try:
                result = future.result()
            except Exception as error:
                result = dict(runtime_status='unverified', reason='runner_error', error=repr(error))
            result.update(evidence=str(OUT/'cases'/key), cached=False)
            results[key] = result
            (OUT/'cases'/key).mkdir(parents=True, exist_ok=True)
            (OUT/'cases'/key/'result.json').write_text(json.dumps(result, indent=2), encoding='utf-8')
            if index%25==0:
                summarize(rows, results, started)
                print(json.dumps(dict(completed=len(results), total=len(executions), elapsed=round(time.time()-started), statuses=dict(collections.Counter(category(r) for r in results.values())))), flush=True)
    assert len(results)==len(executions)
    summary = summarize(rows, results, started, final=True)
    assert summary['groups']['workflow_267']['host_smoke_pass']==260
    print(json.dumps(summary, ensure_ascii=False), flush=True)


if __name__=='__main__':
    main()
