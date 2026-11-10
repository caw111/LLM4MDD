"""Six-arm held-out experiment with replayable calls and per-job checkpoints."""
import argparse,copy,json,re,time,threading,traceback,hashlib,subprocess,shutil
from concurrent.futures import ThreadPoolExecutor,as_completed,wait,FIRST_COMPLETED
from pathlib import Path
from run_controlled_experiment import ROOT,Calls,metrics,extract,compile_code,DIRECT_PROMPT,main,digest
from app.services.sysml import generate_validated_bundle,sysml_bundle_prompt,validate_sysml,SysMLValidationError,SysMLToolError
from app.services.sysml_compiler import compile_sysml,ir_to_il
from app.services.sysml_compiler import validate_executable_sysml
from app.services.arduino_adapter import generate_adapter,adapter_issues,compiled_kernel_text
from app.services.ir_contract import validate_ir
from experiment_metrics import first_response_entries
from app.services.interface_planner import plan_interface_contract, ContractPlanningError, check_explicit_requirements, contract_digest, add_public_behavior_checks
from app.services.hardware_adapter import validate_hardware_contract
from app.services.behavior_contract import validate_sysml_contract
from app.services.llm_json import json_prompt, check_completion, request_options, CompletionText
from app.services.interface_semantics import freeze_model_semantics, model_boundary_prompt

ARMS=('direct','direct_feedback','workflow','no_sysml_feedback','direct_ir','llm_kernel')
CONTRACT_ARMS = ('workflow', 'no_sysml_feedback', 'direct_ir', 'llm_kernel')
CALL_LIMITS = dict(direct=1, direct_feedback=3, workflow=6, no_sysml_feedback=4, direct_ir=6, llm_kernel=7)
CLI=Path(shutil.which('arduino-cli') or ROOT/'.tools/arduino/arduino-cli.exe')
JAVA=threading.Semaphore(3)
BUILD=threading.Semaphore(2)
TRANSLATOR=threading.Lock()

# Atomic writes prevent an interrupted process from leaving a false completed result.
def save(path,value):
    path.parent.mkdir(parents=True,exist_ok=True)
    temp=path.with_suffix(path.suffix+'.tmp')
    temp.write_text(json.dumps(value,ensure_ascii=False,indent=2),encoding='utf-8');temp.replace(path)

class ReplayCalls(Calls):
    def __init__(self,folder,model):
        super().__init__(folder,model);self.cursor=0
        path=folder/'calls.json'
        if path.exists():self.entries=json.loads(path.read_text(encoding='utf-8'))
    def __call__(self,prompt):
        while self.cursor<len(self.entries) and 'error_type' in self.entries[self.cursor]:self.cursor+=1
        if self.cursor<len(self.entries):
            e=self.entries[self.cursor];self.cursor+=1
            if e['prompt']!=prompt:raise RuntimeError('Cached prompt mismatch; frozen experiment cannot be resumed after method changes')
            if e.get('response_format') != request_options(prompt).get('response_format'):
                raise RuntimeError('Cached response format mismatch; use a new experiment directory')
            return e['actual_model'],CompletionText(e['response'],finish_reason=e.get('finish_reason'),
                refusal=e.get('refusal'),response_format=e.get('response_format'))
        answer=super().__call__(prompt);self.cursor=len(self.entries);return answer

def json_response(text):
    payload=text.strip();match=re.fullmatch(r'```(?:json)?\s*\n([\s\S]*?)\n```',payload,re.I)
    return json.loads(match.group(1) if match else payload)

from app.services.ir_prompt import IR_EXAMPLE, IR_V2_PROMPT as IR_PROMPT

