"""Compare the current production pipeline with paired Arduino baselines.

All frozen tasks remain in the denominator. Hidden tests never enter prompts.
The optional frontend module planner and physical upload are outside this study.
"""
import argparse
import copy
import hashlib
import json
import os
import shutil
import threading
import time
import tempfile
import subprocess
from concurrent.futures import ThreadPoolExecutor, wait, FIRST_COMPLETED
from pathlib import Path

from run_controlled_experiment import ROOT, Calls, DIRECT_PROMPT, compile_code, extract, metrics, digest
from run_formal_experiment import ReplayCalls, BackendFailure
from run_contract_development import INTERPRETATIONS, ORACLE_TASKS, behavior, lower_verified, adapt_verified
from app import main as api
from app.services.sysml import SysMLValidationError, SysMLToolError, generate_validated_bundle, sysml_bundle_prompt
from app.services.sysml_compiler import validate_executable_sysml
from app.services.interface_planner import plan_interface_contract, ContractPlanningError
from app.services.llm_json import check_completion
from app.services.artifact_provenance import text_digest
from experiment_metrics import first_response_entries, summarize, paired
from method_snapshot import runtime_identity

ARMS = ('direct', 'direct_feedback', 'workflow')
BUILD = threading.Semaphore(1)
JAVA = threading.Semaphore(4)
TRANSLATOR = threading.Semaphore(2)
CLI = ROOT / '.tools/arduino/arduino-cli.exe'


def save(path, value):
    """Windows readers can briefly prevent replacement; preserve the prior record."""
    path.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.NamedTemporaryFile(mode='w', encoding='utf-8', dir=path.parent,
                                     prefix=path.name+'.', suffix='.tmp', delete=False) as handle:
        temporary = Path(handle.name)
        json.dump(value, handle, ensure_ascii=False, indent=2)
    try:
        for attempt in range(30):
            try:
                temporary.replace(path)
                return
            except PermissionError:
                if attempt == 29:
                    raise
                time.sleep(.1)
    finally:
        temporary.unlink(missing_ok=True)


class ResumableCalls(ReplayCalls):
    """Replay exact responses; retry transport failures without spending model budget."""
    def __call__(self, prompt):
        cursor = self.cursor
        while cursor < len(self.entries) and 'error_type' in self.entries[cursor]:
            cursor += 1
        if cursor < len(self.entries):
            old = self.entries[cursor]['prompt']
            marker = '\nRepair compiler diagnostics without dropping requirements:\n'
            if old != prompt and marker in old and marker in prompt and old.rsplit(marker, 1)[0] == prompt.rsplit(marker, 1)[0]:
                save(self.folder / f'replay_diagnostics_{cursor}.json', {'original': old.rsplit(marker, 1)[1], 'current': prompt.rsplit(marker, 1)[1]})
                prompt = old
        for attempt in range(3):
            try:
                return super().__call__(prompt)
            except RuntimeError as exc:
                if attempt == 2 or not any(name in str(exc) for name in ('APIConnectionError', 'APITimeoutError', 'InternalServerError', 'RateLimitError')):
                    raise
                time.sleep(2 * (attempt + 1))


def validate_candidate(source):
    with JAVA:
        for attempt in range(2):
            try:
                return validate_executable_sysml(source)
            except SysMLToolError:
                if attempt:
                    raise


def compile_candidate(case, folder, code, receipt=None):
    if receipt is not None:
        api.AUTHORITY.verify(receipt, 'arduino', {'arduino_sha256': text_digest(code)})
    with BUILD:
        result, log = compile_code(folder, code, CLI, case['fqbn'])
    artifact = folder / 'sketch/sketch.ino'
    if artifact.read_text(encoding='utf-8') != code:
        raise RuntimeError('Compiled sketch differs from supplied candidate')
    result.update(artifact=str(artifact.relative_to(ROOT)), artifact_sha256=digest(artifact),
                  functional_status='not_generated_or_compile_failed')
    if result['compile_ok']:
        if case['id'] in ORACLE_TASKS:
            try:
                result.update(behavior(folder, ORACLE_TASKS[case['id']], case['fqbn']))
            except (subprocess.TimeoutExpired, OSError) as exc:
                result.update(functional_status='oracle_infrastructure_unverified', oracle_error_type=type(exc).__name__)
            result['functional_scope'] = 'independent_host_hardware_command_trace'
        else:
            result['functional_status'] = 'no_validated_oracle'
    return result, log


