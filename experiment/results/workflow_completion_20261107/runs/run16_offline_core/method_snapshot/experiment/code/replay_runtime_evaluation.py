"""Execute unchanged comparison/ablation artifacts; no generation or score overwrite."""
import argparse
import hashlib
import json
import os
import shutil
from collections import Counter
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

import run_contract_development as host
from run_current_comparison import save

ROOT = host.ROOT
OUT = ROOT/'experiment/output/runtime_validation_20261023'


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def collect():
    baseline = ROOT/'experiment/output/configured_audit_v1_20261015'
    cases = read(baseline/'execution.json')['cases']
    rows = []
    sources = [ROOT/'experiment/output/paper_ablation_v2_20261017/single_round/results.json',
        baseline/'results.json',
        ROOT/'experiment/output/repaired_workflow_continuation_20261016/cumulative_workflow_results.json',
        ROOT/'experiment/output/workflow_gpt61_failures_20261020/cumulative_workflow_results.json',
        ROOT/'experiment/output/direct_gpt61_full_20261021/results.json']
    for index,path in enumerate(sources):
        for original in read(path):
            if index == 1 and original['arm'] != 'direct':
                continue
            arm = ('ablation_'+original['arm'] if index == 0 else
                   ('historical_direct','workflow_267','workflow_280','direct_gpt61')[index-1])
            rows.append(dict(group=arm, original=original, original_results=str(path.relative_to(ROOT))))
    assert len(rows) == len({(r['group'],r['original']['case']) for r in rows}) == 3256
    return cases,rows,sources


def job(item):
    key,source,board,task = item
    folder = OUT/'host_cache'/key
    path = folder/'result.json'
    if path.exists():
        return read(path)
    dest = folder/'build/sketch/sketch.ino.cpp'
    dest.parent.mkdir(parents=True,exist_ok=True)
    shutil.copyfile(source,dest)
    assert sha(source) == sha(dest)
    try:
        result = host.behavior(folder,task,board)
    except Exception as exc:
        result = dict(functional_status='oracle_infrastructure_unverified',error_type=type(exc).__name__)
    result.update(preprocessed_sha256=sha(source),cache_key=key)
    save(path,result)
    return result