def generate_ir(calls,requirement,contract=None):
    base=IR_PROMPT+requirement
    if contract is not None:
        base += '\nFrozen interface and requirement inventory (preserve names, types and all requirements):\n' + json.dumps(contract,ensure_ascii=False)
        base += model_boundary_prompt(contract)
    prompt=base;attempts=[]
    for i in range(3):
        _,raw=calls(json_prompt(prompt))
        try:
            check_completion(raw)
            ir=validate_ir(json_response(raw));ir_to_il(ir)
            if contract is not None:
                from app.services.behavior_contract import check_contract
                coverage=check_contract(ir,contract)
                if coverage['issues']:raise ValueError(json.dumps(coverage['issues'],ensure_ascii=False))
            attempts.append({'ok':True,'completion':getattr(raw,'completion',{})});save(calls.folder/'validation.json',attempts);return ir
        except Exception as exc:
            issue=type(exc).__name__+': '+str(exc)[:2500];attempts.append({'ok':False,'issue':issue,
                'code':getattr(exc,'code','IR_VALIDATION'),'completion':getattr(raw,'completion',{})})
            if getattr(exc,'code',None)=='LLM_REFUSAL':break
            prompt=base+'\nPrevious candidate:\n'+raw+'\nRepair without dropping requirements. Return ONLY the corrected JSON object, without explanations or markdown:\n'+issue
    save(calls.folder/'validation.json',attempts);return None

class BackendFailure(Exception):
    """An input-specific IL parser rejection, not an infrastructure outage."""

def lower(ir,folder):
    folder.mkdir(parents=True,exist_ok=True);save(folder/'ir.json',ir)
    path=folder/'model_IL.xml';path.write_text(ir_to_il(ir),encoding='utf-8')
    try:
        with TRANSLATOR:
            generated=main.convert_il_to_c(main.ConvertIlToCRequest(il_file_path=str(path),out_c_dir=str(folder/'c'),codegen_root=str(ROOT/'CodeGenerator2/CodeGenerator2'),optimize=False,target='Intel'))
    except main.HTTPException as exc:
        detail=exc.detail
        if isinstance(detail,dict) and detail.get('failed_step')=='ILTranslator' and 'ILParser Error:' in detail.get('stdout',''):
            save(folder/'backend_failure.json',detail)
            raise BackendFailure(detail['stdout'][-2000:]) from exc
        raise
    return generated.contents['define.h'],generated.contents['ProgramPage1.c']

def compile_attempt(folder,code,case):
    with BUILD:return compile_code(folder,code,CLI,case['fqbn'])

def finish_code(case,folder,calls,code,repairs,protected=False):
    # Contract-backed adapters are generated by the same deterministic production
    # renderer. A compiler error is retained; free-form repair would invalidate it.
    if protected and case.get('interface_contract') is not None:
        repairs = 0
    original=code;check=None;history=[]
    for attempt in range(repairs+1):
        out=folder/f'attempt_{attempt}';out.mkdir(parents=True,exist_ok=True)
        check,log=compile_attempt(out,code,case)
        history.append(check)
        if check['compile_ok'] or attempt==repairs:break
        if protected:
            marker='// END COMPILED SYSML MODEL'
            prefix=original.split(marker,1)[0]+marker+'\n'
            prompt=('Repair ONLY the Arduino adapter below using compiler diagnostics. Return adapter code only, no model definitions. '
                    'The immutable model is already provided; do not change or reimplement it.\nRequirements:\n'+case['requirement']+
                    '\nProvided model (read only):\n'+prefix+'\nPrevious adapter:\n'+code.split(marker,1)[1]+'\nCompiler diagnostics:\n'+log[-10000:])
            _,raw=calls(prompt);adapter=extract(raw)
            issues=adapter_issues(adapter,header=prefix,contract=case.get('interface_contract'))
            if issues:
                save(out/'repair_rejected.json',issues)
                code=original
            else:code=prefix+'\n'+adapter
        else:
            _,raw=calls(DIRECT_PROMPT+case['requirement']+'\nPrevious sketch:\n'+code+'\nRepair compiler diagnostics without dropping requirements:\n'+log[-10000:]);code=extract(raw)
    save(folder/'compile_attempts.json',history)
    return {**check,'compile_repairs':len(history)-1,'artifact':str((out/'sketch/sketch.ino').relative_to(ROOT)),'artifact_sha256':hashlib.sha256(code.encode()).hexdigest()}