def evaluate(case, repeat, root, model, feedback_calls=6):
    case = copy.deepcopy(case)
    case['requirement'] += ('\n' + INTERPRETATIONS[case['id']]) if case['id'] in INTERPRETATIONS else ''
    from app.services.configuration_policy import resolve_configuration
    resolution = resolve_configuration(case['requirement'], {}, board=case['fqbn'])
    case['requirement'] = resolution['effective_requirement']
    folder = root / case['id'] / f'repeat_{repeat}'
    save(folder / 'configuration_resolution.json', resolution)
    path = folder / 'results.json'
    if path.exists():
        old = json.loads(path.read_text(encoding='utf-8'))
        if {r['arm'] for r in old} == set(ARMS) and not any(r.get('infrastructure_error') for r in old):
            return old
    rows = []
    started = time.monotonic()

    def record(arm, entries, **outcome):
        row = dict(case=case['id'], dataset=case['dataset'], stratum=case['stratum'], split=case['split'],
                   repeat=repeat, arm=arm, compile_ok=False, compile_status='not_generated',
                   functional_status='not_generated_or_compile_failed', **metrics(entries))
        row.update(outcome)
        rows.append(row)
        save(path, rows)
        return row

    direct = ResumableCalls(folder / 'direct_calls', model)
    try:
        _, raw = direct(DIRECT_PROMPT + case['requirement'])
        check_completion(raw)
        first = copy.deepcopy(first_response_entries(direct.entries))
        code = extract(raw)
        result, log = compile_candidate(case, folder / 'direct/attempt_0', code)
        record('direct', first, **result)
        history = [result]
        for attempt in range(1, feedback_calls):
            if result['compile_ok'] or result['compile_status'] == 'missing_dependency':
                break
            _, raw = direct(DIRECT_PROMPT + case['requirement'] + '\nPrevious sketch:\n' + code +
                            '\nRepair compiler diagnostics without dropping requirements:\n' + log[-10000:])
            check_completion(raw)
            code = extract(raw)
            result, log = compile_candidate(case, folder / f'direct_feedback/attempt_{attempt}', code)
            history.append(result)
        save(folder / 'direct_compile_attempts.json', history)
        record('direct_feedback', direct.entries, compile_repairs=len(history)-1, **result)
    except Exception as exc:
        infrastructure = isinstance(exc, (RuntimeError, OSError))
        for arm in ('direct', 'direct_feedback'):
            if arm not in {r['arm'] for r in rows}:
                record(arm, direct.entries, failure_stage='generation_or_compile', error_type=type(exc).__name__,
                       **({'infrastructure_error':str(exc)[:500], 'functional_status':'unverified'} if infrastructure else {}))

    calls = ResumableCalls(folder / 'workflow_calls', model)
    stage = 'interface_contract'
    evidence = {}
    try:
        contract, planning = plan_interface_contract(calls, case['requirement'], {}, target_board=case['fqbn'], max_repairs=2,
                                                    configuration_resolution=resolution)
        save(folder / 'contract.json', contract)
        save(folder / 'contract_planning.json', planning)
        if 'interface_registry' in contract:
            save(folder / 'interface_registry.json', contract['interface_registry'])
            save(folder / 'serial_registry.json', {'version': 1,
                'serial_channels': contract['interface_registry']['serial_channels'],
                'serial_inputs': contract['interface_registry'].get('serial_inputs', []),
                'serial_text_outputs': contract['interface_registry'].get('serial_text_outputs', [])})
        evidence.update(contract_ok=True, contract_sha256=planning['contract_sha256'],
                        registered_relationships=planning.get('registered_relationships'))
        stage = 'sysml_generation'
        accepted = {}
        def validator(source):
            checked = validate_candidate(source)
            if checked['ok']:
                accepted.update(source=source, ir=checked['ir'])
            return checked
        _, bundle, validation = generate_validated_bundle(calls, sysml_bundle_prompt(case['requirement'], contract=contract),
            max_repairs=2, validator=validator, contract=contract)
        save(folder / 'model.json', bundle)
        save(folder / 'model_validation.json', validation)
        evidence.update(model_ok=True, model_initial_ok=validation['attempts'][0]['ok'], model_attempts=len(validation['attempts']))
        source = bundle['system_sysml2']
        # The production API independently compiles and signs the accepted source.
        stage = 'ir_to_code'
        if accepted.get('source') != source:
            raise ValueError('Accepted source differs from the executable validation candidate')
        with TRANSLATOR, JAVA:
            generated = lower_verified(source, contract, accepted['ir'], folder / 'lowered')
        evidence.update(ir_ok=True, backend_ok=True, provenance_status=generated.provenance_status)
        stage = 'hardware_adapter'
        code = adapt_verified(generated, contract, folder / 'lowered')
        receipt = json.loads((folder / 'lowered/arduino_provenance.json').read_text(encoding='utf-8'))
        stage = 'arduino_compile'
        result, _ = compile_candidate(case, folder / 'workflow/attempt_0', code, receipt)
        record('workflow', calls.entries, **evidence, **result)
    except (ContractPlanningError, SysMLValidationError) as exc:
        category = {'blocking_category': exc.blocking_category} if isinstance(exc, ContractPlanningError) else {}
        save(folder / ('contract_planning.json' if stage == 'interface_contract' else 'model_validation.json'),
             {'ok':False,'attempts':exc.attempts, **category})
        last = exc.attempts[-1] if exc.attempts else {}
        record('workflow', calls.entries, **evidence, failure_stage=last.get('failure_stage') or stage,
               error=str(exc), model_ok=False if stage != 'interface_contract' else None, **category)
    except BackendFailure as exc:
        record('workflow', calls.entries, **evidence, failure_stage=stage, error=str(exc), backend_ok=False)
    except api.HTTPException as exc:
        extra = {'error':str(exc.detail)[:3000]} if exc.status_code in (400, 422) else {'infrastructure_error':type(exc).__name__, 'functional_status':'unverified'}
        record('workflow', calls.entries, **evidence, failure_stage=stage, **extra)
    except ValueError as exc:
        record('workflow', calls.entries, **evidence, failure_stage=stage, error=str(exc)[:3000])
    except Exception as exc:
        record('workflow', calls.entries, **evidence, failure_stage=stage, infrastructure_error=type(exc).__name__, functional_status='unverified')
    save(folder / 'timing.json', {'job_seconds':round(time.monotonic()-started, 3), 'scope':'Shared three-arm job; not per-arm wall latency'})
    return rows


