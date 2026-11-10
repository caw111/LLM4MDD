"""Render the completed current-system comparison with explicit oracle coverage."""
import argparse
import hashlib
import json
import statistics
from collections import Counter, defaultdict
from pathlib import Path

from run_controlled_experiment import ROOT
from run_current_comparison import ARMS, ORACLE_TASKS
from experiment_metrics import ci95, functional_rates, paired

LABELS = {'direct':'LLM 直接生成', 'direct_feedback':'LLM 直接生成＋编译反馈', 'workflow':'当前完整流程'}


def fraction(passes, n):
    return f'{passes}/{n}（{100*passes/n:.1f}%）' if n else '无样本'


def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('--run',required=True)
    args=parser.parse_args()
    root=(ROOT/args.run).resolve()
    config=json.loads((root/'execution.json').read_text(encoding='utf-8'))
    stats=json.loads((root/'summary.json').read_text(encoding='utf-8'))
    rows=json.loads((root/'results.json').read_text(encoding='utf-8'))
    planned={(c['id'],r,a) for c in config['cases'] for r in range(1,config['repetitions']+1) for a in ARMS}
    keys=[(r['case'],r['repeat'],r['arm']) for r in rows]
    if len(keys)!=len(set(keys)) or set(keys)!=planned or not stats['generation_complete']:
        raise SystemExit('Run is incomplete or contains duplicate records; final report cannot be issued')
    changed=[p for p,h in config['hashes'].items() if hashlib.sha256((ROOT/p).read_bytes()).hexdigest()!=h]
    if changed:raise SystemExit('Frozen method files changed; results cannot be pooled: '+', '.join(changed))
    tools_changed=[p for p,h in config['runtime']['binaries_sha256'].items()
                   if h is not None and hashlib.sha256((ROOT/p).read_bytes()).hexdigest()!=h]
    if tools_changed:raise SystemExit('Frozen tool binaries changed: '+', '.join(tools_changed))
    artifacts={r['artifact']:r['artifact_sha256'] for r in rows if r.get('artifact')}
    altered=[p for p,h in artifacts.items() if hashlib.sha256((ROOT/p).read_bytes()).hexdigest()!=h]
    if altered:raise SystemExit('Compiled artifacts changed: '+', '.join(altered[:5]))
    source_tasks={c['id']:c for c in config['cases']}
    eligible=set(source_tasks)&set(ORACLE_TASKS)
    host_rows=[r for r in rows if r['case'] in eligible]
    host={a:functional_rates([r for r in host_rows if r['arm']==a]) for a in ARMS}
    intervals={}
    for a in ARMS:
        by_task=defaultdict(list)
        for r in rows:
            if r['arm']==a:by_task[r['case']].append(float(r['compile_ok']))
        intervals[a]=ci95([statistics.mean(v) for v in by_task.values()])
    analysis={'records':len(rows),'tasks':len(source_tasks),'repetitions':config['repetitions'],
        'functional_oracle_tasks':sorted(eligible),'functional_oracle_coverage':len(eligible)/len(source_tasks),
        'independent_host_functional':host,'compilation_cluster_ci95':intervals,
        'paired_host_functional':[dict(left='workflow',right=a,**paired(host_rows,'workflow',a,'functional')) for a in ('direct','direct_feedback')],
        'compiled_artifacts_checked':len(artifacts),'artifact_hashes_unchanged':True,'method_hashes_unchanged':True,
        'scoring_source_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        'results_sha256':hashlib.sha256((root/'results.json').read_bytes()).hexdigest()}
    failures=[]
    for r in rows:
        if r['arm']!='workflow' or r['compile_ok']:continue
        folder=root/r['case']/f"repeat_{r['repeat']}"
        planning=folder/'contract_planning.json'
        detail=json.loads(planning.read_text(encoding='utf-8')) if planning.exists() else {}
        errors=[a.get('error') for a in detail.get('attempts',[]) if a.get('error')]
        failures.append({'case':r['case'],'repeat':r['repeat'],'dataset':r['dataset'],'split':r['split'],
            'failure_stage':r.get('failure_stage',r['compile_status']),'error':r.get('error'),
            'planning_errors':errors,'public_requirement':source_tasks[r['case']]['requirement']})
    (root/'final_analysis.json').write_text(json.dumps(analysis,ensure_ascii=False,indent=2),encoding='utf-8')
    (root/'workflow_failure_details.json').write_text(json.dumps(failures,ensure_ascii=False,indent=2),encoding='utf-8')

    lines=['# 当前系统全量实验报告','',f"模型 `{config['model']}`，{len(source_tasks)}题，每题{config['repetitions']}次，三组共{len(rows)}条记录。",
        '开发集与原测试分区保留原划分；原始需求、板型、电路和公开解释在三组一致。方法、提示和工具身份已冻结，最终候选 sketch 源码散列全部核对。',
        '测试核心需求到固件生产链，包括接口规划、SysML装配与修复、IR/IL/C、产物来源链、确定性Arduino适配和目标板编译；不含可选前端模块蓝图与实板烧录。',
        '', '## 全部任务目标板编译', '', '| 方法 | 通过/计划任务重复 | 按题目聚类95%区间 | 各轮通过率 |','|---|---:|---:|---|']
    if config.get('parent_run'):
        lines.insert(4, '本报告是原多轮实验第一轮的快照，复用同一批原始记录，没有新增模型调用，不能当作另一批独立抽样。')
    for a in ARMS:
        s=stats['arms'][a];c=s['compile'];interval=intervals[a]
        lines.append(f"| {LABELS[a]} | {fraction(c['passes'],c['denominator'])} | {interval[0]*100:.1f}%～{interval[1]*100:.1f}% | "+'、'.join(f'{x*100:.1f}%' for x in s['compile_repeat_rates'])+' |')
    for split,label in [('test','原测试分区'),('development','开发集')]:
        lines += ['',f'## {label}', '', '| 方法 | 编译通过/全部任务重复 |','|---|---:|']
        for a in ARMS:
            c=stats['partitions'][split]['arms'][a]['compile']
            lines.append(f"| {LABELS[a]} | {fraction(c['passes'],c['denominator'])} |")
    lines += ['', '## 按数据来源', '', '| 来源 | 直接生成 | 直接＋反馈 | 完整流程 |','|---|---:|---:|---:|']
    for d,s in stats['by_dataset'].items():
        lines.append('| '+d+' | '+' | '.join(fraction(s['arms'][a]['compile']['passes'],s['arms'][a]['compile']['denominator']) for a in ARMS)+' |')
    lines += ['', '## 独立功能验证范围', '',f"现有独立host轨迹覆盖{len(eligible)}/{len(source_tasks)}题（{len(eligible)/len(source_tasks)*100:.1f}%），均为原开发集任务。该子集不能代表独立测试集或全部需求功能通过率。",
        'PCEval全部50个参考程序在当前工具链编译通过，但Wokwi免费CI分钟额度已用完，本轮没有新PCEval仿真成功证据。其余内部任务和EmbedBench缺少已核验独立oracle。',
        '', '| 方法 | 功能通过/固定oracle子集全部任务重复 | 已编译但功能未验证 |','|---|---:|---:|']
    for a in ARMS:
        c=host[a]['all_tasks'];lines.append(f"| {LABELS[a]} | {fraction(c['passes'],c['denominator'])} | {host[a]['unresolved']} |")
    lines += ['', '无oracle、host不能模拟的API以及服务额度缺测均未伪装成已证明功能成功或功能失败；不能生成/编译的任务仍保留在端到端分母。',
        '', '## 调用与成本', '', '| 方法 | 分摊请求尝试 | tokens | 服务返回费用（美元） | 累计LLM秒数 |','|---|---:|---:|---:|---:|']
    for a in ARMS:
        u=stats['arms'][a]['allocated_usage'];lines.append(f"| {LABELS[a]} | {u['llm_calls']} | {u['tokens']} | {u['reported_cost_usd']:.6f} | {u['llm_seconds']:.1f} |")
    u=stats['physical_usage']
    lines += ['',f"实际全量请求尝试{u['llm_calls']}次，已返回响应{u['responses']}次，记录tokens {u['tokens']}，服务返回费用${u['reported_cost_usd']:.6f}。direct与direct_feedback共享首稿，不能把三组分摊费用相加当成真实消费。",
        '直接首稿最多1次调用；直接反馈和完整流程最多各6次方法调用。不同表示的token量与反馈来源不同，因此这是同调用上限的补充对照，不是严格等token或等费用实验。无usage的传输失败和单独预检不在上述已返回费用内，不能解释为零费用。',
        '', '## 完整流程失败阶段', '', '| 阶段 | 任务重复数 | 涉及独立题目数 |','|---|---:|---:|']
    counts=Counter(r['failure_stage'] for r in failures)
    for stage,n in counts.most_common():lines.append(f"| {stage} | {n} | {len({r['case'] for r in failures if r['failure_stage']==stage})} |")
    lines += ['', '逐题原因与公开需求见workflow_failure_details.json；原模型、全部提示/响应、诊断、来源凭据和编译日志均保留。接口规划失败不全等于不支持设备，具体原因可能为硬件限制、契约形状、依赖声明或需求映射错误，须按原诊断解释。',
        '', '## 解释边界', '', '主表测量生成及目标编译覆盖，不证明功能正确。配对区间按题目聚类，同一题的重复运行不是独立任务。小范围host功能表现不能抵消全量需求覆盖不足。现有证据不包含实板验证，也没有对当前方法做完整功能覆盖或等成本消融。']
    (root/'FINAL_REPORT_ZH.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')

    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    fig,axes=plt.subplots(1,2,figsize=(11,4.5),layout='constrained')
    names=['Direct','Direct + feedback','Workflow'];colors=['#8697ac','#507dab','#238878']
    for split,ax,title in [('test',axes[0],'Original test split compilation'),('development',axes[1],'Development compilation')]:
        rates=[stats['partitions'][split]['arms'][a]['compile']['rate']*100 for a in ARMS]
        bars=ax.bar(names,rates,color=colors)
        ax.bar_label(bars,labels=[f'{x:.1f}%' for x in rates],padding=4)
        n=sum(c['split']==split for c in config['cases'])
        ax.set_ylim(0,108);ax.set_ylabel('Pass rate (%)');ax.set_title(f'{title} ({n} tasks)');ax.grid(axis='y',alpha=.2);ax.set_axisbelow(True)
    fig.savefig(root/'comparison.png',dpi=180);plt.close(fig)
    print(json.dumps({'records':len(rows),'artifacts_checked':len(artifacts),'report':str(root/'FINAL_REPORT_ZH.md')},ensure_ascii=False))


if __name__=='__main__':main()