def evaluate(case,repeat,root,model):
    started=time.monotonic()
    case=copy.deepcopy(case)
    if case.get('interface_contract'):
        case['requirement'] += '\nPublic interface contract:\n'+json.dumps(case['interface_contract'],ensure_ascii=False)
    folder=root/case['id']/f'repeat_{repeat}';folder.mkdir(parents=True,exist_ok=True)
    path=folder/'results.json'
    if path.exists():
        old=json.loads(path.read_text(encoding='utf-8'))
        if len(old)==6 and not any(r.get('infrastructure_error') for r in old):return old
    rows=[];planning_entries=[];contract_info={}
    def row(arm,entries,**extra):
        if arm in CONTRACT_ARMS:
            entries = planning_entries + entries
            extra = {**contract_info, **extra}
        value={'case':case['id'],'dataset':case['dataset'],'stratum':case['stratum'],'repeat':repeat,'arm':arm,
            'compile_ok':False,'compile_status':'not_generated','functional_status':'pending' if case['dataset']=='PCEval' else 'no_validated_oracle',**metrics(entries),**extra}
        rows.append(value);save(path,rows);return value
    try:
        direct=ReplayCalls(folder/'direct_calls',model)
        _,raw=direct(DIRECT_PROMPT+case['requirement']);first=copy.deepcopy(first_response_entries(direct.entries));code=extract(raw)
        initial=finish_code(case,folder/'direct',direct,code,0)
        row('direct',first,**initial)
        if initial['compile_ok']:final=initial
        else:final=finish_code(case,folder/'direct_feedback',direct,code,2)
        row('direct_feedback',direct.entries,**final)
        planning=ReplayCalls(folder/'contract_calls',model)
        try:
            contract=case.get('interface_contract')
            if contract is None:
                contract,planning_report=plan_interface_contract(planning,case['requirement'],{},target_board=case['fqbn'],max_repairs=2)
            else:
                validate_hardware_contract(contract)
                validate_sysml_contract(contract)
                check_explicit_requirements(contract,case['requirement'],{},target_board=case['fqbn'])
                contract=add_public_behavior_checks(contract,case['requirement'])
                contract=freeze_model_semantics(contract)
                validate_sysml_contract(contract)
                planning_report={'contract_sha256':contract_digest(contract),'provenance':'public_supplied','attempts':[]}
            case['interface_contract']=contract
            planning_entries=copy.deepcopy(planning.entries)
            contract_info={'contract_ok':True,'contract_sha256':contract_digest(contract)}
            save(folder/'contract.json',contract);save(folder/'contract_planning.json',planning_report)
        except (ContractPlanningError,ValueError) as exc:
            planning_entries=copy.deepcopy(planning.entries)
            save(folder/'contract_planning.json',{'attempts':getattr(exc,'attempts',[]),'error':str(exc)})
            for arm in CONTRACT_ARMS:
                row(arm,[],contract_ok=False,failure_stage='interface_contract',error=str(exc))
            save(folder/'timing.json',{'job_seconds':round(time.monotonic()-started,3),'scope':'Shared six-arm job wall time; not a per-arm latency estimate'})
            return rows
        calls=ReplayCalls(folder/'model_calls',model);accepted={}
        def validator(source):
            with JAVA:result=validate_executable_sysml(source)
            if result['ok']:
                try:accepted['ir']=validate_ir(result['ir'])
                except ValueError as exc:
                    result['ok']=False;result['stages']['sysml_to_ir']=False
                    result['issues'].append({'severity':'error','stage':'sysml_to_ir','code':'IR_CONTRACT','message':str(exc)[:2500]})
            return result
        bundle=None
        try:
            _,bundle,report=generate_validated_bundle(calls,sysml_bundle_prompt(case['requirement'],contract=case.get('interface_contract')),max_repairs=2,validator=validator,contract=case.get('interface_contract'))
            save(folder/'model.json',bundle);save(folder/'model_validation.json',report)
        except SysMLValidationError as exc:
            report={'attempts':exc.attempts};save(folder/'model_validation.json',report)
        model_entries=copy.deepcopy(calls.entries)
        if bundle is None:
            final_stage=report['attempts'][-1].get('failure_stage') or 'sysml_generation'
            row('workflow',model_entries,failure_stage=final_stage,model_ok=False)
            row('no_sysml_feedback',first_response_entries(model_entries),failure_stage=report['attempts'][0].get('failure_stage') or 'sysml_generation',model_ok=False)
            row('llm_kernel',model_entries,failure_stage=final_stage,model_ok=False)
        else:
            info={'model_ok':True,'ir_ok':True,'model_initial_ok':report['attempts'][0]['ok'],'model_attempts':len(report['attempts'])}
            try:
                header,source=lower(accepted['ir'],folder/'lowered')
                info['backend_ok']=True
                adapter=ReplayCalls(folder/'adapter_calls',model);adapter_code=None;initial_adapter_entries=[]
                try:
                    _,adapter_code=generate_adapter(adapter,header,source,case['requirement'],{},max_repairs=2,contract=case.get('interface_contract'))
                    initial_adapter_entries=copy.deepcopy(adapter.entries[:adapter.cursor])
                    (folder/'initial_adapter.ino').write_text(adapter_code,encoding='utf-8')
                    result=finish_code(case,folder/'workflow',adapter,adapter_code,2,True)
                except ValueError as exc:result={'failure_stage':'adapter','error':str(exc)[:2000]}
                if adapter_code is None:initial_adapter_entries=copy.deepcopy(adapter.entries[:adapter.cursor])
                full=row('workflow',model_entries+adapter.entries,**info,**result)
                if info['model_initial_ok']:
                    excluded={'case','dataset','stratum','repeat','arm','llm_calls','tokens','reported_cost_usd','llm_seconds'}
                    row('no_sysml_feedback',model_entries+adapter.entries,**{k:v for k,v in full.items() if k not in excluded},shared_artifact='workflow')
                else:row('no_sysml_feedback',first_response_entries(model_entries),failure_stage=report['attempts'][0].get('failure_stage') or 'sysml_generation',model_ok=False)
                kernel=ReplayCalls(folder/'kernel_calls',model)
                _,raw=kernel('Generate ONLY ProgramPage1.c implementing the following validated SysML faithfully. No Arduino/hardware code. The fixed header below is automatically prepended; do NOT include any header file or repeat declarations. Define its extern variables and exactly its functions. Reset persistent state in Init; execute actions then compute outputs in ProgramPage1. Use the header types. Output C source only.\nHeader:\n'+header+'\nSysML:\n'+bundle['system_sysml2'])
                llm_source=extract(raw);(folder/'llm_ProgramPage1.c').write_text(llm_source,encoding='utf-8')
                if adapter_code is None:result={'failure_stage':'adapter','error':'Shared initial hardware adapter failed validation'}
                else:
                    try:
                        body=adapter_code.split('// END COMPILED SYSML MODEL',1)[1]
                        sketch='#ifdef ARDUINO\n#include <Arduino.h>\n#endif\n// BEGIN COMPILED SYSML MODEL\n'+compiled_kernel_text(header,llm_source)+'// END COMPILED SYSML MODEL\n'+body
                        result=finish_code(case,folder/'llm_kernel',kernel,sketch,2,True)
                    except ValueError as exc:result={'failure_stage':'kernel','error':str(exc)[:2000]}
                # Shared model and initial adapter costs are charged to each applicable arm.
                row('llm_kernel',model_entries+initial_adapter_entries+kernel.entries,**info,**result)
            except BackendFailure as exc:
                row('workflow',model_entries,**info,backend_ok=False,failure_stage='ir_to_code',error=str(exc))
                if info['model_initial_ok']:
                    row('no_sysml_feedback',model_entries,**info,backend_ok=False,failure_stage='ir_to_code',error=str(exc),shared_artifact='workflow')
                else:row('no_sysml_feedback',first_response_entries(model_entries),failure_stage=report['attempts'][0].get('failure_stage') or 'sysml_generation',model_ok=False)
                row('llm_kernel',model_entries,**info,failure_stage='backend_dependency',error='Frozen ablation requires the shared backend header and adapter; neither was available.')
        ircalls=ReplayCalls(folder/'ir_calls',model);ir=generate_ir(ircalls,case['requirement'],case.get('interface_contract'))
        if ir is None:row('direct_ir',ircalls.entries,failure_stage='ir',ir_ok=False)
        else:
            try:
                header,source=lower(ir,folder/'direct_ir_lowered')
                iradapter=ReplayCalls(folder/'ir_adapter_calls',model)
                try:
                    _,sketch=generate_adapter(iradapter,header,source,case['requirement'],{},max_repairs=2,contract=case.get('interface_contract'))
                    result=finish_code(case,folder/'direct_ir',iradapter,sketch,2,True)
                except ValueError as exc:result={'failure_stage':'adapter','error':str(exc)[:2000]}
                row('direct_ir',ircalls.entries+iradapter.entries,ir_ok=True,backend_ok=True,**result)
            except BackendFailure as exc:
                row('direct_ir',ircalls.entries,ir_ok=True,backend_ok=False,failure_stage='ir_to_code',error=str(exc))
    except Exception as exc:
        save(folder/'run_error.json',{'type':type(exc).__name__,'traceback':traceback.format_exc()[-5000:] if not isinstance(exc,RuntimeError) else str(exc)[:500]})
        for arm in ARMS:
            if arm not in {r['arm'] for r in rows}:row(arm,[],infrastructure_error=type(exc).__name__,failure_stage='infrastructure')
    save(folder/'timing.json',{'job_seconds':round(time.monotonic()-started,3),
                             'scope':'Shared six-arm job wall time; not a per-arm latency estimate'})
    return rows

