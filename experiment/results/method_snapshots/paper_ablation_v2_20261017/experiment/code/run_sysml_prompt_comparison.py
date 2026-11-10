"""Paired prompt evaluation with identical frozen interfaces and host oracles."""
import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import shutil
from concurrent.futures import ThreadPoolExecutor, as_completed
import time

from run_controlled_experiment import ROOT, digest, metrics
from run_current_comparison import ResumableCalls, validate_candidate, lower_verified, adapt_verified, compile_candidate, save
from run_contract_development import INTERPRETATIONS, ORACLE_TASKS
from app.services.sysml import sysml_bundle_prompt, generate_validated_bundle, SysMLValidationError
from app.services.interface_registry import register_contract
from app.services.interface_semantics import freeze_model_semantics
from app.services.hardware_adapter import validate_hardware_contract
from app.services.behavior_contract import validate_sysml_contract
from method_snapshot import runtime_identity

CASES = ['simple_001','simple_002','simple_003','simple_008','medium_004','medium_028',
         'simple_021','medium_029','simple_019','simple_014']
VARIANTS = ('original_prompt','constraints_and_example')


def configuration(folder, model, selected=None):
    manifest = {c['id']: c for c in json.loads((ROOT/'experiment/formal/manifest.json').read_text(encoding='utf-8'))['cases']}
    cases = []
    for name in selected or CASES:
        source = (ROOT/'experiment/output/interface_registry_full_20261014' if name=='simple_014'
                  else ROOT/'experiment/output/interface_repairs_full_20261013')/name/'repeat_1/contract.json'
        contract = register_contract(freeze_model_semantics(json.loads(source.read_text(encoding='utf-8'))))
        validate_hardware_contract(contract); validate_sysml_contract(contract)
        case = copy.deepcopy(manifest[name])
        case['requirement'] += ('\n'+INTERPRETATIONS[name]) if name in INTERPRETATIONS else ''
        case.update(contract=contract, contract_source=str(source.relative_to(ROOT)), contract_source_sha256=digest(source))
        cases.append(case)
    files = [Path(__file__),ROOT/'app/main.py',ROOT/'tools/examples/sysml_prompt_example.json',
             ROOT/'experiment/code/run_current_comparison.py',ROOT/'experiment/code/run_controlled_experiment.py',
             ROOT/'experiment/code/run_formal_experiment.py',ROOT/'experiment/code/run_contract_development.py',
             ROOT/'tools/sysml/SysMLBridge.java']
    files += list((ROOT/'app/services').glob('*.py'))+list((ROOT/'tools/prompts').glob('*.md'))+list((ROOT/'experiment/development/oracles').glob('*'))
    config = dict(model=model,temperature=.2,max_tokens=6000,repetitions=1,model_call_budget=3,
        variants=list(VARIANTS),cases=cases,hashes={str(p.relative_to(ROOT)):digest(p) for p in files if p.is_file()},
        host_oracle_tasks=[c['id'] for c in cases if c['id'] in ORACLE_TASKS],
        scope='Conditional model generation: identical frozen interfaces, compiler and host tests; not full-workflow success or an unseen-data experiment',
        runtime=runtime_identity())
    path=folder/'execution.json'
    if path.exists():
        if json.loads(path.read_text(encoding='utf-8'))!=config:
            raise ValueError('Configuration changed; use a new output directory')
    else:
        save(path,config)
        for name in config['hashes']:
            target=folder/'method_snapshot'/name;target.parent.mkdir(parents=True,exist_ok=True)
            shutil.copyfile(ROOT/name,target)
    return config