def report(plan):
    rows = []
    for item in plan['rows']:
        original = item['original']
        row = dict(group=item['group'],case=original['case'],compile_ok=original['compile_ok'],
                   original_functional_status=original.get('functional_status'))
        key = item.get('cache_key')
        if not original['compile_ok']:
            row.update(runtime_status='not_buildable',test_scope='no_executable_artifact')
        elif key and (OUT/'host_cache'/key/'result.json').exists():
            result = read(OUT/'host_cache'/key/'result.json')
            row.update(runtime_status=result['functional_status'],test_scope='independent_host_behavior',cache_key=key)
        elif key:
            row.update(runtime_status='pending_host_test',test_scope='independent_host_behavior')
        elif original['dataset'] == 'PCEval':
            row.update(runtime_status='blocked_wokwi_quota',test_scope='original_dataset_scenario_requires_reference_audit')
        else:
            row.update(runtime_status='no_executable_oracle',test_scope='requires_test_implementation_and_reference_validation')
        rows.append(row)
    groups = {}
    for group in sorted({r['group'] for r in rows}):
        subset = [r for r in rows if r['group']==group]
        eligible = [r for r in subset if r['case'] in host.ORACLE_TASKS]
        groups[group] = dict(total=296,compile_passes=sum(r['compile_ok'] for r in subset),
            runtime_status=dict(Counter(r['runtime_status'] for r in subset)),
            host_subset_total=len(eligible),host_subset_status=dict(Counter(r['runtime_status'] for r in eligible)))
    source_changes = [p for p,h in plan['hashes'].items() if sha(ROOT/p) != h]
    complete = not any(r['runtime_status']=='pending_host_test' for r in rows) and not source_changes
    summary = dict(host_replay_complete=complete,full_runtime_evaluation_complete=False,
        generated_rows=len(rows),unique_host_jobs=len({r['cache_key'] for r in plan['rows'] if 'cache_key' in r}),
        host_case_coverage=16,pceval_pending_reference_audit=50,remaining_without_executable_oracle=230,
        source_or_artifact_changes=source_changes,new_llm_calls=0,groups=groups,
        blocker='Wokwi Free plan monthly CI minute quota exhausted; one reference preflight recorded; no repeated requests')
    save(OUT/'results.json',rows)
    save(OUT/'summary.json',summary)
    lines = ['# 对比及消融实验：执行与行为验证', '',
        f'本地主机行为测试补跑完成：{complete}；完整296题运行正确性评估完成：False。',
        '不重新生成代码，不修改旧结果。相同任务的相同代码共享一次测试执行结果；每组均保留296题。',
        '功能通过必须实际执行独立测试并满足断言，不能用编译通过、历史模型校验或无崩溃代替。主机测试不是实物HIL。', '',
        '## 固定16题主机行为测试', '',
        '| 组别 | 编译通过/296 | 功能通过/16 | 功能失败 | 未交付可编译代码 | 测试环境未确认 |',
        '|---|---:|---:|---:|---:|---:|']
    for group,value in groups.items():
        counts = value['host_subset_status']
        unknown = sum(n for k,n in counts.items() if k not in ('passed','failed','not_buildable'))
        lines.append(f'| {group} | {value["compile_passes"]} | {counts.get("passed",0)} | {counts.get("failed",0)} | {counts.get("not_buildable",0)} | {unknown} |')
    lines += ['', '## 尚未完成的覆盖', '',
        'PCEval原始参考程序预检被Wokwi免费月CI额度阻断。恢复额度后须先完成参考程序/原始测试审计，再对各组同一题执行同一测试；不把测试本身失败当生成程序失败。',
        'EmbedBench test字段主要是描述性步骤，不是可直接执行的Wokwi场景；未转换和验收的测试仍标为未知，不能给出全量功能正确率。',
        '其余需求缺少独立可执行oracle，需要另行实现、验证并冻结；本次没有用LLM主观判分补齐。',
        'workflow_267和workflow_280为历史累计产物，各消融为独立冻结单轮；不能冒称同版本同预算消融。',
        '完整原始记录、源码及编译工件散列检查：'+json.dumps(source_changes,ensure_ascii=False)]
    (OUT/'REPORT_ZH.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
    return summary


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--report-only',action='store_true')
    args = parser.parse_args()
    OUT.mkdir(parents=True,exist_ok=True)
    os.chdir(ROOT)
    cases,rows,sources = collect()
    case_map = {c['id']:c for c in cases}
    hashes = {str(p.relative_to(ROOT)):sha(p) for p in [Path(__file__),ROOT/'experiment/code/run_contract_development.py',*sources]}
    fixture = OUT/'fixture_root/experiment/development/oracles'
    fixture.mkdir(parents=True,exist_ok=True)
    for source in (ROOT/'experiment/development/oracles').glob('*'):
        if source.is_file():
            hashes[str(source.relative_to(ROOT))] = sha(source)
            shutil.copyfile(source,fixture/source.name)
    jobs = {}
    for item in rows:
        row = item['original']
        if row.get('artifact'):
            artifact = ROOT/row['artifact']
            assert sha(artifact) == row['artifact_sha256'], 'Original artifact changed: '+row['case']
            hashes[str(artifact.relative_to(ROOT))] = sha(artifact)
        if row['compile_ok'] and row['case'] in host.ORACLE_TASKS:
            cpp = artifact.parent.parent/'build/sketch/sketch.ino.cpp'
            hashes[str(cpp.relative_to(ROOT))] = sha(cpp)
            board = case_map[row['case']]['fqbn']
            key = hashlib.sha256((row['case']+'\0'+board+'\0'+sha(cpp)).encode()).hexdigest()
            item['cache_key'] = key
            jobs[key] = (key,cpp,board,host.ORACLE_TASKS[row['case']])
    plan = dict(rows=rows,hashes=hashes,scope='Unchanged artifacts; fixed original host oracles; no new model requests')
    if (OUT/'execution.json').exists():
        assert read(OUT/'execution.json') == plan, 'Runtime replay snapshot changed'
    else:
        save(OUT/'execution.json',plan)
    host.ROOT = OUT/'fixture_root'
    if not args.report_only:
        with ThreadPoolExecutor(max_workers=4) as pool:
            for index,future in enumerate(as_completed([pool.submit(job,value) for value in jobs.values()]),1):
                future.result()
                if index%10 == 0:
                    print(json.dumps(dict(completed_host_jobs=index,total=len(jobs))),flush=True)
    summary = report(plan)
    assert summary['generated_rows']==3256 and len(summary['groups'])==11
    print(json.dumps(summary,ensure_ascii=True),flush=True)


if __name__=='__main__':
    main()