def collect(root):
    rows=[r for path in root.glob('*/repeat_*/results.json') for r in json.loads(path.read_text(encoding='utf-8'))]
    save(root/'results.json',rows)
    save(root/'progress.json',{'records':len(rows),'completed_jobs':len(rows)//6,'infrastructure_records':sum(bool(r.get('infrastructure_error')) for r in rows),'compile_passes':{a:sum(r['compile_ok'] for r in rows if r['arm']==a) for a in ARMS}})
    return rows

def simulations(root,workers):
    from run_wokwi_case import run
    eligible={r['case'] for r in json.loads((ROOT/'experiment/formal/oracle_audit/classified_summary.json').read_text(encoding='utf-8'))['cases'] if r['category']=='passed'}
    jobs={};rows=collect(root)
    for r in rows:
        if r['case'] not in eligible:r['functional_status']='no_validated_oracle';continue
        if not r['compile_ok']:r['functional_status']='not_generated_or_compile_failed';continue
        key=(r['case'],r['repeat'],r['artifact_sha256']);jobs.setdefault(key,[]).append(r)
    def one(item):
        key,group=item;case,repeat,sha=key;folder=root/case/f'repeat_{repeat}'/'simulation'/sha[:16]
        result_path=folder/'result.json'
        if result_path.exists():result=json.loads(result_path.read_text(encoding='utf-8'))
        else:
            with BUILD:result=run(case,ROOT/group[0]['artifact'],folder)
        return group,result,folder
    with ThreadPoolExecutor(max_workers=workers) as pool:
        for future in as_completed([pool.submit(one,item) for item in jobs.items()]):
            group,result,folder=future.result()
            for r in group:r.update(functional_status=result['status'],functional_log=str((folder/'simulation.log').relative_to(ROOT)))
            print('simulation',group[0]['case'],group[0]['repeat'],result['status'],flush=True)
    byjob={}
    for r in rows:byjob.setdefault((r['case'],r['repeat']),[]).append(r)
    for (case,repeat),group in byjob.items():save(root/case/f'repeat_{repeat}'/'results.json',group)
    collect(root)

def cli():
    p=argparse.ArgumentParser();p.add_argument('--out',default='experiment/output/formal_20260928');p.add_argument('--workers',type=int,default=8)
    p.add_argument('--development-check',action='store_true');p.add_argument('--simulate-only',action='store_true');p.add_argument('--generation-only',action='store_true')
    a=p.parse_args();root=(ROOT/a.out).resolve();root.mkdir(parents=True,exist_ok=True)
    manifest=json.loads((ROOT/'experiment/formal/manifest.json').read_text(encoding='utf-8'))
    files=[Path(__file__),Path(__file__).with_name('formal_ir.py'),ROOT/'tools/prompts/system_sysml2.prompt.md',ROOT/'app/services/sysml.py',ROOT/'app/services/sysml_compiler.py',ROOT/'app/services/arduino_adapter.py',Path(__file__).with_name('run_controlled_experiment.py'),Path(__file__).with_name('run_wokwi_case.py')]
    files += [ROOT/'tools/prompts/contract_controller.prompt.md', ROOT/'app/services/sysml_assembly.py', ROOT/'tools/prompts/contract_sysml2.prompt.md', ROOT/'app/services/hardware_adapter.py', ROOT/'app/services/interface_planner.py', ROOT/'app/services/ir_contract.py', ROOT/'app/services/ir_prompt.py', ROOT/'app/main.py', ROOT/'app/services/behavior_contract.py', ROOT/'app/services/failure_analysis.py',
              Path(__file__).with_name('method_snapshot.py'), Path(__file__).with_name('experiment_metrics.py'), ROOT/'tools/sysml/SysMLBridge.java']
    # Freeze transitive production helpers too (device encoders, expression
    # components and contract checks), not just the top-level entry points.
    files=list(dict.fromkeys(files+list((ROOT/'app/services').glob('*.py'))+list((ROOT/'tools/prompts').glob('*.md'))))
    frozen={'source_hashes':{str(f.relative_to(ROOT)):digest(f) for f in files},'manifest_sha256':digest(ROOT/'experiment/formal/manifest.json'),'model':'gpt-4o-mini','temperature':0.2,'max_tokens':6000,'model_repair_budget':2,'adapter_structural_repair_budget':2,'compile_repair_budget':2,'oracle_audit_sha256':digest(ROOT/'experiment/formal/oracle_audit/classified_summary.json'),'functional_tests':'held out; never fed back','repetitions':1 if a.development_check else 5,'arms':list(ARMS)}
    frozen.update(method='contract-first-v2', interface_planning_repair_budget=2,
                  adapter_structural_repair_budget=0, compile_repair_budget={'direct_feedback':2,'contract_arms':0},
                  call_limits=CALL_LIMITS,
                  contract_sharing='One planned contract shared and charged to each contract arm; direct arms receive original public information only')
    from method_snapshot import runtime_identity
    frozen['runtime'] = runtime_identity()
    config=root/'execution.json'
    if config.exists():
        if json.loads(config.read_text(encoding='utf-8'))!=frozen:raise SystemExit('Frozen execution differs; do not mix method versions')
    else:
        save(config,frozen)
        snapshot=root/'method_snapshot';snapshot.mkdir(exist_ok=True)
        for source in files:(snapshot/source.name).write_bytes(source.read_bytes())
    if not a.simulate_only:
        selected=[c for c in manifest['cases'] if c['split']=='test']
        if a.development_check:selected=[c for c in manifest['cases'] if c['id'] in ('simple_021','medium_004')]
        jobs=[(c,r) for r in range(1,frozen['repetitions']+1) for c in selected]
        print('Starting',len(jobs),'task/repeat jobs;',len(jobs)*6,'records',flush=True)
        completed=0;errors=0;remaining=iter(jobs)
        with ThreadPoolExecutor(max_workers=a.workers) as pool:
            futures={}
            def submit_next():
                job=next(remaining,None)
                if job is not None:
                    c,r=job;futures[pool.submit(evaluate,c,r,root,'gpt-4o-mini')]=(c['id'],r)
            for _ in range(a.workers):submit_next()
            while futures:
                done,_=wait(futures,return_when=FIRST_COMPLETED)
                for future in done:
                    case_id,repeat=futures.pop(future);result=future.result();completed+=1
                    errors+=int(any(r.get('infrastructure_error') for r in result))
                    if completed%10==0:collect(root)
                    print('job',completed,'/',len(jobs),case_id,repeat,[(r['arm'],r['compile_status']) for r in result],flush=True)
                    if errors<3:submit_next()
            rows=collect(root)
        if errors:
            save(root/'interrupted.json',{'completed_jobs':completed,'infrastructure_jobs':errors,'reason':'Infrastructure errors require diagnosis and resume; no completion claimed'})
            raise SystemExit(3)
        save(root/'generation_complete.json',{'jobs':len(jobs),'records':len(rows)})
    if not a.generation_only:simulations(root,2)

if __name__=='__main__':cli()