def evaluate(case, variant, folder, model):
    target=folder/case['id']/variant
    prior=target/'result.json'
    if prior.exists():
        row=json.loads(prior.read_text(encoding='utf-8'))
        if not row.get('infrastructure_error'):return row
    contract=case['contract']
    save(target/'contract.json',contract)
    calls=ResumableCalls(target/'calls',model)
    stage='model_generation'
    row=dict(case=case['id'],variant=variant,model_ok=False,initial_model_ok=False,
        compile_ok=False,functional_status='not_generated_or_compile_failed')
    started=time.monotonic()
    try:
        prompt=sysml_bundle_prompt(case['requirement'],contract=contract,
            complete_constraints=variant=='constraints_and_example')
        _,bundle,validation=generate_validated_bundle(calls,prompt,contract=contract,
            max_repairs=2,validator=validate_candidate)
        save(target/'model.json',bundle);save(target/'model_validation.json',validation)
        row.update(model_ok=True,initial_model_ok=validation['attempts'][0]['ok'],model_attempts=len(validation['attempts']))
        stage='ir_il_c'
        # Match the same signed production chain used by the full comparison.
        from app.services.sysml_compiler import validate_executable_sysml
        checked=validate_executable_sysml(bundle['system_sysml2'])
        if not checked['ok']:raise ValueError('Accepted model failed independent revalidation')
        generated=lower_verified(bundle['system_sysml2'],contract,checked['ir'],target/'lowered')
        stage='hardware_adapter'
        code=adapt_verified(generated,contract,target/'lowered')
        receipt=json.loads((target/'lowered/arduino_provenance.json').read_text(encoding='utf-8'))
        stage='board_compile'
        result,_=compile_candidate(case,target/'attempt_0',code,receipt)
        row.update(result)
        if not result['compile_ok']:row['failure_stage']=stage
    except SysMLValidationError as exc:
        save(target/'model_validation.json',dict(ok=False,attempts=exc.attempts))
        row.update(failure_stage=stage,model_attempts=len(exc.attempts),
                   final_issues=exc.attempts[-1].get('issues',[]))
    except Exception as exc:
        # Keep infrastructure evidence separate without persisting provider secrets.
        row.update(failure_stage=stage,error_type=type(exc).__name__)
        status=getattr(exc,'status_code',None)
        if status is not None:row['http_status']=status
        if isinstance(exc,(RuntimeError,OSError)) or status is not None and status>=500:
            row.update(infrastructure_error=type(exc).__name__,functional_status='unverified')
    row.update(metrics(calls.entries),seconds=round(time.monotonic()-started,3))
    save(prior,row)
    return row


