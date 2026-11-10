"""Failure-only, three additional attempts over saved 296-task experiments.

The first artifact is retained. Functional metrics cover the fixed 16 host tasks.
Historical cumulative rows are recovery references, not iid pass@k samples.
"""
import argparse
from collections import Counter
from datetime import datetime, timedelta, timezone
import hashlib
import json
import os
from pathlib import Path
import time

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT/'experiment/output/retry3_failed_20261023'
BASE = ROOT/'experiment/output/runtime_validation_20261023'
ANCHOR = ROOT/'experiment/output/paper_ablation_v2_20261017'
GROUPS = ('workflow_267', 'historical_direct', 'direct_gpt61', 'workflow_280',
          'ablation_direct', 'ablation_direct_feedback', 'ablation_workflow',
          'ablation_no_contract_feedback', 'ablation_no_model_feedback',
          'ablation_blind_resampling', 'ablation_llm_backend')
HISTORICAL = {'workflow_267', 'workflow_280'}
HOST_UNKNOWN = {'oracle_build_unverified', 'oracle_timeout_unverified',
                'oracle_runtime_unverified', 'oracle_infrastructure_unverified'}


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    temp = path.with_suffix('.tmp')
    temp.write_text(json.dumps(value, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    for n in range(30):
        try:
            temp.replace(path)
            return
        except PermissionError:
            if n == 29:
                raise
            time.sleep(.1)


def utc():
    return datetime.now(timezone.utc)


def failed(row, host):
    return not row['compile_ok'] or (host and row['functional_status'] == 'failed')


def outcome(attempts, field, k=4):
    if any(r.get('infrastructure_error') for r in attempts):
        attempts = [r for r in attempts if not r.get('infrastructure_error')]
    if any(r[field] is True for r in attempts):
        return 'passed'
    if any(r[field] is None for r in attempts):
        return 'unverified'
    return 'failed' if len(attempts) >= k else 'pending'


def freeze():
    OUT.mkdir(parents=True, exist_ok=True)
    anchor = read(ANCHOR/'execution.json')
    for path, expected in anchor['hashes'].items():
        assert (ROOT/path).is_file() and sha(ROOT/path) == expected, 'Frozen method changed: '+path
    for path, expected in anchor['runtime']['binaries_sha256'].items():
        if expected is not None:
            assert sha(ROOT/path) == expected, 'Frozen binary changed: '+path
    plan = read(BASE/'execution.json')
    runtime = {(r['group'],r['case']):r for r in read(BASE/'results.json')}
    host_ids = sorted({r['case'] for r in runtime.values() if r['test_scope']=='independent_host_behavior'})
    assert len(host_ids) == 16
    originals = []
    for entry in plan['rows']:
        r = entry['original']
        item = dict(r, group=entry['group'], attempt=1, origin='original_saved_artifact')
        status = runtime[item['group'],r['case']]['runtime_status']
        item['functional_status'] = status
        item['compile_pass'] = bool(r['compile_ok'])
        item['functional_pass'] = (status == 'passed') if r['case'] in host_ids and status not in HOST_UNKNOWN else None
        if not r['compile_ok'] and r['case'] in host_ids:
            item['functional_pass'] = False
        # A demonstrated observation gap must not become a code-failure label.
        if item['group']=='direct_gpt61' and item['case']=='medium_022':
            item.update(functional_pass=None, functional_status='oracle_observation_unverified',
                        observation_note='assistant_functional_review_20261023/runtime_evidence_notes/medium_022_direct_gpt61.json')
        originals.append(item)
    assert len(originals) == len({(r['group'],r['case']) for r in originals}) == 3256
    assert {r['group'] for r in originals} == set(GROUPS)
    assert Counter(r['dataset'] for r in originals) == {'internal':1320,'PCEval':550,'EmbedBench':1386}
    sources = [BASE/'execution.json', BASE/'results.json', ANCHOR/'execution.json',
               ANCHOR/'results.json', ROOT/'experiment/code/run_pass3_followup.py',
               ROOT/'experiment/code/rerun_workflow_gpt61.py']
    config = dict(k=4, additional_rounds=3, cases=anchor['cases'], groups=GROUPS, original_rows=originals,
        host_cases=host_ids, retained_datasets={'internal':120,'PCEval':50,'EmbedBench':126},
        source_hashes={str(p.relative_to(ROOT)):sha(p) for p in sources},
        method_hashes=anchor['hashes'], binaries=anchor['runtime']['binaries_sha256'],
        model_nano=anchor['model'], model_sol='openai/gpt-6.1-sol',
        selection='Only original compile failures or fixed-host-test failures; stop each metric after success; no oracle feedback in prompts.',
        attempt_unit='One complete method invocation, including its original internal structural/compiler repair budget.',
        historical_scope='workflow_267/280 contain cumulative development artifacts. Their extra attempts use the frozen complete workflow (nano/sol respectively); report recovery after up to 3 ADDITIONAL attempts, not iid pass@3.',
        functional_scope='Fixed 16 host tasks only; other 280, including PCEval, unverified. No Wokwi requests.',
        sampling_scope='New ablation attempts run each arm with its specified budget; initial candidates are not shared across arms. Original round was paired.',
        old_repeat_reuse='No old ablation repeats count as new reruns. Only API responses already obtained for this newly authorized follow-up may be replayed after the scope correction; preserve their provenance. Infrastructure errors do not count as failed attempts.')
    config = json.loads(json.dumps(config))
    path = OUT/'execution.json'
    if path.exists():
        assert read(path)==config, 'Follow-up snapshot changed'
    else:
        save(path,config)
    return config


def retry_gate():
    paths = [OUT/'service_retry_status.json',
             ROOT/'experiment/output/code_review_all_20261023/service_retry_status.json',
             ANCHOR/'service_retry_status.json']
    times = [datetime.fromisoformat(read(p)['next_retry_not_before_utc']) for p in paths if p.exists()]
    return max(times) if times else utc()


def block_service(kind):
    now = utc()
    save(OUT/'service_retry_status.json',dict(error_type=kind,failed_utc=now.isoformat(),
        next_retry_not_before_utc=(now+timedelta(hours=6)).isoformat()))


def report(config):
    rows = []
    groups = {}
    for original in config['original_rows']:
        attempts = [original]
        for n in (2,3,4):
            path=OUT/'attempts'/original['group']/original['case']/f'attempt_{n}'/'result.json'
            if path.exists():
                attempts.append(read(path))
        host=original['case'] in config['host_cases']
        rows.append(dict(group=original['group'],case=original['case'],dataset=original['dataset'],
            host_tested=host, compile_first=original['compile_ok'],
            functional_first=original['functional_pass'], compile_after_3_reruns=outcome(attempts,'compile_pass'),
            functional_after_3_reruns=outcome(attempts,'functional_pass') if host else 'unverified',
            attempts=[{k:r.get(k) for k in ('attempt','origin','compile_pass','functional_pass','infrastructure_error','artifact','artifact_sha256')} for r in attempts]))
    for group in GROUPS:
        sub=[r for r in rows if r['group']==group]
        comp=Counter(r['compile_after_3_reruns'] for r in sub)
        host=[r for r in sub if r['host_tested']]
        func=Counter(r['functional_after_3_reruns'] for r in host)
        groups[group]=dict(planned=296,compile_first=sum(r['compile_first'] for r in sub),
            compilation_after_3_reruns_counts=dict(comp), compilation_after_3_reruns_rate=comp['passed']/296 if not comp['pending'] else None,
            host_denominator=16,functional_first=sum(r['functional_first'] is True for r in host),
            functional_after_3_reruns_counts=dict(func),functional_after_3_reruns_rate=func['passed']/16 if not func['pending'] and not func['unverified'] else None,
            no_functional_oracle=280,historical_recovery_reference=group in HISTORICAL)
    complete=all(not s['compilation_after_3_reruns_counts'].get('pending') and not s['functional_after_3_reruns_counts'].get('pending') for s in groups.values())
    audit=None
    if complete:
        changed=[path for path,h in {**config['method_hashes'],**config['source_hashes']}.items() if not (ROOT/path).exists() or sha(ROOT/path)!=h]
        changed += [path for path,h in config['binaries'].items() if h is not None and sha(ROOT/path)!=h]
        all_artifacts=list(config['original_rows'])+[read(p) for p in (OUT/'attempts').rglob('result.json')]
        bad=[r.get('artifact') for r in all_artifacts if r.get('artifact') and (not (ROOT/r['artifact']).exists() or sha(ROOT/r['artifact'])!=r['artifact_sha256'])]
        audit=dict(complete=not changed and not bad and len(rows)==len({(r['group'],r['case']) for r in rows})==3256,
                   source_or_binary_changes=changed,artifact_mismatches=bad,records=len(rows),
                   planned_groups=len(groups),cases_per_group=296)
        save(OUT/'completion_audit.json',audit)
        complete=audit['complete']
    # Completion of scheduling does not make unresolved tests functionally validated.
    entries=[e for p in (OUT/'attempts').rglob('calls.json') for e in read(p)]
    responses=[e for e in entries if 'response' in e and not e.get('error_type')]
    summary=dict(complete=complete,full_functional_evaluation_complete=False,k=4,additional_rounds=3,groups=groups,
        updated_utc=utc().isoformat(),planned_rows=3256,new_requests=len(entries),
        new_responses=len(responses),new_tokens=sum((e.get('usage') or {}).get('total_tokens',0) for e in responses),
        new_reported_cost_usd=sum((e.get('usage') or {}).get('cost',0) or 0 for e in responses),
        cost_missing_responses=sum((e.get('usage') or {}).get('cost') is None for e in responses),
        functional_scope=config['functional_scope'],completion_audit=audit)
    save(OUT/'results.json',rows);save(OUT/'summary.json',summary)
    lines=['# 完整296题：原失败项最多额外重跑3轮','',config['selection'],config['attempt_unit'],
        config['historical_scope'],config['functional_scope'],config['sampling_scope'],'',
        '保留全部PCEval 50题、EmbedBench 126题及内部120题；原始结果不变。',
        '这里是原始1次加额外最多3次，总计最多4次。任一次成功即停止；累计恢复率不能冒称3个独立首稿的标准Pass@3。固定16题有功能断言，其余280题功能未知。',
        '原始审查发现medium_022直接生成的TM1637观察缺口，此项保持未知，未修改旧运行记录。','',
        '| 组别 | 原编译/296 | 额外3轮后累计编译成功/296 | 原主机通过/16 | 额外3轮后累计功能通过/16 |',
        '|---|---:|---:|---:|---:|']
    for g,s in groups.items():
        c=s['compilation_after_3_reruns_counts'];f=s['functional_after_3_reruns_counts']
        lines.append(f"| {g} | {s['compile_first']} | {c.get('passed',0)}，待补{c.get('pending',0)} | {s['functional_first']} | {f.get('passed',0)}，待补{f.get('pending',0)}，未知{f.get('unverified',0)} |")
    (OUT/'REPORT_ZH.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
    return summary


def evaluate(config, original, n):
    # Imported only after the service retry gate, never by --prepare-only.
    import copy
    import run_paper_ablation as ab
    import rerun_workflow_gpt61 as sol
    from app.services.configuration_policy import resolve_configuration
    p=ab.production
    folder=OUT/'attempts'/original['group']/original['case']/f'attempt_{n}'
    group=original['group'];arm=group.removeprefix('ablation_')
    is_sol=group in ('direct_gpt61','workflow_280')
    model=config['model_sol'] if is_sol else config['model_nano']
    case=copy.deepcopy(next(c for c in config['cases'] if c['id']==original['case']))
    if case['id'] in p.INTERPRETATIONS:case['requirement']+='\n'+p.INTERPRETATIONS[case['id']]
    resolution=resolve_configuration(case['requirement'],{},board=case['fqbn'])
    case['requirement']=resolution['effective_requirement']
    save(folder/'configuration_resolution.json',resolution)
    direct=group in ('historical_direct','direct_gpt61','ablation_direct','ablation_direct_feedback')
    limit=1 if direct and arm!='direct_feedback' else ab.LIMITS.get(arm,6)

    class BudgetCalls(sol.GPTCalls if is_sol else ab.ExperimentCalls):
        def __call__(self,prompt):
            replay=self.cursor<len(self.entries)
            if not replay and sum('response' in e for e in self.entries)>=limit:
                raise ValueError('Frozen method response budget exhausted')
            return super().__call__(prompt)

    calls=BudgetCalls(folder/'calls') if is_sol else BudgetCalls(folder/'calls',model)
    stage='direct_generation' if direct else 'interface_contract'
    result=dict(case=case['id'],dataset=case['dataset'],group=group,attempt=n,
        compile_ok=False,compile_status='not_generated',functional_status='not_generated_or_compile_failed',
        origin='new_frozen_method_attempt',model=model)
    try:
        if direct:
            prompt=p.DIRECT_PROMPT+case['requirement'];_,raw=calls(prompt)
            p.check_completion(raw);code=p.extract(raw)
            for i in range(limit):
                stage='arduino_compile'
                checked,log=p.compile_candidate(case,folder/f'code_{i}',code)
                if checked['compile_ok'] or checked['compile_status']=='missing_dependency' or i+1==limit:break
                _,raw=calls(prompt+'\nPrevious sketch:\n'+code+'\nRepair compiler diagnostics without dropping requirements:\n'+log[-10000:])
                p.check_completion(raw);code=p.extract(raw)
            result.update(checked)
        else:
            # Blind resampling hides diagnostics from the model, not from stored evidence.
            class StageCalls:
                def __init__(self):self.initial=None
                def __call__(self,prompt):
                    if self.initial is None:self.initial=prompt
                    return calls(self.initial if arm=='blind_resampling' else prompt)
            contract,planning=p.plan_interface_contract(StageCalls(),case['requirement'],{},
                target_board=case['fqbn'],max_repairs=0 if arm=='no_contract_feedback' else 2,
                configuration_resolution=resolution)
            save(folder/'contract.json',contract);save(folder/'contract_planning.json',planning)
            accepted={};stage='sysml_generation'
            def validator(source):
                checked=p.validate_candidate(source)
                if checked['ok']:accepted.update(source=source,ir=checked['ir'])
                return checked
            _,bundle,validation=p.generate_validated_bundle(StageCalls(),p.sysml_bundle_prompt(case['requirement'],contract=contract),
                max_repairs=0 if arm=='no_model_feedback' else 2,validator=validator,contract=contract)
            save(folder/'model.json',bundle);save(folder/'model_validation.json',validation)
            source=bundle['system_sysml2'];assert source==accepted['source']
            receipt=None
            if arm=='llm_backend':
                stage='llm_backend'
                prompt=ab.BACKEND_PROMPT+case['requirement']+'\nFrozen interface contract:\n'+json.dumps(contract,ensure_ascii=False)+'\nAccepted executable model bundle:\n'+json.dumps(bundle,ensure_ascii=False)
                _,raw=calls(prompt);p.check_completion(raw);code=p.extract(raw)
            else:
                stage='ir_to_code'
                with p.TRANSLATOR,p.JAVA:generated=p.lower_verified(source,contract,accepted['ir'],folder/'lowered')
                stage='hardware_adapter';code=p.adapt_verified(generated,contract,folder/'lowered')
                receipt=read(folder/'lowered/arduino_provenance.json')
            stage='arduino_compile';checked,_=p.compile_candidate(case,folder/'final',code,receipt)
            result.update(checked)
    except Exception as exc:
        if isinstance(exc,(p.ContractPlanningError,p.SysMLValidationError)):
            save(folder/(stage+'_failure.json'),dict(attempts=exc.attempts))
        if isinstance(exc,(p.ContractPlanningError,p.SysMLValidationError,p.BackendFailure,ValueError,TypeError,KeyError)) or (isinstance(exc,p.api.HTTPException) and exc.status_code in (400,422)):
            result.update(error_type=type(exc).__name__,failure_stage=stage)
        else:
            kind=next((t for t in ('PermissionDeniedError','AuthenticationError','RateLimitError','APIConnectionError','APITimeoutError','InternalServerError') if t in str(exc)),type(exc).__name__)
            result.update(infrastructure_error=kind,error_type=type(exc).__name__,functional_status='unverified',failure_stage=stage)
            block_service(kind)
    result.update(p.metrics(calls.entries),compile_pass=bool(result['compile_ok']))
    result['actual_models']=sorted({e['actual_model'] for e in calls.entries if 'actual_model' in e})
    if any(model.removeprefix('openai/') not in name for name in result['actual_models']):
        result.update(infrastructure_error='UnexpectedActualModel',functional_status='unverified')
        block_service('UnexpectedActualModel')
    host=case['id'] in config['host_cases']
    status=result['functional_status']
    result['functional_pass']=(status=='passed') if host and status not in HOST_UNKNOWN and not result.get('infrastructure_error') else None
    if host and not result['compile_ok'] and not result.get('infrastructure_error'):result['functional_pass']=False
    assert sum('response' in e for e in calls.entries)<=limit
    result['method_responses']=sum('response' in e for e in calls.entries)
    save(folder/'result.json',result)
    return result


def self_check():
    assert outcome([dict(compile_pass=True)],'compile_pass')=='passed'
    assert outcome([dict(compile_pass=False)]*4,'compile_pass')=='failed'
    assert outcome([dict(compile_pass=False)],'compile_pass')=='pending'
    assert outcome([dict(functional_pass=None)]*3,'functional_pass')=='unverified'
    assert outcome([dict(compile_pass=False),dict(compile_pass=True,infrastructure_error='403')],'compile_pass')=='pending'
    assert not failed(dict(compile_ok=True,functional_status='no_validated_oracle'),False)
    assert failed(dict(compile_ok=True,functional_status='failed'),True)
    print('Attempt counting, early stopping, unknowns and infrastructure checks passed')


def main():
    parser=argparse.ArgumentParser();parser.add_argument('--prepare-only',action='store_true');parser.add_argument('--self-check',action='store_true')
    args=parser.parse_args()
    if args.self_check:self_check();return
    os.chdir(ROOT);config=freeze();report(config)
    if args.prepare_only:return
    gate=retry_gate()
    if utc()<gate:
        save(OUT/'run_status.json',dict(status='waiting_service_retry',next_retry_not_before_utc=gate.isoformat()))
        return
    # Exclusive file creation prevents duplicate writers without PID assumptions.
    lock=OUT/'running.lock'
    try:handle=lock.open('x',encoding='utf-8')
    except FileExistsError:raise SystemExit('Existing follow-up lock; verify process identity before removing it')
    with handle:handle.write(json.dumps(dict(pid=os.getpid(),started_utc=utc().isoformat())))
    try:
        os.environ['PATH']=str(ROOT/'.tools/arduino')+os.pathsep+os.environ['PATH']
        originals=sorted(config['original_rows'],key=lambda r:(r['case'] not in config['host_cases'],GROUPS.index(r['group']),r['case']))
        for original in originals:
            host=original['case'] in config['host_cases']
            if not failed(original,host):continue
            for n in (2,3,4):
                path=OUT/'attempts'/original['group']/original['case']/f'attempt_{n}'/'result.json'
                if path.exists():
                    value=read(path)
                    if value.get('infrastructure_error'):
                        save(path.parent/('infrastructure_history_'+str(time.time_ns())+'.json'),value)
                    else:
                        if value['compile_ok'] and (not host or value['functional_status']!='failed'):break
                        continue
                value=evaluate(config,original,n)
                report(config)
                print(original['group'],original['case'],n,value['compile_ok'],value['functional_pass'],flush=True)
                if value.get('infrastructure_error'):
                    save(OUT/'run_status.json',dict(status='infrastructure_stopped',error_type=value['infrastructure_error'],updated_utc=utc().isoformat()));return
                if value['compile_ok'] and (not host or value['functional_status']!='failed'):break
        summary=report(config)
        save(OUT/'run_status.json',dict(status='finished',complete=summary['complete'],updated_utc=utc().isoformat()))
    finally:lock.unlink(missing_ok=True)


if __name__=='__main__':main()
