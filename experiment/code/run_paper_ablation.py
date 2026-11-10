"""Frozen, paired component ablations of the current production workflow.

Shared first responses reduce cost and sampling noise. Every arm has the same
external compiler/oracles; acceptance by a method's own gate is not correctness.
"""
import argparse
import copy
import hashlib
import json
import os
import shutil
import threading
import time
from collections import Counter
from concurrent.futures import ThreadPoolExecutor, wait, FIRST_COMPLETED
from datetime import datetime, timezone, timedelta
from pathlib import Path

import run_current_comparison as production
from continue_repaired_workflows import ContinuationCalls
from resume_configured_audit import retry_permission
from experiment_metrics import paired, summarize, first_response_entries
from app.services.configuration_policy import resolve_configuration
from app.services.llm_json import check_completion

ROOT = production.ROOT
ARMS = ('direct', 'direct_feedback', 'workflow', 'no_contract_feedback',
        'no_model_feedback', 'blind_resampling', 'llm_backend')
LIMITS = dict(direct=1, direct_feedback=6, workflow=6, no_contract_feedback=4,
              no_model_feedback=4, blind_resampling=6, llm_backend=7)
LABELS = dict(direct='LLM直接首稿', direct_feedback='LLM编译反馈', workflow='完整工作流',
              no_contract_feedback='关闭接口修复', no_model_feedback='关闭模型修复',
              blind_resampling='无诊断重新采样', llm_backend='LLM替换确定性后端')
CONTRASTS = [('workflow', a) for a in ARMS if a != 'workflow']
SERVICE_BLOCK = threading.Event()
ACTIVE_ROOT = None
BACKEND_PROMPT = '''Generate a complete Arduino sketch implementing the ACCEPTED executable model and frozen hardware contract below.
Replace the deterministic SysML/IR/IL/C backend and hardware adapter with code generation. Preserve the exact controller state, ordered updates, output expressions, pin bindings, timing, units and protocols; implement all original requirements. Do not simplify behavior. Use setup() and loop(). Output ONLY C++ code. No reference solution or hidden test is available.
Original effective requirements:
'''


def save(path, value):
    production.save(path, value)


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def response_prefix(entries, count):
    if count == 0:
        return []
    responses = 0
    for i, entry in enumerate(entries):
        responses += 'response' in entry
        if responses == count:
            return copy.deepcopy(entries[:i+1])
    raise ValueError('Missing original successful response; cannot reconstruct ablation prefix')


def usage(entries):
    return dict(**production.metrics(entries), method_calls=sum('response' in e for e in entries))


class ExperimentCalls(ContinuationCalls):
    def __call__(self, prompt):
        if SERVICE_BLOCK.is_set():
            raise RuntimeError('Experiment requests stopped after service refusal')
        try:
            return super().__call__(prompt)
        except RuntimeError as exc:
            if any(kind in str(exc) for kind in ('PermissionDeniedError', 'AuthenticationError', 'RateLimitError')):
                SERVICE_BLOCK.set()
                if ACTIVE_ROOT is not None:
                    now=datetime.now(timezone.utc)
                    save(ACTIVE_ROOT/'service_retry_status.json',dict(
                        failed_utc=now.isoformat(),next_retry_not_before_utc=(now+timedelta(hours=6)).isoformat(),
                        error_type=next(kind for kind in ('PermissionDeniedError','AuthenticationError','RateLimitError') if kind in str(exc))))
            raise


class BlindCalls(ExperimentCalls):
    """Remove diagnostic information, retaining the same gate and sample limit."""
    def __init__(self, folder, model, seed=(), shared_ref=None):
        super().__init__(folder, model)
        self.seed, self.shared_ref, self.initial_prompt = seed, shared_ref, None

    def __call__(self, prompt):
        if self.initial_prompt is None:
            self.initial_prompt = prompt
            first = next((e for e in self.seed if 'response' in e), None)
            if not self.entries and first is not None and first['prompt'] == prompt:
                self.entries = [dict(copy.deepcopy(first), shared_call_ref=self.shared_ref)]
                save(self.folder/'calls.json', self.entries)
        # The pipeline still computes and records diagnostics, but the LLM sees
        # only the original stage request on every fresh sampling attempt.
        return super().__call__(self.initial_prompt)


