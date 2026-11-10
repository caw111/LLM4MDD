"""Publish supplementary metrics without treating an unfinished semantic review as a result."""
import collections
import hashlib
import json
import statistics
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT/'experiment/output/additional_metrics_20261109'
BASE = ROOT/'experiment/output/picsimlab_comparison_20261108'
LABELS = {
    'direct_gpt61': 'GPT-6.1 直接生成',
    'ablation_direct': '同轮直接生成首稿',
    'ablation_direct_feedback': '直接生成＋编译反馈',
    'ablation_workflow': '完整工作流（同轮原始输出）',
    'ablation_no_contract_feedback': '关闭契约反馈',
    'ablation_no_model_feedback': '关闭模型反馈',
    'ablation_blind_resampling': '无反馈重采样',
    'ablation_llm_backend': 'LLM 替换确定性后端',
}


def load(p):
    return json.loads(p.read_text(encoding='utf-8'))


def percent(value):
    return '—' if value is None else f'{value:.2f}%'


def table(headers, rows):
    return '\n'.join(['| '+' | '.join(headers)+' |', '| '+' | '.join(['---']*len(headers))+' |']+
                     ['| '+' | '.join(map(str, row))+' |' for row in rows])


def main():
    summary = load(OUT/'summary.json')
    rows = load(OUT/'all_project_metrics.json')
    assert len(rows) == 2368
    assert len({(r['group'], r['case']) for r in rows}) == len(rows)
    for group in LABELS:
        rr = [r for r in rows if r['group'] == group]
        assert len(rr) == 296
        assert collections.Counter(r['dataset'] for r in rr) == {'internal':120, 'PCEval':50, 'EmbedBench':126}
        assert all(not (r['status'] == 'passed' and not r['compile_ok']) for r in rr)
        for r in rr:
            c = r['rule_counts']
            assert c['N'] == c['P']+c['F']+c['U'] and c['N'] > 0
    for s in summary['summaries']:
        s['generated'] = s['projects']-s['missing_source_projects']
        s['generation_percent'] = 100*s['generated']/s['projects']
        s['compile_given_generated_percent'] = 100*s['compiled']/s['generated'] if s['generated'] else None
        assert s['missing_source_projects']+s['compile_failed_with_source']+s['compiled_but_basic_failed']+s['basic_passed'] == s['projects']
    # Resource means across the same projects avoid changing task composition.
    compiled_sets = [{r['case'] for r in rows if r['group'] == g and r['compile_ok']} for g in LABELS]
    common = set.intersection(*compiled_sets)
    common_resource = []
    for g in LABELS:
        rr = [r for r in rows if r['group'] == g and r['case'] in common]
        assert all(r.get('memory') for r in rr)
        common_resource.append({'group':g, 'projects':len(rr),
                                **{f'mean_{k}': statistics.mean(r['memory'][k] for r in rr)
                                   for k in ('flash_bytes','sram_static_bytes','flash_percent','sram_static_percent')}})
    summary['resource_common_compiled_case_count'] = len(common)
    summary['resource_common_compiled_cases'] = sorted(common)
    summary['resource_common_compiled'] = common_resource
    summary['labels'] = LABELS
    summary['verification'] = {'project_keys_unique': True, 'dataset_counts_match': True,
                               'pipeline_partition_valid': True, 'source_sha256_verified_during_preparation': 1218,
                               'analyzer_self_check': summary['self_check']}
    summary['definitions'] = {
        'generation_percent': 'archived generated source projects / all tasks',
        'compile_percent': 'native target-board compilation successes / all tasks',
        'basic_percent': 'existing PICSimLab final core-function decisions / all tasks',
        'functional_given_compile_percent': 'core-function successes / compilation successes',
        'macro_rules_percent': 'mean of project-level P/(P+F+U), limited frozen offline rule catalog',
        'macro_rules_unknown_percent': 'mean of project-level U/(P+F+U), not a defect fraction',
        'memory': 'compiler-reported flash and static SRAM, excludes stack/heap high-water marks',
        'requirement_implementation_coverage': 'NOT COMPLETED; draft checklist is not a reviewed denominator or a semantic score',
    }
    (OUT/'final_summary.json').write_text(json.dumps(summary, ensure_ascii=False, indent=2), encoding='utf-8')
    allrows = [s for s in summary['summaries'] if s['dataset'] == 'all']
    main_table = table(['方法','生成完成率','编译通过率 /296','核心功能通过率 /296','编译后功能通过率','Flash均值','静态SRAM均值'],
                       [[LABELS[s['group']], percent(s['generation_percent']), f"{s['compiled']}/296（{s['compile_percent']:.2f}%）",
                         f"{s['basic_passed']}/296（{s['basic_percent']:.2f}%）", percent(s['functional_given_compile_percent']),
                         percent(s['mean_flash_percent']), percent(s['mean_sram_static_percent'])] for s in allrows])
    report = [
        '# 8组补充指标结果（2026-11-09）',
        '本轮共8组×296题=2,368个工程记录：本地120题、PCEval 50题、EmbedBench 126题。移除了用户要求去掉的两个旧版直接生成组。源码快照保持冻结；1,218份不同源码逐份复核SHA-256后进行离线C++ AST分析。',
        '本轮复用已经完成的原生PICSimLab仿真记录和目标板编译日志，没有重新生成候选代码，也没有将主机桩执行当作电路仿真。下面的功能指标沿用此前“核心功能满足基本需要”的最终判定，不等于需求全文完全正确。',
        '## 已完成的指标', main_table,
        '生成完成率=有归档源码的题数/296；端到端编译通过率=编译通过数/296；编译后功能通过率=核心功能通过数/编译通过数。后二者的分母不同，应同时保留。Flash、静态SRAM均值只计算各组编译通过的工程，其样本数等于“编译通过”列的分子；两种板型分别按其目标板可用容量归一化后求工程平均。',
        '静态SRAM不含运行栈、堆峰值；内存小不能单独证明质量高，遗漏功能也可能使固件变小。',
        '## 失败阶段分解',
        table(['方法','无生成源码','有代码但编译失败','编译通过但功能失败','功能通过'],
              [[LABELS[s['group']],s['missing_source_projects'],s['compile_failed_with_source'],s['compiled_but_basic_failed'],s['basic_passed']] for s in allrows]),
        '以上四列之和均为296。完整工作流本轮是39题未生成源码、0题有源码但编译失败、161题编译后核心功能失败、96题核心功能通过。不能把这39题描述成C++编译器拒绝了39份代码。',
        '## 有界嵌入式规则审查（补充分析，尚不能代替完整人工审核指标）',
        table(['方法','工程平均已确认满足率','工程平均U比例','有源码的明确违例工程数','P / F / U条目总数'],
              [[LABELS[s['group']],percent(s['macro_rules_percent']),percent(s['macro_rules_unknown_percent']),
                s['explicit_hardware_violation_projects'],f"{s['rule_totals']['P']} / {s['rule_totals']['F']} / {s['rule_totals']['U']}"] for s in allrows]),
        '先按每个工程P/(P+F+U)计算，再平均296个工程；不是汇总条目后的微平均。U也进入分母，不能解释为缺陷。F总数含“无生成源码”这一明确实现缺失；单独的“有源码违例”列不含无源码工程。高U和方法间不同的解析可判定性使这些分数不能直接作为生成质量排名。',
        '固定规则：GPIO引脚范围；硬件PWM能力（合法数字端点输出不要求PWM）；ADC通道；外部中断映射；已披露串口波特率；必需外设的API引脚绑定；外设初始化；潜在定时器兼容性。规则按规则＋设备/已解析物理引脚去重。未解析库调用、间接接线、动态引脚、初始化支配关系和定时器生命周期留U。完整原始电路的外设保留在分母，不把解析器不认识的外设当成N/A。',
        '这是一轮生成后的补充规则审查，不是生成前预注册实验；规则细节与源码行证据位于source_audits。未宣称形式验证、全面控制流证明或人工审核完成。',
        '## 需求实现覆盖率：尚未完成，不能填入论文为最终数值',
        '配置的语义审查接口返回HTTP403。已保存296题共用的原始需求分句草稿 requirement_checklists.draft.json，但分句不等于人工审核后的条件/输入/动作/状态清单，不能把它当作最终分母。没有把通过的测试条数当作需求条数，也没有把整题仿真通过自动转换成需求覆盖100%。已有源码的未审核条目保留U；无源码是可确认的实现缺失。draft计数只用于待办管理，不是已完成语义评测。',
        '恢复可用审查接口后，仍需核验清单与源代码路径证据，再发布这一指标；也可以由人工逐条完成。当前没有可信的完整需求实现覆盖率。',
        '## 相同任务上的资源比较',
        f'八组共同编译通过的任务交集为{len(common)}题。以下均使用同一批任务，减少不同成功样本组成导致的偏差；这批任务仍不代表全部296题。',
        table(['方法','共同样本数','Flash均值（字节）','Flash占比','静态SRAM均值（字节）','静态SRAM占比'],
              [[LABELS[s['group']],s['projects'],f"{s['mean_flash_bytes']:.1f}",percent(s['mean_flash_percent']),
                f"{s['mean_sram_static_bytes']:.1f}",percent(s['mean_sram_static_percent'])] for s in common_resource]),
        '## 三个数据集分别的结果',
    ]
    for dataset, label in [('internal','本地数据集（120题）'),('PCEval','PCEval（50题）'),('EmbedBench','EmbedBench（126题）')]:
        ss = [s for s in summary['summaries'] if s['dataset'] == dataset]
        report += ['### '+label,
                   table(['方法','有源码','编译通过','核心功能通过','编译后功能通过率','规则确认率 / U','Flash / 静态SRAM'],
                         [[LABELS[s['group']],f"{s['generated']}/{s['projects']}",
                           f"{s['compiled']}/{s['projects']}（{s['compile_percent']:.2f}%）",
                           f"{s['basic_passed']}/{s['projects']}（{s['basic_percent']:.2f}%）",
                           percent(s['functional_given_compile_percent']),
                           percent(s['macro_rules_percent'])+' / '+percent(s['macro_rules_unknown_percent']),
                           percent(s['mean_flash_percent'])+' / '+percent(s['mean_sram_static_percent'])] for s in ss])]
    report += ['## 验证与复现',
               '检查通过：2,368条唯一工程键；每组数据集数量120/50/126；所有功能通过项均编译通过；四阶段计数之和等于样本数；规则N=P+F+U；源码哈希逐份一致；解析器11项边界自检通过。',
               '解析器：tree-sitter 0.25.2、tree-sitter-cpp 0.23.4（独立用户缓存安装）。0.26.0在本机发生原生崩溃，已固定到通过自检的0.25.2，未使用崩溃轮次的残缺输出。',
               '复现顺序：Windows Python执行 experiment/code/prepare_supplementary_metrics.py；WSL Python执行 evaluate_supplementary_metrics.py --bundle <input_bundle.json> --output <本目录>；Windows Python执行 report_supplementary_metrics.py。候选源码和原有仿真文件只读。',
               'GPT-6.1属于跨模型对照；其余同轮对照不能与此前人工逐题修补至283/296的快照混为一组。这里保留原实验源码身份，不以修补版替换消融结果。']
    (OUT/'FINAL_RESULTS.md').write_text('\n\n'.join(report)+'\n', encoding='utf-8')
    checklists = OUT/'requirement_checklists.draft.json'
    integrity = {'source_files_verified':1218, 'projects':len(rows),
                 'files': {str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest()
                           for p in [OUT/'input_bundle.json', checklists, OUT/'final_summary.json',
                                     ROOT/'experiment/code/evaluate_supplementary_metrics.py',
                                     ROOT/'experiment/code/prepare_supplementary_metrics.py', Path(__file__)]}}
    (OUT/'integrity.json').write_text(json.dumps(integrity, ensure_ascii=False, indent=2), encoding='utf-8')
    print(json.dumps({'report': str(OUT/'FINAL_RESULTS.md'), 'common_compiled_cases':len(common), 'checks_passed': True}))
    print(main_table)


if __name__ == '__main__':
    main()
