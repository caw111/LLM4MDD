"""Task-clustered statistics and publication artifacts for the frozen experiment."""
import argparse,json,random,statistics,hashlib
from collections import Counter,defaultdict
from pathlib import Path
from experiment_metrics import summarize as stage_summary, functional_rates

ROOT=Path(__file__).resolve().parents[2]
ARMS=('direct','direct_feedback','workflow','no_sysml_feedback','direct_ir','llm_kernel')
LABELS={'direct':'Direct LLM','direct_feedback':'Direct + feedback','workflow':'Full workflow','no_sysml_feedback':'No model feedback','direct_ir':'Direct IR','llm_kernel':'LLM kernel'}

def percentile(values,p):
    ordered=sorted(values);at=(len(ordered)-1)*p;low=int(at);high=min(low+1,len(ordered)-1)
    return ordered[low]+(ordered[high]-ordered[low])*(at-low)

def interval(values,seed=20260928):
    if not values:return None
    rng=random.Random(seed);n=len(values)
    draws=[sum(rng.choices(values,k=n))/n for _ in range(4000)]
    return [percentile(draws,.025),percentile(draws,.975)]

def task_means(rows,metric):
    grouped=defaultdict(list)
    for r in rows:grouped[r['case']].append(float(r['compile_ok'] if metric=='compile' else r['functional_status']=='passed'))
    return {k:statistics.mean(v) for k,v in grouped.items()}

def summarize(rows,metric):
    means=task_means(rows,metric)
    return {'tasks':len(means),'runs':len(rows),'passes':sum(r['compile_ok'] if metric=='compile' else r['functional_status']=='passed' for r in rows),
            'rate':statistics.mean(means.values()) if means else None,'ci95':interval(list(means.values()))}