def ablation_row(full, arm, entries, failure=None, **extra):
    row = copy.deepcopy(full)
    row.update(arm=arm, **usage(entries), **extra)
    if failure is not None:
        for key in ('artifact', 'artifact_sha256', 'provenance_status', 'backend_ok', 'ir_ok'):
            row.pop(key, None)
        row.update(compile_ok=False, compile_status='not_generated',
                   functional_status='not_generated_or_compile_failed', failure_stage=failure,
                   counterfactual_scope='stopped_at_rejected_first_candidate')
    else:
        row['shared_artifact'] = 'workflow'
    return row


def derive_feedback_ablations(full, entries, planning, validation):
    plan_entries = response_prefix(entries, len(planning['attempts']))
    initial_plan = planning['attempts'][0]['ok']
    no_plan = ablation_row(full, 'no_contract_feedback',
        entries if initial_plan else first_response_entries(plan_entries),
        None if initial_plan else 'interface_contract', initial_contract_ok=initial_plan)
    if not validation:
        no_model = ablation_row(full, 'no_model_feedback', plan_entries)
    else:
        model_entries = entries[len(plan_entries):]
        initial_model = validation['attempts'][0]['ok']
        no_model = ablation_row(full, 'no_model_feedback',
            entries if initial_model else plan_entries + first_response_entries(model_entries),
            None if initial_model else (validation['attempts'][0].get('failure_stage') or 'sysml_generation'),
            initial_model_ok=initial_model)
    return no_plan, no_model, plan_entries


def outcome_error(exc, stage):
    if isinstance(exc, (production.ContractPlanningError, production.SysMLValidationError)):
        last = exc.attempts[-1] if exc.attempts else {}
        return dict(failure_stage=last.get('failure_stage') or stage,
                    error_type=type(exc).__name__, method_diagnostics=last)
    if isinstance(exc, production.BackendFailure):
        return dict(failure_stage=stage, error_type=type(exc).__name__)
    if isinstance(exc, production.api.HTTPException) and exc.status_code in (400, 422):
        return dict(failure_stage=stage, error_type=type(exc).__name__, method_diagnostics=exc.detail)
    if isinstance(exc, (ValueError, TypeError, KeyError)):
        return dict(failure_stage=stage, error_type=type(exc).__name__, method_diagnostics=str(exc)[:3000])
    return dict(failure_stage=stage, infrastructure_error=type(exc).__name__, functional_status='unverified')


def blank(case, repeat, arm):
    return dict(case=case['id'], dataset=case['dataset'], stratum=case['stratum'],
                repeat=repeat, arm=arm, compile_ok=False, compile_status='not_generated',
                functional_status='not_generated_or_compile_failed')


def blind_workflow(case, repeat, folder, model, seed_plan, seed_model, shared_ref):
    row = blank(case, repeat, 'blind_resampling')
    resolution = resolve_configuration(case['requirement'], {}, board=case['fqbn'])
    requirement = resolution['effective_requirement']
    contract_calls = BlindCalls(folder/'contract_calls', model, seed_plan, shared_ref)
    model_calls = BlindCalls(folder/'model_calls', model, seed_model, shared_ref)
    stage = 'interface_contract'
    try:
        contract, planning = production.plan_interface_contract(contract_calls, requirement, {},
            target_board=case['fqbn'], max_repairs=2, configuration_resolution=resolution)
        save(folder/'contract.json', contract); save(folder/'contract_planning.json', planning)
        stage = 'sysml_generation'; accepted = {}
        def validator(source):
            checked = production.validate_candidate(source)
            if checked['ok']:
                accepted.update(source=source, ir=checked['ir'])
            return checked
        _, bundle, validation = production.generate_validated_bundle(model_calls,
            production.sysml_bundle_prompt(requirement, contract=contract),
            max_repairs=2, validator=validator, contract=contract)
        save(folder/'model.json', bundle); save(folder/'model_validation.json', validation)
        source = bundle['system_sysml2']
        if accepted.get('source') != source:
            raise ValueError('Accepted source differs from the validated source')
        stage = 'ir_to_code'
        with production.TRANSLATOR, production.JAVA:
            generated = production.lower_verified(source, contract, accepted['ir'], folder/'lowered')
        stage = 'hardware_adapter'
        code = production.adapt_verified(generated, contract, folder/'lowered')
        receipt = read(folder/'lowered/arduino_provenance.json')
        stage = 'arduino_compile'
        result, _ = production.compile_candidate(case, folder/'final', code, receipt)
        row.update(result, model_attempts=len(validation['attempts']),
                   contract_attempts=len(planning['attempts']))
    except Exception as exc:
        if isinstance(exc, (production.ContractPlanningError, production.SysMLValidationError)):
            save(folder/('contract_planning.json' if stage == 'interface_contract' else 'model_validation.json'),
                 dict(ok=False, attempts=exc.attempts))
        row.update(outcome_error(exc, stage))
    row.update(usage(contract_calls.entries + model_calls.entries))
    return row