def summarize(folder,config):
    rows=[json.loads(p.read_text(encoding='utf-8')) for p in folder.glob('*/*/result.json')]
    keys=[(r['case'],r['variant']) for r in rows]
    if len(keys)!=len(set(keys)):raise ValueError('Duplicate variant records')
    changed=[name for name,h in config['hashes'].items() if digest(ROOT/name)!=h]
    if changed:raise ValueError('Method changed during evaluation: '+', '.join(changed))
    case_ids=[c['id'] for c in config.get('cases',[dict(id=n) for n in CASES])]
    pairs={name:{r['variant']:r for r in rows if r['case']==name} for name in case_ids}
    comparable={name:group for name,group in pairs.items() if set(group)==set(VARIANTS)
                and not any(r.get('infrastructure_error') for r in group.values())}
    host=set(config['host_oracle_tasks'])&set(comparable)
    results={v:dict(initial_model_passes=sum(g[v]['initial_model_ok'] for g in comparable.values()),
        final_model_passes=sum(g[v]['model_ok'] for g in comparable.values()),
        compile_passes=sum(g[v]['compile_ok'] for g in comparable.values()),
        host_passes=sum(comparable[c][v]['functional_status']=='passed' for c in host),
        usage=metrics([e for row in rows if row['variant']==v
            for e in json.loads((folder/row['case']/v/'calls/calls.json').read_text(encoding='utf-8'))])) for v in VARIANTS}
    report=dict(scope=config['scope'],planned_tasks=len(case_ids),records=len(rows),
        generation_complete=len(rows)==len(case_ids)*len(VARIANTS),comparable_tasks=len(comparable),host_denominator=len(host),variants=results,
        infrastructure_records=sum(bool(r.get('infrastructure_error')) for r in rows),method_hashes_unchanged=True,
        recovered=[name for name,g in comparable.items() if g[VARIANTS[1]]['compile_ok'] and not g[VARIANTS[0]]['compile_ok']],
        regressed=[name for name,g in comparable.items() if g[VARIANTS[0]]['compile_ok'] and not g[VARIANTS[1]]['compile_ok']])
    save(folder/'results.json',rows);save(folder/'summary.json',report)
    lines=['# SysML 约束与完整示例提示对照','',
        '固定同一份接口，仅比较原提示与增加完整可执行约束、JSON/SysML示例的新提示。同模型、同参数、每组最多三次模型调用。',
        f"计划 {len(case_ids)} 题，共 {len(rows)} 条记录；完整非基础设施故障配对 {len(comparable)} 题。",
        '接口来自已保存的历史契约，ADC/PWM单位和真实时间输入保留；结果是接口给定条件下的模型质量，不代表296题端到端覆盖。题目与失败日志已被查看，每题一轮包含随机性。','',
        '| 提示 | 模型首稿 | 模型最终 | 目标编译 | 独立主机功能 |','|---|---:|---:|---:|---:|']
    for variant,data in results.items():
        lines.append(f"| {variant} | {data['initial_model_passes']}/{len(comparable)} | {data['final_model_passes']}/{len(comparable)} | {data['compile_passes']}/{len(comparable)} | {data['host_passes']}/{len(host)} |")
    lines+=['',f"基础设施故障 {report['infrastructure_records']} 条，未计为模型能力失败；全量记录完成：{report['generation_complete']}。",
        f"编译恢复：{report['recovered']}；退步：{report['regressed']}。",
        '没有新增Wokwi仿真或实板证据。完整候选、提示、诊断、来源凭据、编译日志和主机断言保存于逐题逐组目录。']
    (folder/'REPORT_ZH.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
    return report


def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('--out',default='experiment/output/sysml_prompt_comparison_20261014')
    parser.add_argument('--model',default='openai/gpt-5.4-nano')
    parser.add_argument('--workers',type=int,default=2)
    parser.add_argument('--report-only',action='store_true')
    parser.add_argument('--case',action='append',choices=CASES)
    args=parser.parse_args()
    folder=(ROOT/args.out).resolve();folder.relative_to((ROOT/'experiment/output').resolve());folder.mkdir(parents=True,exist_ok=True)
    os.environ['PATH']=str(ROOT/'.tools/arduino')+os.pathsep+os.environ['PATH']
    os.environ['WINDOWS_ARDUINO_CLI_PATH']=str(ROOT/'.tools/arduino/arduino-cli.exe')
    config=configuration(folder,args.model,args.case)
    if not args.report_only:
        def pair(case):
            # Alternate order to limit a fixed prompt-order/elapsed-time bias.
            order=VARIANTS if CASES.index(case['id'])%2==0 else VARIANTS[::-1]
            for variant in order:
                row=evaluate(case,variant,folder,args.model)
                print(case['id'],variant,row['model_ok'],row['compile_ok'],row['functional_status'],flush=True)
                if row.get('infrastructure_error'):return False
            return True
        with ThreadPoolExecutor(max_workers=args.workers) as pool:
            remaining=iter(config['cases']);futures={}
            def submit():
                case=next(remaining,None)
                if case:futures[pool.submit(pair,case)]=case['id']
            for _ in range(args.workers):submit()
            failures=0
            while futures:
                for future in as_completed(list(futures)):
                    futures.pop(future)
                    failures+=int(not future.result())
                    summarize(folder,config)
                    if failures<2:submit()
                    break
    print(json.dumps(summarize(folder,config),ensure_ascii=False),flush=True)


if __name__=='__main__':main()