def main():
    parser=argparse.ArgumentParser();parser.add_argument('--out',default='experiment/output/formal_20260928');parser.add_argument('--partial',action='store_true');args=parser.parse_args()
    root=ROOT/args.out
    execution=json.loads((root/'execution.json').read_text(encoding='utf-8'))
    rows=[r for p in root.glob('*/repeat_*/results.json') for r in json.loads(p.read_text(encoding='utf-8'))]
    manifest=json.loads((ROOT/'experiment/formal/manifest.json').read_text(encoding='utf-8'))
    cases=[c for c in manifest['cases'] if c['split']=='test'];ids={c['id'] for c in cases}
    eligible={r['case'] for r in json.loads((ROOT/'experiment/formal/oracle_audit/classified_summary.json').read_text(encoding='utf-8'))['cases'] if r['category']=='passed'}&ids
    # Reconstruct first-generation prefix costs including transport retries, rather than assuming one physical call.
    for r in rows:
        folder=root/r['case']/f"repeat_{r['repeat']}"
        call_path=folder/('direct_calls' if r['arm']=='direct' else 'model_calls')/'calls.json'
        if r['arm']=='direct' or (r['arm']=='no_sysml_feedback' and not r.get('model_ok',False)):
            if call_path.exists():
                entries=json.loads(call_path.read_text(encoding='utf-8'))
                first=next((i+1 for i,e in enumerate(entries) if 'response' in e),len(entries));prefix=entries[:first]
                planning_path=folder/'contract_calls/calls.json'
                if r['arm']=='no_sysml_feedback' and planning_path.exists():
                    prefix=json.loads(planning_path.read_text(encoding='utf-8'))+prefix
                r.update(llm_calls=len(prefix),tokens=sum((e.get('usage') or {}).get('total_tokens',0) for e in prefix),reported_cost_usd=sum((e.get('usage') or {}).get('cost',0) or 0 for e in prefix),llm_seconds=sum(e.get('seconds',0) for e in prefix))
    keys=[(r['case'],r['repeat'],r['arm']) for r in rows]
    expected={(case,r,arm) for case in ids for r in range(1,6) for arm in ARMS}
    assert len(keys)==len(set(keys)), 'Duplicate records'
    assert set(keys)<=expected,'Unexpected formal case/repeat/arm'
    infrastructure=[r for r in rows if r.get('infrastructure_error') or r.get('functional_status','').startswith(('infrastructure_','blocked_'))]
    pending=[r for r in rows if r['case'] in eligible and r.get('functional_status')=='pending']
    complete=set(keys)==expected and not infrastructure and not pending
    if not args.partial and not complete:raise SystemExit(f'Not complete: {len(rows)}/7080 records; {len(infrastructure)} infrastructure; {len(pending)} pending functional results')
    if complete:
        for r in rows:
            if r.get('artifact'):
                artifact=ROOT/r['artifact']
                assert artifact.is_file(),f"Missing artifact: {artifact}"
                assert hashlib.sha256(artifact.read_text(encoding='utf-8').encode()).hexdigest()==r['artifact_sha256'],f"Artifact changed after scoring: {artifact}"
            if r['case'] in eligible and r['compile_ok']:
                assert r['functional_status'] in ('passed','simulation_failed'),f"Unresolved functional status: {r['case']} {r['repeat']} {r['arm']} {r['functional_status']}"
                assert (ROOT/r['functional_log']).is_file(),f"Missing simulation log: {r['case']}"
    stats={'complete':complete,'records':len(rows),'expected_records':len(expected),'functional_tasks':len(eligible),'arms':{},'paired_differences':{},'by_dataset':{},'failures':{}}
    for arm in ARMS:
        group=[r for r in rows if r['arm']==arm];functional=[r for r in group if r['case'] in eligible]
        per_repeat=[statistics.mean(float(r['compile_ok']) for r in group if r['repeat']==i) for i in range(1,6) if any(r['repeat']==i for r in group)]
        stats['arms'][arm]={'compile':summarize(group,'compile'),'functional':summarize(functional,'functional'),
            'compile_repeat_rates':per_repeat,'compile_repeat_sd':statistics.stdev(per_repeat) if len(per_repeat)>1 else None,
            'mean_tokens':statistics.mean(r['tokens'] for r in group) if group else 0,'mean_calls':statistics.mean(r['llm_calls'] for r in group) if group else 0,
            'mean_llm_seconds':statistics.mean(r['llm_seconds'] for r in group) if group else 0,'allocated_cost_usd':sum(r['reported_cost_usd'] for r in group)}
        stats['failures'][arm]=dict(Counter(r.get('failure_stage',r['compile_status']) for r in group if not r['compile_ok']))
    for metric in ('compile','functional'):
        subset=rows if metric=='compile' else [r for r in rows if r['case'] in eligible]
        full=task_means([r for r in subset if r['arm']=='workflow'],metric)
        stats['paired_differences'][metric]={}
        for arm in ARMS:
            if arm=='workflow':continue
            other=task_means([r for r in subset if r['arm']==arm],metric);shared=sorted(full.keys()&other.keys())
            diffs=[full[k]-other[k] for k in shared]
            stats['paired_differences'][metric][arm]={'tasks':len(diffs),'difference':statistics.mean(diffs) if diffs else None,'ci95':interval(diffs)}
    for dataset in ('internal','PCEval','EmbedBench'):
        stats['by_dataset'][dataset]={arm:summarize([r for r in rows if r['arm']==arm and r['dataset']==dataset],'compile') for arm in ARMS}
    # Descriptive, selected subset only: frozen kernel ablation needs a shared adapter.
    ready={(r['case'],r['repeat']) for r in rows if r['arm']=='llm_kernel' and r.get('artifact')}
    conditional=[r for r in rows if r['arm'] in ('workflow','llm_kernel') and (r['case'],r['repeat']) in ready]
    stats['conditional_kernel_ablation']={'selection':'Shared model/header/initial adapter available and LLM kernel artifact produced; descriptive selected subset, not overall causal effect.',
        'task_repeat_pairs':len(ready),'compile':{arm:summarize([r for r in conditional if r['arm']==arm],'compile') for arm in ('workflow','llm_kernel')},
        'functional':{arm:summarize([r for r in conditional if r['arm']==arm and r['case'] in eligible],'functional') for arm in ('workflow','llm_kernel')}}
    entries=[e for p in root.glob('*/repeat_*/*_calls/calls.json') for e in json.loads(p.read_text(encoding='utf-8'))]
    stats['usage_caveat']='Returned-response totals exclude any in-flight requests without a recorded response during concurrency restart; provider invoices may be higher.' if (root/'concurrency_restart.json').exists() else ''
    stats['physical_usage']={'finish_reasons':dict(Counter(e.get('finish_reason','unavailable') for e in entries)),'calls':len(entries),'tokens':sum((e.get('usage') or {}).get('total_tokens',0) for e in entries),
        'reported_cost_usd':sum((e.get('usage') or {}).get('cost',0) or 0 for e in entries),'llm_seconds':sum(e.get('seconds',0) for e in entries),
        'actual_models':dict(Counter(e.get('actual_model','unavailable') for e in entries))}
    reports=[json.loads(p.read_text(encoding='utf-8')) for p in root.glob('*/repeat_*/model_validation.json')]
    stats['stage_and_ablation_metrics'] = stage_summary(rows, reports, expected_keys=expected)
    if 'call_limits' in execution:
        stats['stage_and_ablation_metrics']['max_method_calls_excluding_transport_retries']=execution['call_limits']
    planned_rows = rows + [dict(case=case, repeat=repeat, arm=arm, compile_ok=False,
                               functional_status='not_run', record_missing=True)
                           for case, repeat, arm in sorted(expected-set(keys))]
    for arm in ARMS:
        stats['arms'][arm]['functional_denominators'] = functional_rates([r for r in planned_rows if r['arm'] == arm])
        stats['arms'][arm]['functional_validated_oracle_denominators'] = functional_rates([r for r in planned_rows if r['arm'] == arm and r['case'] in eligible])
    stats['model_validation']={'tasks_runs_with_report':len(reports),'first_pass':sum(bool(p['attempts'][0]['ok']) for p in reports),'eventual_pass':sum(bool(p['attempts'][-1]['ok']) for p in reports),'attempts':sum(len(p['attempts']) for p in reports),
        'final_failure_codes':dict(Counter(issue.get('code','unknown') for p in reports if not p['attempts'][-1]['ok'] for issue in p['attempts'][-1].get('issues',[]) if issue.get('severity')=='error'))}
    (root/'statistics.json').write_text(json.dumps(stats,ensure_ascii=False,indent=2),encoding='utf-8')
    def rate(s):return '—' if s['rate'] is None else f"{100*s['rate']:.1f}% [{100*s['ci95'][0]:.1f}, {100*s['ci95'][1]:.1f}]"
    lines=['# 正式对比与消融实验'+('（已完成）' if complete else '（未完成快照，不可作为最终结果）'),' ',
        f"记录：{len(rows)}/7080；冻结独立测试任务 236；每题 5 次。功能测试有效覆盖 {len(eligible)}/40 个 PCEval 测试任务，整体 {len(eligible)}/236。",'',
        '## 主结果','', '编译指最终 Arduino 目标板编译。功能指标仅在预先通过参考测试的任务上计算，生成/编译失败计为该子集的端到端失败。方括号为按题目聚类的配对设计下 bootstrap 95% 区间（4000 次重采样）；五次重复不作为五个独立任务。','',
        '| 方法 | 编译通过/运行数 | 编译率 [95% CI] | 功能通过/有效运行数 | 功能率 [95% CI] | 平均 tokens | 平均调用数 |','|---|---:|---|---:|---|---:|---:|']
    for arm in ARMS:
        s=stats['arms'][arm];c=s['compile'];f=s['functional']
        lines.append(f"| {LABELS[arm]} | {c['passes']}/{c['runs']} | {rate(c)} | {f['passes']}/{f['runs']} | {rate(f)} | {s['mean_tokens']:.0f} | {s['mean_calls']:.2f} |")
    lines+=['','## 配对差值','', '差值为完整流程减去对照，单位为百分点；负值说明完整流程更差，不预设方法优势。区间跨零时不能据此声称方向明确。','', '| 指标 | 对照 | 差值 | 95% 区间 |','|---|---|---:|---|']
    for metric,comparisons in stats['paired_differences'].items():
        for arm,s in comparisons.items():
            if s['difference'] is not None:lines.append(f"| {metric} | {LABELS[arm]} | {100*s['difference']:+.2f} | [{100*s['ci95'][0]:.2f}, {100*s['ci95'][1]:.2f}] |")
    conditional=stats['conditional_kernel_ablation']
    lines += ['', '## 内核消融的条件子集', '', f"共享模型、接口和初始适配已就绪且产生内核工件的配对运行数：{conditional['task_repeat_pairs']}。这是筛选后的描述性分析，不能替代全任务结果，也不能证明确定性转换器的覆盖优势。", '', '| 方法 | 条件子集编译通过/运行数 | 条件子集功能通过/运行数 |', '|---|---:|---:|']
    for arm in ('workflow','llm_kernel'):
        c=conditional['compile'][arm];f=conditional['functional'][arm]
        lines.append(f"| {LABELS[arm]} | {c['passes']}/{c['runs']} | {f['passes']}/{f['runs']} |")
    mv=stats['model_validation']
    lines += ['', '## 模型检查与反馈', '', f"具有验证报告的任务运行：{mv['tasks_runs_with_report']}；首稿通过 {mv['first_pass']}；修复预算内最终通过 {mv['eventual_pass']}。此处通过要求同时满足 SysML 参考校验及当前 SysML→IR 的可执行子集检查，不等同于仅语法合法；这些是前置模型指标，不是最终固件功能通过数。最终失败代码分布见 statistics.json。"]
    lines += ['', '## 功能测试的两种分母', '',
              '下表覆盖所有任务。存在未测或无独立oracle的样例时，观测成功比例只是下界，未测不能解释为功能失败。有效oracle子集及两种分母另见 statistics.json。', '',
              '| 方法 | 已观测成功/全部运行 | 已观测成功/编译成功运行 | 未决 |', '|---|---:|---:|---:|']
    for arm in ARMS:
        d = stats['arms'][arm]['functional_denominators']; all_runs = d['all_tasks']; compiled = d['compiled_only']
        lines.append(f"| {LABELS[arm]} | {all_runs['passes']}/{all_runs['denominator']} | {compiled['passes']}/{compiled['denominator']} | {d['unresolved']} |")
    lines += ['', '## 预设消融问题与调用预算', '',
              '配对单位为相同任务及repeat；区间按任务聚类。预算不等，不能将额外计算量收益直接解释为方法收益。', '',
              '| 问题 | 左组 − 右组 | 编译差值 | 95%区间 |', '|---|---|---:|---|']
    for contrast in stats['stage_and_ablation_metrics']['contrasts']:
        c = contrast['compile']
        lines.append(f"| {contrast['question']} | {contrast['left']} − {contrast['right']} | {c['difference']} | {c['ci95']} |")
    lines += ['', '不含传输重试的最大方法调用数：' + json.dumps(stats['stage_and_ablation_metrics']['max_method_calls_excluding_transport_retries'])]
    lines += ['', '## 失败发生在哪一阶段', '', '| 方法 | 最终失败阶段或编译状态 | 运行数 |', '|---|---|---:|']
    for arm in ARMS:
        for stage,count in sorted(stats['failures'][arm].items(),key=lambda item:-item[1]):
            lines.append(f"| {LABELS[arm]} | {stage} | {count} |")
    lines+=['','## 数据来源分层','', '| 来源 | 方法 | 编译通过/运行数 | 编译率 |','|---|---|---:|---|']
    for dataset,groups in stats['by_dataset'].items():
        for arm,s in groups.items():lines.append(f"| {dataset} | {LABELS[arm]} | {s['passes']}/{s['runs']} | {rate(s)} |")
    usage=stats['physical_usage']
    lines+=['','## 成本与可复现性','',f"已记录调用 {usage['calls']} 次，{usage['tokens']} tokens，提供商报告消费 ${usage['reported_cost_usd']:.6f}。这是去除组间共享重复计费后的已记录调用统计；并发重启时未返回的请求可能额外收费，因此消费金额不是账单精确总额。各组独立成本见 statistics.json，不应相加。",
        '模型、提示词、方法文件散列见 execution.json / method_snapshot；运行中基础设施修复的补充记录见 runtime_recovery_20260928.json 和 harness_amendment_20260930.json。成本由调用日志重建，包含传输重试；耗时为累计 LLM API 调用时间，不代表含排队、校验和编译的完整端到端时延。逐次请求、响应、诊断和编译产物见每题 repeat_* 子目录。修复提示仅包含原需求、上一个候选和诊断，不自动附带跨任务聊天历史。隐藏功能场景不用于生成或修复。','',
        '## 局限与结论边界','',
        '- 236 题的编译实验不能替代全部题目的功能验证；仅 19 个独立任务具备本轮审计通过的功能 oracle。其余基准测试不兼容或尚未转换，明确标记为未验证。',
        '- SysML/IR 编译器覆盖标量数据流与有限状态更新；数组、一般并发和专用设备语义等超出当前范围。端到端统计仍保留这些失败。',
        '- 完整流程与直接生成的中间诊断预算不同，本实验不是等费用比较；应结合调用消耗解释结果。',
        '- LLM 内核消融复用有效模型、后端接口和初始硬件适配；共享准备失败计入端到端失败，包括 backend_dependency。该设计不能独立检验后端覆盖率，条件子集结果不能冒充全任务成功率。',
        '- 原测试采用精确串口文本和屏幕截图等断言，结果反映对该测试契约的符合程度；失败原因需结合日志解释，不能一概归因于控制逻辑错误。',
        '- 公开数据集可能进入预训练数据；本实验不能排除记忆效应，模型返回的具体版本和调用消耗保存在原始日志中。',
        '- 仿真不能验证真实设备时序漂移、噪声、供电与校准；结果不构成实际硬件正确性的证明。',
        '- 任务之间独立生成，本实验不验证在已有系统上逐次增加新需求时的长期模型一致性。']
    (root/'REPORT_ZH.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
    if complete:
        import matplotlib
        matplotlib.use('Agg')
        import matplotlib.pyplot as plt
        fig,axes=plt.subplots(1,2,figsize=(12,4.8),constrained_layout=True)
        for ax,metric,title in zip(axes,('compile','functional'),('Arduino compilation (236 tasks)','Behavioral tests (19 validated tasks)')):
            values=[stats['arms'][a][metric] for a in ARMS];rates=[100*s['rate'] for s in values]
            errors=[[rates[i]-100*s['ci95'][0] for i,s in enumerate(values)],[100*s['ci95'][1]-rates[i] for i,s in enumerate(values)]]
            ax.bar(range(6),rates,yerr=errors,capsize=3,color=['#8b9db1','#6284a5','#247d66','#9d8aaf','#c09a59','#ad7777'])
            ax.set_xticks(range(6),[LABELS[a] for a in ARMS],rotation=35,ha='right');ax.set_ylim(0,105);ax.set_ylabel('Pass rate (%)');ax.set_title(title);ax.grid(axis='y',alpha=.2)
        fig.savefig(root/'formal_results.pdf');fig.savefig(root/'formal_results.png',dpi=220);plt.close(fig)
        tex=['% Generated from completed real experiment; compilation and functional correctness are distinct.',r'\begin{tabular}{lrr}',r'\hline',r'Method & Compile (\%) & Functional (\%) \\',r'\hline']
        for arm in ARMS:
            s=stats['arms'][arm];tex.append(f"{LABELS[arm]} & {s['compile']['rate']*100:.1f} & {s['functional']['rate']*100:.1f} "+r'\\')
        tex += [r'\hline',r'\end{tabular}'];(root/'results_table.tex').write_text('\n'.join(tex)+'\n',encoding='utf-8')
    print(json.dumps({'complete':complete,'records':len(rows),'usage':usage},ensure_ascii=False))

if __name__=='__main__':
    assert interval([1,1,1])==[1,1]
    assert percentile([0,10],.5)==5
    main()