def llm_backend(case, repeat, folder, model, full, upstream, source_folder):
    row = blank(case, repeat, 'llm_backend')
    if not (source_folder/'model.json').exists():
        row.update({k:v for k,v in full.items() if k not in ('arm',)})
        row.update(arm='llm_backend', shared_upstream_failure=True, **usage(upstream))
        return row
    bundle, contract = read(source_folder/'model.json'), read(source_folder/'contract.json')
    calls = ExperimentCalls(folder/'calls', model)
    try:
        prompt = (BACKEND_PROMPT + case['requirement'] + '\nFrozen interface contract:\n' +
                  json.dumps(contract, ensure_ascii=False) + '\nAccepted executable model bundle:\n' +
                  json.dumps(bundle, ensure_ascii=False))
        _, raw = calls(prompt)
        check_completion(raw)
        result, _ = production.compile_candidate(case, folder/'final', production.extract(raw))
        row.update(result)
    except Exception as exc:
        row.update(outcome_error(exc, 'llm_backend'))
    row.update(usage(upstream + calls.entries), model_accepted=True,
               final_code_provenance='LLM replacement; deterministic provenance not claimed')
    return row


def evaluate(case, repeat, root, model):
    folder = root/'jobs'/case['id']/f'repeat_{repeat}'
    result_path = folder/'ablation_results.json'
    if result_path.exists():
        previous = read(result_path)
        if {r['arm'] for r in previous} == set(ARMS) and not any(r.get('infrastructure_error') for r in previous):
            return previous
    # Existing production runner executes fresh baseline and complete workflow.
    rows = production.evaluate(case, repeat, root/'jobs', model, feedback_calls=6)
    if any(r.get('infrastructure_error') for r in rows):
        save(result_path, rows)
        return rows
    full = next(r for r in rows if r['arm'] == 'workflow')
    entries = read(folder/'workflow_calls/calls.json')
    direct_entries = read(folder/'direct_calls/calls.json')
    for row in rows:
        allocated = entries if row['arm']=='workflow' else (
            first_response_entries(direct_entries) if row['arm']=='direct' else direct_entries)
        row['method_calls'] = sum('response' in e for e in allocated)
    planning = read(folder/'contract_planning.json')
    validation = read(folder/'model_validation.json') if (folder/'model_validation.json').exists() else None
    no_plan, no_model, plan_entries = derive_feedback_ablations(full, entries, planning, validation)
    rows += [no_plan, no_model]
    effective = copy.deepcopy(case)
    effective['requirement'] += ('\n' + production.INTERPRETATIONS[case['id']]) if case['id'] in production.INTERPRETATIONS else ''
    effective['requirement'] = read(folder/'configuration_resolution.json')['effective_requirement']
    blind_path = folder/'blind_resampling'
    checkpoint = blind_path/'result.json'
    if checkpoint.exists() and not read(checkpoint).get('infrastructure_error'):
        blind = read(checkpoint)
    else:
        # Start from the same original requirement as production to preserve
        # the configuration-resolution provenance embedded in the contract.
        original = copy.deepcopy(case)
        original['requirement'] += ('\n' + production.INTERPRETATIONS[case['id']]) if case['id'] in production.INTERPRETATIONS else ''
        blind = blind_workflow(original, repeat, blind_path, model,
            plan_entries, entries[len(plan_entries):], str((folder/'workflow_calls/calls.json').relative_to(ROOT)))
        save(checkpoint, blind)
    rows.append(blind)
    backend_path = folder/'llm_backend'; checkpoint = backend_path/'result.json'
    if checkpoint.exists() and not read(checkpoint).get('infrastructure_error'):
        backend = read(checkpoint)
    else:
        backend = llm_backend(effective, repeat, backend_path, model, full, entries, folder)
        save(checkpoint, backend)
    rows.append(backend)
    for row in rows:
        row.setdefault('method_calls', row['llm_calls'])
        if row['method_calls'] > LIMITS[row['arm']]:
            raise ValueError('Method response budget exceeded')
    save(result_path, rows)
    return rows