def collect(root):
    rows = [r for p in root.glob('*/repeat_*/results.json') for r in json.loads(p.read_text(encoding='utf-8'))]
    save(root / 'results.json', rows)
    save(root / 'progress.json', {'records':len(rows), 'completed_jobs':len(rows)//3,
        'infrastructure_records':sum(bool(r.get('infrastructure_error')) for r in rows),
        'compile_passes':{a:sum(r['compile_ok'] for r in rows if r['arm']==a) for a in ARMS}})
    return rows


def report(root, config):
    rows = collect(root)
    expected = [(c['id'], r, a) for c in config['cases'] for r in range(1,config['repetitions']+1) for a in ARMS]
    reports = [json.loads(p.read_text(encoding='utf-8')) for p in root.glob('*/repeat_*/model_validation.json')]
    def group(values, keys, model_reports=()):
        result = summarize(values, model_reports, expected_keys=keys)
        result['max_method_calls_excluding_transport_retries'] = config['call_limits']
        functional_rows = [r for r in values if r['case'] in ORACLE_TASKS]
        result['contrasts'] = [dict(left='workflow',right=a,compile=paired(values,'workflow',a,'compile'),
            functional_observed=paired(functional_rows,'workflow',a,'functional')) for a in ('direct','direct_feedback')]
        return result
    stats = group(rows, expected, reports)
    stats['partitions'] = {}
    for split in ('development','test'):
        ids = {c['id'] for c in config['cases'] if c['split']==split}
        stats['partitions'][split] = group([r for r in rows if r['case'] in ids], [k for k in expected if k[0] in ids])
    stats['by_dataset'] = {d:group([r for r in rows if r['dataset']==d], [k for k in expected if k[0] in {c['id'] for c in config['cases'] if c['dataset']==d}]) for d in sorted({c['dataset'] for c in config['cases']})}
    stats['by_stratum'] = {d:group([r for r in rows if r['stratum']==d], [k for k in expected if k[0] in {c['id'] for c in config['cases'] if c['stratum']==d}]) for d in sorted({c['stratum'] for c in config['cases']})}
    entries = [e for p in root.glob('*/repeat_*/*_calls/calls.json') for e in json.loads(p.read_text(encoding='utf-8'))]
    stats['physical_usage'] = metrics(entries)
    stats['physical_usage'].update(responses=sum('response' in e for e in entries),transport_failures=sum('error_type' in e for e in entries),
                                   calls_with_reported_cost=sum((e.get('usage') or {}).get('cost') is not None for e in entries))
    stats['source_hashes_unchanged'] = all(digest(ROOT/p)==h for p,h in config['hashes'].items())
    stats['generation_complete'] = stats['missing_records']==0 and not any(r.get('infrastructure_error') for r in rows)
    save(root / 'summary.json', stats)
    lines = ['# 当前系统全量对照实验', '', f"模型：{config['model']}；任务 {len(config['cases'])}；每题重复 {config['repetitions']} 次。",
        '完整流程调用生产模型编译、来源签发和确定性硬件适配；不含可选前端模块规划与物理烧录。',
        '直接反馈组与完整流程最多各 6 次方法调用，直接首稿 1 次；共享首稿不重复计入物理费用。隐藏测试不用于修复。',
        '', '| 方法 | 编译通过/全部任务 | 功能已证明通过/全部任务 | 已编译但功能未验证 |', '|---|---:|---:|---:|']
    for a in ARMS:
        s=stats['arms'][a];c=s['compile'];f=s['functional']
        lines.append(f"| {a} | {c['passes']}/{c['denominator']} | {f['all_tasks']['passes']}/{f['all_tasks']['denominator']} | {f['unresolved']} |")
    lines += ['', '功能已证明比例在测试未覆盖时仅为下界；不能将无 oracle 或服务额度不足记为功能失败。',
              f"生成记录完整：{stats['generation_complete']}；缺失记录：{stats['missing_records']}；源码散列未变：{stats['source_hashes_unchanged']}。",
              '', '## 失败阶段', '', '| 方法 | 阶段 | 数量 |', '|---|---|---:|']
    for a in ARMS:
        lines += [f'| {a} | {k} | {v} |' for k,v in stats['arms'][a]['failures'].items()]
    lines += ['', '完整数据、分区/来源统计、调用成本和配对区间见 summary.json；候选、提示、诊断和编译日志保存在逐题目录。']
    (root/'REPORT_ZH.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
    return stats


def cli():
    parser=argparse.ArgumentParser()
    parser.add_argument('--out',required=True)
    parser.add_argument('--model',default='openai/gpt-5.4-nano')
    parser.add_argument('--repetitions',type=int,default=5)
    parser.add_argument('--workers',type=int,default=16)
    parser.add_argument('--case',action='append')
    parser.add_argument('--report-only',action='store_true')
    args=parser.parse_args()
    if args.repetitions<1 or args.workers<1:parser.error('repetitions and workers must be positive')
    os.chdir(ROOT)
    os.environ['PATH']=str(CLI.parent)+os.pathsep+os.environ['PATH']
    root=(ROOT/args.out).resolve();root.relative_to((ROOT/'experiment/output').resolve());root.mkdir(parents=True,exist_ok=True)
    cases=json.loads((ROOT/'experiment/formal/manifest.json').read_text(encoding='utf-8'))['cases']
    if args.case:
        cases=[c for c in cases if c['id'] in args.case]
        if {c['id'] for c in cases}!=set(args.case):parser.error('unknown case ID')
    files=[Path(__file__),ROOT/'experiment/code/run_controlled_experiment.py',ROOT/'experiment/code/run_formal_experiment.py',
           ROOT/'experiment/code/run_contract_development.py',ROOT/'experiment/code/experiment_metrics.py',ROOT/'experiment/code/method_snapshot.py',ROOT/'tools/sysml/SysMLBridge.java',ROOT/'app/main.py']
    files+=list((ROOT/'app/services').glob('*.py'))+list((ROOT/'tools/prompts').glob('*.md'))+list((ROOT/'experiment/development/oracles').glob('*'))
    hashes={str(p.relative_to(ROOT)):digest(p) for p in files if p.is_file()}
    config=dict(cases=cases,repetitions=args.repetitions,model=args.model,temperature=.2,max_tokens=6000,arms=list(ARMS),
        primary_metric='target_board_compilation',supplementary_metric='existing_independent_host_traces',
        concurrency=dict(job_workers=args.workers,arduino_compile_workers=1,java_workers=4,translator_workers=2),
        call_limits=dict(direct=1,direct_feedback=6,workflow=6),hashes=hashes,manifest_sha256=digest(ROOT/'experiment/formal/manifest.json'),
        public_interpretations={k:v for k,v in INTERPRETATIONS.items() if k in {c['id'] for c in cases}},
        pipeline_scope='Current production contract/SysML/IR/IL/C/provenance/Arduino pipeline; optional UI planner and hardware upload excluded',
        oracle_scope='Independent host traces for ten previously defined development tasks; additional PCEval simulations require audited references',runtime=runtime_identity())
    config['runtime']['binaries_sha256']['CodeGenerator2/CodeGenerator2/Exe/ILTranslator/ILTranslator.exe']=digest(ROOT/'CodeGenerator2/CodeGenerator2/Exe/ILTranslator/ILTranslator.exe')
    path=root/'execution.json'
    if path.exists():
        if json.loads(path.read_text(encoding='utf-8'))!=config:raise SystemExit('Frozen configuration changed; use a new run directory')
    else:
        save(path,config)
        for name in hashes:
            target=root/'method_snapshot'/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(ROOT/name,target)
    if args.report_only:
        report(root,config);return
    jobs=[(c,r) for r in range(1,args.repetitions+1) for c in cases]
    started=time.monotonic();errors=0;completed=0;remaining=iter(jobs)
    print(f'Starting {len(jobs)} jobs / {len(jobs)*3} records',flush=True)
    with ThreadPoolExecutor(max_workers=args.workers) as pool:
        futures={}
        def submit():
            job=next(remaining,None)
            if job:
                c,r=job;futures[pool.submit(evaluate,c,r,root,args.model)]=(c['id'],r)
        for _ in range(args.workers):submit()
        while futures:
            done,_=wait(futures,timeout=30,return_when=FIRST_COMPLETED)
            if not done:
                collect(root);print(f'Heartbeat: {completed}/{len(jobs)} jobs',flush=True)
            for future in done:
                case_id,repeat=futures.pop(future);rows=future.result();completed+=1
                errors+=int(any(r.get('infrastructure_error') for r in rows))
                collect(root)
                print('job',completed,'/',len(jobs),case_id,repeat,[(r['arm'],r['compile_status'],r['functional_status']) for r in rows],flush=True)
                if errors<3:submit()
    stats=report(root,config)
    save(root/'run_status.json',dict(generation_complete=stats['generation_complete'],completed_jobs=completed,
        infrastructure_jobs=errors,wall_seconds=time.monotonic()-started))
    if errors:raise SystemExit(3)


if __name__=='__main__':cli()
