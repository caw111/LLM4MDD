"""Summarize completed paired runs without treating compilation as correctness."""
import json, math
from pathlib import Path
import argparse

def main():
    p=argparse.ArgumentParser();p.add_argument('folder',type=Path);args=p.parse_args();root=args.folder
    manifest=json.loads((root/'manifest.json').read_text(encoding='utf-8'))
    rows=json.loads((root/'results.json').read_text(encoding='utf-8'))
    arms=('direct','direct_feedback','workflow','no_sysml_feedback','llm_codegen')
    if len(rows)!=len(manifest['cases'])*len(arms):raise SystemExit('Run incomplete: do not report a complete experiment')
    lines=['# 五组对比与消融实验结果','',f"固定样本 {len(manifest['cases'])} 个，五组评估记录齐全（前置模型失败的组记为未生成）。模型为 {manifest['model']}，每题每组一轮；编译指标指最终 Arduino 程序的真实目标板编译。",'',
           '| 组别 | 编译通过 | 模型通过 | 调用均值 | Token 均值 |', '|---|---:|---:|---:|---:|']
    for arm in arms:
        group=[r for r in rows if r['arm']==arm]
        model='不适用' if arm.startswith('direct') else str(sum(bool(r.get('model_ok')) for r in group))+'/'+str(len(group))
        lines.append(f"| {arm} | {sum(r['compile_ok'] for r in group)}/{len(group)} | {model} | {sum(r['llm_calls'] for r in group)/len(group):.2f} | {sum(r['tokens'] for r in group)/len(group):.0f} |")
    lines+=['','## 配对结果','', '| 比较 A / B | A 过、B 不过 | A 不过、B 过 | 双侧精确 McNemar p |','|---|---:|---:|---:|']
    paired={a:{r['case']:r for r in rows if r['arm']==a} for a in arms}
    stats={}
    for a,b in [('workflow','direct'),('workflow','direct_feedback'),('workflow','no_sysml_feedback'),('workflow','llm_codegen'),('direct_feedback','direct')]:
        win=sum(paired[a][i]['compile_ok'] and not paired[b][i]['compile_ok'] for i in paired[a])
        loss=sum(not paired[a][i]['compile_ok'] and paired[b][i]['compile_ok'] for i in paired[a])
        n=win+loss
        value=min(1.0,2*sum(math.comb(n,k) for k in range(min(win,loss)+1))/(2**n)) if n else 1.0
        stats[a+'/'+b]={'wins':win,'losses':loss,'exact_mcnemar_p':value}
        lines.append(f'| {a} / {b} | {win} | {loss} | {value:.6f} |')
    lines+=['','以上为探索性配对检验，没有多重比较校正，也没有覆盖跨次随机波动。不能把未显著解释为等效。','', '## 按数据集','', '| 数据集 | direct | direct_feedback | workflow | no_sysml_feedback | llm_codegen |','|---|---:|---:|---:|---:|---:|']
    for dataset in ('internal','PCEval','EmbedBench'):
        vals=[]
        for arm in arms:
            group=[r for r in rows if r['dataset']==dataset and r['arm']==arm]
            vals.append(f"{sum(r['compile_ok'] for r in group)}/{len(group)}")
        lines.append('| '+dataset+' | '+' | '.join(vals)+' |')
    lines+=['','## 逐题结果','', '| 样本 | direct | direct_feedback | workflow | no_sysml_feedback | llm_codegen |','|---|---|---|---|---|---|']
    for case in manifest['cases']:
        lines.append('| '+case['id']+' | '+' | '.join(paired[a][case['id']]['compile_status'] for a in arms)+' |')
    lines+=['','## 最终模型诊断','']
    for path in sorted(root.glob('*/workflow/validation.json')):
        report=json.loads(path.read_text(encoding='utf-8'));attempt=report['attempts'][-1]
        if not attempt['ok']:
            messages=list(dict.fromkeys(i.get('message','') for i in attempt['issues']))[:3]
            lines.append('- '+path.parent.parent.name+'：'+'；'.join(messages))
    successful_models = sum(bool(r.get('model_ok')) for r in rows if r['arm']=='workflow')
    lines += ['', '## 本轮能支持的结论', '']
    if not successful_models:
        lines += ['本轮完整工作流没有模型通过生成/校验阶段，不能证明当前工作流优于直接生成。反馈循环确实执行，但在所选样本与预算内未将失败模型修复至可编译。', '',
                  'llm_codegen 组因共同前置模型失败而没有实际进入 LLM 代码生成；因此确定性编译器消融没有可观察的下游结果，不能据此断言编译器有效或无效。', '',
                  '应先提高 GPT-4o-mini 在受支持 SysML 子集上的生成可靠性，例如提供可通过校验的状态动作样例、区分硬件字符串输出与模型计算、强化针对具体诊断的修复指导。随后在新的保留样本和多个种子上重跑。当前 12 题已经用于诊断，后续调优不能将其冒充未见测试集。']
    else:
        lines += [f'共有 {successful_models} 个模型通过。模型阶段通过不等于最终 Arduino 编译通过，也不等于行为正确；请结合逐题编译结果与未执行的功能验证解读。']
    usage=json.loads((root/'actual_usage.json').read_text(encoding='utf-8'))
    lines+=['','## 费用与结论边界','',f"本轮实际调用 {usage['llm_calls']} 次，{usage['tokens']} tokens，服务端返回费用合计 ${usage['reported_cost_usd']:.5f}。共享前缀只在实际调用量中计算一次；未包括前期探测和中止的开发预实验。",'',
            '本轮没有 Wokwi 功能仿真或实板测试，编译通过不能写成行为正确或需求满足。参考程序和原测试均未作为提示输入。结果只描述冻结的 12 题、一个种子和当前编译器可执行子集。', '',
            '消融中的 no_sysml_feedback 仅去掉模型修复，其他下游流程保持一致；llm_codegen 的反馈诊断与完整流程不同，其差异不能单独归因于确定性编译。详细设计见 experiment/CONTROLLED_EXPERIMENT.md。']
    (root/'REPORT_ZH.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
    (root/'paired_statistics.json').write_text(json.dumps(stats,indent=2),encoding='utf-8')
if __name__=='__main__':main()