def collect(root, config):
    rows = [r for p in sorted((root/'jobs').glob('*/repeat_*/ablation_results.json')) for r in read(p)]
    keys = [(r['case'], r['repeat'], r['arm']) for r in rows]
    expected = {(c['id'], r, a) for c in config['cases'] for r in range(1, config['repetitions']+1) for a in ARMS}
    if len(keys) != len(set(keys)) or not set(keys) <= expected:
        raise ValueError('Duplicate or unexpected experiment record')
    save(root/'results.json', rows)
    save(root/'progress.json', dict(records=len(rows), planned=len(expected),
        jobs_complete=sum({r['arm'] for r in read(p)} == set(ARMS) and not any(r.get('infrastructure_error') for r in read(p))
                          for p in (root/'jobs').glob('*/repeat_*/ablation_results.json')),
        infrastructure_records=sum(bool(r.get('infrastructure_error')) for r in rows),
        compile_passes={a:sum(r.get('compile_ok', False) for r in rows if r['arm']==a) for a in ARMS}))
    return rows, expected


def summarize_run(root, config):
    rows, expected = collect(root, config)
    stats = summarize(rows, expected_keys=expected)
    oracle_ids = set(production.ORACLE_TASKS)
    stats['functional_subset'] = {a:dict(Counter(r['functional_status'] for r in rows if r['arm']==a and r['case'] in oracle_ids)) for a in ARMS}
    stats['functional_subset_denominator_per_arm'] = len(oracle_ids & {c['id'] for c in config['cases']}) * config['repetitions']
    stats['contrasts'] = [dict(left=l, right=r, compile=paired(rows,l,r,'compile'),
        functional=paired([x for x in rows if x['case'] in oracle_ids],l,r,'functional')) for l,r in CONTRASTS]
    stats['by_dataset'] = {d:{a:dict(passes=sum(r['compile_ok'] for r in rows if r['arm']==a and r['dataset']==d),
        denominator=sum(c['dataset']==d for c in config['cases'])*config['repetitions']) for a in ARMS} for d in sorted({c['dataset'] for c in config['cases']})}
    calls = [e for p in (root/'jobs').rglob('calls.json') for e in read(p)]
    physical = [e for e in calls if not e.get('shared_call_ref')]
    stats['physical_usage'] = dict(**usage(physical), reported_cost_calls=sum((e.get('usage') or {}).get('cost') is not None for e in physical))
    stats['shared_response_replays'] = sum(bool(e.get('shared_call_ref')) for e in calls)
    stats['source_changes'] = [p for p,h in config['hashes'].items() if not (ROOT/p).is_file() or production.digest(ROOT/p)!=h]
    stats['artifact_mismatches'] = [dict(case=r['case'],repeat=r['repeat'],arm=r['arm']) for r in rows
        if r.get('artifact') and (not (ROOT/r['artifact']).is_file() or production.digest(ROOT/r['artifact'])!=r['artifact_sha256'])]
    unresolved = [dict(case=r['case'],repeat=r['repeat'],arm=r['arm'],status=r['functional_status']) for r in rows
        if r.get('infrastructure_error') or (r['case'] in oracle_ids and r.get('compile_ok') and r['functional_status'] not in ('passed','failed','simulation_failed'))]
    stats['unresolved_environment'] = unresolved
    stats['complete'] = not(stats['missing_records'] or unresolved or stats['source_changes'] or stats['artifact_mismatches'])
    stats['max_method_calls_excluding_transport_retries'] = LIMITS
    stats['budget_caveat'] = 'Feedback-removal arms use fewer responses. Blind resampling controls the 3+3 response ceiling. LLM backend adds one call to the same accepted upstream model. Physical usage excludes shared response copies.'
    save(root/'summary.json', stats)
    lines = ['# 当前生产工作流组件消融实验', '',
        f"阶段：{config['phase']}；模型：{config['model']}；{len(config['cases'])}题×{config['repetitions']}轮×7组。核查完成：{stats['complete']}。", '',
        '所有结果为本次冻结版本重新生成；不拼接历史修复成功。各组使用相同有效需求、目标板编译器和独立功能测试。',
        '关闭修复组保留首稿、校验及拒绝门槛；共享同一首稿，仅移除后续修复。无诊断重新采样组保留相同3+3次预算与全部门槛，但后续调用只收到原始提示，不收到失败代码或诊断。',
        'LLM后端组复用完整工作流已验收的契约与模型，另用一次LLM调用生成整个Arduino程序，替换确定性后端及适配器。其总调用上限7次，完整流程6次；不能宣称所有组等预算。', '',
        '| 方法 | 目标板编译通过 | 三轮通过率标准差 | 独立功能通过/有测试的计划记录 | 调用数（分摊） | 费用美元（分摊） |',
        '|---|---:|---:|---:|---:|---:|']
    for a in ARMS:
        s=stats['arms'][a]; c=s['compile']; u=s['allocated_usage']; f=stats['functional_subset'][a]
        sd=s['compile_repeat_sd']
        spread=f'{sd:.2%}' if sd is not None else '单轮'
        lines.append(f"| {LABELS[a]} | {c['passes']}/{c['denominator']} ({c['rate']:.1%}) | {spread} | {f.get('passed',0)}/{stats['functional_subset_denominator_per_arm']} | {u['llm_calls']} | {u['reported_cost_usd']:.4f} |")
    lines += ['', '## 配对效应', '', '| 完整工作流减去 | 编译率差（百分点） | 按题聚类95%区间（百分点） |', '|---|---:|---:|']
    for contrast in stats['contrasts']:
        c=contrast['compile']
        if c['ci95']:
            lines.append(f"| {LABELS[contrast['right']]} | {100*c['difference']:.2f} | [{100*c['ci95'][0]:.2f}, {100*c['ci95'][1]:.2f}] |")
    lines += ['', '## 使用范围与局限', '',
        '296题来自既有内部需求、PCEval与EmbedBench。该集合已用于开发，是固定回归评估，不是完全未见测试集。3轮表示独立重复生成，不能以任一轮成功充当单次成功率。',
        '功能测试覆盖预先固定的16题（每轮各组相同）；其余题仅有目标板编译证据。通过模型校验或编译不等于完整功能正确。未覆盖部分不算功能成功。',
        '共享上游输出形成配对设计，按题聚类计算2000次bootstrap区间；重复轮次不作为互相独立的新任务。区间为探索性比较，不进行未经校正的多重显著性宣称。',
        '关闭反馈可能降低可接受工件产出，这是门控工作流的结构性效应；只有与同预算盲采样对比及独立功能结果结合，才能讨论诊断反馈的额外价值。',
        '不包含可选前端模块规划、物理烧录及真实设备验证。LLM后端替换同时覆盖代码生成与硬件适配，不能据此单独归因于IL编译器。',
        f"物理调用总消耗：{json.dumps(stats['physical_usage'],ensure_ascii=False)}；共享响应重放{stats['shared_response_replays']}次。各组分摊费用不可相加。",
        f"未处理环境记录{len(unresolved)}条；源码变化{len(stats['source_changes'])}项；工件散列异常{len(stats['artifact_mismatches'])}条。", '',
        '完整逐题数据、配对区间、来源分层、失败阶段和成本见summary.json与results.json；提示、原始响应、模型、诊断及实际编译工件位于jobs目录。']
    (root/'REPORT_ZH.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
    if stats['complete'] and config['phase']=='formal':
        write_paper(root,config,stats)
    return stats


def write_paper(root, config, stats):
    n=len(config['cases']);reps=config['repetitions']
    lines=['# 可写入论文的消融实验段落（冻结版本回归研究）','',
        '### 实验设置','',
        f'为分析各组成部分对工件产出的贡献，我们在固定的{n}个嵌入式需求任务上开展组件消融。任务来自内部需求集、PCEval和EmbedBench，分别包含120、50和126题。每题独立重复生成{reps}轮，七种配置共形成{stats["planned_records"]}条计划记录。全部配置使用相同有效需求、统一硬件默认规则和真实目标板编译环境。该集合曾用于系统开发，因而本实验定位为固定回归评估，不将其描述为完全未见测试集。',
        f'统一采用{config["model"]}，temperature为0.2，单响应输出上限6000 tokens。完整工作流的接口规划与模型生成分别允许首稿及最多两次修复。直接编译反馈基线最多六次调用。接口/模型无反馈配置仅删除相应修复循环，其余检验和后端保持一致。另设置相同3+3次响应上限的盲采样配置：保留校验门槛，但重采样时不返回历史候选或诊断，从而考察反馈信息相对于更多采样的贡献。LLM后端配置复用相同已验收契约和模型，再通过一次LLM调用替换确定性代码生成及硬件适配，总上限为七次；不同配置的预算差异在结果中明确披露。',
        '所有配置均以最终Arduino代码在指定Uno或Mega目标板上的真实编译结果为主要交付指标。独立功能测试仅覆盖预先确定的16题，所有配置共享同一评估器；生成器不可访问其测试期望。未覆盖任务仅报告可编译性，不宣称功能正确。相同首稿和验收后的相同下游工件在适用配置间共享，以构成配对干预。按题聚类进行2000次bootstrap，保留同题重复生成的依赖关系；所给区间用于探索性分析。','',
        '### 结果','', '| 配置 | 编译通过/计划题次 | 编译率 | 独立功能通过/功能子集计划题次 |', '|---|---:|---:|---:|']
    for a in ARMS:
        c=stats['arms'][a]['compile'];f=stats['functional_subset'][a]
        lines.append(f'| {LABELS[a]} | {c["passes"]}/{c["denominator"]} | {c["rate"]:.1%} | {f.get("passed",0)}/{stats["functional_subset_denominator_per_arm"]} |')
    lines += ['']
    for contrast in stats['contrasts']:
        c=contrast['compile'];ci=c['ci95']
        if ci:
            interpretation=('区间未跨零，观察到正向差值' if ci[0]>0 else
                            '区间未跨零，观察到负向差值' if ci[1]<0 else '区间跨零，当前样本无法确立稳定的方向性差异')
            lines.append(f'完整工作流相对{LABELS[contrast["right"]]}的编译率差为{c["difference"]*100:.2f}个百分点，按题聚类95%区间为[{ci[0]*100:.2f}, {ci[1]*100:.2f}]个百分点；{interpretation}。')
    lines += ['', '### 解释边界','',
        '关闭反馈后未通过首稿验收的任务停止，因而产出下降首先说明修复对于门控工件交付的作用，不能仅据此证明语义反馈优于重复采样。该判断应主要结合盲采样对照与独立功能测试。接口修复消融不等于取消接口契约，模型修复消融不等于取消SysML，因此本实验不将上述差值解释为中间表示本身的独立收益。',
        '确定性后端替换同时涉及代码生成与硬件适配，其差值不单独归因于IL翻译器。强编译反馈基线保留在同一表中；若该基线表现更好，应据实报告，并将工作流的适用价值限定于模型可检查性、约束追踪及相应实测产出，而不宣称普遍提高代码生成成功率。',
        '本研究仅使用一个模型、三轮生成和已经用于开发的固定任务集，独立功能oracle覆盖有限，且不包含真实硬件验证。上述因素限制外部有效性；最终编译通过不等同于完整需求满足。',
        f'本轮实际物理调用{stats["physical_usage"]["llm_calls"]}次、总tokens为{stats["physical_usage"]["tokens"]}，API报告费用为{stats["physical_usage"]["reported_cost_usd"]:.4f}美元。共享响应仅计一次物理消耗；分组费用为上游分摊，不能相加当作真实总费用。']
    (root/'PAPER_EXPERIMENT_ZH.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')


def plot_results(root, stats):
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    fig, axes = plt.subplots(1,2,figsize=(13,5),layout='constrained')
    labels = ['Direct','Compile feedback','Full workflow','No contract repair','No model repair','Blind resampling','LLM backend']
    values = [stats['arms'][a]['compile']['rate']*100 for a in ARMS]
    axes[0].barh(labels,values,color=['#999999','#777777','#176b91','#8da8b8','#8da8b8','#8da8b8','#8da8b8'])
    axes[0].set(xlim=(0,105),xlabel='Target-board compilation (%)',title='All planned task-repetition records')
    axes[0].invert_yaxis()
    for i,v in enumerate(values): axes[0].text(v+.5,i,f'{v:.1f}',va='center',fontsize=9)
    contrasts=stats['contrasts']; ys=[]; vs=[]; lower=[]; upper=[]
    for i,c in enumerate(contrasts):
        q=c['compile']
        if q['ci95']:
            ys.append(i);vs.append(q['difference']*100)
            lower.append((q['difference']-q['ci95'][0])*100);upper.append((q['ci95'][1]-q['difference'])*100)
    axes[1].errorbar(vs,ys,xerr=[lower,upper],fmt='o',capsize=4,color='#176b91')
    axes[1].set(yticks=range(len(contrasts)),yticklabels=[labels[ARMS.index(c['right'])] for c in contrasts],
                xlabel='Full workflow minus comparator (percentage points)',title='Paired task-cluster bootstrap 95% CI')
    axes[1].axvline(0,color='gray',linestyle='--');axes[1].invert_yaxis()
    fig.savefig(root/'ablation_results.png',dpi=220);fig.savefig(root/'ablation_results.pdf');plt.close(fig)


def freeze(root, phase, cases, repetitions, workers, model):
    files = [ROOT/'app/main.py', *sorted((ROOT/'app/services').glob('*.py')),
             *sorted((ROOT/'tools/prompts').glob('*.md')), ROOT/'tools/sysml/SysMLBridge.java',
             *sorted((ROOT/'experiment/code').glob('*.py')), *sorted((ROOT/'experiment/development/oracles').glob('*')),
             ROOT/'experiment/formal/manifest.json',ROOT/'experiment/PAPER_ABLATION_PROTOCOL_ZH.md',
             ROOT/'tests/paper_ablation_unittest.py',ROOT/'tests/preflight_normalization_unittest.py']
    config = dict(phase=phase,cases=cases,repetitions=repetitions,workers=workers,model=model,
        temperature=.2,max_tokens=6000,arms=ARMS,call_limits=LIMITS,oracle_tasks=production.ORACLE_TASKS,
        public_interpretations=production.INTERPRETATIONS,claim_scope='development-set regression study; not held-out',
        hashes={str(p.relative_to(ROOT)):production.digest(p) for p in files if p.is_file()})
    config=json.loads(json.dumps(config))
    path=root/'execution.json'
    if path.exists():
        previous=read(path)
        if any(previous.get(k)!=v for k,v in config.items()):
            raise ValueError('Frozen configuration or source changed; original results retained. Use a new version.')
        for name,expected in previous['runtime']['binaries_sha256'].items():
            if expected is not None and (not (ROOT/name).is_file() or production.digest(ROOT/name)!=expected):
                raise ValueError('Frozen tool binary changed')
        return previous
    runtime=production.runtime_identity()
    binary='CodeGenerator2/CodeGenerator2/Exe/ILTranslator/ILTranslator.exe'
    runtime['binaries_sha256'][binary]=production.digest(ROOT/binary)
    runtime['binaries_sha256'][str(production.CLI.relative_to(ROOT))]=production.digest(production.CLI)
    config.update(runtime=runtime,created_utc=datetime.now(timezone.utc).isoformat())
    save(path, config)
    for name in config['hashes']:
        dest=root/'method_snapshot'/name;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(ROOT/name,dest)
    return config


def run_phase(root, phase, cases, repetitions, workers, model):
    global ACTIVE_ROOT
    ACTIVE_ROOT=root
    config=freeze(root,phase,cases,repetitions,workers,model)
    # The legacy runner is reused unchanged; replace only resumable I/O behavior.
    production.ResumableCalls=ExperimentCalls
    remaining=iter((c,r) for r in range(1,repetitions+1) for c in cases)
    stop=False
    with ThreadPoolExecutor(max_workers=workers) as pool:
        pending={}
        def submit():
            if not stop and (job:=next(remaining,None)) is not None:
                pending[pool.submit(evaluate,*job,root,model)]=job
        for _ in range(workers):submit()
        while pending:
            done,_=wait(pending,timeout=45,return_when=FIRST_COMPLETED)
            if not done:
                collect(root,config)
                continue
            for future in done:
                job=pending.pop(future)
                try:
                    rows=future.result()
                    stop |= any(r.get('infrastructure_error') for r in rows)
                except Exception as exc:
                    save(root/'harness_failure.json',dict(case=job[0]['id'],repeat=job[1],error_type=type(exc).__name__,
                        diagnostic=str(exc)[:500] if isinstance(exc,(ValueError,AssertionError)) else 'See local raw records; no provider text persisted'))
                    stop=True
                collect(root,config)
                submit()
    stats=summarize_run(root,config)
    if stats['complete']:
        plot_results(root,stats)
    save(root/'run_status.json',dict(complete=stats['complete'],stopped_on_environment_or_harness=stop,
        checked_utc=datetime.now(timezone.utc).isoformat()))
    return stats


def ordered_cases():
    cases=read(ROOT/'experiment/formal/manifest.json')['cases']
    assert len(cases)==len({c['id'] for c in cases})==296
    groups={d:sorted([c for c in cases if c['dataset']==d],key=lambda c:hashlib.sha256(('ablation-v1:'+c['id']).encode()).hexdigest()) for d in sorted({c['dataset'] for c in cases})}
    return [group[i] for i in range(max(map(len,groups.values()))) for group in groups.values() if i<len(group)]


def main():
    parser=argparse.ArgumentParser();parser.add_argument('--phase',choices=('pilot','formal','all'),default='all')
    parser.add_argument('--out',default='experiment/output/paper_ablation_v2_20261017')
    parser.add_argument('--workers',type=int,default=8);parser.add_argument('--model',default='openai/gpt-5.4-nano')
    parser.add_argument('--report-only',action='store_true');args=parser.parse_args()
    if args.workers<1:parser.error('workers must be positive')
    os.chdir(ROOT);os.environ['PATH']=str(production.CLI.parent)+os.pathsep+os.environ['PATH']
    os.environ['WINDOWS_ARDUINO_CLI_PATH']=str(production.CLI)
    root=(ROOT/args.out).resolve();root.relative_to((ROOT/'experiment/output').resolve())
    for status_path in (root/'service_retry_status.json',root/'pilot/service_retry_status.json'):
        if status_path.exists() and datetime.now(timezone.utc)<datetime.fromisoformat(read(status_path)['next_retry_not_before_utc']):
            print('Service retry cooldown active; no API request made.',flush=True)
            return
    original_read,original_write=Path.read_text,Path.write_text
    def storage_read(path,*a,**kw):
        return retry_permission(original_read,path,*a,**kw) if path.resolve().is_relative_to(root) else original_read(path,*a,**kw)
    def storage_write(path,*a,**kw):
        return retry_permission(original_write,path,*a,**kw) if path.resolve().is_relative_to(root) else original_write(path,*a,**kw)
    Path.read_text,Path.write_text=storage_read,storage_write
    if args.report_only:
        config=read(root/'execution.json');stats=summarize_run(root,config);plot_results(root,stats)
        print(json.dumps(dict(complete=stats['complete'],records=stats['records']),ensure_ascii=False));return
    cases=ordered_cases()
    pilot_ids=set(production.ORACLE_TASKS)
    for dataset in ('PCEval','EmbedBench'):
        pilot_ids.update(c['id'] for c in [c for c in cases if c['dataset']==dataset and c['id'] not in pilot_ids][:3])
    if args.phase in ('pilot','all'):
        stats=run_phase(root/'pilot','pilot',[c for c in cases if c['id'] in pilot_ids],1,args.workers,args.model)
        print(json.dumps(dict(phase='pilot',complete=stats['complete'],records=stats['records']),ensure_ascii=False),flush=True)
        if not stats['complete']:
            return
    if args.phase in ('formal','all'):
        # No success-rate threshold chooses whether to launch the formal run.
        stats=run_phase(root,'formal',cases,3,args.workers,args.model)
        print(json.dumps(dict(phase='formal',complete=stats['complete'],records=stats['records']),ensure_ascii=False),flush=True)


if __name__=='__main__':main()
